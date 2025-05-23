/*引入必要的头文件*/
#include <stdio.h>
#include<stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

/*宏定义*/
#define MAP_HEIGHT 20	//定义地图高度
#define MAP_WIDTH 40	//定义地图宽度
#define UP 'w'			//定义上移键
#define DOWN 's'		//定义下移键
#define LEFT 'a'		//定义左移键
#define RIGHT 'd'		//定义右移键

/*结构体定义*/
typedef struct		//定义障碍物和蛇节点位置的结构体
{
	int x;			//x坐标位置
	int y;			//y坐标位置
}Obstacle[4], Snakenode;

typedef struct
{
	int x;
	int y;
	int type;
	char mark;
}Food[3];

typedef struct		//定义蛇的结构体
{
	Snakenode snakeNode[1000];   //蛇长最多包含1000个节点
	int length;                  //蛇长度
	int speed;					 //蛇移动速度
}Snake;

typedef struct
{
	unsigned int score;
	time_t end_time;
	int diff_time;
}Rank[5];

/*函数定义*/
void GotoXY(int, int);	//光标定位函数
void Hide();			//隐藏光标函数
int Menu();				//主菜单函数
void Help();			//帮助信息
void About();			//关于信息
void RankBoard();		//排行榜
void DoRank(int);		//排行榜处理
void InitMap();			//地图初始化
void GameReset();		//游戏重置
void PrintFood(int);	//生成食物
void PrintObstacle();	//生成障碍物
int MoveSnake();		//蛇移动
int IsCorrect();		//自撞或撞墙检测
void SpeedControl();	//速度控制