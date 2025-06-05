#include <stdio.h>
#include "types.h"
#include "menu.h"
#include "event_mgmt.h"
#include "student_mgmt.h"
#include "registration_mgmt.h"
#include "auth.h"
#include "utils.h"
#include "display.h"

// 统计查询菜单
void statistics_query_menu() {
    int choice;
    do {
        clear_screen();
        display_statistics_menu();
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            clear_screen();
            count_participants_per_event();
            wait_for_enter();
            break;
        case 2:
            clear_screen();
            count_registrations_per_student();
            wait_for_enter();
            break;
        case 3:
            clear_screen();
            query_scores();
            wait_for_enter();
            break;
        case 0:
            return;
        default:
            print_error_message("无效选项！");
            wait_for_enter();
        }
    } while (1);
}

// 管理员主菜单
void admin_menu() {
    int choice;
    do {
        clear_screen();
        display_admin_menu();
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            event_management_menu();
            break;
        case 2:
            student_management_menu();
            break;
        case 3:
            registration_management_menu();
            break;
        case 4:
            statistics_query_menu();
            break;
        case 5:
            clear_screen();
            enter_score();
            wait_for_enter();
            break;
        case 0:
            logout();
            break;
        default:
            print_error_message("无效选项！");
            wait_for_enter();
        }
    } while (1);
}
