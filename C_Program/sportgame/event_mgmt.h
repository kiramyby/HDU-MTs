#ifndef EVENT_MGMT_H
#define EVENT_MGMT_H

#include "types.h"

// 运动项目管理函数声明
SportEvent* find_event(const char* event_id);
void add_event(void);
void show_all_events(void);
void modify_event(void);
void search_event(void);
void event_management_menu(void);

// 成绩录入和统计查询
void enter_score(void);
void count_participants_per_event(void);
void query_scores(void);

#endif
