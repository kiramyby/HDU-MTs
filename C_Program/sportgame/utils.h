#ifndef UTILS_H
#define UTILS_H

// 获取当前时间字符串
void get_time_string(char* buffer);

// 生成报名ID（时间戳+随机数）
void generate_reg_id(char* buffer);

// 清空输入缓冲区
void clear_input_buffer(void);

#endif
