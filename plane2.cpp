#include<iostream>
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include<algorithm>
#include<stdlib.h>
#include<cstring>

#pragma comment(lib,"Winmm.lib")							// ���� Windows Multimedia API

#pragma warning(disable : 4996)								//����4996����

/*Dev�������в��ˣ���Ҫ��VS�����½���Ŀ�������븴�ƽ�ȥ
  ���轫�����е��ַ�����Unicode�ĳ�ʹ�ö��ֽ��ַ�����Ҫ������
  �Ҽ�����������Ŀ������->�������->���->�߼�->�ҵ��ַ���->�ĳ�ʹ�ö��ֽ��ַ���*/

using namespace std;

#define height 598											//���ڴ�С�� 55*35 
#define width 396											//�����С 
#define enemySum 20	 										//�������� 
#define enemyNum 3											//һ�γ��ֵĵ������� 
#define enemySpeed 20										//���˷����ӵ����ٶ� 
#define EnemyBulletTime 2000								//һ�������ӵ������ʱ���� 
#define planeSpeed 2										//�����ӵ��ٶ�
#define enemyDown 25										//�л�������ٶ�
int EnemySum;												//�����ѶȲ���
int EnemySpeed;
int EnemyNum;
int EnemyDown;

IMAGE img_background1;										//��ӭ����
IMAGE img_background2;										//����ѡ�񱳾�
IMAGE img_background3;										//��Ϸ����
IMAGE img_background4;										//���ڱ���
IMAGE img_background5;										//��������
IMAGE img_background6;										//ʤ����Ϸ��������
IMAGE img_background7;										//ʧ����Ϸ��������
IMAGE img_background8;										//�Ѷ�ѡ�񱳾�
IMAGE img_background9;										//��Ϸ��ͣ����
IMAGE img_plane;											//�ɻ�
IMAGE img_zplane;											//����ͼ�ɻ�
IMAGE img_enemy;											//�л�
IMAGE img_zenemy;											//����ͼ�л�							
IMAGE img_bullet;											//�ӵ�
IMAGE img_zbullet;											//����ͼ�ӵ�
IMAGE img_ebullet;											//�л��ӵ�
IMAGE img_zebullet;											//����ͼ�л�
IMAGE img_dieenemy;											//�л�����
IMAGE img_zdieenemy;										//����ͼ�л�����
IMAGE img_dieplane;											//�ɻ�����
IMAGE img_zdieplane;										//����ͼ�ɻ�����

int bk_no;													//�����л����
bool bk_change;												//�Ƿ񻻱���
int plane_x, plane_y;										//�ɻ�����
int canvas[1000][1000];										//����
int bulletTime[1000][1000];									//���Ƶл��ӵ������ٶ�
int enemy_x[20], enemy_y[20];								//�л����� 
int enemyDownTime[20];										//�ɻ�����ʱ��
int enemyTime[20];											//�л��ӵ�����ʱ���� 
int score;													//�÷�
int killSum;												//��ɱ���˵�����
bool stop;													//���ƻ����Ƿ�ͣ��

void initialization();										//���ݳ�ʼ��
void startup();												//����ͼƬ
bool checkPos(int x, int y);								//���л�λ���Ƿ��ظ�
void bkShow();												//������ʾ
void gameShow();											//��Ϸ��ʾ
void show();												//��ʾ
void updateWithInput();										//�в�������
void updateWithoutInput();									//�޲�������
void bk3Control();											//����3
void bk45Control();											//����4 5
void bk67Control();											//����6 7
void bk8Control();											//����8
void control();												//��������

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
	memset(canvas, 0, sizeof(canvas));						//�������
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
		enemy_x[i] = rand() % (width - 35);						//ע��л����� 
		enemy_y[i] = 0;
		if (checkPos(enemy_x[i], enemy_y[i]))					//��ֹ�ص�
		{
			i--;
			continue;
		}
		canvas[enemy_y[i]][enemy_x[i]] = 3;
		canvas[enemy_y[i] + 36][enemy_x[i] + 14] = 4;
		enemyTime[i] = rand() % EnemyBulletTime;
	}
}


