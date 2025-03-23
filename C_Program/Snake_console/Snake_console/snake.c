#include "snake.h"

/*全局变量定义*/
Snake snake;	//定义蛇结构体变量
unsigned int ungrow = 0;		//未生长蛇长
Food food = { { 0,0,1,'*' },{0,0,5,'%'}, {0,0,-1,'!'} };	//定义食物结构体变量, type为1，5表示增量，-1表示加速
Obstacle obstacle = { { 0,0 }, { 0,0 }, { 0,0 }, { 0,0 } };	//定义障碍物结构体变量
Rank rank = { {0},{0},{0},{0},{0} };	//定义排行榜结构体变量
char now_Dir = RIGHT;	//当前蛇头方向
char direction = RIGHT; //预期蛇头方向
time_t start_time = 0, end_time = 0;	//开始时间，结束时间
unsigned int remain = 0;				//剩余变速步数

/*时间函数*/
//flag为1时格式化返回当前时间，为0时返回unix时间
char* GetTime(time_t unixTime) {
	char str_t[100];

	if (unixTime == 0) {
		return "undefine (^_^)";	//未定义
	}
	struct tm* tm_local = localtime(&unixTime); //获取本地时间
	strftime(str_t, sizeof(str_t), "%G-%m-%d %H:%M:%S", tm_local);  //格式化时间

	return str_t;
}

/*主菜单实现*/
int Menu() {
	GotoXY(40, 12); 			     //定位光标位置
	printf("欢迎来到贪吃蛇小游戏");
	GotoXY(43, 14);
	printf("1.开始游戏");
	GotoXY(43, 16);
	printf("2.帮助");
	GotoXY(43, 18);
	printf("3.关于");
	GotoXY(43, 20);
	printf("4.排行榜");
	GotoXY(43, 22);
	printf("其他任意键退出游戏");
	Hide();	                       //隐藏光标
	char ch;
	int result = 0;
	ch = _getch();   			    //接收用户输入的菜单选项
	switch (ch) {				//根据选项设置返回结果值
		case '1': result = 1; break;
		case '2': result = 2; break;
		case '3': result = 3; break;
		case '4': result = 4; break;
	}
	system("cls");  				//调用系统命令cls完成清屏操作
	return result;
}

/*排行榜实现*/
// 比较函数
int DoComp(const void *a, const void *b) {
	return *(int*)b - *(int*)a;
}

//排行排序函数
void DoRank(int newScore) {
	if (newScore > rank[4].score) {
		rank[4].score = newScore;
		rank[4].end_time = end_time;
		rank[4].diff_time = difftime(end_time, start_time);
	}
    qsort(&(rank[0].score), 5, sizeof(rank[0]), DoComp);
}

//排行榜菜单函数
void RankBoard() {
	GotoXY(40, 8);
	printf("排行榜");
	GotoXY(40, 12);
	printf("排名\t成绩\t耗时（s）");
	for (int i = 1; i <= 5; i++) {
		GotoXY(40, 12 + i);
        printf("%d\t%d\t%d\t\t%s", i, rank[i - 1].score, rank[i-1].diff_time, GetTime(rank[i-1].end_time));
	};
	GotoXY(40, 20);
	printf("按任意键返回上级菜单");
	Hide();		//隐藏光标
	char ch = _getch();
	system("cls");
}

//光标定位函数，将光标定位到(x,y)坐标位置  
void GotoXY(int x, int y) {
	HANDLE hout;
	COORD cor;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = x;
	cor.Y = y;
	SetConsoleCursorPosition(hout, cor);
}

/*隐藏光标*/
void Hide() {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cor_info = { 1, 0 };
	SetConsoleCursorInfo(hout, &cor_info);
}


/*关于菜单实现*/
void About() {
	GotoXY(30, 12);
	printf("杭州电子科技大学--程序设计综合实践案例");
	GotoXY(43, 14);
	printf("贪吃蛇-控制台游戏");
	GotoXY(43, 16);
	printf("按任意键返回上级菜单");
	Hide();		//隐藏光标
	char ch = _getch();
	system("cls");
}

