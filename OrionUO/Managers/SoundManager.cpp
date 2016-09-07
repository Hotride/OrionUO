/***********************************************************************************
**
** SoundManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "SoundManager.h"
#include "../OrionWindow.h"
#include "../Wisp/WispLogger.h"
#include "ConfigManager.h"
#include "../MulStruct.h"
#include <fstream>
#include <string>
//----------------------------------------------------------------------------------
CSoundManager g_SoundManager;
//----------------------------------------------------------------------------------
#pragma region BASS.DLL error code descriptions

struct BASS_ErrorDescription
{
	int errorCode;
	char desc[64];
};
//----------------------------------------------------------------------------------
BASS_ErrorDescription BASS_ErrorTable[] = {
	{ -2, "unspecified error" },
	{ BASS_OK, "OK" },
	{ BASS_ERROR_MEM, "memory error" },
	{ BASS_ERROR_FILEOPEN, "can't open the file" },
	{ BASS_ERROR_DRIVER, "can't find a free/valid driver" },
	{ BASS_ERROR_BUFLOST, "the sample buffer was lost" },
	{ BASS_ERROR_HANDLE, "invalid handle" },
	{ BASS_ERROR_FORMAT, "unsupported sample format" },
	{ BASS_ERROR_POSITION, "invalid position" },
	{ BASS_ERROR_INIT, "BASS_Init has not been successfully called" },
	{ BASS_ERROR_START, "BASS_Start has not been successfully called" },
	{ BASS_ERROR_SSL, "SSL/HTTPS support isn't available" },
	{ BASS_ERROR_ALREADY, "already initialized/paused/whatever" },
	{ BASS_ERROR_NOCHAN, "can't get a free channel" },
	{ BASS_ERROR_ILLTYPE, "an illegal type was specified" },
	{ BASS_ERROR_ILLPARAM, "an illegal parameter was specified" },
	{ BASS_ERROR_NO3D, "no 3D support" },
	{ BASS_ERROR_NOEAX, "no EAX support" },
	{ BASS_ERROR_DEVICE, "illegal device number" },
	{ BASS_ERROR_NOPLAY, "not playing" },
	{ BASS_ERROR_FREQ, "illegal sample rate" },
	{ BASS_ERROR_NOTFILE, "the stream is not a file stream" },
	{ BASS_ERROR_NOHW, "no hardware voices available" },
	{ BASS_ERROR_EMPTY, "the MOD music has no sequence data" },
	{ BASS_ERROR_NONET, "no internet connection could be opened" },
	{ BASS_ERROR_CREATE, "couldn't create the file" },
	{ BASS_ERROR_NOFX, "effects are not available" },
	{ BASS_ERROR_NOTAVAIL, "requested data is not available" },
	{ BASS_ERROR_DECODE, "the channel is/isn't a \"decoding channel\"" },
	{ BASS_ERROR_DX, "a sufficient DirectX version is not installed" },
	{ BASS_ERROR_TIMEOUT, "connection timedout" },
	{ BASS_ERROR_FILEFORM, "unsupported file format" },
	{ BASS_ERROR_SPEAKER, "unavailable speaker" },
	{ BASS_ERROR_VERSION, "invalid BASS version (used by add-ons)" },
	{ BASS_ERROR_CODEC, "codec is not available/supported" },
	{ BASS_ERROR_ENDED, "the channel/file has ended" },
	{ BASS_ERROR_BUSY, "the device is busy" },
	{ BASS_ERROR_UNKNOWN, "some other mystery problem" },
};
//----------------------------------------------------------------------------------
char* BASS_ErrorGetDescription()
{
	auto currentErrorCode = BASS_ErrorGetCode();

	for (auto &bassError : BASS_ErrorTable) {
		if (bassError.errorCode == currentErrorCode)
			return bassError.desc;
	}
	return BASS_ErrorTable[0].desc;
}
#pragma endregion
//----------------------------------------------------------------------------------
CSoundManager::CSoundManager()
: m_Music(0)
{
}
//----------------------------------------------------------------------------------
CSoundManager::~CSoundManager()
{
}
//----------------------------------------------------------------------------------
bool CSoundManager::Init()
{
	LOG("Initializing bass sound system.\n");
	// initialize default output device	
	if (!BASS_Init(-1, 48000, BASS_DEVICE_3D, g_OrionWindow.Handle, NULL))
	{
		LOG("Can't initialize device: %s\n", BASS_ErrorGetDescription());
		return false;
	}
	else
	{
		LOG("Sound init successfull.\n");
		BASS_SetConfig(BASS_CONFIG_SRC, 3); // interpolation method

		if (!BASS_SetConfig(BASS_CONFIG_3DALGORITHM, BASS_3DALG_FULL))
			LOG("Error setting 3d sound: %s\n", BASS_ErrorGetDescription());
	}
	return true;
}
//----------------------------------------------------------------------------------
void CSoundManager::Free()
{
	StopMusic();
	BASS_Free();
}
//----------------------------------------------------------------------------------
void CSoundManager::PauseSound()
{
	BASS_Pause();
	//g_Orion->AdjustSoundEffects(g_Ticks + 100000);
}
//----------------------------------------------------------------------------------
void CSoundManager::ResumeSound()
{
	BASS_Start();
}
//----------------------------------------------------------------------------------
/// <summary>Расчитывает громкость звука с учетом громкости в клиенте
/// и возможной дистанции для эффетов.</summary>
/// <param name="distance">расстояние для эффектов.
///При значении -1 расстояние не учитывается.</param>
/// <returns>громкость для BASS библиотеки.</returns>
float CSoundManager::GetVolumeValue(int distance, bool music)
{
	float volume = BASS_GetVolume();
	WORD clientConfigVolume = music ? g_ConfigManager.MusicVolume : g_ConfigManager.SoundVolume;
	if (volume == 0 || clientConfigVolume == 0) return 0;
	float clientsVolumeValue = (static_cast<float>(255) / static_cast<float>(clientConfigVolume));
	volume /= clientsVolumeValue;
	if (distance > g_ConfigManager.UpdateRange || distance < 1)
		return volume;
	else
	{
		float soundValuePerDistance = volume / g_ConfigManager.UpdateRange;
		return volume - (soundValuePerDistance * distance);
	}
		 
}
//----------------------------------------------------------------------------------
/// <summary>Создаёт в памяти 16 битный wave файл для последующего
/// проигрывания.</summary>
/// <param name="is">ссылка на запись звука в MUL файле</param>
/// <returns>Wave файл в виде вектора байтов</returns>
UCHAR_LIST CSoundManager::CreateWaveFile(CIndexSound &is)
{
	size_t dataSize = is.DataSize - sizeof(SOUND_BLOCK);
	auto waveSound = std::vector<BYTE>(dataSize + sizeof(WaveHeader));
	auto waveHeader = reinterpret_cast<WaveHeader*>(waveSound.data());

	strcpy(waveHeader->chunkId, "RIFF");
	strcpy(waveHeader->format, "WAVE");
	strcpy(waveHeader->subChunkId, "fmt ");
	strcpy(waveHeader->dataChunkId, "data");

	waveHeader->chunkSize = waveSound.size();
	waveHeader->subChunkSize = 16;
	waveHeader->audioFormat = 1;
	waveHeader->numChannels = 1;
	waveHeader->sampleRate = 22050;
	waveHeader->bitsPerSample = 16;
	waveHeader->bytesPerSecond = 88200;
	waveHeader->blockAlign = 4;	
	waveHeader->dataSize = dataSize;

	is.Delay = static_cast<DWORD>((dataSize - 16) / 88.2f);

	auto sndDataPtr = reinterpret_cast<PBYTE>(is.Address + sizeof(SOUND_BLOCK));	
	std::copy_n(sndDataPtr + 16, dataSize - 16, waveSound.begin() + sizeof(WaveHeader));	

	return waveSound;
}
//----------------------------------------------------------------------------------
HSTREAM CSoundManager::LoadSoundEffect(CIndexSound &is)
{
	if (is.m_WaveFile.empty())
		is.m_WaveFile.swap(CreateWaveFile(is));

	size_t waveFileSize = is.DataSize - sizeof(SOUND_BLOCK) +sizeof(WaveHeader);	
	
	auto hStream = BASS_StreamCreateFile(true, is.m_WaveFile.data(), 0,
		is.m_WaveFile.size() - 16, BASS_SAMPLE_FLOAT | BASS_SAMPLE_3D | BASS_SAMPLE_SOFTWARE);

	if (hStream == 0)
	{
		LOG("BASS create stream error: %s\n", BASS_ErrorGetDescription());
		is.m_WaveFile.clear();
	}
	
	return hStream;
}
//----------------------------------------------------------------------------------
void CSoundManager::PlaySoundEffect(HSTREAM hStream, float volume)
{
	if (hStream == 0 || GetForegroundWindow() != g_OrionWindow.Handle)
		return;

	BASS_ChannelSetAttribute(hStream, BASS_ATTRIB_VOL, volume);

	if (!BASS_ChannelPlay(hStream, false))
		LOG("Bass sound play error: %s\n", BASS_ErrorGetDescription());

}
//----------------------------------------------------------------------------------
/// <summary>Очистка звукового потока и высвобождение памяти.</summary>
/// <param name="hSteam">stream handle</param>
bool CSoundManager::FreeStream(HSTREAM hSteam)
{
	return BASS_StreamFree(hSteam);
}
//----------------------------------------------------------------------------------
void CSoundManager::PlayMidi(int index)
{
	const char *musicSubCat[3] =
	{
		"",
		"\\512K",
		"\\4MB"
	};

	if (index >= 0 && index < MIDI_MUSIC_COUNT)
	{
		StopMusic();

		MCI_OPEN_PARMS openParm = {0};
		openParm.dwCallback = (uint)g_OrionWindow.Handle;
		openParm.lpstrDeviceType = L"sequencer";
		uint flags = MCI_OPEN_ELEMENT;

		char musicPath[100] = {0};
		sprintf_s(musicPath, "music%s\\%s", musicSubCat[1], m_MusicName[index]);

		wstring midiName = ToWString(musicPath);
		openParm.lpstrElementName = midiName.c_str();

		MCIERROR error = mciSendCommand(0, MCI_OPEN, flags, (uint)&openParm);

		if (!error)
		{
			m_Music = openParm.wDeviceID;

			SetMusicVolume(g_ConfigManager.MusicVolume);

			MCI_PLAY_PARMS playParm = {0};
			playParm.dwCallback = (uint)g_OrionWindow.Handle;
			playParm.dwFrom = 0;

			error = mciSendCommand(m_Music, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (uint)&playParm);

			if (error)
				TraceMusicError(error);
		}
		else
			TraceMusicError(error);
	}
	else
		LOG("Music ID is out of range: %i\n", index);
}
//----------------------------------------------------------------------------------
void CSoundManager::PlayMP3(std::string fileName, bool loop, bool warmode)
{
	if (warmode && m_WarMusic != 0)
		return;

	if (warmode)
		BASS_ChannelStop(m_Music);
	else
		StopMusic();
	HSTREAM streamHandle = BASS_StreamCreateFile(FALSE, fileName.c_str(), 0, 0, 0);
	BASS_ChannelSetAttribute(streamHandle, BASS_ATTRIB_VOL, GetVolumeValue(-1, true));
	BASS_ChannelPlay(streamHandle, loop ? 1 : 0);
	if (warmode)
		m_WarMusic = streamHandle;
	else
		m_Music = streamHandle;
}
//----------------------------------------------------------------------------------
void CSoundManager::StopWarMusic()
{
	//midi music stopping code via mci.
	/*MCI_GENERIC_PARMS mciGen;
	DWORD error = mciSendCommand(m_Music, MCI_STOP, MCI_WAIT, (DWORD)(LPMCI_GENERIC_PARMS)&mciGen);

	TraceMusicError(error);*/
	BASS_ChannelStop(m_WarMusic);
	m_WarMusic = 0;
	if (m_Music != 0 && !BASS_ChannelIsActive(m_Music))
		BASS_ChannelPlay(m_Music, 1);
}
//----------------------------------------------------------------------------------
void CSoundManager::StopMusic()
{
	//midi music stopping code via mci.
	/*MCI_GENERIC_PARMS mciGen;
	DWORD error = mciSendCommand(m_Music, MCI_STOP, MCI_WAIT, (DWORD)(LPMCI_GENERIC_PARMS)&mciGen);

	TraceMusicError(error);*/
	BASS_ChannelStop(m_Music);
	m_Music = 0;
}
//----------------------------------------------------------------------------------
void CSoundManager::SetMusicVolume(float volume)
{
	if (m_Music != 0 && BASS_ChannelIsActive(m_Music))
		BASS_ChannelSetAttribute(m_Music, BASS_ATTRIB_VOL, volume);

	if (m_WarMusic != 0 && BASS_ChannelIsActive(m_WarMusic))
		BASS_ChannelSetAttribute(m_WarMusic, BASS_ATTRIB_VOL, volume);
}
//----------------------------------------------------------------------------------
void CSoundManager::TraceMusicError(DWORD error)
{
	if (error)
	{
		wchar_t szBuf[MAXERRORLENGTH];

		if (mciGetErrorString(error, szBuf, MAXERRORLENGTH))
			LOG("Midi error: %s\n", ToString(szBuf).c_str());
		else
			LOG("Midi error: #%i\n", error);
	}
}
//----------------------------------------------------------------------------------
const char *CSoundManager::m_MusicName[MIDI_MUSIC_COUNT] =
{
	"oldult01.mid",
	"create1.mid",
	"dragflit.mid",
	"oldult02.mid",
	"oldult03.mid",
	"oldult04.mid",
	"oldult05.mid",
	"oldult06.mid",
	"stones2.mid",
	"britain1.mid",
	"britain2.mid",
	"bucsden.mid",
	"jhelom.mid",
	"lbcastle.mid",
	"linelle.mid",
	"magincia.mid",
	"minoc.mid",
	"ocllo.mid",
	"samlethe.mid",
	"serpents.mid",
	"skarabra.mid",
	"trinsic.mid",
	"vesper.mid",
	"wind.mid",
	"yew.mid",
	"cave01.mid",
	"dungeon9.mid",
	"forest_a.mid",
	"intown01.mid",
	"jungle_a.mid",
	"mountn_a.mid",
	"plains_a.mid",
	"sailing.mid",
	"swamp_a.mid",
	"tavern01.mid",
	"tavern02.mid",
	"tavern03.mid",
	"tavern04.mid",
	"combat1.mid",
	"combat2.mid",
	"combat3.mid",
	"approach.mid",
	"death.mid",
	"victory.mid",
	"btcastle.mid",
	"nujelm.mid",
	"dungeon2.mid",
	"cove.mid",
	"moonglow.mid",
	"oldult02.mid",
	"serpents.mid",
	"oldult04.mid",
	"dragflit.mid",
	"create1.mid",
	"approach.mid",
	"combat3.mid",
	"jungle_a.mid"
};
//----------------------------------------------------------------------------------
