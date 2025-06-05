#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

// 获取当前时间字符串
void get_time_string(char* buffer) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 20, "%Y%m%d%H%M%S", timeinfo); // 格式化时间为YYYYMMDDHHMMSS
}

// 生成报名ID（时间戳+随机数）
void generate_reg_id(char* buffer) {
    char timestamp[15];
    get_time_string(timestamp);
    sprintf(buffer, "R%s%03d", timestamp, rand() % 1000); // 生成格式为RYYYYMMDDHHMMSSXXX的报名ID
}

// 清空输入缓冲区
void clear_input_buffer() {
    while (getchar() != '\n'); // 读取并丢弃直到换行符的所有字符
}
