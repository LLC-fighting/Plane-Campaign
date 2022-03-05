#include<iostream>
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include<algorithm>
#include<stdlib.h>
#include<cstring>

#pragma comment(lib,"Winmm.lib")							// 引用 Windows Multimedia API

#pragma warning(disable : 4996)								//忽略4996警告

/*Dev里面运行不了，需要在VS里面新建项目，将代码复制进去
  还需将属性中的字符集从Unicode改成使用多字节字符集，要不报错
  右键点击右面的项目的名字->点击属性->点击->高级->找到字符集->改成使用多字节字符集*/

using namespace std;

#define height 598											//窗口大小是 55*35 
#define width 396											//画面大小 
#define enemySum 20	 										//敌人总数 
#define enemyNum 3											//一次出现的敌人数量 
#define enemySpeed 20										//敌人发射子弹的速度 
#define EnemyBulletTime 2000								//一个敌人子弹发射的时间间隔 
#define planeSpeed 2										//己方子弹速度
#define enemyDown 25										//敌机下落的速度
int EnemySum;												//调整难度参数
int EnemySpeed;
int EnemyNum;
int EnemyDown;

IMAGE img_background1;										//欢迎背景
IMAGE img_background2;										//功能选择背景
IMAGE img_background3;										//游戏背景
IMAGE img_background4;										//关于背景
IMAGE img_background5;										//帮助背景
IMAGE img_background6;										//胜利游戏结束背景
IMAGE img_background7;										//失败游戏结束背景
IMAGE img_background8;										//难度选择背景
IMAGE img_background9;										//游戏暂停背景
IMAGE img_plane;											//飞机
IMAGE img_zplane;											//遮罩图飞机
IMAGE img_enemy;											//敌机
IMAGE img_zenemy;											//遮罩图敌机							
IMAGE img_bullet;											//子弹
IMAGE img_zbullet;											//遮罩图子弹
IMAGE img_ebullet;											//敌机子弹
IMAGE img_zebullet;											//遮罩图敌机
IMAGE img_dieenemy;											//敌机击毁
IMAGE img_zdieenemy;										//遮罩图敌机击毁
IMAGE img_dieplane;											//飞机击毁
IMAGE img_zdieplane;										//遮罩图飞机击毁

int bk_no;													//背景切换编号
bool bk_change;												//是否换背景
int plane_x, plane_y;										//飞机坐标
int canvas[1000][1000];										//画布
int bulletTime[1000][1000];									//控制敌机子弹飞行速度
int enemy_x[20], enemy_y[20];								//敌机坐标 
int enemyDownTime[20];										//飞机下落时间
int enemyTime[20];											//敌机子弹发射时间间隔 
int score;													//得分
int killSum;												//击杀敌人的数量
bool stop;													//控制画面是否停顿

void initialization();										//数据初始化
void startup();												//载入图片
bool checkPos(int x, int y);								//检查敌机位置是否重复
void bkShow();												//背景显示
void gameShow();											//游戏显示
void show();												//显示
void updateWithInput();										//有操作更新
void updateWithoutInput();									//无操作更新
void bk3Control();											//操作3
void bk45Control();											//操作4 5
void bk67Control();											//操作6 7
void bk8Control();											//操作8
void control();												//操作控制

int main()
{
	startup();
	while (1)
	{
		show();
		control();
	}
}


void initialization()
{
	memset(canvas, 0, sizeof(canvas));						//画布清空
	memset(bulletTime, 0, sizeof(bulletTime));
	memset(enemyDownTime, 0, sizeof(enemyDownTime));
	plane_x = 396 / 2;
	plane_y = 518;
	score = 0;
	killSum = 0;
	stop = 1;
	canvas[plane_y][plane_x] = 1;
	for (int i = 1; i <= EnemyNum; i++)
	{
		enemy_x[i] = rand() % (width - 35);						//注意敌机长度 
		enemy_y[i] = 0;
		if (checkPos(enemy_x[i], enemy_y[i]))					//防止重叠
		{
			i--;
			continue;
		}
		canvas[enemy_y[i]][enemy_x[i]] = 3;
		canvas[enemy_y[i] + 36][enemy_x[i] + 14] = 4;
		enemyTime[i] = rand() % EnemyBulletTime;
	}
}


