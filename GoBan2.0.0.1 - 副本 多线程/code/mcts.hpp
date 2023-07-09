#define select_num 570        //选择次数
#define sta_num 20            //每个状态模拟次数

#include<iostream>
#include<map>
#include<cmath>
#include<vector>
#include<utility>
#include"random_int.h"
using std::max, std::min, std::make_pair; 

const int row = 15, col = 15;//棋盘的行和列
const int search_range = 6;//搜索范围 
 
const double inf = 1e9 + 7;

typedef struct chess{//定义棋盘 
	int g[16][16];
	void clear(){	memset(g, 0, sizeof g);	}
}chess;

//0代表无棋子，1代表白棋，2代表黑棋 //reverse it!!
 
bool operator < (const chess& x, const chess& y){//用于搜索棋盘状态 
	for(int i = 0;i <= row; i++){
		for(int j = 0;j <= col; j++){
			if(x.g[i][j] < y.g[i][j])return 1;
			else if(x.g[i][j] > y.g[i][j])return 0;
		}
	}
	return 0;
}

int rd = 0;//回合数

typedef struct property{//棋盘状态的一些性质 
	double a,b;
	std::vector<chess> vec;
}property; 
 
std::map<chess, property> mp;//用于记录棋盘状态的一些性质 
 
std::map<chess, chess> fa;//棋盘状态的父节点,用于反向传播 
 
std::pair<int, int> center;//搜索中心 

std::pair<int, int> mcts_main(int chsbd[16][16]);
chess UCT_search(chess x, std::pair<int, int> center, int player);//蒙特卡洛树搜索 
std::pair<chess,int> tree_policy(chess x, std::pair<int,int> center, int player);//选择子节点 
chess expand(chess x, std::pair<int,int> center, int player);//扩展当前节点 
double UCB(chess x, int player);//计算节点的UCB值 
std::pair<int, int> cal_centre(chess x);//计算当前局面的搜索中心 
double default_policy(chess x, int player);//模拟结果 
void back_up(chess x, chess y, int value);//反向传播 
std::pair< int, std::pair<int,int> > check_four(chess x);//预言胜利
std::pair< int, std::pair<int,int> > check_three(chess x);//预言胜利优化 
int check(chess x);//检查是否胜利，1为白胜，2为黑胜 
int check_win(chess x, int player);//分别检查黑白棋是否胜利 
int is_terminal(chess x);//检查是否为叶子节点 
int cnt_num(chess x, int x1, int x2, int y1, int y2);//查看节点的棋子数量 


void init_chess(chess x){
	property p;
	p.a = p.b = 0.0;
	mp[x] = p;
} 

#include <windows.h>
#include <pthread.h>

struct ThreadArgument{
	chess x, y;
}t;

//启用多线程
void* threadRun(void* ptr_temp = NULL){
////	fprintf(stderr, "thread created succesfully.\n");
	chess x = ((ThreadArgument*)ptr_temp) -> x;
	chess y = ((ThreadArgument*)ptr_temp) -> y;

	std::pair<chess, int> select_point = tree_policy(x, center,1);
	for(int j = 1; j <= sta_num;j++){//每个状态多次模拟，增强效果 
		double s = default_policy(select_point.first, select_point.second ^ 1);
		back_up(select_point.first, y, s);		
	}
	return NULL;
}

chess UCT_search(chess x, std::pair<int, int> center, int player){//x
	chess y = x;pthread_t tid;
	chess ans = y;
	if(check_four(y).first){
		std::pair<int,int> u = check_four(y).second; 
		ans.g[u.first][u.second] = player+1;
		return ans;
	}
	if(check_three(y).first){
		std::pair<int,int> u=check_three(y).second;
		ans.g[u.first][u.second]=player+1;
		return ans;
	}
	if(mp.find(x)==mp.end())init_chess(x);
	t.x = x, t.y = y;
	int cnt = 0;//选择次数 
	while(cnt <= select_num){
		cnt++;
		if (pthread_create(&tid, NULL, threadRun, &t) != 0){
	        fprintf(stderr, "create thread error\n");
	        exit(-1);
	    }
	}
	std::vector<chess>::iterator it;
	double maxn = UCB(*(mp[y].vec.begin() ), player);
	for(it = mp[y].vec.begin(); it != mp[y].vec.end(); it++){
		if(UCB(*it,player) >= maxn){
			maxn = UCB(*it, player);
			ans = *it;
		}
	}
	std::vector<chess>().swap(mp[y].vec);//释放内存 
	return ans;
}
 
