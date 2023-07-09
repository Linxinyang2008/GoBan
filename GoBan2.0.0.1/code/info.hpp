#include <windows.h>
#include <graphics.h>

void helper(void){
	initgraph(712, 469, SHOWCONSOLE);
	IMAGE img;
	loadimage(&img, "./src/copyright.jpg", 712, 469);
	putimage(0, 0, &img);
	MessageBox(NULL, "Core Algorithm Developed by Linxinyang2008\nSource Code: ** ", "About </>", MB_OK);
	exit(0);

}

/*un- #define BASIC_ALGORITHM_AVAILABLE -> UNAVAILABLE*/
#define WEIGHT_UNAVAILABLE 
/*un- #define MCTS_AVAILABLE -> AVAILABLE*/