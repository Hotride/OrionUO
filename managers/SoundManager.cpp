/****************************************************************************
**
** SoundManager.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TSoundManager SoundManager;
//---------------------------------------------------------------------------
TSoundManager::TSoundManager()
: m_Music(0)
{
}
//---------------------------------------------------------------------------
TSoundManager::~TSoundManager()
{
}
//---------------------------------------------------------------------------
bool TSoundManager::Init()
{
	/*if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return false;
	
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 1024) < 0)
	{
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return false;
	}	*/

	trace_printf("Initializing bass sound system. g_hWnd=%d\n", g_hWnd);
	// initialize default output device
	if (!BASS_Init(-1, 48000, BASS_DEVICE_3D, g_hWnd, NULL))
	{
		trace_printf("Can't initialize device! (Error code: %d)\n", BASS_ErrorGetCode());
		return false;
	} else {
		trace_printf("Sound init successfull.\n");	
		BASS_SetConfig( BASS_CONFIG_SRC, 3 );
		BASS_SetConfig(BASS_CONFIG_3DALGORITHM, BASS_3DALG_FULL);
	}
	
	return true;
}
//---------------------------------------------------------------------------
void TSoundManager::Free()
{
	StopMusic();

	//SDL_CloseAudio();
	//SDL_Quit();

	BASS_Free();
}

/// <summary>Создаёт в памяти 16 битный wave файл для последующего
/// проигрывания.</summary>
/// <param name="is">ссылка на запись звука в MUL файле</param>
/// <returns>Wave файл в виде вектора байтов</returns>
std::vector<BYTE> TSoundManager::CreateWaveFile(TIndexSound &is)
{
	int dataSize = is.Size - sizeof(SOUND_BLOCK);

	auto waveSound = std::vector<BYTE>(dataSize + sizeof(WaveHeader));
	auto waveHeader = (WaveHeader*) waveSound.data();

	strcpy(waveHeader->chunkId, "RIFF");
	strcpy(waveHeader->format, "WAVE");
	strcpy(waveHeader->subChunkId, "fmt ");
	strcpy(waveHeader->dataChunkId, "data");

	waveHeader->chunkSize = waveSound.size();
	waveHeader->subChunkSize = 16;
	waveHeader->audioFormat = 1;
	waveHeader->numChannels = 1;
	waveHeader->sampleRate = 22050;
	//waveHeader->bytesPerSecond = 88200;
	waveHeader->bitsPerSample = 16;
	waveHeader->bytesPerSecond = waveHeader->sampleRate * 
		(waveHeader->bitsPerSample/8) * waveHeader->numChannels;
	waveHeader->blockAlign = 2;	
	waveHeader->dataSize = dataSize;

	is.Timer = (DWORD)(dataSize / (((float)waveHeader->bytesPerSecond) / 1000.0f));

	auto sndDataPtr = (PBYTE)(is.Address + sizeof(SOUND_BLOCK));
	std::copy_n( sndDataPtr, dataSize, waveSound.begin() + sizeof(WaveHeader));	

	//FILE *ptr_myfile;

	//ptr_myfile = fopen("test.wav", "wb");
	//if (!ptr_myfile)
	//{
	//	printf("Unable to open file!");
	//}
	//	
	//fwrite(waveSound.data(), waveSound.size(), 1, ptr_myfile);
	//
	//fclose(ptr_myfile);

	//HSTREAM hStream = BASS_StreamCreateFile(true, &waveSound[0], 0, waveSound.size(), 0);
	//BASS_ChannelPlay(hStream, true);	

	return waveSound;
}
//---------------------------------------------------------------------------
//Mix_Chunk *TSoundManager::LoadSoundEffect(TIndexSound &is)
//{
//	auto waveSound = this->CreateWaveFile(is);
//
//	SDL_RWops *rWops = SDL_RWFromMem(waveSound.data(), waveSound.size());
//	Mix_Chunk *mix = Mix_LoadWAV_RW(rWops, 1);
//
//	if (mix == NULL)
//		trace_printf("SDL sound error: %s\n", SDL_GetError());
//
//	return mix;
//}

