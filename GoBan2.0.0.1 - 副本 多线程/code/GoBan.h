//White 2 
//Black 1
//Empty 0
//Beyond -1

#include"va_algo.hpp"
#include"info.hpp"

#ifndef MCTS_UNAVAILABLE 
	#define MCTS_AVAILABLE mcts_v03
	#include"mcts.hpp"
#endif

#ifndef WEIGHT_UNAVAILABLE 
	#define WEIGHT_AVAILABLE weight_v19
	#include"spj.hpp"
#endif

#ifdef _g_
using namespace std; 

pair<int, int> p, black[151];
int step;

bool block(int* play1){
	return reach4(play1) || hsk_3(play1);
}
 
bool va_find(int* play1){
	fprintf(stderr, "Enter va_find() with virable *(int)play1 = %d\n", *play1);
	p.first = p.second = 0;
	
	
#ifdef MCTS_AVAILABLE
	p = mcts_main(crt);
	if(p.first != 999 && p.second != 999){
		crt[p.first][p.second] = 2;
		*play1 = 1;
		printf("%d %d W %d\n", p.first, p.second, ++stp);
		setfillcolor(WHITE);
		solidcircle(20 + p.second * 30, 20 + p.first * 30, 12);		
		return true;
	}
#endif	
	
#ifdef SPJ_AVAILABLE
	if(spj(p)){//特判，详见spj.hpp
		crt[p.first][p.second] = 2;
		*play1 = 1;
		printf("%d %d W %d\n", p.first, p.second, ++stp);
		setfillcolor(WHITE);
		solidcircle(20 + p.second * 30, 20 + p.first * 30, 12);		
		return true;
	}
#endif
	
#ifdef BASIC_ALGORITHM_AVAILABLE
	//思路：找到自己的棋子，连的最多的且可以组成五个的
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			if (crt[i][j] == 2){
				/*横向*/
				if (*play1 == 0){
					if (j < 12 && (i == 0 || crt[i - 1][j] == 1))
						//只能向右判断
						if (inc(i, j + 1) != 1 && inc(i, j + 2) != 1 && crt[i ][j+3] != 1 && crt[i][j+4] != 1){
							for (int k = j + 1; k <= j + 4; k++)
								if (crt[i][k] == 0){
									crt[i][k] = 2;
									printf("%d %d W %d\n", i, k, ++stp);
									*play1 = 1;
									setfillcolor(WHITE);
									solidcircle(20 + k * 30, 20 + i * 30, 12);
									return true;//找成功了
								}
						}
						else ;
					else if ((j > 3) && (i == 15 || crt[i+1][j] == 1)){
						//只能向左判断
						if (crt[i][j - 1] != 1 && crt[i][j - 2] != 1 && crt[i][j - 3] != 1 && crt[i][j - 4] != 1){
							for (int k = j - 1; k >= j - 4; k--){
								if (crt[i][k] == 0){
									printf("%d %d W %d\n", i, k, ++stp);
									crt[i][k] = 2;
									*play1 = 1;
									setfillcolor(WHITE);
									solidcircle(20 + k * 30, 20 + i * 30, 12);
									return true;//找成功了
								}
							}
						}
					}
				}
			}
		}
	}
#endif 

	return false;
}

