/****************************************************************************
**
** SoundManager.h
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
//------------------------------------------------------------------------------
#ifndef SoundManagerH
#define SoundManagerH
//------------------------------------------------------------------------------
#pragma pack (push,1)
struct WaveHeader
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
	//data;
};
#pragma pack (pop)
//--------------------------------------------------------------------------
class TSoundManager
{
private:
	static const int MIDI_MUSIC_COUNT = 57;
	static const char *m_MusicName[MIDI_MUSIC_COUNT];
	HSTREAM m_Music = 0;

	void TraceMusicError(DWORD error);
	//std::map<HSTREAM, BYTE*> streams;
public:
	TSoundManager();
	~TSoundManager();
	
	bool Init();
	void Free();
	void ResumeSound();
	void PauseSound();

	//Mix_Chunk *LoadSoundEffect(TIndexSound &is);

	bool FreeStream(HSTREAM hSteam);

	//Метод расчета звука. При расчете учитываются: звук клиента, дистанция для эффектов.
	float GetVolumeValue(int distance = -1);

	HSTREAM LoadSoundEffect(TIndexSound &is);

	std::vector<BYTE> CreateWaveFile(TIndexSound &is);

	//void PlaySoundEffect(Mix_Chunk *mix, int volume);
	void PlaySoundEffect(HSTREAM stream, float volume);

	void PlayMidi(int index);

	void PlayMP3(std::string fileName, bool loop);

	void StopMusic();

	void SetMusicVolume(int volume);
};
//--------------------------------------------------------------------------
extern TSoundManager SoundManager;
//--------------------------------------------------------------------------
#endif