std::pair<chess,int> tree_policy(chess x,std::pair<int,int> center,int player){
	while(!check(x) && !is_terminal(x))
	{
		int x1=max(0,center.first-search_range);
		int x2=min(row,center.first+search_range);
		int y1=max(0,center.second-search_range);
		int y2=min(col,center.second+search_range);
		if(cnt_num(x,x1,x2,y1,y2)+(int)mp[x].vec.size()<(x2-x1+1)*(y2-y1+1))
		{
			return make_pair(expand(x,center,player), player);
		}
		else{
			chess y = x;
			std::vector<chess>::iterator it;
			if(mp[y].vec.size() == 0)break;
			double maxn = UCB(*(mp[y].vec.begin()), player);
			for(it=mp[y].vec.begin();it != mp[y].vec.end(); it++){
				if(UCB(*it,player)>=maxn){
					maxn = UCB(*it,player);
					x = *it;
				}
			}
			fa[x] = y;
		}
		player ^= 1;
	}
	return make_pair(x, player);
}
 
chess expand(chess x,std::pair<int,int> center,int player){
	chess y=x;
	int x1=max(0,center.first-search_range);
	int x2=min(row,center.first+search_range);
	int y1=max(0,center.second-search_range);
	int y2=min(col,center.second+search_range);
	int cnt=0;
	while(cnt <= 1000){
		int i = x1 + random_number(0, x2 - x1 + 1);
		int j = y1 + random_number(0, y2 - y1 + 1);
		int o=x.g[i][j];
		y.g[i][j]=player+1;
		if(!x.g[i][j]&&mp.find(y)==mp.end()){
			init_chess(y);
			mp[x].vec.push_back(y);
			fa[y]=x;
			return y;
		}	
		y.g[i][j]=o;
		cnt++;
	}
	while(1)
	{
		int i=x1+random_number(0, x2-x1+1);
		int j=y1+random_number(0, y2-y1+1);
		int o=x.g[i][j];
		y.g[i][j]=player+1;
		if(!x.g[i][j]){
			if(mp.find(y)==mp.end()){
				init_chess(y);
			}
			return y;	
		}
		y.g[i][j]=o;
	}
}
//#include<limits.h>
double UCB(chess x,int player){
	if(mp[x].b==0)return 0;
	double a1=mp[x].a,b1=mp[x].b;
	if(a1+b1==0)return -inf;
	if(player == 1)return a1 / b1 + sqrt(log(a1 + b1) / b1);
	else if(player == 0)return -a1 / b1 + sqrt(log(a1 + b1) / b1);
	return sqrt(-1);
}
 
std::pair<int, int> cal_centre(chess x){//以每个棋子横纵坐标的均值为搜索中心 
	int cnt = 0,p1 = 0,p2=0;
	for(int i = 0;i <= row; i++){
		for(int j=0;j<=col;j++){
			if(x.g[i][j]){
				cnt++;
				p1+=i; 
				p2+=j;
			}
		}
	} 
	p1 = max(0, p1/cnt);
	p2 = max(0, p2/cnt);
	return make_pair(p1,p2);
}

double default_policy(chess x, int player){
	int check_result = 0;
	while(1){
		check_result = check(x);
		if(check_result || is_terminal(x))break;
		std::pair<int, int> h = cal_centre(x);
		int o = random_number(0, 100);
		int i, j;
		if(o < 93){
			i = min(max(0,h.first-search_range+random_number(0, search_range*2+1)),row);
			j = min(max(0,h.second-search_range+random_number(0, search_range*2+1)),col);
		}
		else{
			i = random_number(0, (row + 1));
			j = random_number(0, (col + 1));
		}
		if(!x.g[i][j]){
			x.g[i][j] = player + 1;	
			player ^= 1;
		}
	}
	
	if(check_result == 1)return -1;
	else if(check_result == 2)return 1;
	else return 0;
}

bool operator == (chess x,chess y){//用于判断棋盘状态是否相等 
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<col;j++)
		{
			if(x.g[i][j]!=y.g[i][j])return 0;
		}
	}
	return 1;
}
 
void back_up(chess x, chess y, int value){
	mp[x].a+=value;
	mp[x].b++;
	while(!(x == y)){
		if(fa.find(x) == fa.end())break;
		x = fa[x];
		mp[x].a += value;
		mp[x].b ++;
	}
}
 
std::pair< int,std::pair<int,int> > check_four(chess x){
	chess y=x;
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<=col;j++)
		{
			if(!x.g[i][j])
			{
				x.g[i][j]=2;
				if(check(x)==2)return make_pair(2,make_pair(i,j));
				x.g[i][j]=0;
			}
		}
	}
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<=col;j++)
		{
			if(!y.g[i][j])
			{
				y.g[i][j]=1;
				if(check(y)==1)return make_pair(1,make_pair(i,j));
				y.g[i][j]=0;
			}
		}
	}
	return make_pair(0,make_pair(0,0));
}
 
