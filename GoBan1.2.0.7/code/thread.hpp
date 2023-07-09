#include <windows.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <mmsystem.h>
//#pragma comment (lib, "winmm.lib")

using namespace std;

bool music = 0;

void* Music(void *arg){
	fprintf(stderr, "Thread created.\n");
	if (0 == PlaySound("./src/1.wav", GetModuleHandle(NULL), SND_FILENAME)){
		MessageBox(NULL,"Background Music Player denied.", "", MB_ICONERROR|MB_OK);
	}
	if(music)return NULL;
	if (0 == PlaySound("./src/background.wav", GetModuleHandle(NULL), SND_FILENAME | SND_LOOP)){
		MessageBox(NULL,"Background Music Player denied.", "", MB_ICONERROR|MB_OK);
	}	
	return NULL;
}

void thread(){
	pthread_t tid;
	if (pthread_create(&tid, NULL, Music, NULL) != 0){
	    fprintf(stderr, "create thread error( %d ) = %s\n", errno, strerror(errno));
		MessageBox(NULL,"Background Music Player denied.", "", MB_ICONERROR|MB_OK);
	}
	return ;
}

char *name0 = NULL, *err_name = NULL, *out_name = NULL;//自身进程名，stderr输出文件， stdout输出文件

int kill(){
	fprintf(stderr, "end: = /> taskkill == %s\n", name0);
	char cmd[64] = {0};
	sprintf(cmd, "taskkill -im %s -f", name0);
	system(cmd);
	
	system("taskkill /PID 21028");
    exit(0);
    return 0;
}

void helper(void){
	initgraph(712, 469, SHOWCONSOLE);
	IMAGE img;
	loadimage(&img, "./src/copyright.jpg", 712, 469);
	putimage(0, 0, &img);
	MessageBox(NULL, "Core Algorithm Developed by Linxinyang2008\nSource Code: ** ", "About </>", MB_OK);
	exit(0);
//	ShowWindow(GetForegroundWindow(), 0);
}