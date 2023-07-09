#define _g_

#pragma once

#include<graphics.h>
#include<cstdio>
#include<conio.h>
#include<windows.h>
#include<iostream>
#include<algorithm>
#include<utility>
using namespace std;

int crt[16][16] = {0}, stp = 0;

inline int inc(const int& x, const int& y){
	if(0 <= x && x < 16 && 0 <= y && y < 16)return crt[x][y];
	else return -1;
}

bool reach4(int *play1){
	//冲四---横纵、斜几个情况进行分析
	/*横向*/
	for (int i = 0; i < 16; i++){
		//判断当前行是否存在连续四个黑子，且该连续四个黑子的左侧或者右侧存在一个白子
		for (int j = 0; j < 16; j++){
			if (j <= 12 && inc(i, j) == 1){
				//判断是否会从j开始出现连续四个
				if (inc(i, j + 1) == 1 && inc(i, j + 2) == 1 && inc(i, j + 3) == 1){
					//判断该四连的左右是否有白子或者左右已经到达边界
					if (j + 3 == 15 && inc(i, j - 1) == 0){
						crt[i][j - 1] = 2;
						printf("%d %d W %d\n", i, j - 1, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j - 1) * 30, 20 + i * 30, 12);
						return true;//堵成功了
					}
					else if (j == 0 && inc(i, j + 4) == 0){
						crt[i][j + 4] = 2;
						printf("%d %d W %d\n", i, j + 4, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j + 4) * 30, 20 + i * 30, 12);
						return true;//堵成功了
					}
					else if (j < 12 && inc(i, j - 1) == 2 && inc(i, j + 4) == 0){
						crt[i][j + 4] = 2;
						printf("%d %d W %d\n", i, j - 1, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j + 4) * 30, 20 + i * 30, 12);
						return true;//堵成功了
					}
					else if (j > 0 && crt[i][j + 4] == 2 && crt[i][j - 1] == 0){
						crt[i][j - 1] = 2;
						printf("%d %d W %d\n", i, j - 1, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j - 1) * 30, 20 + i * 30, 12);
						return true;//堵成功了
					}
				}
			}
		}
	}
	/*纵向*/
	for (int i = 0; i < 16; i++){
		//判断当前行是否存在连续四个黑子，且该连续四个黑子的左侧或者右侧存在一个白子
		for (int j = 0; j < 16; j++){
			if (j <= 12 && crt[j][i] == 1){
				//判断是否会从j开始出现连续四个
				if (crt[j + 1][i] == 1 && crt[j + 2][i] == 1 && crt[j + 3][i] == 1){
					if (j == 0 && crt[j + 4][i] == 0){
						crt[j + 4][i] = 2;
						printf("%d %d W %d\n", j + 4, i, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + i * 30, 20 + (j + 4) * 30, 12);
						return true;//堵成功了
					}
					else if (j + 3 == 15 && crt[j - 1][i] == 0){
						crt[j - 1][i] = 2;
						printf("%d %d W %d\n", j - 1, i, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + i * 30, 20 + (j - 1) * 30, 12);
						return true;//堵成功了
					}
					else if (j > 0 && crt[j + 4][i] == 2 && crt[j - 1][i] == 0){
						crt[j - 1][i] = 2;
						printf("%d %d W %d\n", j - 1, i, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + i * 30, 20 + (j - 1) * 30, 12);
						return true;//堵成功了
					}
					else if (j < 12 && crt[j - 1][i] == 2 && crt[j + 4][i] == 0){
						crt[j + 4][i] = 2;
						printf("%d %d W %d\n", j + 4, i, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + i * 30, 20 + (j + 4) * 30, 12);
						return true;//堵成功了
					}
				}
			}
		}
	}
	/*左斜*/
	for (int i = 0; i < 13; i++){
		for (int j = 3; j < 16; j++){
			if (!((i == 0 && j == 3) || (i == 12 && j == 15))){
				if (crt[i][j] == 1){
					if (crt[i + 1][j - 1] == 1 && crt[i + 2][j - 2] == 1 && crt[i + 3][j - 3] == 1){
						if (i == 0 && crt[i + 4][j - 4] == 0){
							crt[i + 4][j - 4] = 2;
							printf("%d %d W %d\n", i + 4, j - 4, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j - 4) * 30, 20 + (i + 4) * 30, 12);
							return true;//堵成功了
						}
						else if (i == 12 && crt[i - 1][j + 1] == 0){
							crt[i - 1][j + 1] = 2;
							printf("%d %d W %d\n", i - 1, j + 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j + 1) * 30, 20 + (i - 1) * 30, 12);
							return true;//堵成功了
						}
						else if (j == 3 && crt[i - 1][j + 1] == 0){
							crt[i - 1][j + 1] = 2;
							printf("%d %d W %d\n", i - 1, j + 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j + 1) * 30, 20 + (i - 1) * 30, 12);
							return true;//堵成功了
						}
						else if (j == 15 && crt[i + 4][j - 4] == 0){
							crt[i + 4][j - 4] = 2;
							printf("%d %d W %d\n", i + 4, j - 4, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j - 4) * 30, 20 + (i + 4) * 30, 12);
							return true;//堵成功了
						}
						else if (i < 12 && crt[i - 1][j + 1] == 2 && crt[i + 4][j - 4] == 0){
							crt[i + 4][j - 4] = 2;
							printf("%d %d W %d\n", i + 4, j - 4, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j - 4) * 30, 20 + (i + 4) * 30, 12);
							return true;//堵成功了
						}
						else if (i > 0 && crt[i + 4][j - 4] == 2 && crt[i - 1][j + 1] == 0){
							crt[i - 1][j + 1] = 2;
							printf("%d %d W %d\n", i - 1, j + 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j + 1) * 30, 20 + (i - 1) * 30, 12);
							return true;//堵成功了
						}
					}
				}
			}
		}
	}
	/*右斜*/
	for (int i = 0; i < 13; i++){
		for (int j = 0; j < 13; j++){
			if (!(i == 0 && j == 12) || (i == 12 && j == 0)){
				if (crt[i][j] == 1){
					if (crt[i + 1][j + 1] == 1 && crt[i + 2][j + 2] == 1 && crt[i + 3][j + 3] == 1){
						if (i == 0 && crt[i + 4][j + 4] == 0){
							crt[i + 4][j + 4] = 2;
							printf("%d %d W %d\n", i + 4, j + 4, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j + 4) * 30, 20 + (i + 4) * 30, 12);
							return true;//堵成功了
						}
						else if (i == 12 && crt[i - 1][j - 1] == 0){
							crt[i - 1][j - 1] = 2;
							printf("%d %d W %d\n", i - 1, j - 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j - 1) * 30, 20 + (i - 1) * 30, 12);
							return true;//堵成功了
						}
						else if (j == 12 && crt[i - 1][j - 1] == 0){
							crt[i - 1][j - 1] = 2;
							printf("%d %d W %d\n", i - 1, j - 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j - 1) * 30, 20 + (i - 1) * 30, 12);
							return true;//堵成功了
						}
						else if (j == 0 && crt[i + 4][j + 4] == 0){
							crt[i + 4][j + 4] = 2;
							printf("%d %d W %d\n", i + 4, j + 4, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j + 4) * 30, 20 + (i + 4) * 30, 12);
							return true;//堵成功了
						}
						else if (j < 12 && crt[i - 1][j - 1] == 2 && crt[i + 4][j + 4] == 0){
							crt[i + 4][j + 4] = 2;
							printf("%d %d W %d\n", i + 4, j + 4, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j + 4) * 30, 20 + (i + 4) * 30, 12);
							return true;//堵成功了
						}
						else if (i > 0 && inc(i + 4, j + 4) == 2 && crt[i - 1][j - 1] == 0){
							crt[i - 1][j - 1] = 2;
							printf("%d %d W %d\n", i - 1, j - 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j - 1) * 30, 20 + (i - 1) * 30, 12);
							return true;//堵成功了
						}
					}
				}
			}
		}
	}
	return false;
}

