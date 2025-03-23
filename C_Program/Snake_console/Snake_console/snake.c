#include "snake.h"

/*ȫ�ֱ�������*/
Snake snake;	//�����߽ṹ�����
unsigned int ungrow = 0;		//δ�����߳�
Food food = { { 0,0,1,'*' },{0,0,5,'%'}, {0,0,-1,'!'} };	//����ʳ��ṹ�����, typeΪ1��5��ʾ������-1��ʾ����
Obstacle obstacle = { { 0,0 }, { 0,0 }, { 0,0 }, { 0,0 } };	//�����ϰ���ṹ�����
Rank rank = { {0},{0},{0},{0},{0} };	//�������а�ṹ�����
char now_Dir = RIGHT;	//��ǰ��ͷ����
char direction = RIGHT; //Ԥ����ͷ����
time_t start_time = 0, end_time = 0;	//��ʼʱ�䣬����ʱ��
unsigned int remain = 0;				//ʣ����ٲ���

/*ʱ�亯��*/
//flagΪ1ʱ��ʽ�����ص�ǰʱ�䣬Ϊ0ʱ����unixʱ��
char* GetTime(time_t unixTime) {
	char str_t[100];

	if (unixTime == 0) {
		return "undefine (^_^)";	//δ����
	}
	struct tm* tm_local = localtime(&unixTime); //��ȡ����ʱ��
	strftime(str_t, sizeof(str_t), "%G-%m-%d %H:%M:%S", tm_local);  //��ʽ��ʱ��

	return str_t;
}

/*���˵�ʵ��*/
int Menu() {
	GotoXY(40, 12); 			     //��λ���λ��
	printf("��ӭ����̰����С��Ϸ");
	GotoXY(43, 14);
	printf("1.��ʼ��Ϸ");
	GotoXY(43, 16);
	printf("2.����");
	GotoXY(43, 18);
	printf("3.����");
	GotoXY(43, 20);
	printf("4.���а�");
	GotoXY(43, 22);
	printf("����������˳���Ϸ");
	Hide();	                       //���ع��
	char ch;
	int result = 0;
	ch = _getch();   			    //�����û�����Ĳ˵�ѡ��
	switch (ch) {				//����ѡ�����÷��ؽ��ֵ
		case '1': result = 1; break;
		case '2': result = 2; break;
		case '3': result = 3; break;
		case '4': result = 4; break;
	}
	system("cls");  				//����ϵͳ����cls�����������
	return result;
}

/*���а�ʵ��*/
// �ȽϺ���
int DoComp(const void *a, const void *b) {
	return *(int*)b - *(int*)a;
}

//����������
void DoRank(int newScore) {
	if (newScore > rank[4].score) {
		rank[4].score = newScore;
		rank[4].end_time = end_time;
		rank[4].diff_time = difftime(end_time, start_time);
	}
    qsort(&(rank[0].score), 5, sizeof(rank[0]), DoComp);
}

//���а�˵�����
void RankBoard() {
	GotoXY(40, 8);
	printf("���а�");
	GotoXY(40, 12);
	printf("����\t�ɼ�\t��ʱ��s��");
	for (int i = 1; i <= 5; i++) {
		GotoXY(40, 12 + i);
        printf("%d\t%d\t%d\t\t%s", i, rank[i - 1].score, rank[i-1].diff_time, GetTime(rank[i-1].end_time));
	};
	GotoXY(40, 20);
	printf("������������ϼ��˵�");
	Hide();		//���ع��
	char ch = _getch();
	system("cls");
}

//��궨λ����������궨λ��(x,y)����λ��  
void GotoXY(int x, int y) {
	HANDLE hout;
	COORD cor;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = x;
	cor.Y = y;
	SetConsoleCursorPosition(hout, cor);
}

/*���ع��*/
void Hide() {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cor_info = { 1, 0 };
	SetConsoleCursorInfo(hout, &cor_info);
}


/*���ڲ˵�ʵ��*/
void About() {
	GotoXY(30, 12);
	printf("���ݵ��ӿƼ���ѧ--��������ۺ�ʵ������");
	GotoXY(43, 14);
	printf("̰����-����̨��Ϸ");
	GotoXY(43, 16);
	printf("������������ϼ��˵�");
	Hide();		//���ع��
	char ch = _getch();
	system("cls");
}

