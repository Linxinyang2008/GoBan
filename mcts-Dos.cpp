#include<iostream>
#include<map>
#include<time.h>
#include<stdlib.h>
#include<cmath>
#include<vector>
#include<utility>
#include<windows.h>
#include<conio.h>

#define select_num 400        //选择次数
#define sta_num 17            //每个状态模拟次数

#define mp(x,y) make_pair(x,y)
const int row = 11, col = 11;//棋盘的行和列
const int search_range = 3;//搜索范围 
 
const double inf=1e9+7;
 
using namespace std;
 
typedef struct chess{//定义棋盘 
	int g[12][12];
}chess;
 
//0代表无棋子，1代表白棋，2代表黑棋 
 
bool operator < (chess x,chess y){//用于搜索棋盘状态 
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<=col;j++)
		{
			if(x.g[i][j]<y.g[i][j])return 1;
			else if(x.g[i][j]>y.g[i][j])return 0;
		}
	}
	return 0;
	
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
 
typedef struct property{//棋盘状态的一些性质 
	double a,b;
	vector<chess> vec;
}property; 
 
int rd=0;//回合数 
 
map<chess,property> mp;//用于记录棋盘状态的一些性质 
 
map<chess,chess> fa;//棋盘状态的父节点,用于反向传播 
 
pair<int,int> center;//搜索中心 
 
void init_chess(chess x);//初始化棋盘状态 
void init_window();//初始化窗口
void game_window();//游戏窗口 
void print_board(chess x);//打印棋盘 
void set_pos(int x,int y);//调整光标位置 
void white_win();//白棋胜利 
void black_win();//黑棋胜利 
 
chess UCT_search(chess x,pair<int,int> center,int player);//蒙特卡洛树搜索 
pair<chess,int> tree_policy(chess x,pair<int,int> center,int player);//选择子节点 
chess expand(chess x,pair<int,int> center,int player);//扩展当前节点 
double UCB(chess x,int player);//计算节点的UCB值 
pair<int,int> cal_centre(chess x);//计算当前局面的搜索中心 
double default_policy(chess x,int player);//模拟结果 
void back_up(chess x,chess y,int value);//反向传播 
pair< int,pair<int,int> > check_four(chess x);//预言胜利
pair< int,pair<int,int> > check_three(chess x);//预言胜利优化 
int check(chess x);//检查是否胜利，1为白胜，2为黑胜 
int check_win(chess x,int player);//分别检查黑白棋是否胜利 
int is_terminal(chess x);//检查是否为叶子节点 
int cnt_num(chess x,int x1,int x2,int y1,int y2);//查看节点的棋子数量 
 
//player：0为白，1为黑 
 
void init_chess(chess x){
	property p;
	p.a = p.b = 0.0;
	mp[x] = p;
} 
 
void set_pos(int x,int y){
    HANDLE winHandle;
    COORD pos;
	pos.X = x, pos.Y = y; 
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(winHandle, pos); 
}
 
void init_window(){
	system("cls");
	for(int i=1;i<=7;i++)putchar(0xa);
	for(int i=1;i<=6;i++)putchar('\t');
	wprintf(L"五子棋\n\n\n\n");
	for(int i=1;i<=5;i++)printf("\t  ");
	wprintf(L"输入任意键开始游戏"); 
	getch();
}
 
void game_window(){
	rd = 0;
	mp.clear(),fa.clear();
	chess now;
//	for(int i=0;i<=row;i++)
//	{
//		for(int j=0;j<=col;j++)
//		{
//			now.g[i][j]=0;
//		}
//	}
	memset(now.g, 0, sizeof now.g);
	init_chess(now);
	
	//pair<int,int> center;
	
	center.first= row/2, center.second= col/2;
 
	srand(time(0));
	
	print_board(now);
 
	int check_result = 0;
	while(check_result == 0){
		
		while(true){
			print_board(now);
			set_pos(65, 12);
			wprintf(L"轮到您执棋，请输入坐标：");
			set_pos(65, 14);
			int x,y;
			scanf("%d%d", &x, &y);	
			x--, y--;
			if(x < 0 || x > row || y < 0 || y > col){
				set_pos(65, 16);
				wprintf(L"您输入的数据超出棋盘范围\n");
				Sleep(1500);
			}
			else if(now.g[x][y]){
				set_pos(65, 16);
				wprintf(L"该位置已有棋子");
				Sleep(1500);	
			}
			else{
				now.g[x][y]=1;
				center.first=cal_centre(now).first,center.second=cal_centre(now).second;
				rd++;
				break;
			}
		}
		print_board(now);
		now = UCT_search(now, center, 1);
		check_result = check(now);
		if(check_result == 2){
			print_board(now);
			black_win();
		}
		print_board(now);
		if(check_result == 1)white_win();
	}
}
 