HSTREAM TSoundManager::LoadSoundEffect(TIndexSound &is)
{
	auto waveSound = this->CreateWaveFile(is);
	BYTE* waveFileMem = new BYTE[waveSound.size()];	
	std::copy(waveSound.begin(), waveSound.end(), &waveFileMem[0]);
	
	HSTREAM hStream = BASS_StreamCreateFile(true, waveFileMem, 0, waveSound.size(), BASS_SAMPLE_FLOAT | BASS_SAMPLE_3D | BASS_SAMPLE_SOFTWARE);
	
	if (hStream==0)
		trace_printf("BASS create stream error: %d\n", BASS_ErrorGetCode());

	//trace_printf("Loaded stream: %d\n", hStream);	

	//PlaySoundEffect(hStream, 1);
	
	return hStream;
}

//---------------------------------------------------------------------------
//void TSoundManager::PlaySoundEffect(Mix_Chunk *mix, int volume)
//{
//	if (mix == NULL || GetForegroundWindow() != g_hWnd)
//		return;
//
//	Mix_VolumeChunk(mix, volume);
//
//	if (Mix_PlayChannel(-1, mix, 0) < 0)
//		trace_printf("Sound mix error: %s\n", Mix_GetError());
//}

void TSoundManager::PlaySoundEffect(HSTREAM hStream, int volume)
{
	if (volume > 255)
		volume = 255;
	else if (volume < 0)
		volume = 0;	

	if (hStream == 0 || GetForegroundWindow() != g_hWnd)
		return;

	BASS_ChannelSetAttribute(hStream, BASS_ATTRIB_VOL, ((float)volume) / 2.55f);

	//trace_printf("PlaySoundEffect(HSTREAM %d, ..)", hStream);

	if (!BASS_ChannelPlay(hStream, false))
		trace_printf("Bass sound play error: %s\n", BASS_ErrorGetCode());

}
//---------------------------------------------------------------------------
void TSoundManager::PlayMidi(int index)
{
	const char *musicSubCat[3] =
	{
		"",
		"\\512K",
		"\\4MB"
	};

	if (index >= 0 && index < MUSIC_COUNT)
	{
		StopMusic();

		if (!ConfigManager.Music || GetForegroundWindow() != g_hWnd)
			return;

		MCI_OPEN_PARMS openParm = {0};
		openParm.dwCallback = (DWORD)g_hWnd;
		openParm.lpstrDeviceType = L"sequencer";
		DWORD flags = MCI_OPEN_ELEMENT;

		char musicPath[100] = {0};
		sprintf(musicPath, "music%s\\%s", musicSubCat[1], m_MusicName[index]);

		wstring midiName = ToWString(musicPath);
		openParm.lpstrElementName = midiName.c_str();

		MCIERROR error = mciSendCommand(0, MCI_OPEN, flags,(DWORD)&openParm);

		if (!error)
		{
			m_Music = openParm.wDeviceID;

			SetMusicVolume(ConfigManager.MusicVolume);

			MCI_PLAY_PARMS playParm = {0};
			playParm.dwCallback = (DWORD)g_hWnd;
			playParm.dwFrom = 0;

			error = mciSendCommand(m_Music, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)&playParm);

			if (error)
				TraceMusicError(error);
		}
		else
			TraceMusicError(error);
	}
	else
		trace_printf("Music ID is out of renge: %i\n",index);
}
//---------------------------------------------------------------------------
void TSoundManager::StopMusic()
{
	if (m_Music != 0)
	{
		MCI_GENERIC_PARMS mciGen;
		DWORD error = mciSendCommand(m_Music, MCI_STOP, MCI_WAIT, (DWORD)(LPMCI_GENERIC_PARMS)&mciGen);

		TraceMusicError(error);

		m_Music = 0;
	}
}
//---------------------------------------------------------------------------
void TSoundManager::SetMusicVolume(int volume)
{
	if (m_Music != 0)
	{
	}
}
//---------------------------------------------------------------------------
void TSoundManager::TraceMusicError(DWORD error)
{
	if (error)
	{
		wchar_t szBuf[MAXERRORLENGTH];

		if (mciGetErrorString(error, szBuf, MAXERRORLENGTH))
			trace_printf("Midi error: %s\n", ToString(szBuf).c_str());
		else
			trace_printf("Midi error: #%i\n", error);
	}
}
//---------------------------------------------------------------------------
const char *TSoundManager::m_MusicName[MUSIC_COUNT] =
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
//---------------------------------------------------------------------------