class Menu{
public:
	void Display(){
//		thread();
		
		//初始化绘图窗口
		initgraph(416, 624, SHOWCONSOLE);
 
		/*设置背景图*/
		IMAGE img;
		//缩放因子，例如设置宽度为100的单元格，实际的绘制宽度为（100*缩放因子）
		setaspectratio(1.1, 1);
		//从图片文件获取图像（图像的image指针，图像名，资源名称，图片的拉伸宽度、高度，是否自适应图片大小）
		loadimage(&img, "./src/begin.jpg", 377, 624, 1);
		putimage(0, 0, &img);
 
		/*控制鼠标移动操作*/
		MOUSEMSG m;//鼠标操作
		while (true){
			m = GetMouseMsg();//获取鼠标消息
			//左键按下：WM_LBUTTONDOWN
			if (m.uMsg == WM_LBUTTONDOWN && (
						(m.x >= 72 && m.x <= 307 && m.y >= 340 && m.y <= 400) || 
						(m.x >= 72 && m.x <= 307 && m.y >= 420 && m.y <= 480) || 
						(m.x >= 295 && m.x <= 347 && m.y >= 35 && m.y >= 87) 
				)	){
				//uMsg鼠标信息  WM_MOUSEMOVE鼠标移动消息  x y表示鼠标位置坐标
				//当鼠标在"人机对战、双人对战"上时，显示红色边框
				if (m.x >= 72 && m.x <= 307 && m.y >= 340 && m.y <= 400){
					setlinecolor(YELLOW);
					setlinestyle(PS_SOLID | PS_JOIN_ROUND, 2);
					//空心矩形框
					rectangle(72, 340, 300, 400);
				}
				else if (m.x >= 72 && m.x <= 307 && m.y >= 420 && m.y <= 480){
					setlinecolor(YELLOW);
					//空心矩形框
					rectangle(72, 420, 300, 480);
				}
				else if(m.x >= 295 && m.x <= 347 && m.y >= 35 && m.y >= 87){
					setlinecolor(BLUE);
					circle(321, 61, 26);
					helper();
				}
				
				Sleep(500);
				//清除屏幕内容
				cleardevice();
				//休眠五秒
				Sleep(300);
				//关闭窗口
				closegraph();
				//使用匿名对象打开棋盘界面
				Menu().ChessBoard(m);
				break;
			}
		}
	}
	
	void ChessBoard(MOUSEMSG m);
};
 
class Play{
public:
	void TwoPlayerGame(MOUSEMSG m);
	void ComputerUserGame(MOUSEMSG m);
	
	//判断是否点击右侧工具栏
	void buttonRingth(MOUSEMSG m, MOUSEMSG ms, int win){
	
		if (ms.x >= 500 && ms.x <= 655 && ms.y >= 30 && ms.y <= 80){
			memset(crt, 0, sizeof(crt));
			//重新开始
			setlinecolor(RED);
			//空心矩形框
			rectangle(500, 30, 655, 80);
			Sleep(300);
			Menu().ChessBoard(m);
		}
		else if (ms.x >= 500 && ms.x <= 655 && ms.y >= 115 && ms.y <= 165){
			memset(crt, 0, sizeof(crt));
			//返回菜单
			setlinecolor(RED);
			//空心矩形框
			rectangle(500, 115, 655, 165);
			Sleep(300);
			Menu().Display();
		}
		else if (win == 0 && ms.x >= 500 && ms.x <= 655 && ms.y >= 200 && ms.y <= 250){
			//悔棋
			setlinecolor(RED);
			//空心矩形框
			rectangle(500, 200, 655, 250);
			MessageBoxW(NULL, L"落子无悔！", L"提示", MB_OK);
		}
	}
	
	//显示哪一方赢了,n1为0表示双人为1表示人机，n2为0表示黑、人为1表示白、机
	void displayWin(int n1, int n2){
		memset(crt, 0, sizeof(crt));
		//显示哪一方赢了,n1为0表示双人为1表示人机，n2为0表示黑、人为1表示白、机
		IMAGE img;
		// 读取图片至绘图窗口
		if (n1 == 0 && n2 == 0)
			loadimage(&img, "./src/blackWin.jpg",306,141);
		if (n1 == 0 && n2 == 1)
			loadimage(&img, "./src/whiteWin.jpg", 306, 141);
		if (n1 == 1 && n2 == 0)
			loadimage(&img, "./src/youWin.jpg", 306, 141);
		if (n1 == 1 && n2 == 1)
			loadimage(&img, "./src/computerWin.jpg", 306, 141);
		putimage(100, 200, &img);
 
		MOUSEMSG m;//鼠标操作
		while (1){
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 215 && m.x <= 270 && m.y >= 285 && m.y <= 320){
				setlinecolor(YELLOW);
				//空心矩形框
				rectangle(215, 285, 270, 320);
				Sleep(300);
				Menu().Display();
				break;
			}
			else if (m.uMsg == WM_LBUTTONDOWN){
				ShowWindow(GetForegroundWindow(), 1);
				//kill();
				return ;
			}
				
		}
	}
	
	void PlayGame(MOUSEMSG ms, int* play1, int* play2, bool* l);
	int Win();
};


