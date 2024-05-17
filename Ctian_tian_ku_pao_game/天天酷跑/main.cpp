/*
天天酷跑开发日志
1.创建项目
2.导入素材
3.创建游戏界面
  实际的开发流程
  对于初学者，最好的方式，建议从用户界面入手

  选择图形库或者其它引擎
  天天酷跑，是基于“easyx”图形库的
  1)创建游戏窗口
  2)设计游戏背景
   a.3重背景不同的速度同时移动
   b.循环滚动背景的实现
  3)实现游戏背景
   a.加载背景资源
   b.渲染(背景知识：坐标，定左上角坐标)
   遇到问题:背景图片的png格式图片出现黑色
   解决方案：导入tools.cpp和tools.h
4.实现玩家的奔跑
5.实现玩家的跳跃

*/

#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include "tools.h"
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396


IMAGE imgBgs[3];//背景图片
int bgX[3];//背景图片的x坐标
int bgSpeed[3] = { 1,2,4 };//背景移动速度，单位像素

IMAGE imgHeros[12];//存储玩家图像
int heroX;//玩家的x坐标
int heroY;//玩家的y坐标
int heroIndex;//玩家奔跑的图片帧序号

bool heroJump;//表示玩家正在跳跃

int jumpHeightMax;//跳跃最高极限
int heroJumpOff;//跳跃速度控制（/像素）
int update;//表示是否需要马上刷新画面

IMAGE imgTortoise;//小乌龟
int torToiseX;//小乌龟的水平坐标
int torToiseY;//小乌龟的垂直坐标
bool torToiseExist;//当前窗口是否有小乌龟

//游戏的初始化
void init() {
	//创建游戏窗口
	initgraph(WIN_WIDTH,WIN_HEIGHT);//由美工素材确定大小
	

	//加载背景资源
	char name[64];
	for (int i = 0; i < 3;i++) {
		//  res/bg001.png  res/bg002.png  res/bg003.png
		sprintf(name, "res/bg%03d.png", i+1);
		loadimage(&imgBgs[i],name);

		bgX[i] = 0;
	}
	//加载玩家奔跑的图片帧素材
	for (int i = 0; i < 12; i++) {
		//res/hero1.png ...res/hero12.png
		sprintf(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}

	//设置玩家的初始位置
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;

	heroJump = false;
	jumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	heroJumpOff = -4;

	update = true;

	//加载小乌龟素材
	loadimage(&imgTortoise, "res/t1.png");
	torToiseExist = false;//初始化，开始没小乌龟
	torToiseY = 345 - imgTortoise.getheight()+5;
}

void fly() {
	for (int i = 0; i < 3; i++) {
		bgX[i] -= bgSpeed[i];
		if (bgX[i] < -WIN_WIDTH) {
			bgX[i] = 0;
		}
	}


	//实现跳跃
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
	else {//不跳跃
		heroIndex = (heroIndex + 1) % 12;
	}
	//创建小乌龟
	static int frameCount = 0;
	static int torToiseFre = 100;
	frameCount++;
	if (frameCount > torToiseFre) {
		frameCount = 0;//出现一次小乌龟，清零一次
		if (!torToiseExist) {//没有小乌龟才出现一个小乌龟
			torToiseExist = true;
			torToiseX = WIN_WIDTH;

			//产生小乌龟后频率变化一下
			torToiseFre =200+rand()%300;//200..500/200+0~300
		}
	}
	if (torToiseExist) {
	    torToiseX -= bgSpeed[2];
	    if (torToiseX < -imgTortoise.getwidth()) {//小乌龟完全移除屏幕后消失
		torToiseExist = false;
	    }
    }
}

//渲染“游戏背景”
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
	if (_kbhit()) {//判断按键输入,有按键按下时，_kbhit()返回true
		ch=_getch();//_getch()不需要按下回车即可直接读取
		if (ch == ' ') {
			jump();
		}
	}

}

void updateEnemy() {
	//渲染小乌龟
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

			BeginBatchDraw();//缓存渲染，准备一次性发出 updateBg()

			updateBg();

			putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);

			updateEnemy();

			EndBatchDraw();//结束缓存，发出

			fly();
		}
		//Sleep(30);休眠  有无效时间状态，遂取缔
	}
	system("pause");
	return 0;
}

