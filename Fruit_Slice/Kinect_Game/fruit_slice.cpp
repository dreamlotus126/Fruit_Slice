#include <graphics.h>
#include <cmath>
#include <list>
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

const int WINDOW_WID = 800; // ���ڿ��
const int WINDOW_HEI = 500; // ���ڸ߶�
const int CURSOR_LEN = 10; // ���׼�Ǵ�ϸ
const double PAI = 3.1415926; // �Լ������pai
const double TimeofCreateCD = 0.5; // �����߶ε���ȴʱ��
const double LineExist = 2.0; // ����������߶ε�����ʱ��
const double LineDown = 0.05; // ÿ֡�߶ε�����ʱ�������
bool mIsRunning; // ʾ���Ƿ������еı�־

double Mousex; // ���ĺ�����
double Mousey; // ����������
double MouseDegree; // ���׼�ǵ���ת�Ƕ�(Ĭ������Ϊ����ת������Ӧע��ȡ������)
double MouseAnguSpeed; // ���׼��ÿ֡���ӵ���ת�Ƕ�
bool MouseClick; // �������Ƿ���

double CreateCD; // ���ڼ�¼��ǰ�������߶Ρ���CD�Ƿ���ȴ���

IMAGE img_bk;//����
IMAGE img_apple, img_apple_mask;       // ƻ��ͼƬ������ͼ
IMAGE img_banana, img_banana_mask; // �㽶ͼƬ������ͼ
IMAGE img_melon, img_melon_mask;     // ����ͼƬ������ͼ
IMAGE img_peach, img_peach_mask;      // ����ͼƬ������ͼ
IMAGE img_berry, img_berry_mask;       // ��ݮͼƬ������ͼ\

double vx=1, vy=10;
double apple_vx = 1, apple_vy = 8;
double banana_vx = 1, banana_vy = 10;
double melon_vx = 1, melon_vy = 7;
double peach_vx = -1, peach_vy = 9;
double berry_vx = -2, berry_vy = 11;

double apple_x, apple_y;//ƻ��λ��
double banana_x, banana_y;
double melon_x, melon_y;
double peach_x, peach_y;
double berry_x, berry_y;

double xspan = 0;//�����ˮ������
double yspan = 0;//
double miss;//���
double score;//�÷�
double rate;//��ȷ��
TCHAR text[20];

class Point
{
public:
	int mPosx;
	int mPosy;
	double mExist;

public:
	Point();
	Point(int x, int y, double e);
	~Point();

	void CountDown(double t);
}; // �Զ���ĺ�������
Point::Point()
{
	mPosx = 0;
	mPosy = 0;
	mExist = 2.0;
}
Point::Point(int x, int y, double e)
{
	mPosx = x;
	mPosy = y;
	mExist = e;
}
Point::~Point()
{

}
void Point::CountDown(double t)
{
	mExist -= t;
}
list<Point> points; // ����list����Ԫ���Զ�������������������

void Initialize(); // ��ʼ������(�����½����ڻ���һЩ��������)
void Update(); // ����Ҫ��ʾ�Ķ����������(�����߶δ�ϸ)
void ProcessInput(); // �����������(Easyx�Դ���������������ʽ�ģ���Ҫת��)
void GenerateOutput(); // ��ӡ����Ļ
void RunLoop(); // ѭ������
void ShutDown(); // ��������
void Delay(DWORD ms); // ÿ֡ͣ��ʱ��

int main()
{
	Initialize();
	RunLoop();
	ShutDown();

	return 0;
}

void Initialize()
{
	initgraph(WINDOW_WID, WINDOW_HEI);
	BeginBatchDraw();
	loadimage(&img_bk, _T("../wood.jpg"), WINDOW_WID, WINDOW_HEI);

	loadimage(&img_apple, _T("../apple.bmp"));
	loadimage(&img_apple_mask, _T("../apple_mask.bmp"));

	loadimage(&img_banana, _T("../banana.bmp"));
	loadimage(&img_banana_mask, _T("../banana_mask.bmp"));

	loadimage(&img_melon, _T("../melon.bmp"));
	loadimage(&img_melon_mask, _T("../melon_mask.bmp"));

	loadimage(&img_peach, _T("../peach.bmp"));
	loadimage(&img_peach_mask, _T("../peach_mask.bmp"));

	loadimage(&img_berry, _T("../strawberry.bmp"));
	loadimage(&img_berry_mask, _T("../strawberry_mask.bmp"));
	mIsRunning = true;
	Mousex = 0.0;
	Mousey = 0.0;
	MouseClick = false;
	MouseDegree = 0.0;
	MouseAnguSpeed = 0.0;

	CreateCD = 1.0;
	apple_y = WINDOW_HEI + 100;
	banana_y = WINDOW_HEI + 100;
	melon_y = WINDOW_HEI + 100;
	peach_y = WINDOW_HEI + 100;
	berry_y = WINDOW_HEI + 100;
	apple_x = 150;
	banana_x = 300;
	melon_x = 200;
	peach_x = 600;
	berry_x = 650;
	score = 0;
	miss = 0;
	rate = 0;
}