void startup()													//���ݳ�ʼ��
{
	initgraph(396, 598);										//���廭����С
	loadimage(&img_background1, _T(".\\picture\\background1.png"));//����ͼƬ
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
	mciSendString("open  .\\sound\\gamemusic.mp3 alias bkmusic", NULL, 0, NULL);//�򿪱�������		//��Ҫ�ĳ�ʹ�ö��ֽ��ַ���
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // ѭ������
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
	BeginBatchDraw();													//��ʼ���Ʊ��� ��ֹ����ͼƬ��˸
	putimage(0, 0, &img_background3);									//�����������룬��ֹ����
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
	settextstyle(18, 9, _T("����"));										//���������ʽ
	TCHAR s[] = _T("�÷֣�");
	outtextxy(5, 570, s);													//��ʾ�÷�
	TCHAR sc[5];
	_stprintf(sc, _T("%d"), score);											//ת������
	outtextxy(50, 570, sc);
	EndBatchDraw();															//��������
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


void updateWithInput()										//�в������� 
{
	char input;
	if (kbhit())  											//�ж��Ƿ�������
	{
		input = getch();
		if (input == 'a')									//���Ʋ������߽� 
		{
			canvas[plane_y][plane_x] = 0;
			if(plane_x>0)
				plane_x-=2;									//1�ƶ�̫��
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
		if (input == ' ')  										//�����ӵ�
		{
			canvas[plane_y-28][plane_x+18] = 2;					//�ӵ��ĳ�ʼλ���ڷɻ������Ϸ�
			mciSendString("close firing", NULL, 0, NULL);		// �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString("open .\\sound\\firing.wav alias firing", NULL, 0, NULL); // ������
			mciSendString("play firing", NULL, 0, NULL);		// ������һ��
		}
		if (input == 0x1b)
		{
			bk_no = 9;
			bk_change = 1;
		}
	}
}


void updateWithoutInput()										//�޲������� 
{
	for (int i = 0; i <= height; i++)
	{
		for (int j = 0; j <= width; j++)
		{
			if (canvas[i][j] == 2)								//�ж��ӵ����ел� 
			{
				for (int k = 1; k <= EnemyNum; k++)				//����л� 
				{
					if (((j >= enemy_x[k] - 5 && j <= enemy_x[k] + 35) &&(i >= enemy_y[k]&&i == enemy_y[k] + 35)))
					{
						putimage(enemy_x[k], enemy_y[k], &img_zdieenemy, NOTSRCERASE);	//�л�����ͼƬ
						putimage(enemy_x[k], enemy_y[k], &img_dieenemy, SRCINVERT);
						mciSendString("close enemyExplode", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
						mciSendString("open .\\sound\\enemyExplode.wav alias enemyExplode", NULL, 0, NULL); // ������
						mciSendString("play enemyExplode", NULL, 0, NULL); // ������һ��
						score++;
						canvas[enemy_y[k]][enemy_x[k]] = 0;
						while (1)								//�л����� 
						{
							enemy_x[k] = rand() % (width - 35);
							enemy_y[k] = 0;
							if (checkPos(enemy_x[k],enemy_y[k]))
								continue;
							break;
						}
						canvas[enemy_y[k]][enemy_x[k]] = 3;
						canvas[enemy_y[k] + 36][enemy_x[k] + 14] = 4;//�µĵл��ӵ� 
						canvas[i][j] = 0;							//�ӵ���ʧ 
						enemyTime[i] = rand() % EnemyBulletTime;
						killSum++;
					}
				}
			}
			static int plspeed = 1;
			if (canvas[i][j] == 2)									//�ӵ����� 
			{
				if (plspeed == planeSpeed)
				{
					canvas[i][j] = 0;
					if (i - 1 >= 0)									//���ɳ��߽�
						canvas[i - 1][j] = 2;
					plspeed = 1;
				}
				else
					plspeed++;				
			}
			if (canvas[i][j] == 4)
			{														//�ɻ��ӵ����� 
				if (i>=plane_y&&i<=plane_y+50&&j>=plane_x&&j<=plane_x+50)
				{
					score--;
					canvas[i][j] = 0;
				}
			}
		}
	}

	for (int i = height; i; i--)									//�л��ӵ����������ߣ���Ҫ���µ���ɨ�� 
	{
		for (int j = 0; j <= width; j++)
		{
			if (canvas[i][j] == 4)
			{
				if (bulletTime[i][j] == EnemySpeed/10)				//ʱ���һЩ���ڿ����Ѷ�ѡ��
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
		
	for (int i = 1; i <= EnemyNum; i++)								//�����µ��ӵ� 
	{
		if (enemyTime[i] == EnemyBulletTime)
		{
			canvas[enemy_y[i] + 36][enemy_x[i] + 14] = 4;
			enemyTime[i] = 0;
		}
		else
			enemyTime[i]++;

		if (enemyDownTime[i] == EnemyDown)							//�л�����
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
				while (1)											//�л����� 
				{
					enemy_x[i] = rand() % (width - 35);
					enemy_y[i] = 0;
					if (checkPos(enemy_x[i], enemy_y[i]))
						continue;
					break;
				}
				canvas[enemy_y[i]][enemy_x[i]] = 3;
				canvas[enemy_y[i] + 36][enemy_x[i] + 14] = 4;		//�µĵл��ӵ� 
			}
		}
		else
		{
			enemyDownTime[i]++;
		}
																	//�л�ײ��
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
	canvas[plane_y][plane_x] = 1;									//�ɻ��ָ�����ֹ���������� 
}


void bk2Control()												//���˵�
{
	MOUSEMSG m;													//�����Ϣ
	if (MouseHit())												//��궯��
	{
		m = GetMouseMsg();										//��ȡ�����Ϣ
		if (m.uMsg == WM_LBUTTONDOWN)							//�������
		{
			if (m.x >= 108 && m.x <= 298 && m.y <= 256 && m.y >= 215)
			{													//���ָ��λ��
				memset(canvas, 0, sizeof(canvas));				//��������
				initialization();										//�����Ǻ�����Ϸ�������ز˵���������������
				bk_no = 8;										//��ʼ
				bk_change = 1;									//���¿��Ʊ���
			}
			else if (m.x >= 108 && m.x <= 298 && m.y <= 330 && m.y >= 283)
			{
				bk_no = 5;										//����
				bk_change = 1;
			}
			else if (m.x >= 108 && m.x <= 298 && m.y <= 400 && m.y >= 350)
			{
				bk_no = 4;										//����
				bk_change = 1;
			}
			else if (m.x >= 108 && m.x <= 298 && m.y <= 471 && m.y >= 420)
				exit(1);										//�˳�
		}
	}
}


void bk3Control()
{
	if (stop)
	{
		Sleep(1000);												//����Ϸ��1s��Ӧʱ��
		stop = 0;
	}
	updateWithInput();
	updateWithoutInput();
	if (score < 0)													//��Ϸ����
	{
		bk_change = 1;
		bk_no = 7;
		putimage(plane_x, plane_y, &img_zdieplane, NOTSRCERASE);	//�ɻ�����ͼƬ
		putimage(plane_x, plane_y, &img_dieplane, SRCINVERT);
		mciSendString("close gameover", NULL, 0, NULL);				//ʧ����Ч
		mciSendString("open .\\sound\\gameover.mp3 alias gameover", NULL, 0, NULL); 
		mciSendString("play gameover", NULL, 0, NULL);
		Sleep(2000);
		
	}
	if (killSum == EnemySum)										//�������е���
	{
		bk_change = 1;
		bk_no = 6;
		mciSendString("close victory", NULL, 0, NULL);				//ʤ����Ч
		mciSendString("open .\\sound\\victory.wav alias victory", NULL, 0, NULL);
		mciSendString("play victory", NULL, 0, NULL);
	}
	if (killSum > EnemySum / 4 * 3)									//��ɱһ���������˺���ߵ����ӵ������ٶ�
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
			{													//����������ѡ��
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
		settextstyle(40, 20, _T("����"));						//���������ʽ
		_stprintf(sc, _T("%d"), score);
		outtextxy(302, 265, sc);
	}
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x <= 267 && m.x >= 130 && m.y <= 430 && m.y >= 393)	//����һ��
			{
				memset(canvas, 0, sizeof(canvas));
				initialization();
				bk_no = 8;
				bk_change = 1;
			}
			if (m.x <= 282 && m.x >= 114 && m.y <= 497 && m.y >= 461)	//�������˵�
			{
				bk_no = 2;
				bk_change = 1;
			}
			if (m.x <= 270 && m.x >= 130 && m.y <= 562 && m.y >= 528)	//�˳���Ϸ
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
			if (m.x <= 235 && m.x >= 147 && m.y <= 292 && m.y >= 249)//��
			{														
				EnemySum = enemySum;
				EnemySpeed = enemySpeed;
				EnemyNum = enemyNum;
				EnemyDown = enemyDown;
				bk_no = 3;
				bk_change = 1;
				initialization();
			}
			else if (m.x <= 235 && m.x >= 147 && m.y <= 364 && m.y >= 320)//��ͨ
			{
				EnemySum = enemySum*2;
				EnemySpeed = enemySpeed/2;
				EnemyNum = enemyNum+1;
				EnemyDown = enemyDown/2;
				bk_no = 3;
				bk_change = 1;
				initialization();
			}
			else if (m.x <= 235 && m.x >= 147 && m.y <= 428 && m.y >= 389)//����
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
	settextstyle(40, 20, _T("����"));										
	TCHAR sc[5];
	_stprintf(sc, _T("%d"), score);											
	outtextxy(241, 212, sc);

	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x <= 288 && m.x >= 107 && m.y <= 326 && m.y >= 277)		//������Ϸ
			{	
				stop = 1;
				bk_no = 3;
				bk_change = 1;
			}
			if (m.x <= 288 && m.x >= 107 && m.y <= 393 && m.y >= 348)		//���¿�ʼ
			{
				bk_no = 8;
				bk_change = 1;
			}
			if (m.x <= 315 && m.x >= 85 && m.y <= 470 && m.y >= 421)		//�������˵�
			{
				bk_no = 2;
				bk_change = 1;
			}
			if (m.x <= 288 && m.x >= 107 && m.y <= 546 && m.y >= 500)		//�˳���Ϸ
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
