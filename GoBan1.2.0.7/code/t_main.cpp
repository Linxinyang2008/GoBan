#define _CRT_SECURE_NO_WARNINGS 1
#include"GoBan.h"
#include<windows.h>
#include<cstring>

bool showwindow = 1, save = 0, disable = 1, help = 0;
char* ch = NULL;
string opt;

int main(int argc, char** argv){
	setlocale(LC_ALL, "Chinese");
	system("color f0");
	wprintf(L"程序主窗口，请勿关闭\n");
	freopen(".stderr", "w+", stderr);
	system("mode con cols=15 lines=3");
	ios::sync_with_stdio(0);
	
	name0 = argv[0];
	fprintf(stderr, "Argument value** \\argv :\n");
    for ( int i = 0; i < argc; ++i ){
        fprintf(stderr, "\t%d : %s \n", i, argv[ i ] );
		opt = argv[i];
		switch(opt[1]){
			case 'c':
				showwindow = 0;
				break;
			case 'e':
				err_name = argv[i + 1];++i;
				break;
			case 'o':
				out_name = argv[i + 1];++i;
				break;
			case 'h':
			case '?':
				help = 1;
				break;
			case 'd':
				disable = 0;
				break;
		}
    }
    
    if(help){
		helper();
		return kill();
	}

		if(!showwindow)ShowWindow(GetForegroundWindow(), 0);
		if(save)freopen(ch, "w", stdout);
	
	memset(crt, 0, sizeof crt);
	if(err_name != NULL)freopen(err_name, "w", stderr);
	
	Menu m;	m.Display();
	
	return 0;
}
/*
 commands:
 	-c            Hide console Windows;
 	-out [Direct] Save chess manual;
 	-err [Direct] Debug Information; (Default ".stderr")
 	-h            Help?
 	-d            Enable \\envp displaying
*/ 