void Update()
{
	if (MouseClick) // ȡ��ע�����������׼����ת
	{
		//MouseAnguSpeed += 0.05;
	}
	else
	{
		if (MouseAnguSpeed > 0.0)
		{
			MouseAnguSpeed -= 0.01;
		}
		if (MouseAnguSpeed < 0.0)
		{
			MouseAnguSpeed = 0.0;
		}
	}
	MouseDegree += MouseAnguSpeed;

	// Update tail line effect
	CreateCD -= 0.1; // ��ò�Ҫ���������߶Σ���������������
	if (MouseClick && CreateCD < 0.0)
	{
		points.push_back(Point(Mousex, Mousey, LineExist));
		CreateCD = TimeofCreateCD; // ���´����߶ε���ȴʱ��
	}
	if (points.size()) // ���ڵ��вż���Ƿ������߶�
	{
		for (auto iter = points.begin(); iter != points.end(); ++iter)
		{
			iter->CountDown(LineDown);
		}
		if (points.begin()->mExist < 0.0)
		{
			points.pop_front(); // ���߶��Ѿ���ϸ����������
		}
	}
}

void ProcessInput()
{
	MOUSEMSG m;
	while (MouseHit())
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			Mousex = m.x;
			Mousey = m.y;
			break;

		case WM_LBUTTONDOWN:
			MouseClick = true;
			break;

		case WM_LBUTTONUP:
			MouseClick = false;
			break;

		/*case WM_RBUTTONDOWN:
			mIsRunning = false;
			break;*/

		case WM_CLOSE:
			ShutDown();
			break;
		}
	}
}

void GenerateOutput()
{
	cleardevice(); // ˢ����Ļ
	putimage(0, 0, &img_bk);

	setbkmode(TRANSPARENT);//��ʾ͸������
	_stprintf_s(text, _T("score��%.0f"), score);//��scoreת�����ַ���
	settextcolor(WHITE);//����������ɫΪ��ɫ
	settextstyle(20,0,_T("Verdana"));//��������
	outtextxy(650, 30, text);//�������

	setbkmode(TRANSPARENT);//��ʾ͸������
	_stprintf_s(text, _T("miss��%.0f"), miss);//��scoreת�����ַ���
	settextcolor(WHITE);//����������ɫΪ��ɫ
	settextstyle(20, 0, _T("Verdana"));//��������
	outtextxy(35, 30, text);//�������

	if (score == 0 && miss == 0)
		rate = 0;
	else
		rate = score / (score + miss) * 100;
	setbkmode(TRANSPARENT);//��ʾ͸������
	_stprintf_s(text, _T("rate��%.2f %%"), rate);//��scoreת�����ַ���
	settextcolor(WHITE);//����������ɫΪ��ɫ
	settextstyle(20, 0, _T("Verdana"));//��������
	outtextxy(650, 50, text);//�������

	// Draw Cursor
	for (int i = 0; i < 4; ++i)
	{
		int tempx1 = Mousex + CURSOR_LEN * cos(MouseDegree + double(i) * 2.0 * PAI / 4.0);
		int tempy1 = Mousey + CURSOR_LEN * sin(MouseDegree + double(i) * 2.0 * PAI / 4.0);
		int tempx2 = Mousex + 2.0 * CURSOR_LEN * cos(MouseDegree + double(i) * 2.0 * PAI / 4.0);
		int tempy2 = Mousey + 2.0 * CURSOR_LEN * sin(MouseDegree + double(i) * 2.0 * PAI / 4.0);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
		line(tempx1, tempy1, tempx2, tempy2); // ����׼���߶�
	}

	// Draw tail
	if (points.size() >= 2)
	{
		for (auto iter = points.begin(); iter != points.end(); ++iter)
		{
			int tempx1 = iter->mPosx;
			int tempy1 = iter->mPosy;
			iter++;
			if (iter != points.end())
			{
				int tempx2 = iter->mPosx;
				int tempy2 = iter->mPosy;
				setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, int(7.0 * (iter->mExist / LineExist)));
				line(tempx1, tempy1, tempx2, tempy2);
			}
			iter--;
		}
	}
	putimage(banana_x, banana_y, &img_banana_mask, SRCAND);
	putimage(banana_x, banana_y, &img_banana, SRCPAINT);

	putimage(apple_x,apple_y, &img_apple_mask, SRCAND);
	putimage(apple_x,apple_y, &img_apple, SRCPAINT);

	putimage(melon_x,melon_y, &img_melon_mask, SRCAND);
	putimage(melon_x,melon_y, &img_melon, SRCPAINT);

	putimage(peach_x,peach_y, &img_peach_mask, SRCAND);
	putimage(peach_x,peach_y, &img_peach, SRCPAINT);

	putimage(berry_x,berry_y, &img_berry_mask, SRCAND);
	putimage(berry_x,berry_y, &img_berry, SRCPAINT);
	FlushBatchDraw(); // ������ͼ(����˫���壿)
}

