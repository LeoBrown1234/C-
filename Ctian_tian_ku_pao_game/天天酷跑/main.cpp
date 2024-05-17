/*
������ܿ�����־
1.������Ŀ
2.�����ز�
3.������Ϸ����
  ʵ�ʵĿ�������
  ���ڳ�ѧ�ߣ���õķ�ʽ��������û���������

  ѡ��ͼ�ο������������
  ������ܣ��ǻ��ڡ�easyx��ͼ�ο��
  1)������Ϸ����
  2)�����Ϸ����
   a.3�ر�����ͬ���ٶ�ͬʱ�ƶ�
   b.ѭ������������ʵ��
  3)ʵ����Ϸ����
   a.���ر�����Դ
   b.��Ⱦ(����֪ʶ�����꣬�����Ͻ�����)
   ��������:����ͼƬ��png��ʽͼƬ���ֺ�ɫ
   �������������tools.cpp��tools.h
4.ʵ����ҵı���
5.ʵ����ҵ���Ծ

*/

#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include "tools.h"
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396


IMAGE imgBgs[3];//����ͼƬ
int bgX[3];//����ͼƬ��x����
int bgSpeed[3] = { 1,2,4 };//�����ƶ��ٶȣ���λ����

IMAGE imgHeros[12];//�洢���ͼ��
int heroX;//��ҵ�x����
int heroY;//��ҵ�y����
int heroIndex;//��ұ��ܵ�ͼƬ֡���

bool heroJump;//��ʾ���������Ծ

int jumpHeightMax;//��Ծ��߼���
int heroJumpOff;//��Ծ�ٶȿ��ƣ�/���أ�
int update;//��ʾ�Ƿ���Ҫ����ˢ�»���

IMAGE imgTortoise;//С�ڹ�
int torToiseX;//С�ڹ��ˮƽ����
int torToiseY;//С�ڹ�Ĵ�ֱ����
bool torToiseExist;//��ǰ�����Ƿ���С�ڹ�

//��Ϸ�ĳ�ʼ��
void init() {
	//������Ϸ����
	initgraph(WIN_WIDTH,WIN_HEIGHT);//�������ز�ȷ����С
	

	//���ر�����Դ
	char name[64];
	for (int i = 0; i < 3;i++) {
		//  res/bg001.png  res/bg002.png  res/bg003.png
		sprintf(name, "res/bg%03d.png", i+1);
		loadimage(&imgBgs[i],name);

		bgX[i] = 0;
	}
	//������ұ��ܵ�ͼƬ֡�ز�
	for (int i = 0; i < 12; i++) {
		//res/hero1.png ...res/hero12.png
		sprintf(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}

	//������ҵĳ�ʼλ��
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;

	heroJump = false;
	jumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	heroJumpOff = -4;

	update = true;

	//����С�ڹ��ز�
	loadimage(&imgTortoise, "res/t1.png");
	torToiseExist = false;//��ʼ������ʼûС�ڹ�
	torToiseY = 345 - imgTortoise.getheight()+5;
}

void fly() {
	for (int i = 0; i < 3; i++) {
		bgX[i] -= bgSpeed[i];
		if (bgX[i] < -WIN_WIDTH) {
			bgX[i] = 0;
		}
	}


	//ʵ����Ծ
	if (heroJump) {
		if (heroY < jumpHeightMax) {
			heroJumpOff = 4;
		}

		heroY += heroJumpOff;

		if (heroY > 345 - imgHeros[0].getheight()) {
			heroJump = false;
			heroJumpOff = -4;
		}
	}
	else {//����Ծ
		heroIndex = (heroIndex + 1) % 12;
	}
	//����С�ڹ�
	static int frameCount = 0;
	static int torToiseFre = 100;
	frameCount++;
	if (frameCount > torToiseFre) {
		frameCount = 0;//����һ��С�ڹ꣬����һ��
		if (!torToiseExist) {//û��С�ڹ�ų���һ��С�ڹ�
			torToiseExist = true;
			torToiseX = WIN_WIDTH;

			//����С�ڹ��Ƶ�ʱ仯һ��
			torToiseFre =200+rand()%300;//200..500/200+0~300
		}
	}
	if (torToiseExist) {
	    torToiseX -= bgSpeed[2];
	    if (torToiseX < -imgTortoise.getwidth()) {//С�ڹ���ȫ�Ƴ���Ļ����ʧ
		torToiseExist = false;
	    }
    }
}

//��Ⱦ����Ϸ������
void updateBg(){
	putimagePNG2(bgX[0], 0, &imgBgs[0]);//(x,y)
	putimagePNG2(bgX[1], 119, &imgBgs[1]);//(x,y)
	putimagePNG2(bgX[2], 330, &imgBgs[2]);//(x,y)
}

void jump() {
	heroJump = true;
	update = true;
}

void keyEvent() {
	char ch;
	if (_kbhit()) {//�жϰ�������,�а�������ʱ��_kbhit()����true
		ch=_getch();//_getch()����Ҫ���»س�����ֱ�Ӷ�ȡ
		if (ch == ' ') {
			jump();
		}
	}

}

void updateEnemy() {
	//��ȾС�ڹ�
	if (torToiseExist) {
		putimagePNG2(torToiseX, torToiseY,WIN_WIDTH,&imgTortoise);

	}
}

int main(void) {
	init();

	int timer = 0;

	while (1) {
		keyEvent();

		timer +=getDelay();

		if (timer > 30) {

			timer = 0;

			update = true;

		}
		if (update) {
			update = false;

			BeginBatchDraw();//������Ⱦ��׼��һ���Է��� updateBg()

			updateBg();

			putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);

			updateEnemy();

			EndBatchDraw();//�������棬����

			fly();
		}
		//Sleep(30);����  ����Чʱ��״̬����ȡ��
	}
	system("pause");
	return 0;
}