/*帮助菜单实现*/
void Help() {
	GotoXY(40, 12 );
	printf("w 上");
	GotoXY(40, 14);
	printf("s 下");
	GotoXY(40, 16 );
	printf("a 左");
	GotoXY(40, 18);
	printf("d 右");
	GotoXY(40, 20);
	printf("当蛇撞到自身或撞墙时游戏结束");
	GotoXY(45, 22);
	printf("按任意键返回上级菜单");
	Hide();		//隐藏光标
	char ch = _getch();
	system("cls");
}

/*初始化地图函数*/
void InitMap() {
	Hide();	   				//隐藏光标
	//设置蛇头位置在地图中心
	snake.snakeNode[0].x = MAP_WIDTH / 2 - 1;
	snake.snakeNode[0].y = MAP_HEIGHT / 2 - 1;
	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y); 	  //将光标移动到蛇头位置
	printf("@"); 				//打印蛇头
	snake.length = 3;   		//设置蛇长初始值为3节
	snake.speed = 250;  		//设置蛇初始移动速度为250
	now_Dir = RIGHT;            //当前蛇头方向
	//显示蛇身
	for (int i = 1; i < snake.length; i++) {
		//设置蛇身的纵坐标位置和蛇头位置相同
		snake.snakeNode[i].y = snake.snakeNode[i - 1].y;
		//设置蛇身的横坐标位置，蛇身在蛇头的左边,所以横坐标依次减1
		snake.snakeNode[i].x = snake.snakeNode[i - 1].x - 1;
		GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y); 	//移动光标到蛇身位置
		printf("o");   		//打印蛇身
	}
	//生成地图上下边界
	for (int i = 0; i < MAP_WIDTH; i++) {
		GotoXY(i, 0);
		printf("-");
		GotoXY(i, MAP_HEIGHT - 1);
		printf("-");
	}
	//生成地图左右边界
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		GotoXY(0, i);
		printf("|");
		GotoXY(MAP_WIDTH - 1,i);
		printf("|");
	}
	//生成食物
	PrintFood(-1);
	//生成障碍物
	PrintObstacle();
	//开始时间
	start_time = time(NULL);
	//得分说明
	GotoXY(50, 5);
	printf("当前得分:0");
	//排行榜说明
	GotoXY(50, 8);
	printf("排行榜");
	GotoXY(50, 10);
	printf("排名\t成绩\t耗时（s）");
	for (int i = 1; i <= 5; i++) {
		GotoXY(50, 10 + i);
		printf("%d\t%d\t%d\t\t%s", i, rank[i - 1].score, rank[i-1].diff_time, GetTime(rank[i-1].end_time));
	};
}

/*生成食物函数*/
void PrintFood(int newFood) {
	if( newFood != -1) {
		int flag = 1;
		while (flag)
		{
			flag = 0;
			//设置随机的食物坐标位置
			food[newFood].x = rand() % (MAP_WIDTH - 2) + 1;
			food[newFood].y = rand() % (MAP_HEIGHT - 2) + 1;
			//循环判断食物位置是否和蛇的位置重叠，如果重叠则需要重新设置食物位置
			for (int k = 0; k <= snake.length - 1; k++) {
				if (snake.snakeNode[k].x == food[newFood].x && snake.snakeNode[k].y == food[newFood].y) {
					flag = 1;  				//位置有重叠，需要继续循环
					break;
				}
			}
		}
		GotoXY(food[newFood].x, food[newFood].y);
		printf("%c", food[newFood].mark);
		return;
	}
	for (int i = 0; i < 3; i++)
	{
		int flag = 1;
		while (flag)
		{
			flag = 0;
			//设置随机的食物坐标位置
			food[i].x = rand() % (MAP_WIDTH - 2) + 1;
			food[i].y = rand() % (MAP_HEIGHT - 2) + 1;
			//循环判断食物位置是否和蛇的位置重叠，如果重叠则需要重新设置食物位置
			for (int k = 0; k <= snake.length - 1; k++) {
				if (snake.snakeNode[k].x == food[i].x && snake.snakeNode[k].y == food[i].y) {
					flag = 1;  				//位置有重叠，需要继续循环
					break;
				}
			}
		}
		GotoXY(food[i].x, food[i].y);
		printf("%c", food[i].mark);
	}
}