void startup()													//数据初始化
{
	initgraph(396, 598);										//定义画布大小
	loadimage(&img_background1, _T(".\\picture\\background1.png"));//载入图片
	loadimage(&img_background2, _T(".\\picture\\background2.png"));
	loadimage(&img_background3, _T(".\\picture\\background3.jpg"));
	loadimage(&img_background4, _T(".\\picture\\background4.png"));
	loadimage(&img_background5, _T(".\\picture\\background5.png"));
	loadimage(&img_background6, _T(".\\picture\\background6.png"));
	loadimage(&img_background7, _T(".\\picture\\background7.png"));
	loadimage(&img_background8, _T(".\\picture\\background8.png"));
	loadimage(&img_background9, _T(".\\picture\\background9.png"));
	loadimage(&img_plane, _T(".\\picture\\plane.png"));
	loadimage(&img_zplane, _T(".\\picture\\zplane.png"));
	loadimage(&img_enemy, _T(".\\picture\\enemy.png"));
	loadimage(&img_zenemy, _T(".\\picture\\zenemy.png"));
	loadimage(&img_bullet, _T(".\\picture\\bullet.png"));
	loadimage(&img_zbullet, _T(".\\picture\\zbullet.png"));
	loadimage(&img_ebullet, _T(".\\picture\\ebullet.png"));
	loadimage(&img_dieenemy, _T(".\\picture\\dieenemy.png"));
	loadimage(&img_zdieenemy, _T(".\\picture\\zdieenemy.png"));
	loadimage(&img_dieplane, _T(".\\picture\\dieplane.png"));
	loadimage(&img_zdieplane, _T(".\\picture\\zdieplane.png"));							
	mciSendString("open  .\\sound\\gamemusic.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐		//需要改成使用多字节字符集
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 循环播放
	bk_no = 1;
	bk_change = 1;
}


bool checkPos(int x,int y)
{
	for (int i = 0; i <= 35; i++)
		if (canvas[y][x + i]||canvas[y][x-i])
			return true;
	return false;
}


void bkShow()
{
	if (bk_no == 1)
	{
		putimage(0, 0, &img_background1);
		Sleep(3000);
		bk_no = 2;
		bk_change = 1;
	}
	else if (bk_no == 2)
		putimage(0, 0, &img_background2);
	else if (bk_no == 3)
		putimage(0, 0, &img_background3);
	else if (bk_no == 4)
		putimage(0, 0, &img_background4);
	else if (bk_no == 5)
		putimage(0, 0, &img_background5);
	else if(bk_no==6)
		putimage(0, 0, &img_background6);
	else if (bk_no == 7)
		putimage(0, 0, &img_background7);
	else if(bk_no==8)
		putimage(0, 0, &img_background8);
	else if (bk_no == 9)
		putimage(0, 0, &img_background9);
}


void gameShow()
{
	BeginBatchDraw();													//开始绘制背景 防止加载图片闪烁
	putimage(0, 0, &img_background3);									//背景重新载入，防止闪动
	putimage(plane_x, plane_y, &img_zplane,NOTSRCERASE);
	putimage(plane_x, plane_y, &img_plane, SRCINVERT);
	for (int i = 0; i <= height; i++)
	{
		for (int j = 0; j <= width; j++)
		{
			if (canvas[i][j] == 2)
			{
				putimage(j, i, &img_zbullet, NOTSRCERASE);
				putimage(j, i, &img_bullet, SRCINVERT);
			}
			else if (canvas[i][j] == 3)
			{
				putimage(j, i, &img_zenemy, NOTSRCERASE);
				putimage(j, i, &img_enemy, SRCINVERT);

			}
			else if (canvas[i][j] == 4)
			{
				putimage(j, i, &img_ebullet, NOTSRCERASE);
				putimage(j, i, &img_zebullet, SRCINVERT);
			}
		}
	}
	settextstyle(18, 9, _T("黑体"));										//设置字体格式
	TCHAR s[] = _T("得分：");
	outtextxy(5, 570, s);													//显示得分
	TCHAR sc[5];
	_stprintf(sc, _T("%d"), score);											//转换类型
	outtextxy(50, 570, sc);
	EndBatchDraw();															//结束绘制
}


void show()
{
	if (bk_change)
	{
		bk_change = 0;
		bkShow();		
	}
	if (bk_no == 3)
		gameShow();

}


void updateWithInput()										//有操作更新 
{
	char input;
	if (kbhit())  											//判断是否有输入
	{
		input = getch();
		if (input == 'a')									//控制不超过边界 
		{
			canvas[plane_y][plane_x] = 0;
			if(plane_x>0)
				plane_x-=2;									//1移动太慢
			canvas[plane_y][plane_x] = 1;
		}
		if (input == 'd')
		{
			canvas[plane_y][plane_x] = 0;
			if(plane_x<346)
				plane_x+=2;
			canvas[plane_y][plane_x] = 1;
		}
		if (input == 'w')
		{
			canvas[plane_y][plane_x] = 0;
			if(plane_y>25)
				plane_y-=2;
			canvas[plane_y][plane_x] = 1;
		}
		if (input == 's')
		{
			canvas[plane_y][plane_x] = 0;
			if(plane_y<510)
				plane_y+=2;
			canvas[plane_y][plane_x] = 1;
		}
		if (input == ' ')  										//发射子弹
		{
			canvas[plane_y-28][plane_x+18] = 2;					//子弹的初始位置在飞机的正上方
			mciSendString("close firing", NULL, 0, NULL);		// 先把前面一次的音乐关闭
			mciSendString("open .\\sound\\firing.wav alias firing", NULL, 0, NULL); // 打开音乐
			mciSendString("play firing", NULL, 0, NULL);		// 仅播放一次
		}
		if (input == 0x1b)
		{
			bk_no = 9;
			bk_change = 1;
		}
	}
}


void updateWithoutInput()										//无操作更新 
{
	for (int i = 0; i <= height; i++)
	{
		for (int j = 0; j <= width; j++)
		{
			if (canvas[i][j] == 2)								//判断子弹击中敌机 
			{
				for (int k = 1; k <= EnemyNum; k++)				//多个敌机 
				{
					if (((j >= enemy_x[k] - 5 && j <= enemy_x[k] + 35) &&(i >= enemy_y[k]&&i == enemy_y[k] + 35)))
					{
						putimage(enemy_x[k], enemy_y[k], &img_zdieenemy, NOTSRCERASE);	//敌机击毁图片
						putimage(enemy_x[k], enemy_y[k], &img_dieenemy, SRCINVERT);
						mciSendString("close enemyExplode", NULL, 0, NULL); // 先把前面一次的音乐关闭
						mciSendString("open .\\sound\\enemyExplode.wav alias enemyExplode", NULL, 0, NULL); // 打开音乐
						mciSendString("play enemyExplode", NULL, 0, NULL); // 仅播放一次
						score++;
						canvas[enemy_y[k]][enemy_x[k]] = 0;
						while (1)								//敌机更新 
						{
							enemy_x[k] = rand() % (width - 35);
							enemy_y[k] = 0;
							if (checkPos(enemy_x[k],enemy_y[k]))
								continue;
							break;
						}
						canvas[enemy_y[k]][enemy_x[k]] = 3;
						canvas[enemy_y[k] + 36][enemy_x[k] + 14] = 4;//新的敌机子弹 
						canvas[i][j] = 0;							//子弹消失 
						enemyTime[i] = rand() % EnemyBulletTime;
						killSum++;
					}
				}
			}
			static int plspeed = 1;
			if (canvas[i][j] == 2)									//子弹上移 
			{
				if (plspeed == planeSpeed)
				{
					canvas[i][j] = 0;
					if (i - 1 >= 0)									//不飞出边界
						canvas[i - 1][j] = 2;
					plspeed = 1;
				}
				else
					plspeed++;				
			}
			if (canvas[i][j] == 4)
			{														//飞机子弹击中 
				if (i>=plane_y&&i<=plane_y+50&&j>=plane_x&&j<=plane_x+50)
				{
					score--;
					canvas[i][j] = 0;
				}
			}
		}
	}

	for (int i = height; i; i--)									//敌机子弹从上往下走，需要从下到上扫描 
	{
		for (int j = 0; j <= width; j++)
		{
			if (canvas[i][j] == 4)
			{
				if (bulletTime[i][j] == EnemySpeed/10)				//时间大一些便于控制难度选择
				{
					canvas[i][j] = 0;
					bulletTime[i][j]=0;
					if (i < height)
						canvas[i + 1][j] = 4;
				}
				else
					bulletTime[i][j]++;
			}
		}
	}
		
	for (int i = 1; i <= EnemyNum; i++)								//发射新的子弹 
	{
		if (enemyTime[i] == EnemyBulletTime)
		{
			canvas[enemy_y[i] + 36][enemy_x[i] + 14] = 4;
			enemyTime[i] = 0;
		}
		else
			enemyTime[i]++;

		if (enemyDownTime[i] == EnemyDown)							//敌机下移
		{
			canvas[enemy_y[i]][enemy_x[i]] = 0;
			enemyDownTime[i] = 0;
			if (enemy_y[i] < height - 20)
			{
				enemy_y[i]++;
				canvas[enemy_y[i]][enemy_x[i]] = 3;
			}
			else
			{				
				while (1)											//敌机更新 
				{
					enemy_x[i] = rand() % (width - 35);
					enemy_y[i] = 0;
					if (checkPos(enemy_x[i], enemy_y[i]))
						continue;
					break;
				}
				canvas[enemy_y[i]][enemy_x[i]] = 3;
				canvas[enemy_y[i] + 36][enemy_x[i] + 14] = 4;		//新的敌机子弹 
			}
		}
		else
		{
			enemyDownTime[i]++;
		}
																	//敌机撞上
		if (enemy_y[i] <= plane_y + 35 && enemy_y[i] >= plane_y - 50 && enemy_x[i] <= plane_x + 50 && enemy_x[i] >= plane_x - 35)
		{
			score =score- 2;
			putimage(enemy_x[i], enemy_y[i], &img_zdieenemy, NOTSRCERASE);	
			putimage(enemy_x[i], enemy_y[i], &img_dieenemy, SRCINVERT);
			mciSendString("close enemyExplode", NULL, 0, NULL); 
			mciSendString("open .\\sound\\enemyExplode.wav alias enemyExplode", NULL, 0, NULL); 
			mciSendString("play enemyExplode", NULL, 0, NULL); 
			canvas[enemy_y[i]][enemy_x[i]] = 0;
			while (1)							
			{
				enemy_x[i] = rand() % (width - 35);
				enemy_y[i] = 0;
				if (checkPos(enemy_x[i], enemy_y[i]))
					continue;
				break;
			}
			canvas[enemy_y[i]][enemy_x[i]] = 3;
			canvas[enemy_y[i] + 36][enemy_x[i] + 14] = 4;
			enemyTime[i] = rand() % EnemyBulletTime;
			killSum++;
		}
	}
	canvas[plane_y][plane_x] = 1;									//飞机恢复，防止被其他覆盖 
}


void bk2Control()												//主菜单
{
	MOUSEMSG m;													//鼠标信息
	if (MouseHit())												//鼠标动了
	{
		m = GetMouseMsg();										//获取鼠标信息
		if (m.uMsg == WM_LBUTTONDOWN)							//按下左键
		{
			if (m.x >= 108 && m.x <= 298 && m.y <= 256 && m.y >= 215)
			{													//点击指定位置
				memset(canvas, 0, sizeof(canvas));				//画布清零
				initialization();										//可能是后面游戏结束返回菜单，所以数据重置
				bk_no = 8;										//开始
				bk_change = 1;									//更新控制变量
			}
			else if (m.x >= 108 && m.x <= 298 && m.y <= 330 && m.y >= 283)
			{
				bk_no = 5;										//帮助
				bk_change = 1;
			}
			else if (m.x >= 108 && m.x <= 298 && m.y <= 400 && m.y >= 350)
			{
				bk_no = 4;										//关于
				bk_change = 1;
			}
			else if (m.x >= 108 && m.x <= 298 && m.y <= 471 && m.y >= 420)
				exit(1);										//退出
		}
	}
}


void bk3Control()
{
	if (stop)
	{
		Sleep(1000);												//给游戏者1s反应时间
		stop = 0;
	}
	updateWithInput();
	updateWithoutInput();
	if (score < 0)													//游戏结束
	{
		bk_change = 1;
		bk_no = 7;
		putimage(plane_x, plane_y, &img_zdieplane, NOTSRCERASE);	//飞机击毁图片
		putimage(plane_x, plane_y, &img_dieplane, SRCINVERT);
		mciSendString("close gameover", NULL, 0, NULL);				//失败音效
		mciSendString("open .\\sound\\gameover.mp3 alias gameover", NULL, 0, NULL); 
		mciSendString("play gameover", NULL, 0, NULL);
		Sleep(2000);
		
	}
	if (killSum == EnemySum)										//击灭所有敌人
	{
		bk_change = 1;
		bk_no = 6;
		mciSendString("close victory", NULL, 0, NULL);				//胜利音效
		mciSendString("open .\\sound\\victory.wav alias victory", NULL, 0, NULL);
		mciSendString("play victory", NULL, 0, NULL);
	}
	if (killSum > EnemySum / 4 * 3)									//击杀一定数量敌人后，提高敌人子弹发射速度
		EnemySpeed / 5 * 4;
	else if (killSum > EnemySum / 4 * 2)
		EnemySpeed / 5 * 4;
	else if (killSum > EnemySum / 4 * 1)
		EnemySpeed / 5 * 4;
}


void bk45Control()
{
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x <= 396 && m.x >= 300 && m.y <= 598 && m.y >= 555)
			{													//返回至功能选择
				bk_no = 2;
				bk_change= 1;
			}
		}
	}
}


