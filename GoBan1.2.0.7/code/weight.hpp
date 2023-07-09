#include"va_algo.hpp"
#include<queue>
#include<limits.h>
#include"random_int.h"
#include<time.h>
using namespace std;

/*
策略思路：
主要是特判与动态规划相结合，在多状态下多情况下做出尽可能优的选择。

通过在dp[][]上添加权值，达到哈希式规划，但总规则不变，就是先考虑防守再考虑进攻。

优先级：
如下。注|左侧字符代表遍历中心，x为己方棋子，o为对方棋子
*/

pair<int, int> p, black[151];
typedef pair<int, int> PII;

queue<PII> bk_dis;

int dp[16][16], dis[16][16], step;

//方向数组
const int dirx[9] = {0, -1, -1, 0, 1, 1, 1, 0, -1};
const int diry[9] = {0, 0, 1, 1, 1, 0, -1, -1, -1};

//权值单位
const int A = 1057, B = 500, C = 400, D = 180;
const int Y = 31, Z = 37;

//匹配模式串
const int pst[][16] = { {0},//注：权值不能为负
	{0, 2, 2, 2, 2, -1},//A*3
	
	{0, 2, 2, 2, 0, -1},//B+C+Y
	{0, 0, 2, 2, 2, 0, 0, -1},//B
	//己方活三扫两次，因为形成跳四连四都是可以的，同时避免被边界横挡
	//但是往往连四是最好的
	
	{0, 2, 2, 0, 2, 0, -1},//A p=2
	{1, 2, 2, 2, 2, 0, -1},//A*2 + 0x3f3f
	{1, 2, 2, 2, 0, -1},//B-Y
	{1, 2, 2, 0, 2, -1},//B p=2
	{1, 0, 2, 2, 2, 0, -1},//B+Y
	{0, 2, 2, 2, 2, 1, -1},//A*2
	{0, 2, 0, 2, 2, 0, -1},//A
	{0, 1, 1, 1, 0, -1, -1},//B+Y+Z
	{1, 2, 2, 2, 2, 0, -1, -1},//A*3
	{1, 1, 0, 1, 1, -1, -1},//A
	
	{-1, -1, -1, -1, -1, -1, -1, -1},//-1
	{-1}//-1
};

//模式串的权值,应当与上面的模式串注释中标记的权值相匹配
const int psw[] = {0, A*2+B+0x9f, B+Y+C, B, A, 2*A + 0x3f3f, B-Y, B, B+Y, 2*A, A, B+Y+Z, A*3, A*2, /*-15, -7, -20, -Y,*/-1, -1};
//
//struct cv{
//	int cx, cy;
//	int cons[3][3]
//}
/*
dp[][] 更新规则：
	先预处理dis[][]表示每个空格到crt[][] == 1的最小曼哈顿距离
	设wt表示八连通格子内白棋的个数，wb表示八连通格子内黑棋的个数,则
		dp = (16 - dis) * (Y - 5 * dis) + (wt * Z)/2 + [0 .. 10]~rand() - 是否在边界上? max(wb - 3, 0) * D : 0;
	//rand()使得dp[][]中每个元素尽量不同且不影响全局最大值位置
*/
inline void init_dp(void){
	memset(dp, 0, sizeof dp);
	memset(dis, -1, sizeof dis);
	while (!bk_dis.empty()) bk_dis.pop();
	
	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j){
			int wt1 = 0, wb = 0;
			for(int k = 1; k < 9; ++k)
				if(inc(i + dirx[k], j + diry[k]) == 2)wt1++;
				else if(inc(i + dirx[k], j + diry[k]) == 1)wb++;
			dp[i][j] += (wt1 * Z >> 1);//wt[i][j] = wt1;
			if((i == 0 || j == 0 || i == 15 || j == 15) && wb >= 3)//边界上会被堵住
				dp[i][i] -= (wb - 3) * D;
			
			dp[i][j] += random_number(0, 10);
		}
	
	//预处理dis[][]
	for(int i = 1; i <= step; ++i)bk_dis.push(black[i]);
	
	while(!bk_dis.empty()){
		PII u = bk_dis.front();
		bk_dis.pop();
		for(int k = 0; k < 9; ++k){
			int dx = u.first + dirx[k], dy = u.second + diry[k];
			if(inc(dx, dy) != -1 && dis[dx][dy] == -1){
				dis[dx][dy] = dis[u.first][u.second] + 1;
				bk_dis.push(make_pair(dx, dy) );
			}
		}
	}
	
	//总体更新
	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)
			if(crt[i][j] == 0)dp[i][j] += (random_number(13, 16) - dis[i][j]) * (Y - 5 * dis[i][j]);
			else dp[i][j] = INT_MIN;
	
	return ;
}//Accepted

#include<vector>
using std::vector;
	vector<int> pos;
	
bool walk(const int& x, const int& y, const int *pat, int weight){
	int l = 0;pos.clear();bool b = 1, ld = 0;
	
	for(int k = 1; k < 9; ++k){//1
		b = 1;
		for(int i = 0; i < 9; ++i){//2
			if(pat[i] == 0){//统计匹配串中可落子位置，方便加权
				pos.push_back(i);
				++l;
			}
			if(pat[i] == -1){
				ld = 1;
				break;//匹配成功，pat结束了
			}
			if(inc(x + i*dirx[k], y + i*diry[k]) != pat[i] || inc(x + i*dirx[k], y + i*diry[k]) == -1){
				l = 0; break;//由于越界或棋盘在此位置无法适配pat
			}
		}
		for(int i = 1; i <= l && b && inc(x + pos[i]*dirx[k] ,y + pos[i]*diry[k]) != -1; ++i){
			dp[x + pos[i]*dirx[k]][y + pos[i]*diry[k]] += weight;
		}
	}
	return ld;
}//Accepted

bool spj(PII& ans, int dp_max = INT_MIN) {
	init_dp();bool kl = false;
	
	fprintf(stderr, "init_dp() exited successfully.\n");
	
	for(int i = 0; i < 16; ++i){
		for(int j = 0; j < 16; ++j){
			for(int k = 1; psw[k] != -1; ++k)
				kl |= walk(i, j, pst[k], psw[k]);
			}
	}

		fprintf(stderr, "All walk()s exited successfully.\n");

	for(int i = 0; i < 16; ++i){
		for(int j = 0; j < 16; ++j)
			if(dp[i][j] > dp_max){
				dp_max = dp[i][j];
				ans = make_pair(i, j);	
			}		
	}		
		fprintf(stderr, "dp[][] summery finished successfully.\n");

	fprintf(stderr, "spj() exited successfully.\n");
	return kl;
}//Accepted