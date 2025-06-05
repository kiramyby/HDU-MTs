#ifndef REGISTRATION_MGMT_H
#define REGISTRATION_MGMT_H

#include "types.h"

// 报名管理函数声明
Registration* find_registration(const char* reg_id);
void register_event(void);
void cancel_registration(void);
void show_all_registrations(void);
void search_registration(void);
void registration_management_menu(void);

#endif
