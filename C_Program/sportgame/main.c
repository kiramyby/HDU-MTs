#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include "file_ops.h"
#include "auth.h"
#include "menu.h"
#include "display.h"

// 全局变量定义
SportEvent* event_head = NULL;
Student* student_head = NULL;
Registration* reg_head = NULL;
int logged_in = 0;

// 主函数入口
int main() {
    srand(time(0)); // 初始化随机数种子
    
    // 显示系统启动信息
    print_header("运动会管理系统启动");
    print_info_message("正在加载系统数据...");
    
    load_events_from_file(); // 加载所有运动项目数据
    load_students_from_file(); // 加载所有学生信息数据
    load_registrations_from_file(); // 加载所有报名记录数据
    
    print_success_message("系统数据加载完成！");
    
    login(); // 管理员登录
    admin_menu(); // 进入管理员主菜单
    
    print_header("👋 感谢使用运动会管理系统");
    return 0;
}
