#include "sim.h"

// 计算文件内容的校验和
unsigned char calculateChecksum(FILE* fp) {
    unsigned char sum = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        sum += (unsigned char)ch;
    }
    return sum;
}

// 保存时写入校验和
void writeChecksum(FILE* fp) {
    fseek(fp, 0, SEEK_SET); 
    unsigned char sum = calculateChecksum(fp);
    fseek(fp, 0, SEEK_END); 
    fwrite(&sum, sizeof(unsigned char), 1, fp);
}

// 加载时验证校验和
int verifyChecksum(FILE* fp) {
    unsigned char stored_sum;
    fseek(fp, -sizeof(unsigned char), SEEK_END);
    fread(&stored_sum, sizeof(unsigned char), 1, fp);
    
    fseek(fp, 0, SEEK_SET);
    unsigned char actual_sum = calculateChecksum(fp);
    
    return (stored_sum == actual_sum);
}