void Menu::ChessBoard(MOUSEMSG m){
		//初始化绘图窗口
		initgraph(665, 490, SHOWCONSOLE);
///		if(out_name != NULL)freopen(out_name, "w", stdout);
 
		/*设置棋盘背景背景图*/
		IMAGE img;
		//缩放因子，例如设置宽度为100的单元格，实际的绘制宽度为（100*缩放因子）
		//setaspectratio(1.1, 1);
		//从图片文件获取图像（图像的image指针，图像名，资源名称，图片的拉伸宽度、高度，是否自适应图片大小）
		loadimage(&img, "./src/chessBoard.jpg", 665,490);
		putimage(0, 0, &img);
 
		//绘制棋盘
		while (true){
			for (int i = 20; i <= 470; i += 30){
				setlinecolor(WHITE);
				line(20,i,470,i);
				line(i,20,i,470);
			}
			//如果左键双人，跳入双人游戏
			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 72 && m.x <= 307 && m.y >= 420 && m.y <= 480)
				Play().TwoPlayerGame(m);
			else		
				Play().ComputerUserGame(m);
		}
}

void Play::ComputerUserGame(MOUSEMSG m){
	int win = 0;
	int play1 = 1, play2 = 0;//play1表示玩家，play2表示电脑，每次玩家先落子
 
	MOUSEMSG ms;
	//一直获取鼠标信息，判断操做
	bool looper = true;
	while (win == 0 && looper){
		//判断是否点击右侧工具栏或者棋盘
		ms = GetMouseMsg();
		if (ms.uMsg == WM_LBUTTONDOWN){
			//判断是否点击右侧工具栏
			buttonRingth(m, ms, win);
			//判断是否点击棋盘并且判断是否该玩家落子
			PlayGame(ms, &play1, &play2, &looper);
			//判断玩家是否赢
		}
	}
}


void Play::TwoPlayerGame(MOUSEMSG m){
	int win = 0;
	int play1 = 1;//, play2 = 0;
	MOUSEMSG ms;
	//一直获取鼠标信息，判断操作
	while (win == 0){
		//判断是否点击右侧工具栏或者棋盘
		ms = GetMouseMsg();
		if (ms.uMsg == WM_LBUTTONDOWN){
			//判断是否点击右侧工具栏
			buttonRingth(m, ms, win);
			//判断是否点击棋盘
			for (int lie = 20; lie <= 490; lie += 30){
				if (ms.x <= lie + 15 && ms.x >= lie - 15){
					for (int hang = 20; hang <= 490; hang += 30){
						if (ms.y <= hang + 15 && ms.y >= hang - 15){
							if (play1 == 1 && crt[hang / 30 - 1][lie / 30 - 1] == 0){
								setfillcolor(BLACK);
								solidcircle(lie, hang, 12);
								crt[hang / 30 - 1][lie / 30 - 1] = 1;
								play1 = 0;
								break;
							}
							if (play1 == 0 && crt[hang / 30 - 1][lie / 30 - 1] == 0){
								setfillcolor(WHITE);
								solidcircle(lie, hang, 12);
								crt[hang / 30 - 1][lie / 30 - 1] = 2;
								play1 = 1;
								break;
							}
						}
					}
				}
			}
			//判断玩家是否赢
			win = Play().Win();
			if (win == 1){
				//黑棋赢
				displayWin(0, 0);
				break;
			}
			else if (win == 2){
				//白棋赢
				displayWin(0, 1);
				break;
			}
		}
	}
}