/*�����˵�ʵ��*/
void Help() {
	GotoXY(40, 12 );
	printf("w ��");
	GotoXY(40, 14);
	printf("s ��");
	GotoXY(40, 16 );
	printf("a ��");
	GotoXY(40, 18);
	printf("d ��");
	GotoXY(40, 20);
	printf("����ײ�������ײǽʱ��Ϸ����");
	GotoXY(45, 22);
	printf("������������ϼ��˵�");
	Hide();		//���ع��
	char ch = _getch();
	system("cls");
}

/*��ʼ����ͼ����*/
void InitMap() {
	Hide();	   				//���ع��
	//������ͷλ���ڵ�ͼ����
	snake.snakeNode[0].x = MAP_WIDTH / 2 - 1;
	snake.snakeNode[0].y = MAP_HEIGHT / 2 - 1;
	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y); 	  //������ƶ�����ͷλ��
	printf("@"); 				//��ӡ��ͷ
	snake.length = 3;   		//�����߳���ʼֵΪ3��
	snake.speed = 250;  		//�����߳�ʼ�ƶ��ٶ�Ϊ250
	now_Dir = RIGHT;            //��ǰ��ͷ����
	//��ʾ����
	for (int i = 1; i < snake.length; i++) {
		//���������������λ�ú���ͷλ����ͬ
		snake.snakeNode[i].y = snake.snakeNode[i - 1].y;
		//��������ĺ�����λ�ã���������ͷ�����,���Ժ��������μ�1
		snake.snakeNode[i].x = snake.snakeNode[i - 1].x - 1;
		GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y); 	//�ƶ���굽����λ��
		printf("o");   		//��ӡ����
	}
	//���ɵ�ͼ���±߽�
	for (int i = 0; i < MAP_WIDTH; i++) {
		GotoXY(i, 0);
		printf("-");
		GotoXY(i, MAP_HEIGHT - 1);
		printf("-");
	}
	//���ɵ�ͼ���ұ߽�
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		GotoXY(0, i);
		printf("|");
		GotoXY(MAP_WIDTH - 1,i);
		printf("|");
	}
	//����ʳ��
	PrintFood(-1);
	//�����ϰ���
	PrintObstacle();
	//��ʼʱ��
	start_time = time(NULL);
	//�÷�˵��
	GotoXY(50, 5);
	printf("��ǰ�÷�:0");
	//���а�˵��
	GotoXY(50, 8);
	printf("���а�");
	GotoXY(50, 10);
	printf("����\t�ɼ�\t��ʱ��s��");
	for (int i = 1; i <= 5; i++) {
		GotoXY(50, 10 + i);
		printf("%d\t%d\t%d\t\t%s", i, rank[i - 1].score, rank[i-1].diff_time, GetTime(rank[i-1].end_time));
	};
}

