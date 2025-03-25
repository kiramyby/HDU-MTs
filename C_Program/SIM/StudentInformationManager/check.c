#include "sim.h"

// 计算文件内容的校验和（不包括最后一个字节的校验和本身）
unsigned char calculateChecksum(FILE* fp) {
    unsigned char sum = 0;
    long currentPos = ftell(fp);
    long endPos;
    
    // 获取文件大小（除了最后一个字节）
    fseek(fp, 0, SEEK_END);
    endPos = ftell(fp) - 1; // 减去校验和字节
    
    // 回到开始位置
    fseek(fp, 0, SEEK_SET);
    
    // 计算校验和
    int ch;
    long pos = 0;
    while ((ch = fgetc(fp)) != EOF && pos < endPos) {
        sum += (unsigned char)ch;
        pos++;
    }
    
    // 恢复文件指针位置
    fseek(fp, currentPos, SEEK_SET);
    return sum;
}

// 保存时写入校验和
void writeChecksum(FILE* fp) {
    long fileSize;
    unsigned char sum = 0;
    
    // 获取当前位置（文件末尾）
    fileSize = ftell(fp);
    
    // 从头开始计算校验和
    fseek(fp, 0, SEEK_SET);
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        sum += (unsigned char)ch;
    }
    
    // 回到文件末尾并写入校验和
    fseek(fp, fileSize, SEEK_SET);
    fwrite(&sum, sizeof(unsigned char), 1, fp);
}

// 加载时验证校验和
int verifyChecksum(FILE* fp) {
    unsigned char stored_sum;
    unsigned char calculated_sum;
    long fileSize;
    
    // 获取文件大小
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    
    if (fileSize <= sizeof(unsigned char)) {
        // 文件太小，无法包含有效数据和校验和
        return 0;
    }
    
    // 读取存储的校验和
    fseek(fp, fileSize - sizeof(unsigned char), SEEK_SET);
    if (fread(&stored_sum, sizeof(unsigned char), 1, fp) != 1) {
        return 0;
    }
    
    // 计算文件内容的校验和（不包括校验和字节本身）
    fseek(fp, 0, SEEK_SET);
    calculated_sum = 0;
    long pos = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF && pos < fileSize - sizeof(unsigned char)) {
        calculated_sum += (unsigned char)ch;
        pos++;
    }
    
    return (stored_sum == calculated_sum);
}