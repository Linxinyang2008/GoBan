#include"GoBan.h"
#include<windows.h>
#include<cstring>

int main(int argc, char** argv){
	setlocale(LC_ALL, "Chinese");
	system("color f0");
	wprintf(L"程序主窗口，请勿关闭\n");
//	freopen(".stderr", "w+", stderr);
//	system("mode con cols=15 lines=3");
	ios::sync_with_stdio(0);
	
	Menu m;	m.Display();
	
	return 0;
}