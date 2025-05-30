#include "sim.h"

int main(){
    // 设置控制台编码为 UTF-8，必须在任何输出之前执行
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // 确保所有数据目录存在
    ensureAllDirectoriesExist();
    
    int uid = -1, op = -1;
    srand((unsigned int)time(0));

    while (1)
    {
        //登录
        if (uid == -1)
        {
            system("cls"); // 清屏确保UI干净
            GotoXY(LEFT_HEAD, UP_HEAD);
            printf("Welcome to Student Information Manager\n");
            printf("Please login first with your ID\n");
            uid = Login();
            continue;
        }

        //菜单
        op = Menu();
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
            uid = -1; // 重置uid以便重新登录
            break;
        default:
            break;
        }
        
        // 简化的菜单导航逻辑 - 只有在登出时才不需要等待用户按键
        if (op != 5) {
            system("cls");
        }
    }
    
    return 0; // 正常情况下不会执行到这里
}