void location_banana()
{
	banana_x += banana_vx;
	banana_vy -= 0.1;
	banana_y -= banana_vy;
	xspan = abs(Mousex - (banana_x + 65));
	yspan = abs(Mousey - (banana_y + 30));
	if (banana_y >= 600)
	{
		banana_x = 300 + rand() % 100;
		banana_y = WINDOW_HEI + 100;
		banana_vy = 8 + rand() % 3;
		banana_vx = -2 + rand() % 5;
		miss++;
		
	}
	if (MouseClick && xspan<=65 && yspan<=25)
	{
		banana_x = 300 + rand() % 100;
		banana_y = WINDOW_HEI + 100;
		banana_vy = 8 + rand() % 3;
		banana_vx = -2 + rand() % 5;
		score++;
		
	}
}

void location_apple()
{
	apple_x += apple_vx;
	apple_vy -= 0.1;
	apple_y -= apple_vy;
	xspan = abs(Mousex - (apple_x + 35));
	yspan = abs(Mousey - (apple_y + 35));
	if (apple_y >= 600)
	{
		apple_x = 100 + rand() % 50;
		apple_y = WINDOW_HEI + 100;
		apple_vy = 9 + rand() % 3;
		apple_vx = 2 + rand() % 3;
		miss++;
	}
	if (MouseClick && xspan <= 35 && yspan <= 35)
	{
		apple_x = 100 + rand() % 50;
		apple_y = WINDOW_HEI + 100;
		apple_vy = 9 + rand() % 3;
		apple_vx = 2 + rand() % 3;
		score++;
	}
}

void location_melon()
{
	melon_x += melon_vx;
	melon_vy -= 0.1;
	melon_y -= melon_vy;
	xspan = abs(Mousex - (melon_x + 50));
	yspan = abs(Mousey - (melon_y + 40));
	if (melon_y >= 600)
	{
		melon_x = 200 + rand() % 100;
		melon_y = WINDOW_HEI + 100;
		melon_vy = 7.5 + rand() % 2;
		melon_vx = -2 + rand() % 5;
		miss++;
	}
	if (MouseClick && xspan <= 55 && yspan <= 45)
	{
		melon_x = 200 + rand() % 100;
		melon_y = WINDOW_HEI + 100;
		melon_vy = 7.5 + rand() % 2;
		melon_vx = -2 + rand() % 5;
		score++;
	}
}

void location_peach()
{
	peach_x += peach_vx;
	peach_vy -= 0.1;
	peach_y -= peach_vy;
	xspan = abs(Mousex - (peach_x + 30));
	yspan = abs(Mousey - (peach_y + 30));
	if (peach_y >= 600)
	{
		peach_x = 400 + rand() % 100;
		peach_y = WINDOW_HEI + 100;
		peach_vy = 8 + rand() % 3;
		peach_vx = -2 + rand() % 5;
		miss++;
	}
	if (MouseClick && xspan <= 35 && yspan <= 35)
	{
		peach_x = 400 + rand() % 100;
		peach_y = WINDOW_HEI + 100;
		peach_vy = 8 + rand() % 3;
		peach_vx = -2 + rand() % 5;
		score++;
	}
}

void location_berry()
{
	berry_x += berry_vx;
	berry_vy -= 0.1;
	berry_y -= berry_vy;
	xspan = abs(Mousex - (berry_x + 30));
	yspan = abs(Mousey - (berry_y + 30));
	if (berry_y >= 600)
	{
		berry_x = 550 + rand() % 100;
		berry_y = WINDOW_HEI + 100;
		berry_vy = 10 + rand() % 2;
		berry_vx = -1 - rand() % 4;
		miss++;
	}
	if (MouseClick && xspan <= 35 && yspan <= 35)
	{
		berry_x = 550 + rand() % 100;
		berry_y = WINDOW_HEI + 100;
		berry_vy = 10 + rand() % 2;
		berry_vx = -1 - rand() % 4;
		score++;
	}
}

void RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput(); // ��������
		Update(); // ���¶���
		location_banana();//����ˮ��λ��
		location_apple();
		location_melon();
		location_peach();
		location_berry();
		GenerateOutput(); // ��ʾ	
		Delay(10); // ��ʾһ��ʱ��
	}
}

void ShutDown()
{
	EndBatchDraw();
	closegraph();
}

void Delay(DWORD ms)
{
	static DWORD oldtime = GetTickCount();

	while (GetTickCount() - oldtime < ms)
		Sleep(1);

	oldtime = GetTickCount();
}