void bk67Control()
{
	if (bk_no == 6)
	{
		TCHAR sc[5];
		settextstyle(40, 20, _T("黑体"));						//设置字体格式
		_stprintf(sc, _T("%d"), score);
		outtextxy(302, 265, sc);
	}
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x <= 267 && m.x >= 130 && m.y <= 430 && m.y >= 393)	//再来一局
			{
				memset(canvas, 0, sizeof(canvas));
				initialization();
				bk_no = 8;
				bk_change = 1;
			}
			if (m.x <= 282 && m.x >= 114 && m.y <= 497 && m.y >= 461)	//返回主菜单
			{
				bk_no = 2;
				bk_change = 1;
			}
			if (m.x <= 270 && m.x >= 130 && m.y <= 562 && m.y >= 528)	//退出游戏
			{
				exit(1);
			}
		}
	}
	
}


void bk8Control()
{
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x <= 235 && m.x >= 147 && m.y <= 292 && m.y >= 249)//简单
			{														
				EnemySum = enemySum;
				EnemySpeed = enemySpeed;
				EnemyNum = enemyNum;
				EnemyDown = enemyDown;
				bk_no = 3;
				bk_change = 1;
				initialization();
			}
			else if (m.x <= 235 && m.x >= 147 && m.y <= 364 && m.y >= 320)//普通
			{
				EnemySum = enemySum*2;
				EnemySpeed = enemySpeed/2;
				EnemyNum = enemyNum+1;
				EnemyDown = enemyDown/2;
				bk_no = 3;
				bk_change = 1;
				initialization();
			}
			else if (m.x <= 235 && m.x >= 147 && m.y <= 428 && m.y >= 389)//困难
			{
				EnemySum = enemySum*3;
				EnemySpeed = enemySpeed/3;
				EnemyNum = enemyNum+2;
				EnemyDown = enemyDown/3;
				bk_no = 3;
				bk_change = 1;
				initialization();
			}
		}
	}
}