std::pair< int,std::pair<int,int> > check_three(chess x){
	chess y1=x,y2=x;
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<=col;j++)
		{
			if(!x.g[i][j])
			{
				x.g[i][j]=2;
				int flag=1;
				for(int k1=0;k1<=row;k1++)
				{
					for(int k2=0;k2<=col;k2++)
					{
						if(!x.g[k1][k2]){
							x.g[k1][k2]=1;
							if(check_four(x).first!=2){
								flag=0;
								x.g[k1][k2]=0;
								break;
							}else x.g[k1][k2]=0;
						}
					}
					if(!flag)break;
				}	
				if(flag)return make_pair(2,make_pair(i,j));
				x.g[i][j]=0;
			}	
		} 
	}
	
	std::vector< std::pair<int,int> > vec;
	
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<=col;j++)
		{
			if(!y1.g[i][j])
			{
				y1.g[i][j]=1;
				int flag=1;
				for(int k1=0;k1<=row;k1++)
				{
					for(int k2=0;k2<=col;k2++)
					{
						if(!y1.g[k1][k2]){
							y1.g[k1][k2]=2;
							if(check_four(y1).first!=1){
								flag=0;
								y1.g[k1][k2]=0;
								break;
							}else y1.g[k1][k2]=0;
						}
					}
					if(!flag)break;
				}	
				if(flag)return make_pair(1,make_pair(i,j));
				//if(flag)s.push_back(make_pair(i,j));
				y1.g[i][j]=0;
			}	
		} 
	}
	
	std::vector< std::pair<int,int> >::iterator it;
	
	std::pair<int,int> ret;
	int minn=1e9+7;
	
	for(it=vec.begin();it!=vec.end();it++)
	{
		std::pair<int,int> k=*it;
		if((k.first-cal_centre(y2).first)+(k.second-cal_centre(y2).second)<minn)
		{
			minn=(k.first-cal_centre(y2).first)+(k.second-cal_centre(y2).second);
			ret=k;
		}
	}
	
	if(vec.size())return make_pair(1,ret);
	
	return make_pair(0,make_pair(0,0));
}
 
int check(chess x){
	if(check_win(x,1))return 1;
	else if(check_win(x,2))return 2;
	else return 0;
}
 
int check_win(chess x,int gamer){
	int cnt=0;
	for(int i=0;i<=row;i++)
	{
		cnt=0;
		for(int j=0;j<=col;j++)
		{
			if(x.g[i][j]==gamer)cnt++;
			else cnt=0;
			if(cnt>=5)return 1;
		}	
	}	
	cnt=0;
	for(int i=0;i<=col;i++)
	{
		cnt=0;
		for(int j=0;j<=row;j++)
		{
			if(x.g[j][i]==gamer)cnt++;
			else cnt=0;
			if(cnt>=5)return 1;
		}	
	}
	cnt=0;
	for(int i=0;i<=row;i++)
	{
		cnt=0;
		int l=i,r=0;
		while(l<=col&&r<=col)
		{
			if(x.g[r][l]==gamer)cnt++;
			else cnt=0;
			if(cnt>=5)return 1;
			l++,r++;
		}
	}
	cnt=0;
	for(int i=0;i<=row;i++)
	{
		cnt=0;
		int l=i,r=0;
		while(l<=col&&r<=col)
		{
			if(x.g[l][r]==gamer)cnt++;
			else cnt=0;
			if(cnt>=5)return 1;
			l++,r++;
		}
	}
	cnt=0;
	for(int i=row;i>=0;i--)
	{
		cnt=0;
		int l=i,r=0;
		while(l>=0&&r<=col)
		{
			if(x.g[r][l]==gamer)cnt++;
			else cnt=0;
			if(cnt>=5)return 1;
			l--,r++;
		}
	}
	cnt=0;
	for(int i=0;i<=row;i++)
	{
		cnt=0;
		int l=i,r=col;
		while(l<=col&&r>=0)
		{
			if(x.g[l][r]==gamer)cnt++;
			else cnt=0;
			if(cnt>=5)return 1;
			l++,r--;
		}
	}
	return 0;
}
 
int is_terminal(chess x){
	
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<=col;j++)
		{
			if(!x.g[i][j])return 0;
		}
	}
	
	return 1;
}
 
int cnt_num(chess x,int x1,int x2,int y1,int y2){
	int sum=0;
	for(int i=x1;i<=x2;i++)
	{
		for(int j=y1;j<=y2;j++)
		{
			if(x.g[i][j])sum++;
		}
	}
	return sum;
}

std::pair<int, int> mcts_main(int chsbd[16][16]){
	chess a, b;
	init_chess(a);
	
	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j){
			if(chsbd[i][j] == 1)a.g[i][j] = 2;
			if(chsbd[i][j] == 2)a.g[i][j] = 1;
		}

	b = UCT_search(a, make_pair(7, 7), 1);

	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)	
			if(b.g[i][j] != a.g[i][j])
				return make_pair(i, j);
				
	return make_pair(-999, -999);
}