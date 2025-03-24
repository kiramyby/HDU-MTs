#include "sim.h"

int main(){
    int uid = -1, op;
    srand((unsigned int)time(0));

    while (1)
    {
        //登录
        if (uid == -1)
        {
            GotoXY(LEFT_HEAD, UP_HEAD);
            printf("Welcome to Student Information Manager\n");
            printf("Please login first\n");
            uid = Login();
            continue;
        }

        //菜单
        op = Menu(uid);
        switch (op)
        {
        case 0:
            Create(uid); 
            break;
        case 1:
            Read(uid);
            break;
        case 2:
            Update(uid);
            break;
        case 3:
            Delete(uid);
            break;
        case 4:
            Logging(uid);
            break;
        case 5:
            Logout(uid);
        default:
            break;
        }

        //退出
        Final();
        return 0;
    }

}