/*����ʳ�ﺯ��*/
void PrintFood(int newFood) {
	if( newFood != -1) {
		int flag = 1;
		while (flag)
		{
			flag = 0;
			//���������ʳ������λ��
			food[newFood].x = rand() % (MAP_WIDTH - 2) + 1;
			food[newFood].y = rand() % (MAP_HEIGHT - 2) + 1;
			//ѭ���ж�ʳ��λ���Ƿ���ߵ�λ���ص�������ص�����Ҫ��������ʳ��λ��
			for (int k = 0; k <= snake.length - 1; k++) {
				if (snake.snakeNode[k].x == food[newFood].x && snake.snakeNode[k].y == food[newFood].y) {
					flag = 1;  				//λ�����ص�����Ҫ����ѭ��
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
			//���������ʳ������λ��
			food[i].x = rand() % (MAP_WIDTH - 2) + 1;
			food[i].y = rand() % (MAP_HEIGHT - 2) + 1;
			//ѭ���ж�ʳ��λ���Ƿ���ߵ�λ���ص�������ص�����Ҫ��������ʳ��λ��
			for (int k = 0; k <= snake.length - 1; k++) {
				if (snake.snakeNode[k].x == food[i].x && snake.snakeNode[k].y == food[i].y) {
					flag = 1;  				//λ�����ص�����Ҫ����ѭ��
					break;
				}
			}
		}
		GotoXY(food[i].x, food[i].y);
		printf("%c", food[i].mark);
	}
}

/*�����ϰ��ﺯ��*/
//�������5���ϰ���
void  PrintObstacle() {	
	for (int i = 0; i < 4; i++) {
		int flag = 1;
		while (flag)
		{
			flag = 0;
			//����������ϰ�������λ��
			obstacle[i].x = rand() % (MAP_WIDTH - 2) + 1;
			obstacle[i].y = rand() % (MAP_HEIGHT - 2) + 1;
			//ѭ���ж��ϰ���λ���Ƿ���ߵ�λ���ص�������ص�����Ҫ���������ϰ���λ��
			for (int k = 0; k <= snake.length - 1; k++) {
				if (snake.snakeNode[k].x == obstacle[i].x && snake.snakeNode[k].y == obstacle[i].y) {
					flag = 1;  				//λ�����ص�����Ҫ����ѭ��
					break;
				}
			}
			//ѭ���ж��ϰ���λ���Ƿ��ʳ���λ���ص�������ص�����Ҫ���������ϰ���λ��
			for (int k = 0; k < 3; k++) {
				if (food[k].x == obstacle[i].x && food[k].y == obstacle[i].y) {
					flag = 1;  				//λ�����ص�����Ҫ����ѭ��
					break;
				}
			}
		}
		GotoXY(obstacle[i].x,obstacle[i].y);
		printf("#");
	}
}

/*���ƶ�����ʵ��,����ֵΪ1��ʾ�����ƶ���Ϊ0��ʾֹͣ�ƶ�*/
int MoveSnake() {
	Snakenode tail;		//��β

	tail = snake.snakeNode[snake.length - 1]; 			//��¼��β
	for (int i = snake.length - 1; i >= 1; i--)
		snake.snakeNode[i] = snake.snakeNode[i - 1];	//������������ǰ�ƶ�һ��λ��
	GotoXY(snake.snakeNode[1].x, snake.snakeNode[1].y);
	printf("o");  			//ǰ�������ӡһ����������������Ҫ��ӡ
	//��Ӧ�����޸�
	if (_kbhit()) {			//�������뷵��1���Ǽ������뷵��0
		direction = _getch();
		switch (direction) {
		case UP:  //����w��
			if (now_Dir != DOWN)		//�����ͷ���£��������ƶ��ļ�wʱ��������
				now_Dir = direction;
			break;
		case DOWN:  //����s��
			if (now_Dir != UP)			//�����ͷ���ϣ��������ƶ��ļ�sʱ��������
				now_Dir = direction;
			break;
		case LEFT:  //����a��
			if (now_Dir != RIGHT)		//�����ͷ���ң��������ƶ��ļ�aʱ��������
				now_Dir = direction;
			break;
		case RIGHT:  //����d��
			if (now_Dir != LEFT)		//�����ͷ���󣬰������ƶ��ļ�dʱ��������
				now_Dir = direction;
			break;
		}
	}
	switch (now_Dir) {   					//�������ڵķ����޸���ͷ��λ��
	case UP:snake.snakeNode[0].y--; break;		//�����ƶ�
	case DOWN:snake.snakeNode[0].y++; break;		//�����ƶ�
	case LEFT:snake.snakeNode[0].x--; break;		//�����ƶ�
	case RIGHT:snake.snakeNode[0].x++; break;	//�����ƶ�
	}
	//��ӡ��ͷ
	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
	printf("@");
	for (int i = 0; i < 3; i++) {
		//�ж��Ƿ�Ե�ʳ������ͷ��λ�ú�ʳ���λ����ͬ��ʾ�Ե�ʳ��
		if (snake.snakeNode[0].x == food[i].x && snake.snakeNode[0].y == food[i].y) {
			if (food[i].type != -1) {		//���ʳ�����Ͳ��Ǽ�������
				ungrow += food[i].type;		//�����߳��仯
				if (snake.length + ungrow >= 1000) {	//�߳�����1000����Ϸʤ��
					system("cls");
					int newScore = snake.length - 3;
					DoRank(newScore);
					GotoXY(45, 14);
					printf("�÷ֳ���1000! ");
					GotoXY(45, 16);
					printf("��Ӯ�ˣ�");
					GotoXY(45, 18);
					printf("��������������˵�");
					char c = _getch();
					GameReset();		//������Ϸ
					return 0;
				}
            }
			else {
				remain = 10;		//����10��
			}
			//�Ե�ʳ�����������ʳ��
			if (rand() % 100 < 20) { // 20%�ĸ��ʵ���PrintFood(1)
				PrintFood(1);
			}
			if (rand() % 100 < 40) { // 40%�ĸ��ʵ���PrintFood(2)
				PrintFood(2);
			}else {
				PrintFood(0);
			}
		}
	}
	//����ߴ�ʱ״̬
	if (ungrow > 0) {		//�����߳�����0ʱ���߳���һ��
		snake.length++;
		snake.snakeNode[snake.length - 1] = tail;
		GotoXY(tail.x, tail.y);
		printf("o");
		ungrow--;
	}
	else {
		GotoXY(tail.x, tail.y);
		printf(" ");	//û��δ�����߳�ʱ�����β
	}
	//�ж��Ƿ�����
	if (!IsCorrect()) { 	 	//�����ײ��ײǽ���������Ļ����ӡ���յ÷֣���Ϸ����
		system("cls");
		int newScore = snake.length - 3;
		DoRank(newScore);
		GotoXY(45, 14);
		printf("���յ÷֣�%d", newScore);
		GotoXY(45, 16);
		printf("�����ˣ�");
		GotoXY(45, 18);
		printf("��������������˵�");
		char c = _getch();
		GameReset();		//������Ϸ
		return 0;
	}
	//��ӡ�÷�
	GotoXY(50, 5);
	printf("��ǰ�÷�:%d", snake.length - 3);  	//��ӡ�÷֣��÷�Ϊ�߳���ԭʼ����3
	//�����ٶ�
	SpeedControl();
	if (remain > 0) {
		snake.speed = 100;
		remain--;
	}
	Sleep(snake.speed);		//�ѽ��̹���һ��ʱ�䣬���ڿ������ƶ����ٶ�
	return 1;
}

/*��β�������*/
void CutTail(Snake* snake ,int newTail, int oldTail) {
	int count = 0;
	// �Ӿ�β����ǰ��������β��
	for (oldTail; oldTail > newTail; oldTail--) {
		GotoXY(snake->snakeNode[oldTail].x, snake->snakeNode[oldTail].y);
		printf(" ");
		//�����β����
		snake->snakeNode[oldTail].x = 0;
		snake->snakeNode[oldTail].y = 0;
	}
}

/*�ж��Ƿ���ײ��ײǽ,����ֵΪ0��ʾ��ײ��ײǽ������Ϊ1*/
int IsCorrect() {
	//�ж���ͷ�Ƿ�ײǽ
	if (snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 || snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1) {
		end_time = time(NULL);
		return 0;
	} 
	//�ж���ͷ�Ƿ�ײ�ϰ���
	for (int i = 0; i < 4; i++) {
		if (snake.snakeNode[0].x == obstacle[i].x && snake.snakeNode[0].y == obstacle[i].y) {
			end_time = time(NULL);
			return 0;
		}
	}
	//�ж���ͷ�Ƿ�������ص����ص���ʾ��ײ
	for (int i = 1; i < snake.length; i++) {   
		if (snake.snakeNode[0].x == snake.snakeNode[i].x && snake.snakeNode[0].y == snake.snakeNode[i].y) {
			CutTail(&snake, i, snake.length - 1); //��β���
			snake.length = i;	//�����߳�
			GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
			printf("o");
			GotoXY(50, 5);
			printf("��ǰ�÷�:%d     ", snake.length - 3);  	//��ӡ�÷֣��÷�Ϊ�߳���ԭʼ����3�������������
			return 1;
		}
	}
	return 1;
}

/*���ú���*/
void GameReset() {
	system("cls");			//����
	start_time = end_time = 0;			//ʱ������Ϊ0
	remain = 0;				//ʣ����ٲ�������Ϊ0
}

/*�ٶȵ�������*/
void SpeedControl() {
	switch (snake.length) {  		//�����߳������ߵ��ƶ��ٶ�
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