//活三
bool hsk_3(int* play1) {	//三连两边都有空格
	/*横向*/
	for (int i = 0; i < 16; i++){
		for (int j = 1; j < 13; j++){
			if (crt[i][j] == 1){
				if (crt[i][j + 1] == 1 && crt[i][j + 2] == 1){
					if (crt[i][j - 1] == 0 && crt[i][j + 3] == 0){
						if (j == 1 || j != 12){
							//必然堵右边
							crt[i][j + 3] = 2;
							printf("%d %d W %d\n", i, j + 3, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j + 3) * 30, 20 + i * 30, 12);
							return true;//堵成功了
						}
						if (j == 12){
							//必然堵左边
							crt[i][j - 1] = 2;
							printf("%d %d W %d\n", i, j - 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j - 1) * 30, 20 + i * 30, 12);
							return true;//堵成功了
						}
					}
				}
			}
		}
	}
	/*纵向*/
	for (int j = 0; j < 16; j++){
		for (int i = 1; i < 13; i++){
			if (crt[i][j] == 1){
				if (crt[i + 1][j] == 1 && crt[i + 2][j] == 1){
					if (crt[i - 1][j] == 0 && crt[i + 3][j] == 0){
						if (i == 1 || i != 12){
							crt[i + 3][j] = 2;
							printf("%d %d W %d\n", i + 3, j, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + j * 30, 20 + (i+3) * 30, 12);
							return true;//堵成功了
						}
						else if (i == 12){
							crt[i - 1][j] = 2;
							printf("%d %d W %d\n", i - 1, j, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + j * 30, 20 + (i-1) * 30, 12);
							return true;//堵成功了
						}
					}
				}
			}
		}
	}
	/*左斜*/
	for (int i = 1; i < 13; i++){
		for (int j = 3; j < 15; j++){
			if (crt[i][j] == 1){
				if (crt[i + 1][j - 1] == 1 && crt[i + 2][j - 2] == 1){
					if (crt[i - 1][j + 1] == 0 && crt[i + 3][j - 3] == 0){
						if (i == 12){
							//必然堵上边
							crt[i - 1][j + 1] = 2;
							printf("%d %d W %d\n", i - 1, j + 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j+1) * 30, 20 + (i - 1) * 30, 12);
							return true;//堵成功了
						}
						else{
							//堵下边
							crt[i + 3][j - 3] = 2;
							printf("%d %d W %d\n", i + 3, j - 3, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j-3) * 30, 20 + (i + 3) * 30, 12);
							return true;//堵成功了
						}
					}
				}
			}
		}
	}
	/*右斜*/
	for (int i = 1; i < 13; i++){
		for (int j = 1; j < 13; j++){
			if (crt[i][j] == 1){
				if (crt[i + 1][j + 1] == 1 && crt[i + 2][j + 2] == 1){
					if (crt[i + 3][j + 3] == 0 && crt[i - 1][j - 1] == 0){
						if (i == 1){
							//堵下边
							crt[i + 3][j + 3] = 2;
							printf("%d %d W %d\n", i + 3, j + 3, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j + 3) * 30, 20 + (i + 3) * 30, 12);
							return true;//堵成功了
						}
						else{
							//堵上边
							crt[i - 1][j - 1] = 2;
							printf("%d %d W %d\n", i - 1, j - 1, ++stp);
							*play1 = 1;
							setfillcolor(WHITE);
							solidcircle(20 + (j - 1) * 30, 20 + (i - 1) * 30, 12);
							return true;//堵成功了
						}
					}
				}
			}
		}
	}
	//两连空一个在有一个且两边有空格
	/*横向*/
	for (int i = 0; i < 16; i++){
		for (int j = 1; j < 12; j++){
			if (crt[i][j] == 1){
				if (crt[i][j + 1] == 1 && crt[i][j + 2] == 0 && crt[i][j + 3] == 1){
					if (crt[i][j - 1] == 0 && crt[i][j + 4] == 0){
						crt[i][j + 2] = 2;
						printf("%d %d W %d\n", i, j + 2, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j + 2) * 30, 20 + i * 30, 12);
						return true;//堵成功了
					}
				}
				else if (crt[i][j + 1] == 0 && crt[i][j + 2] == 1 && crt[i][j + 3] == 1){
					if (crt[i][j - 1] == 0 && crt[i][j + 4] == 0){
						crt[i][j + 1] = 2;
						printf("%d %d W %d\n", i, j + 1, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j + 1) * 30, 20 + i * 30, 12);
						return true;//堵成功了
					}
				}
			}
		}
	}
	/*纵向*/
	for (int j = 0; j < 16; j++){
		for (int i = 1; i < 12; i++){
			if (crt[i][j] == 1){
				if (crt[i + 1][j] == 1 && crt[i + 2][j] == 0 && crt[i + 3][j] == 1){
					if (crt[i - 1][j] == 0 && crt[i + 4][j] == 0){
						crt[i + 2][j] = 2;
						printf("%d %d W %d\n", i + 2, j, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + j * 30, 20 + (i+2) * 30, 12);
						return true;//堵成功了
					}
				}
				else if (crt[i + 1][j] == 0 && crt[i + 2][j] == 1 && crt[i + 3][j] == 1){
					if (crt[i - 1][j] == 0 && crt[i + 4][j] == 0){
						crt[i + 1][j] = 2;
						printf("%d %d W %d\n", i + 1, j, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + j * 30, 20 + (i+1) * 30, 12);
						return true;//堵成功了
					}
				}
			}
		}
	}
	/*左斜*/
	for (int i = 1; i < 12; i++){
		for (int j = 4; j < 15; j++){
			if (crt[i][j] == 1){
				if (crt[i + 1][j - 1] == 0 && crt[i + 2][j - 2] == 1 && crt[i + 3][j - 3] == 1){
					if (crt[i - 1][j + 1] == 0 && crt[i + 4][j - 4] == 0){
						crt[i + 1][j - 1] = 2;
						printf("%d %d W %d\n", i + 1, j - 1, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j-1) * 30, 20 + (i + 1) * 30, 12);
						return true;//堵成功了
					}
				}
				else if (crt[i + 1][j - 1] == 1 && crt[i + 2][j - 2] == 0 && crt[i + 3][j - 3] == 1){
					if (crt[i - 1][j + 1] == 0 && crt[i + 4][j - 4] == 0){
						crt[i + 2][j - 2] = 2;
						printf("%d %d W %d\n", i + 2, j - 2, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j-2) * 30, 20 + (i + 2) * 30, 12);
						return true;//堵成功了
					}
				}
			}
		}
	}
	/*右斜*/
	for (int i = 1; i < 12; i++){
		for (int j = 1; j < 12; j++){
			if (crt[i][j] == 1){
				if (crt[i + 1][j + 1] == 0 && crt[i + 2][j + 2] == 1 && crt[i + 3][j + 3] == 1){
					if (crt[i - 1][j - 1] == 0 && crt[i + 4][j + 4] == 0){
						crt[i + 1][j + 1] = 2;
						printf("%d %d W %d\n", i + 1, j + 1, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j + 1) * 30, 20 + (i + 1) * 30, 12);
						return true;//堵成功了
					}
				}
				else if (crt[i + 1][j + 1] == 1 && crt[i + 2][j + 2] == 0 && crt[i + 3][j + 3] == 1){
					if (crt[i - 1][j - 1] == 0 && crt[i + 4][j + 4] == 0){
						crt[i + 2][j + 2] = 2;
						printf("%d %d W %d\n", i + 2, j + 2, ++stp);
						*play1 = 1;
						setfillcolor(WHITE);
						solidcircle(20 + (j + 2) * 30, 20 + (i + 2) * 30, 12);
						return true;//堵成功了
					}
				}
			}
		}
	}
	return false;
}

#include<wchar.h>
#include<locale.h>