void print_board(chess x){
	system("cls");
	for(int i=1;i<=2;i++)putchar('\t'); 
	for(int i=0;i<=col;i++){
		if(i+1<10)cout<<" "<<i+1<<" ";
		else cout<<" "<<i+1;
	}
	cout<<"\n";char p = 0;
	for(int i=0;i<=row;i++)
	{
		for(int j=1;j<=2;j++)cout<<"\t";
		for(int j=0;j<=col;j++)cout<<" __";
		cout<<"\n";
		for(int j=1;j<=1;j++)cout<<"\t";
		cout<<i+1;
		putchar('\t');
		for(int j = 0; j <= col; j++){
			if(x.g[i][j]==0)p=' ';
			else if(x.g[i][j]==1)p='O';
			else if(x.g[i][j]==2)p='X';
			cout<<"|"<<p<<" ";
		}
		printf("|\n");
	}
	for(int j = 1;j <= 2; j++)putchar('\t');
	for(int i=0;i<=col;i++)cout<<" __";
} 
 
void white_win(){
	Sleep(1500); 
	system("cls");
	for(int i=1;i<=8;i++)cout<<'\n';
	for(int i=1;i<=4;i++)cout<<'\t';
	cout<<"白棋胜利";
	cout<<"\n\n";
	Sleep(1500);
	init_window();
}
 
void black_win(){
	Sleep(1500);
	system("cls");
	for(int i = 1; i <= 8; i++)cout<<'\n';
	for(int i = 1; i <= 4; i++)cout<<'\t';
	cout<<"黑棋胜利";
	cout<<"\n\n";
	Sleep(1500);
	init_window();
}
 
chess UCT_search(chess x,pair<int,int> center,int player){
	chess y=x;
	chess ans=y;
	if(check_four(y).first)
	{
		pair<int,int> u=check_four(y).second; 
		ans.g[u.first][u.second]=player+1;
		return ans;
	}
	if(check_three(y).first)
	{
		pair<int,int> u=check_three(y).second;
		ans.g[u.first][u.second]=player+1;
		return ans;
	}
	if(mp.find(x)==mp.end())
	{
		init_chess(x);
	}
	int cnt = 0;//选择次数 
	while(cnt<=select_num)	{
		cnt++;
		pair<chess,int> select_point=tree_policy(x,center,1);
		for(int j=1;j<=sta_num;j++)//每个状态多次模拟，增强效果 
		{
			double s=default_policy(select_point.first,select_point.second^1);
			back_up(select_point.first,y,s);
		}
	}
	vector<chess>::iterator it;
	double maxn=UCB(*(mp[y].vec.begin()),player);
	for(it=mp[y].vec.begin();it!=mp[y].vec.end();it++)
	{
		if(UCB(*it,player)>=maxn)
		{
			maxn=UCB(*it,player);
			ans=*it;
		}
		//print_board(*it);
		//cout<<mp[*it].a<<" "<<mp[*it].b<<'\n';
		//Sleep(1500);
	}
	vector<chess>().swap(mp[y].vec);//释放内存 
	return ans;
}
 
pair<chess,int> tree_policy(chess x,pair<int,int> center,int player){
	while(!check(x)&&!is_terminal(x))
	{
		int x1=max(0,center.first-search_range);
		int x2=min(row,center.first+search_range);
		int y1=max(0,center.second-search_range);
		int y2=min(col,center.second+search_range);
		if(cnt_num(x,x1,x2,y1,y2)+mp[x].vec.size() < (unsigned long long)(x2-x1+1)*(y2-y1+1))
			return mp(expand(x,center,player),player);
		
		else{
			chess y=x;
			vector<chess>::iterator it;
			if(mp[y].vec.size()==0)break;
			double maxn=UCB(*(mp[y].vec.begin()),player);
			for(it=mp[y].vec.begin();it!=mp[y].vec.end();it++)
			{
				if(UCB(*it,player)>=maxn)
				{
					maxn=UCB(*it,player);
					x=*it;
				}
			}
			fa[x]=y;
		}
		player^=1;
	}
	return mp(x,player);
}
 
