/*�����Ҫ��ͷ�ļ�*/
#include <stdio.h>
#include<stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

/*�궨��*/
#define MAP_HEIGHT 20	//�����ͼ�߶�
#define MAP_WIDTH 40	//�����ͼ���
#define UP 'w'			//�������Ƽ�
#define DOWN 's'		//�������Ƽ�
#define LEFT 'a'		//�������Ƽ�
#define RIGHT 'd'		//�������Ƽ�

/*�ṹ�嶨��*/
typedef struct		//�����ϰ�����߽ڵ�λ�õĽṹ��
{
	int x;			//x����λ��
	int y;			//y����λ��
}Obstacle[4], Snakenode;

typedef struct
{
	int x;
	int y;
	int type;
	char mark;
}Food[3];

typedef struct		//�����ߵĽṹ��
{
	Snakenode snakeNode[1000];   //�߳�������1000���ڵ�
	int length;                  //�߳���
	int speed;					 //���ƶ��ٶ�
}Snake;

typedef struct
{
	unsigned int score;
	time_t end_time;
	int diff_time;
}Rank[5];

/*��������*/
void GotoXY(int, int);	//��궨λ����
void Hide();			//���ع�꺯��
int Menu();				//���˵�����
void Help();			//������Ϣ
void About();			//������Ϣ
void RankBoard();		//���а�
void DoRank(int);		//���а���
void InitMap();			//��ͼ��ʼ��
void GameReset();		//��Ϸ����
void PrintFood(int);	//����ʳ��
void PrintObstacle();	//�����ϰ���
int MoveSnake();		//���ƶ�
int IsCorrect();		//��ײ��ײǽ���
void SpeedControl();	//�ٶȿ���