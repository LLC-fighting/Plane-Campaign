#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<conio.h>
#include<windows.h>

#pragma warning(disable : 4996)								//VS/VC��Ҫ����ע�� 

#define height 32											//���ڴ�С�� 55*35 
#define width 52											//�����С 
#define EnemySum 40 										//�������� 
#define EnemyNum 3											//һ�γ��ֵĵ������� 
#define EnemySpeed 4										//���˷����ӵ����ٶ� 
#define EnemyBulletTime 50									//һ�������ӵ������ʱ���� 

using namespace std;

//0�ո�  1�ɻ����Ͻ�   2�ɻ��ӵ�  3�л����Ͻ�  4�л��ӵ� 
int canvas[height+1][width+1]={0};							//���� 
int enemy_x[EnemyNum+1],enemy_y[EnemyNum+1];				//�л����� 
int enemyTime[EnemyNum+1];									//�л��ӵ�����ʱ���� 
int plane_x, plane_y;										//�ɻ����� 
int score;													//�÷� 
int killEnemy;												//��ɱ�л������� 

void gotoxy(int x, int y);									//����ƶ� 
void HideCursor(); 											//���ع�� 
void startup();												//��ʼ������ 
void show();												//������ʾ 
void updateWithoutInput();									//�޲������� 
void updateWithInput();										//�в������� 

int main()
{
	system("mode con cols=55 lines=35");					//���ô��ڴ�С  55*35
	startup();
	bool flag=1;
	show();
	Sleep(3000);											//���û����뷴Ӧ         
	while (1)
	{
		show();
		updateWithoutInput();
		updateWithInput();
		if(score<0)											//û�� ��Ϸ���� 
		{
			flag=0;
			break;
		}
		if(killEnemy>=EnemySum)								//����ȫ������ ��Ϸ���� 
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

void gotoxy(int x, int y)									//����ƶ� 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void HideCursor() 											//���ع�� 
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}

void startup()												//��ʼ������ 
{
	HideCursor(); 
	plane_x = width / 2-1;									//�ɻ��������Ͻ� 
	plane_y = height-2;
	canvas[plane_y][plane_x]=1;
	for(int i=1;i<=EnemyNum;i++)
	{
		enemy_x[i]=rand()%(width-3);						//ע��л����� 
		enemy_y[i]=0;
		if(canvas[enemy_y[i]][enemy_x[i]-3]==3||canvas[enemy_y[i]][enemy_x[i]-2]==3||canvas[enemy_y[i]][enemy_x[i]-1]==3)
		{													//��ֹ�ص� 
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

void show()													//������ʾ 
{
	gotoxy(0,0);
	for(int i=0;i<=height;i++)
	{
		for(int j=0;j<=width;j++)
		{			
			if(canvas[i][j]==1)
			{
				printf("/=\\");								//�ɻ������ڴ������������Ϊ�ɻ�����������ӵ� 
				j+=2;										//���λ�ø��� 
			}
			else if(i-1>=0&&j+1<=width&&canvas[i-1][j+1]==1)//ע������Խ�� 
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
				printf("\\=/");								//�л� 
				j+=2;
			}
			else if(i-1>=0&&j-1>=0&&canvas[i-1][j-1]==3)
				printf("|");
			else if(canvas[i][j]==2)
				printf("|");
			else if(canvas[i][j]==4)
				printf("!"); 
			else if(canvas[i][j]==0)						//�ո�ŵ���� 
				printf(" ");
		}
		printf("\n");
	}
	printf("�÷֣� %d\n", score);
}


void updateWithoutInput()									//�޲������� 
{
	static int speed=0;
	for(int i=0;i<=height;i++)
	{
		for(int j=0;j<=width;j++)
		{
			if(canvas[i][j]==2)								//�ж��ӵ����ел� 
			{
				for(int k=1;k<=EnemyNum;k++)				//����л� 
				{
					if (((j >= enemy_x[k]&&j<=enemy_x[k]+3) && (i == enemy_y[k]))||(j==enemy_x[k]+1&&i==enemy_y[k]+1))
					{														
						score++;
						killEnemy++;
						canvas[enemy_y[k]][enemy_x[k]]=0; 
						while(1)							//�л����� 
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
						canvas[enemy_y[k]+2][enemy_x[k]+1]=4;//�µĵл��ӵ� 
						canvas[i][j]=0;						//�ӵ���ʧ 
						killEnemy++;
					}
				}
			}
			if(canvas[i][j]==2)								//�ӵ����� 
			{
				canvas[i][j]=0;
				if(i-1>=0)
					canvas[i-1][j]=2;
			}	
			if(canvas[i][j]==4)
			{												//�л��ӵ����� 
				if((j==plane_x&&j<=plane_x+2&&i==plane_y)||(j>=plane_x-1&&j<=plane_x+3&&i==plane_y+1))
				{
					score--;
					canvas[i][j]=0;
				}			
			}
		}
	}	
	for(int i=height;i;i--)									//�л��ӵ����������ߣ���Ҫ���µ���ɨ�� 
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
	for(int i=1;i<=EnemyNum;i++)							//�����µ��ӵ� 
	{
		if(enemyTime[i]==EnemyBulletTime)
		{
			canvas[enemy_y[i]+2][enemy_x[i]+1]=4;
			enemyTime[i]=0;
		}
		else	
			enemyTime[i]++;	
	}
	canvas[plane_y][plane_x]=1;								//�ɻ��ָ�����ֹ���������� 
}



void updateWithInput()										//�в������� 
{
	char input;
	if (kbhit())  											//�ж��Ƿ�������
	{
		input = getch();  								
		if (input == 'a'&&plane_x-1)						//���Ʋ������߽� 
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
		if (input == ' ')  									//�����ӵ�
		{
			canvas[plane_y-1][plane_x+1]=2;					//�ӵ��ĳ�ʼλ���ڷɻ������Ϸ�
		}
	}
}