int Play::Win(){
	int win = 0;
	//判断是否赢
	for (int j = 0; j<16 && (win == 0); j++){
		for (int i = 0; i<16; i++){
 
			if ((crt[j][i] == 1 && crt[j][i + 1] == 1 && crt[j][i + 2] == 1 && crt[j][i + 3] == 1 && crt[j][i + 4] == 1)
				|| (crt[i][j] == 1 && crt[i + 1][j] == 1 && crt[i + 2][j] == 1 && crt[i + 3][j] == 1 && crt[i + 4][j] == 1))//横纵是5个子play1 win
{
				win = 1;
				Sleep(100);
				break;
			}
			if ((crt[j][i] == 2 && crt[j][i + 1] == 2 && crt[j][i + 2] == 2 && crt[j][i + 3] == 2 && crt[j][i + 4] == 2)
				|| (crt[i][j] == 2 && crt[i + 1][j] == 2 && crt[i + 2][j] == 2 && crt[i + 3][j] == 2 && crt[i + 4][j] == 2))//横纵是5个子play2 win
{
				win = 2;
				Sleep(100);
				break;
			}
		}
	}
	for (int j = 0; j<12 && (win == 0); j++){
		for (int i = 0; i<12; i++){
			if (crt[j][i] == 1 && crt[j + 1][i + 1] == 1 && crt[j + 2][i + 2] == 1 && crt[j + 3][i + 3] == 1 && crt[j + 4][i + 4] == 1)//向右倾斜时候play1 win
{
				win = 1;
				Sleep(100);
				break;
 
			}
			if (crt[j][i] == 2 && crt[j + 1][i + 1] == 2 && crt[j + 2][i + 2] == 2 && crt[j + 3][i + 3] == 2 && crt[j + 4][i + 4] == 2)//向右倾斜时候play2 win
{
				win = 2;
				Sleep(100);
				break;
			}
		}
		for (int i = 4; i<16 && (win == 0); i++) {
			if (crt[j][i] == 1 && crt[j + 1][i - 1] == 1 && crt[j + 2][i - 2] == 1 && crt[j + 3][i - 3] == 1 && crt[j + 4][i - 4] == 1)//向左倾斜时候play1 win
{
				win = 1;
				Sleep(100);
				break;
			}
			if (crt[j][i] == 2 && crt[j + 1][i - 1] == 2 && crt[j + 2][i - 2] == 2 && crt[j + 3][i - 3] == 2 && crt[j + 4][i - 4] == 2)//向左倾斜时候play2 win
{
				win = 2;
				Sleep(100);
				break;
			}
		}
	}
	return win;
}

void Play::PlayGame(MOUSEMSG ms, int* play1, int* play2, bool* l){
	//人走
	int success = 1;
//	int vx = 0, int vy = 0;
	while (success){
		ms = GetMouseMsg();
		for (int lie = 20; lie <= 490; lie += 30){
			if (ms.x <= lie + 15 && ms.x >= lie - 15){
				for (int hang = 20; hang <= 490; hang += 30){
					if (ms.y <= hang + 15 && ms.y >= hang - 15 && *play1 == 1 && crt[hang / 30][lie / 30] == 0){
						setfillcolor(BLACK);
						solidcircle(lie, hang, 12);
							
						crt[hang / 30][lie / 30] = 1;
						black[++step] = make_pair(hang / 30, lie / 30);
						cout << hang / 30 << ' ' << lie / 30 << " B " << ++stp << endl;
//						vx = hang / 30, vy = lie / 30;
							
						*play1 = 0;
						success = 0;
						break;
					}
				}
			}
		}
	}
	
	int win = Play().Win();
	if (win == 1){//人赢
		displayWin(1, 0);
		*l = false;
		return ;
	}
	else if (win == 2){//电脑赢
		displayWin(1, 1);
		*l = false;
		return ;
	}
	
	//电脑走
	/*思路：遍历棋盘查找对方是否存在成功的可能，如果有堵住对方；如果没有，找自己即将能成功的位置落子*/
	if (block(play1) == false){
		//找自己可以成的位置，如果没有随机落子
		if (va_find(play1) == false){//如果va_find函数无法判定的话，直接判人赢，因为游戏已经没必要进行下去了
			//MessageBox(NULL, "Algorithm Error\nComputer ", "Warning", MB_OK|MB_ICONERROR);
			MessageBoxW(NULL, L"作者代码技术太菜，竟然没想到有人能让程序弹出这个框框。\n如果您愿意将棋谱通过Github Issue分享给作者, 帮助作者进步就更好了，非常感谢！\n", L"和棋", MB_OK);
			displayWin(1, 0);
			*l = false;
			return ;
		}
	}
	
	win = Play().Win();
	if (win == 1){//人赢
		displayWin(1, 0);
		*l = false;
		return ;
	}
	else if (win == 2){	//电脑赢
		displayWin(1, 1);
		*l = false;
		return ;
	}
	return ;
}
#endif