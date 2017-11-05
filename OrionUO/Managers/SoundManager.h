/***********************************************************************************
**
** SoundManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
//----------------------------------------------------------------------------------
#pragma pack (push,1)
struct MidiInfoStruct
{
	const char* musicName;
	bool loop;
};
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
//----------------------------------------------------------------------------------
class CSoundManager
{
	SETGET(int, CurrentMusicIndex, -1);
private:
	static const int MIDI_MUSIC_COUNT = 57;
	static const MidiInfoStruct MidiInfo[MIDI_MUSIC_COUNT];
	HSTREAM m_Music{ NULL };
	HSTREAM m_WarMusic{ NULL };

	void TraceMusicError(DWORD error);
	//std::map<HSTREAM, BYTE*> streams;
public:
	CSoundManager();
	~CSoundManager();
	
	bool Init();
	void Free();
	void ResumeSound();
	void PauseSound();

	//Mix_Chunk *LoadSoundEffect(TIndexSound &is);

	bool FreeStream(HSTREAM hSteam);

	bool IsPlayingNormalMusic();

	//Метод расчета звука. При расчете учитываются: звук клиента, дистанция для эффектов.
	float GetVolumeValue(int distance = -1, bool music = false);

	HSTREAM LoadSoundEffect(CIndexSound &is);

	UCHAR_LIST CreateWaveFile(CIndexSound &is);

	//void PlaySoundEffect(Mix_Chunk *mix, int volume);
	void PlaySoundEffect(HSTREAM stream, float volume);

	void PlayMidi(int index, bool warmode);

	void PlayMP3(const string &fileName, int index, bool loop, bool warmode = false);

	void StopMusic();

	void StopWarMusic();

	void SetMusicVolume(float volume);
};
//----------------------------------------------------------------------------------
extern CSoundManager g_SoundManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