/*生成障碍物函数*/
//随机生成5个障碍物
void  PrintObstacle() {	
	for (int i = 0; i < 4; i++) {
		int flag = 1;
		while (flag)
		{
			flag = 0;
			//设置随机的障碍物坐标位置
			obstacle[i].x = rand() % (MAP_WIDTH - 2) + 1;
			obstacle[i].y = rand() % (MAP_HEIGHT - 2) + 1;
			//循环判断障碍物位置是否和蛇的位置重叠，如果重叠则需要重新设置障碍物位置
			for (int k = 0; k <= snake.length - 1; k++) {
				if (snake.snakeNode[k].x == obstacle[i].x && snake.snakeNode[k].y == obstacle[i].y) {
					flag = 1;  				//位置有重叠，需要继续循环
					break;
				}
			}
			//循环判断障碍物位置是否和食物的位置重叠，如果重叠则需要重新设置障碍物位置
			for (int k = 0; k < 3; k++) {
				if (food[k].x == obstacle[i].x && food[k].y == obstacle[i].y) {
					flag = 1;  				//位置有重叠，需要继续循环
					break;
				}
			}
		}
		GotoXY(obstacle[i].x,obstacle[i].y);
		printf("#");
	}
}

/*蛇移动函数实现,返回值为1表示继续移动，为0表示停止移动*/
int MoveSnake() {
	Snakenode tail;		//蛇尾

	tail = snake.snakeNode[snake.length - 1]; 			//记录蛇尾
	for (int i = snake.length - 1; i >= 1; i--)
		snake.snakeNode[i] = snake.snakeNode[i - 1];	//将所有蛇身向前移动一个位置
	GotoXY(snake.snakeNode[1].x, snake.snakeNode[1].y);
	printf("o");  			//前进方向打印一节蛇身，其他蛇身不需要打印
	//响应键盘修改
	if (_kbhit()) {			//键盘输入返回1，非键盘输入返回0
		direction = _getch();
		switch (direction) {
		case UP:  //按下w键
			if (now_Dir != DOWN)		//如果蛇头向下，按向上移动的键w时不起作用
				now_Dir = direction;
			break;
		case DOWN:  //按下s键
			if (now_Dir != UP)			//如果蛇头向上，按向下移动的键s时不起作用
				now_Dir = direction;
			break;
		case LEFT:  //按下a键
			if (now_Dir != RIGHT)		//如果蛇头向右，按向左移动的键a时不起作用
				now_Dir = direction;
			break;
		case RIGHT:  //按下d键
			if (now_Dir != LEFT)		//如果蛇头向左，按向右移动的键d时不起作用
				now_Dir = direction;
			break;
		}
	}
	switch (now_Dir) {   					//根据现在的方向修改蛇头的位置
	case UP:snake.snakeNode[0].y--; break;		//向上移动
	case DOWN:snake.snakeNode[0].y++; break;		//向下移动
	case LEFT:snake.snakeNode[0].x--; break;		//向左移动
	case RIGHT:snake.snakeNode[0].x++; break;	//向右移动
	}
	//打印蛇头
	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
	printf("@");
	for (int i = 0; i < 3; i++) {
		//判断是否吃到食物，如果蛇头的位置和食物的位置相同表示吃到食物
		if (snake.snakeNode[0].x == food[i].x && snake.snakeNode[0].y == food[i].y) {
			if (food[i].type != -1) {		//如果食物类型不是加速类型
				ungrow += food[i].type;		//待生蛇长变化
				if (snake.length + ungrow >= 1000) {	//蛇长超过1000，游戏胜利
					system("cls");
					int newScore = snake.length - 3;
					DoRank(newScore);
					GotoXY(45, 14);
					printf("得分超过1000! ");
					GotoXY(45, 16);
					printf("你赢了！");
					GotoXY(45, 18);
					printf("按任意键返回主菜单");
					char c = _getch();
					GameReset();		//重置游戏
					return 0;
				}
            }
			else {
				remain = 10;		//变速10步
			}
			//吃到食物后，重新生成食物
			if (rand() % 100 < 20) { // 20%的概率调用PrintFood(1)
				PrintFood(1);
			}
			if (rand() % 100 < 40) { // 40%的概率调用PrintFood(2)
				PrintFood(2);
			}else {
				PrintFood(0);
			}
		}
	}
	//输出蛇此时状态
	if (ungrow > 0) {		//待生蛇长大于0时，蛇长加一节
		snake.length++;
		snake.snakeNode[snake.length - 1] = tail;
		GotoXY(tail.x, tail.y);
		printf("o");
		ungrow--;
	}
	else {
		GotoXY(tail.x, tail.y);
		printf(" ");	//没有未生长蛇长时清除蛇尾
	}
	//判断是否死亡
	if (!IsCorrect()) { 	 	//如果自撞或撞墙，则清除屏幕，打印最终得分，游戏结束
		system("cls");
		int newScore = snake.length - 3;
		DoRank(newScore);
		GotoXY(45, 14);
		printf("最终得分：%d", newScore);
		GotoXY(45, 16);
		printf("你输了！");
		GotoXY(45, 18);
		printf("按任意键返回主菜单");
		char c = _getch();
		GameReset();		//重置游戏
		return 0;
	}
	//打印得分
	GotoXY(50, 5);
	printf("当前得分:%d", snake.length - 3);  	//打印得分，得分为蛇长减原始长度3
	//调整速度
	SpeedControl();
	if (remain > 0) {
		snake.speed = 100;
		remain--;
	}
	Sleep(snake.speed);		//把进程挂起一段时间，用于控制蛇移动的速度
	return 1;
}

