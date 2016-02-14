/*
	BASS simple console player
	Copyright (c) 1999-2015 Un4seen Developments Ltd.
*/

#include <stdlib.h>
#include <stdio.h>
#include "bass.h"

#ifdef _WIN32 // Windows
#include <conio.h>
#else // OSX/Linux
#include <sys/time.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>

#define Sleep(x) usleep(x*1000)

int _kbhit()
{
	int r;
	fd_set rfds;
	struct timeval tv={0};
	struct termios term,oterm;
	tcgetattr(0,&oterm);
	memcpy(&term,&oterm,sizeof(term));
	cfmakeraw(&term);
	tcsetattr(0,TCSANOW,&term);
	FD_ZERO(&rfds);
	FD_SET(0,&rfds);
	r=select(1,&rfds,NULL,NULL,&tv);
	tcsetattr(0,TCSANOW,&oterm);
	return r;
}
#endif

// display error messages
void Error(const char *text) 
{
	printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
	BASS_Free();
	exit(0);
}

void ListDevices()
{
	BASS_DEVICEINFO di;
	int a;
	for (a=1;BASS_GetDeviceInfo(a,&di);a++) {
		if (di.flags&BASS_DEVICE_ENABLED) // enabled output device
			printf("dev %d: %s\n",a,di.name);
	}
}

void main(int argc, char **argv)
{
	DWORD chan,act,time,level;
	BOOL ismod;
	QWORD pos;
	int a,device=-1;

	printf("Simple console mode BASS example : MOD/MPx/OGG/WAV player\n"
			"---------------------------------------------------------\n");

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		printf("An incorrect version of BASS was loaded");
		return;
	}

	for (a=1;a<argc;a++) {
		if (!strcmp(argv[a],"-l")) {
			ListDevices();
			return;
		} else if (!strcmp(argv[a],"-d") && a+1<argc) device=atoi(argv[++a]);
		else break;
	}
	if (a!=argc-1) {
		printf("\tusage: contest [-l] [-d #] <file>\n"
			"\t-l = list devices\n"
			"\t-d = device number\n");
		return;
	}

	// initialize output device
	if (!BASS_Init(device,44100,0,0,NULL))
		Error("Can't initialize device");

	// try streaming the file/url
	if ((chan=BASS_StreamCreateFile(FALSE,argv[argc-1],0,0,BASS_SAMPLE_LOOP))
		|| (chan=BASS_StreamCreateURL(argv[argc-1],0,BASS_SAMPLE_LOOP,0,0))) {
		pos=BASS_ChannelGetLength(chan,BASS_POS_BYTE);
		if (BASS_StreamGetFilePosition(chan,BASS_FILEPOS_DOWNLOAD)!=-1) {
			// streaming from the internet
			if (pos!=-1)
#ifdef _WIN32
				printf("streaming internet file [%I64d bytes]",pos);
#else
				printf("streaming internet file [%lld bytes]",pos);
#endif
			else
				printf("streaming internet file");
		} else
#ifdef _WIN32
			printf("streaming file [%I64d bytes]",pos);
#else
			printf("streaming file [%lld bytes]",pos);
#endif
		ismod=FALSE;
	} else {
		// try loading the MOD (with looping, sensitive ramping, and calculate the duration)
		if (!(chan=BASS_MusicLoad(FALSE,argv[argc-1],0,0,BASS_SAMPLE_LOOP|BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN,1)))
			// not a MOD either
			Error("Can't play the file");
		{ // count channels
			float dummy;
			for (a=0;BASS_ChannelGetAttribute(chan,BASS_ATTRIB_MUSIC_VOL_CHAN+a,&dummy);a++);
		}
		printf("playing MOD music \"%s\" [%u chans, %u orders]",
			BASS_ChannelGetTags(chan,BASS_TAG_MUSIC_NAME),a,(DWORD)BASS_ChannelGetLength(chan,BASS_POS_MUSIC_ORDER));
		pos=BASS_ChannelGetLength(chan,BASS_POS_BYTE);
		ismod=TRUE;
	}

	// display the time length
	if (pos!=-1) {
		time=(DWORD)BASS_ChannelBytes2Seconds(chan,pos);
		printf(" %u:%02u\n",time/60,time%60);
	} else // no time length available
		printf("\n");

	BASS_ChannelPlay(chan,FALSE);

	while (!_kbhit() && (act=BASS_ChannelIsActive(chan))) {
		// display some stuff and wait a bit
		level=BASS_ChannelGetLevel(chan);
		pos=BASS_ChannelGetPosition(chan,BASS_POS_BYTE);
		time=BASS_ChannelBytes2Seconds(chan,pos);
#ifdef _WIN32
		printf("pos %09I64u",pos);
#else
		printf("pos %09llu",pos);
#endif
		if (ismod) {
			pos=BASS_ChannelGetPosition(chan,BASS_POS_MUSIC_ORDER);
			printf(" (%03u:%03u)",LOWORD(pos),HIWORD(pos));
		}
		printf(" - %u:%02u - L ",time/60,time%60);
		if (act==BASS_ACTIVE_STALLED) { // playback has stalled
			printf("-- buffering : %05u --",(DWORD)BASS_StreamGetFilePosition(chan,BASS_FILEPOS_BUFFER));
		} else {
			for (a=27204;a>200;a=a*2/3) putchar(LOWORD(level)>=a?'*':'-');
			putchar(' ');
			for (a=210;a<32768;a=a*3/2) putchar(HIWORD(level)>=a?'*':'-');
		}
		printf(" R - cpu %.2f%%  \r",BASS_GetCPU());
		fflush(stdout);
		Sleep(50);
	}
	printf("                                                                             \r");

	// wind the frequency down...
	BASS_ChannelSlideAttribute(chan,BASS_ATTRIB_FREQ,1000,500);
	Sleep(300);
	// ...and fade-out to avoid a "click"
	BASS_ChannelSlideAttribute(chan,BASS_ATTRIB_VOL,-1,200);
	// wait for slide to finish
	while (BASS_ChannelIsSliding(chan,0)) Sleep(1);

	BASS_Free();
}