void bk9Control()
{
	MOUSEMSG m;
	settextstyle(40, 20, _T("黑体"));										
	TCHAR sc[5];
	_stprintf(sc, _T("%d"), score);											
	outtextxy(241, 212, sc);

	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x <= 288 && m.x >= 107 && m.y <= 326 && m.y >= 277)		//继续游戏
			{	
				stop = 1;
				bk_no = 3;
				bk_change = 1;
			}
			if (m.x <= 288 && m.x >= 107 && m.y <= 393 && m.y >= 348)		//重新开始
			{
				bk_no = 8;
				bk_change = 1;
			}
			if (m.x <= 315 && m.x >= 85 && m.y <= 470 && m.y >= 421)		//返回主菜单
			{
				bk_no = 2;
				bk_change = 1;
			}
			if (m.x <= 288 && m.x >= 107 && m.y <= 546 && m.y >= 500)		//退出游戏
			{
				exit(1);
			}
		}
	}
}


void control()
{
	if (bk_no == 2)
		bk2Control();
	if (bk_no == 4 || bk_no == 5)
		bk45Control();
	if (bk_no == 3)
		bk3Control();
	if (bk_no == 6||bk_no==7)
		bk67Control();
	if (bk_no == 8)
		bk8Control();
	if (bk_no == 9)
		bk9Control();
}