/*断尾清除函数*/
void CutTail(Snake* snake ,int newTail, int oldTail) {
	int count = 0;
	// 从旧尾部向前遍历到新尾部
	for (oldTail; oldTail > newTail; oldTail--) {
		GotoXY(snake->snakeNode[oldTail].x, snake->snakeNode[oldTail].y);
		printf(" ");
		//清除断尾数据
		snake->snakeNode[oldTail].x = 0;
		snake->snakeNode[oldTail].y = 0;
	}
}

/*判断是否自撞或撞墙,返回值为0表示自撞或撞墙，否则为1*/
int IsCorrect() {
	//判断蛇头是否撞墙
	if (snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 || snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1) {
		end_time = time(NULL);
		return 0;
	} 
	//判断蛇头是否撞障碍物
	for (int i = 0; i < 4; i++) {
		if (snake.snakeNode[0].x == obstacle[i].x && snake.snakeNode[0].y == obstacle[i].y) {
			end_time = time(NULL);
			return 0;
		}
	}
	//判断蛇头是否和蛇身重叠，重叠表示自撞
	for (int i = 1; i < snake.length; i++) {   
		if (snake.snakeNode[0].x == snake.snakeNode[i].x && snake.snakeNode[0].y == snake.snakeNode[i].y) {
			CutTail(&snake, i, snake.length - 1); //断尾清除
			snake.length = i;	//更新蛇长
			GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
			printf("o");
			GotoXY(50, 5);
			printf("当前得分:%d     ", snake.length - 3);  	//打印得分，得分为蛇长减原始长度3，清除残余数字
			return 1;
		}
	}
	return 1;
}

/*重置函数*/
void GameReset() {
	system("cls");			//清屏
	start_time = end_time = 0;			//时间重置为0
	remain = 0;				//剩余变速步数重置为0
}

/*速度调整函数*/
void SpeedControl() {
	switch (snake.length) {  		//根据蛇长调整蛇的移动速度
	case 6:snake.speed = 200; break;
	case 9:snake.speed = 180; break;
	case 12:snake.speed = 160; break;
	case 15:snake.speed = 140; break;
	case 18:snake.speed = 120; break;
	case 21:snake.speed = 100; break;
	case 24:snake.speed = 80; break;
	case 27:snake.speed = 60; break;
	case 30:snake.speed = 40; break;
	default:break;
	}
}