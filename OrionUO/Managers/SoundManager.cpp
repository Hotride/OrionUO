/***********************************************************************************
**
** SoundManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
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
BASS_ErrorDescription BASS_ErrorTable[38] = {
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
const char *BASS_ErrorGetDescription()
{
	WISPFUN_DEBUG("c_errgetdesc");
	int currentErrorCode = BASS_ErrorGetCode();

	IFOR(i, 0, 38)
	{
		if (BASS_ErrorTable[i].errorCode == currentErrorCode)
			return BASS_ErrorTable[i].desc;
	}

	return BASS_ErrorTable[0].desc;
}
#pragma endregion
//----------------------------------------------------------------------------------
CSoundManager::CSoundManager()
{
}
//----------------------------------------------------------------------------------
CSoundManager::~CSoundManager()
{
}
//----------------------------------------------------------------------------------
bool CSoundManager::Init()
{
	WISPFUN_DEBUG("c156_f1");
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
		string path = g_App.ExeFilePath("uo_4mb_2.sf2");
		if (!BASS_SetConfigPtr(BASS_CONFIG_MIDI_DEFFONT, path.c_str()))
			LOG("Could not load soundfont file for midi");
	}
	return true;
}
//----------------------------------------------------------------------------------
void CSoundManager::Free()
{
	WISPFUN_DEBUG("c156_f2");
	StopMusic();
	BASS_Free();
}
//----------------------------------------------------------------------------------
void CSoundManager::PauseSound()
{
	WISPFUN_DEBUG("c156_f3");
	BASS_Pause();
	g_Orion.AdjustSoundEffects(g_Ticks + 100000);
}
//----------------------------------------------------------------------------------
void CSoundManager::ResumeSound()
{
	WISPFUN_DEBUG("c156_f4");
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
	WISPFUN_DEBUG("c156_f5");
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
	WISPFUN_DEBUG("c156_f6");
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
	WISPFUN_DEBUG("c156_f7");
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
	WISPFUN_DEBUG("c156_f8");
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
	WISPFUN_DEBUG("c156_f9");
	return BASS_StreamFree(hSteam);
}
bool CSoundManager::IsPlayingNormalMusic()
{
	WISPFUN_DEBUG("c156_f10");
	return BASS_ChannelIsActive(m_Music);
}
//----------------------------------------------------------------------------------
void CSoundManager::PlayMidi(int index, bool warmode)
{
	WISPFUN_DEBUG("c156_f11");
	if (index >= 0 && index < MIDI_MUSIC_COUNT)
	{
		if (warmode && m_WarMusic != 0)
			return;

		if (warmode)
			BASS_ChannelStop(m_Music);
		else
			StopMusic();

		char musicPath[100] = {0};
		MidiInfoStruct midiInfo = MidiInfo[index];
		sprintf_s(musicPath, "music\\%s", midiInfo.musicName);

		wstring midiName = ToWString(musicPath);
		HSTREAM streamHandle = BASS_MIDI_StreamCreateFile(FALSE, midiName.c_str(), 0, 0, BASS_MIDI_DECAYEND, 0);
		float volume = GetVolumeValue(-1, true);
		BASS_ChannelSetAttribute(streamHandle, BASS_ATTRIB_VOL, volume);
		BASS_ChannelPlay(streamHandle, midiInfo.loop);
		if (warmode)
			m_WarMusic = streamHandle;
		else
		{
			m_CurrentMusicIndex = index;
			m_Music = streamHandle;
		}
	}
	else
		LOG("Music ID is out of range: %i\n", index);
}
//----------------------------------------------------------------------------------
void CSoundManager::PlayMP3(std::string fileName, int index, bool loop, bool warmode)
{
	WISPFUN_DEBUG("c156_f12");
	if (warmode && m_WarMusic != 0)
		return;

	if (warmode)
		BASS_ChannelStop(m_Music);
	else
		StopMusic();
	HSTREAM streamHandle = BASS_StreamCreateFile(FALSE, fileName.c_str(), 0, 0, loop ? BASS_SAMPLE_LOOP : 0);
	BASS_ChannelSetAttribute(streamHandle, BASS_ATTRIB_VOL, GetVolumeValue(-1, true));
	BASS_ChannelPlay(streamHandle, 0);
	if (warmode)
		m_WarMusic = streamHandle;
	else
	{
		m_Music = streamHandle;
		m_CurrentMusicIndex = index;
	}

}
//----------------------------------------------------------------------------------
void CSoundManager::StopWarMusic()
{
	WISPFUN_DEBUG("c156_f13");
	BASS_ChannelStop(m_WarMusic);
	m_WarMusic = 0;

	if (m_Music != 0 && !BASS_ChannelIsActive(m_Music))
		BASS_ChannelPlay(m_Music, 1);
}
//----------------------------------------------------------------------------------
void CSoundManager::StopMusic()
{
	WISPFUN_DEBUG("c156_f14");
	BASS_ChannelStop(m_Music);
	m_Music = 0;
	BASS_ChannelStop(m_WarMusic);
	m_WarMusic = 0;
}
//----------------------------------------------------------------------------------
void CSoundManager::SetMusicVolume(float volume)
{
	WISPFUN_DEBUG("c156_f15");
	if (m_Music != 0 && BASS_ChannelIsActive(m_Music))
		BASS_ChannelSetAttribute(m_Music, BASS_ATTRIB_VOL, volume);

	if (m_WarMusic != 0 && BASS_ChannelIsActive(m_WarMusic))
		BASS_ChannelSetAttribute(m_WarMusic, BASS_ATTRIB_VOL, volume);
}
//----------------------------------------------------------------------------------
void CSoundManager::TraceMusicError(DWORD error)
{
	WISPFUN_DEBUG("c156_f16");
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
const MidiInfoStruct CSoundManager::MidiInfo[MIDI_MUSIC_COUNT] = 
{
	{ "oldult01.mid", true },
	{ "create1.mid", false },
	{ "dragflit.mid", false },
	{ "oldult02.mid", true },
	{ "oldult03.mid", true },
	{ "oldult04.mid", true },
	{ "oldult05.mid", true },
	{ "oldult06.mid", true },
	{ "stones2.mid", true },
	{ "britain1.mid", true },
	{ "britain2.mid", true },
	{ "bucsden.mid", true },
	{ "jhelom.mid", false },
	{ "lbcastle.mid", false },
	{ "linelle.mid", false },
	{ "magincia.mid", true },
	{ "minoc.mid", true },
	{ "ocllo.mid", true },
	{ "samlethe.mid", false },
	{ "serpents.mid", true },
	{ "skarabra.mid", true },
	{ "trinsic.mid", true },
	{ "vesper.mid", true },
	{ "wind.mid", true },
	{ "yew.mid", true },
	{ "cave01.mid", false },
	{ "dungeon9.mid", false },
	{ "forest_a.mid", false },
	{ "intown01.mid", false },
	{ "jungle_a.mid", false },
	{ "mountn_a.mid", false },
	{ "plains_a.mid", false },
	{ "sailing.mid", false },
	{ "swamp_a.mid", false },
	{ "tavern01.mid", false },
	{ "tavern02.mid", false },
	{ "tavern03.mid", false },
	{ "tavern04.mid", false },
	{ "combat1.mid", false },
	{ "combat2.mid", false },
	{ "combat3.mid", false },
	{ "approach.mid", false },
	{ "death.mid", false },
	{ "victory.mid", false },
	{ "btcastle.mid", false },
	{ "nujelm.mid", true },
	{ "dungeon2.mid", false },
	{ "cove.mid", true },
	{ "moonglow.mid", true },
	{ "oldult02.mid", false },
	{ "serpents.mid", true },
	{ "oldult04.mid", false },
	{ "dragflit.mid", false },
	{ "create1.mid", false },
	{ "approach.mid", false },
	{ "combat3.mid", false },
	{ "jungle_a.mid", false }
};
//----------------------------------------------------------------------------------
