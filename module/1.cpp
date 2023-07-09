#include<cstdio>

int main(void){
	freopen("1.out", "w", stdout);
	for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)
			printf("%d%c", 0, " \n"[j == 15]);
			
}