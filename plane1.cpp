#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<conio.h>
#include<windows.h>

#pragma warning(disable : 4996)								//VS/VC需要该条注释 

#define height 32											//窗口大小是 55*35 
#define width 52											//画面大小 
#define EnemySum 40 										//敌人总数 
#define EnemyNum 3											//一次出现的敌人数量 
#define EnemySpeed 4										//敌人发射子弹的速度 
#define EnemyBulletTime 50									//一个敌人子弹发射的时间间隔 

using namespace std;

//0空格  1飞机左上角   2飞机子弹  3敌机左上角  4敌机子弹 
int canvas[height+1][width+1]={0};							//画布 
int enemy_x[EnemyNum+1],enemy_y[EnemyNum+1];				//敌机坐标 
int enemyTime[EnemyNum+1];									//敌机子弹发射时间间隔 
int plane_x, plane_y;										//飞机坐标 
int score;													//得分 
int killEnemy;												//击杀敌机的数量 

void gotoxy(int x, int y);									//光标移动 
void HideCursor(); 											//隐藏光标 
void startup();												//初始化设置 
void show();												//画面显示 
void updateWithoutInput();									//无操作更新 
void updateWithInput();										//有操作更新 

int main()
{
	system("mode con cols=55 lines=35");					//设置窗口大小  55*35
	startup();
	bool flag=1;
	show();
	Sleep(3000);											//给用户三秒反应         
	while (1)
	{
		show();
		updateWithoutInput();
		updateWithInput();
		if(score<0)											//没分 游戏结束 
		{
			flag=0;
			break;
		}
		if(killEnemy>=EnemySum)								//敌人全部消灭 游戏结束 
			break;
	}
	system("cls");
	for(int i=1;i<=10;i++)
		cout<<endl;
	for(int i=1;i<=10;i++)
		cout<<" ";
	if(!flag)
		cout<<"game over";
	else
		cout<<"Congratulations!!!    Your score is "<<score;
	cout<<endl<<endl<<endl;
	return 0;
}

void gotoxy(int x, int y)									//光标移动 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void HideCursor() 											//隐藏光标 
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}

void startup()												//初始化设置 
{
	HideCursor(); 
	plane_x = width / 2-1;									//飞机坐标左上角 
	plane_y = height-2;
	canvas[plane_y][plane_x]=1;
	for(int i=1;i<=EnemyNum;i++)
	{
		enemy_x[i]=rand()%(width-3);						//注意敌机长度 
		enemy_y[i]=0;
		if(canvas[enemy_y[i]][enemy_x[i]-3]==3||canvas[enemy_y[i]][enemy_x[i]-2]==3||canvas[enemy_y[i]][enemy_x[i]-1]==3)
		{													//防止重叠 
			i--;
			continue;
		}
		if(canvas[enemy_y[i]][enemy_x[i]+3]==3||canvas[enemy_y[i]][enemy_x[i]+2]==3||canvas[enemy_y[i]][enemy_x[i]+1]==3)
		{
			i--;
			continue;
		}
		canvas[enemy_y[i]][enemy_x[i]]=3;
		canvas[enemy_y[i]+2][enemy_x[i]+1]=4; 
		enemyTime[i]=0;
	}
	score=0;
	plane_x = width / 2-1;
	killEnemy=0; 
} 

void show()													//画面显示 
{
	gotoxy(0,0);
	for(int i=0;i<=height;i++)
	{
		for(int j=0;j<=width;j++)
		{			
			if(canvas[i][j]==1)
			{
				printf("/=\\");								//飞机不能在次整体输出，因为飞机后面可能有子弹 
				j+=2;										//光标位置更新 
			}
			else if(i-1>=0&&j+1<=width&&canvas[i-1][j+1]==1)//注意数组越界 
			{
				printf("<<*>>");
				j+=4;
			}
			else if(i-2>=0&&canvas[i-2][j]==1)
			{
				printf("* *");
				j+=2;
			}
			else if(canvas[i][j]==3)
			{
				printf("\\=/");								//敌机 
				j+=2;
			}
			else if(i-1>=0&&j-1>=0&&canvas[i-1][j-1]==3)
				printf("|");
			else if(canvas[i][j]==2)
				printf("|");
			else if(canvas[i][j]==4)
				printf("!"); 
			else if(canvas[i][j]==0)						//空格放到最后 
				printf(" ");
		}
		printf("\n");
	}
	printf("得分： %d\n", score);
}


