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
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return false;
	
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 1024) < 0)
	{
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return false;
	}	

	trace_printf("Initializing bass sound system.\n");
	// initialize default output device
	if (!BASS_Init(-1,44100, 0, g_hWnd, NULL)) {
		trace_printf("Can't initialize device! (Error code: %d)\n", BASS_ErrorGetCode());
	} else {
		trace_printf("Sound init successfull.\n");
	}
	
	return true;
}
//---------------------------------------------------------------------------
void TSoundManager::Free()
{
	StopMusic();

	SDL_CloseAudio();
	SDL_Quit();
}
//---------------------------------------------------------------------------
Mix_Chunk *TSoundManager::LoadSoundEffect(TIndexSound &is)
{
	int dataSize = is.Size - sizeof(SOUND_BLOCK);

	PBYTE mem = new BYTE[dataSize + sizeof(WaveHeader)];
	WaveHeader *wh = (WaveHeader*)mem;
	sprintf(wh->chunkId, "RIFF");
	wh->chunkSize = dataSize + sizeof(WaveHeader);
	sprintf(wh->format, "WAVE");

	sprintf(wh->subChunkId, "fmt ");
	wh->subChunkSize = 16;

	wh->audioFormat = 1;
	wh->numChannels = 1;
	wh->sampleRate = 22050;
	wh->bytesPerSecond = 88200;
	wh->blockAlign = 4;
	wh->bitsPerSample = 16;
	
	is.Timer = (DWORD)(dataSize / 88.2f);

	sprintf(wh->dataChunkId, "data");
	wh->dataSize = dataSize;
	
	PBYTE ptr = mem + sizeof(WaveHeader);
	PBYTE from = (PBYTE)(is.Address + sizeof(SOUND_BLOCK));

	memcpy(&ptr[0], &from[0], dataSize);

	SDL_RWops *tmp = SDL_RWFromMem(mem, dataSize + sizeof(WaveHeader));
	Mix_Chunk *mix = Mix_LoadWAV_RW(tmp, 1);
	delete mem;

	if (mix == NULL)
		trace_printf("Sound error: %s\n", SDL_GetError());

	return mix;
}
//---------------------------------------------------------------------------
void TSoundManager::PlaySoundEffect(Mix_Chunk *mix, int volume)
{
	if (mix == NULL || GetForegroundWindow() != g_hWnd)
		return;

	Mix_VolumeChunk(mix, volume);

	if (Mix_PlayChannel(-1, mix, 0) < 0)
		trace_printf("Sound mix error: %s\n", Mix_GetError());
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