chess expand(chess x,pair<int,int> center,int player){
	chess y=x;
	int x1=max(0,center.first-search_range);
	int x2=min(row,center.first+search_range);
	int y1=max(0,center.second-search_range);
	int y2=min(col,center.second+search_range);
	int cnt=0;
	while(cnt<=1000)
	{
		int i=x1+rand()%(x2-x1+1);
		int j=y1+rand()%(y2-y1+1);
		int o=x.g[i][j];
		y.g[i][j]=player+1;
		if(!x.g[i][j]&&mp.find(y)==mp.end())
		{
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
		int i=x1+rand()%(x2-x1+1);
		int j=y1+rand()%(y2-y1+1);
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
 
double UCB(chess x,int player){
	if(mp[x].b==0)return 0;
	double a1=mp[x].a,b1=mp[x].b;
	if(a1+b1==0)return -inf;
	if(player==1)return a1/b1+sqrt(log(a1+b1)/b1);
	else if(player==0)return -a1/b1+sqrt(log(a1+b1)/b1);
	
	fprintf(stderr, "UCB(): control reaches end of non-void function. = -1.0\n");
	return -1.00;
}
 
pair<int,int> cal_centre(chess x){//以每个棋子横纵坐标的均值为搜索中心 
	int cnt=0,p1=0,p2=0;
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<=col;j++){
			if(x.g[i][j]){
				cnt++;
				p1+=i; 
				p2+=j;
			}
		}
	} 
	p1=max(0,p1/cnt);
	p2=max(0,p2/cnt);
	return mp(p1,p2);
}
 
double default_policy(chess x,int player){
	while(1)
	{
		if(check(x)||is_terminal(x))break;
		pair<int,int> h=cal_centre(x);
		int o=rand()%100;
		int i,j;
		if(o<75){
			i=min(max(0,h.first-search_range+rand()%(search_range*2+1)),row);
			j=min(max(0,h.second-search_range+rand()%(search_range*2+1)),col);
		}else{
			i=rand()%(row+1);
			j=rand()%(col+1);
		}
		if(!x.g[i][j])
		{
			x.g[i][j]=player+1;	
			player^=1;
		}
	}
	
	if(check(x)==1)return -1;
	else if(check(x)==2)return 1;
	else return 0;
}
 
void back_up(chess x,chess y,int value){
	mp[x].a+=value;
	mp[x].b++;
	while(!(x==y))
	{
		if(fa.find(x)==fa.end())break;
		x=fa[x];
		mp[x].a+=value;
		mp[x].b++;
	}
}
 
pair< int,pair<int,int> > check_four(chess x){
	chess y=x;
	for(int i=0;i<=row;i++)
	{
		for(int j=0;j<=col;j++)
		{
			if(!x.g[i][j])
			{
				x.g[i][j]=2;
				if(check(x)==2)return mp(2,mp(i,j));
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
				if(check(y)==1)return mp(1,mp(i,j));
				y.g[i][j]=0;
			}
		}
	}
	return mp(0,mp(0,0));
}
 
pair< int,pair<int,int> > check_three(chess x){
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
				if(flag)return mp(2,mp(i,j));
				x.g[i][j]=0;
			}	
		} 
	}
	
	vector< pair<int,int> > vec;
	
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
				if(flag)return mp(1,mp(i,j));
				//if(flag)s.push_back(mp(i,j));
				y1.g[i][j]=0;
			}	
		} 
	}
	
	vector< pair<int,int> >::iterator it;
	
	pair<int,int> ret;
	int minn=1e9+7;
	
	for(it=vec.begin();it!=vec.end();it++)
	{
		pair<int,int> k=*it;
		if((k.first-cal_centre(y2).first)+(k.second-cal_centre(y2).second)<minn)
		{
			minn=(k.first-cal_centre(y2).first)+(k.second-cal_centre(y2).second);
			ret=k;
		}
	}
	
	if(vec.size())return mp(1,ret);
	
	return mp(0,mp(0,0));
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

#include<wchar.h>
#include<locale.h>
int main(){	
	setlocale(LC_ALL, "Chinese");
	init_window();
	game_window();
	return 0;	
} 
 
 
 
 
 
 
 
 
 
 
 