void updateWithoutInput()									//无操作更新 
{
	static int speed=0;
	for(int i=0;i<=height;i++)
	{
		for(int j=0;j<=width;j++)
		{
			if(canvas[i][j]==2)								//判断子弹击中敌机 
			{
				for(int k=1;k<=EnemyNum;k++)				//多个敌机 
				{
					if (((j >= enemy_x[k]&&j<=enemy_x[k]+3) && (i == enemy_y[k]))||(j==enemy_x[k]+1&&i==enemy_y[k]+1))
					{														
						score++;
						killEnemy++;
						canvas[enemy_y[k]][enemy_x[k]]=0; 
						while(1)							//敌机更新 
						{
							enemy_x[k]=rand()%(width-3);						
							enemy_y[k]=0;
							if(canvas[enemy_y[k]][enemy_x[k]-3]==3||canvas[enemy_y[k]][enemy_x[k]-2]==3||canvas[enemy_y[k]][enemy_x[k]-1]==3)
								continue;
							if(canvas[enemy_y[k]][enemy_x[k]+3]==3||canvas[enemy_y[k]][enemy_x[k]+2]==3||canvas[enemy_y[k]][enemy_x[k]+1]==3)
								continue;
							break;
						}
						canvas[enemy_y[k]][enemy_x[k]]=3;
						canvas[enemy_y[k]+2][enemy_x[k]+1]=4;//新的敌机子弹 
						canvas[i][j]=0;						//子弹消失 
						killEnemy++;
					}
				}
			}
			if(canvas[i][j]==2)								//子弹上移 
			{
				canvas[i][j]=0;
				if(i-1>=0)
					canvas[i-1][j]=2;
			}	
			if(canvas[i][j]==4)
			{												//敌机子弹击中 
				if((j==plane_x&&j<=plane_x+2&&i==plane_y)||(j>=plane_x-1&&j<=plane_x+3&&i==plane_y+1))
				{
					score--;
					canvas[i][j]=0;
				}			
			}
		}
	}	
	for(int i=height;i;i--)									//敌机子弹从上往下走，需要从下到上扫描 
	{
		for(int j=0;j<=width;j++)
		{
			if(canvas[i][j]==4)
			{
				if(speed==EnemySpeed)
				{
					canvas[i][j]=0;
					if(i<height)
						canvas[i+1][j]=4;
				}
				else 
					speed++;
			} 
		}
	}	
	for(int i=1;i<=EnemyNum;i++)							//发射新的子弹 
	{
		if(enemyTime[i]==EnemyBulletTime)
		{
			canvas[enemy_y[i]+2][enemy_x[i]+1]=4;
			enemyTime[i]=0;
		}
		else	
			enemyTime[i]++;	
	}
	canvas[plane_y][plane_x]=1;								//飞机恢复，防止被其他覆盖 
}



void updateWithInput()										//有操作更新 
{
	char input;
	if (kbhit())  											//判断是否有输入
	{
		input = getch();  								
		if (input == 'a'&&plane_x-1)						//控制不超过边界 
		{
			canvas[plane_y][plane_x]=0;
			plane_x--;  
			canvas[plane_y][plane_x]=1;
		}										
		if (input == 'd'&&plane_x+3<width)
		{
			canvas[plane_y][plane_x]=0;
			plane_x++; 
			canvas[plane_y][plane_x]=1;
		}	 
		if (input == 'w'&&plane_y)
		{
			canvas[plane_y][plane_x]=0;
			plane_y--; 
			canvas[plane_y][plane_x]=1;
		}	 
		if (input == 's'&&plane_y+2<height)
		{
			canvas[plane_y][plane_x]=0;
			plane_y++;
			canvas[plane_y][plane_x]=1;
		}	 
		if (input == ' ')  									//发射子弹
		{
			canvas[plane_y-1][plane_x+1]=2;					//子弹的初始位置在飞机的正上方
		}
	}
}
