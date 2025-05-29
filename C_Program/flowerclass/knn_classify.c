#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_SAMPLES 200
#define FEATURES 4
#define MAX_SPECIES_NAME 20

// 鸢尾花数据结构
typedef struct {
    double features[FEATURES];  // 四个特征：花萼长度、花萼宽度、花瓣长度、花瓣宽度
    int species;               // 物种类别：1-Setosa, 2-Versicolor, 3-Virginica
    char species_name[MAX_SPECIES_NAME];
} IrisData;

// 距离结构，用于排序
typedef struct {
    double distance;
    int species;
    int index;
} Distance;

// 全局变量
IrisData dataset[MAX_SAMPLES];
int total_samples = 0;

// 函数声明
int load_data(const char* filename);
void preprocess_data();
void split_dataset(IrisData* train_set, int* train_size, IrisData* test_set, int* test_size, double train_ratio);
double calculate_distance(const double* point1, const double* point2);
int knn_classify(const IrisData* train_set, int train_size, const double* test_point, int k);
double evaluate_accuracy(const IrisData* train_set, int train_size, const IrisData* test_set, int test_size, int k);
void shuffle_array(IrisData* arr, int size);
int compare_distances(const void* a, const void* b);
int species_name_to_number(const char* name);
const char* species_number_to_name(int number);

int main() {
    printf("=== 鸢尾花KNN分类算法 ===\n\n");
    
    // 1. 数据读入 (20分)
    printf("1. 正在读取数据...\n");
    if (load_data("data.txt") == -1) {
        printf("错误：无法读取数据文件!\n");
        return -1;
    }
    printf("成功读取 %d 个样本\n\n", total_samples);
    
    // 2. 数据预处理 (20分)
    printf("2. 正在进行数据预处理...\n");
    preprocess_data();
    printf("数据预处理完成（数据打乱、类别转换）\n\n");
    
    // 3. 数据集切分 (10分)
    printf("3. 正在切分数据集...\n");
    IrisData train_set[MAX_SAMPLES], test_set[MAX_SAMPLES];
    int train_size, test_size;
    double train_ratio = 0.7;  // 70%训练集，30%测试集
    
    split_dataset(train_set, &train_size, test_set, &test_size, train_ratio);
    printf("训练集大小: %d, 测试集大小: %d\n\n", train_size, test_size);
    
    // 4. 测试集分类计算 (40分)
    printf("4. 正在进行KNN分类...\n");
    int k_values[] = {1, 3, 5, 7, 9};
    int num_k = sizeof(k_values) / sizeof(k_values[0]);
    
    printf("不同K值的分类结果:\n");
    printf("K值\t准确率\n");
    printf("----\t------\n");
    
    double best_accuracy = 0.0;
    int best_k = 1;
    
    for (int i = 0; i < num_k; i++) {
        int k = k_values[i];
        
        // 5. 预测准确度评估 (10分)
        double accuracy = evaluate_accuracy(train_set, train_size, test_set, test_size, k);
        printf("%d\t%.2f%%\n", k, accuracy * 100);
        
        if (accuracy > best_accuracy) {
            best_accuracy = accuracy;
            best_k = k;
        }
    }
    
    printf("\n最佳K值: %d, 最高准确率: %.2f%%\n", best_k, best_accuracy * 100);
    
    // 显示详细分类结果
    printf("\n=== 详细分类结果（K=%d）===\n", best_k);
    printf("样本序号\t实际类别\t\t预测类别\t\t结果\n");
    printf("--------\t--------\t\t--------\t\t----\n");
    
    int correct = 0;
    for (int i = 0; i < test_size; i++) {
        int predicted = knn_classify(train_set, train_size, test_set[i].features, best_k);
        int actual = test_set[i].species;
        
        printf("%d\t\t%s\t%s\t%s\n", 
               i + 1,
               species_number_to_name(actual),
               species_number_to_name(predicted),
               (predicted == actual) ? "✓" : "✗");
        
        if (predicted == actual) correct++;
    }
    
    printf("\n最终统计：\n");
    printf("总测试样本数: %d\n", test_size);
    printf("预测正确数: %d\n", correct);
    printf("预测错误数: %d\n", test_size - correct);
    printf("最终准确率: %.2f%%\n", (double)correct / test_size * 100);
    
    return 0;
}

// 1. 数据读入函数
int load_data(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[256];
    total_samples = 0;
    
    while (fgets(line, sizeof(line), file) && total_samples < MAX_SAMPLES) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;
        
        // 解析CSV格式数据
        char* token = strtok(line, ",");
        if (token == NULL) continue;
        
        // 读取四个特征值
        for (int i = 0; i < FEATURES; i++) {
            if (token == NULL) break;
            dataset[total_samples].features[i] = atof(token);
            token = strtok(NULL, ",");
        }
        
        // 读取物种名称并转换为数字
        if (token != NULL) {
            strcpy(dataset[total_samples].species_name, token);
            dataset[total_samples].species = species_name_to_number(token);
            total_samples++;
        }
    }
    
    fclose(file);
    return total_samples;
}

// 2. 数据预处理函数
void preprocess_data() {
    // 打乱数据集
    shuffle_array(dataset, total_samples);
    
    // 数据已经在读取时进行了类别转换
    printf("数据集已打乱，物种类别已转换为数字编码\n");
    printf("Iris-setosa -> 1, Iris-versicolor -> 2, Iris-virginica -> 3\n");
}

// 3. 数据集切分函数
void split_dataset(IrisData* train_set, int* train_size, IrisData* test_set, int* test_size, double train_ratio) {
    *train_size = (int)(total_samples * train_ratio);
    *test_size = total_samples - *train_size;
    
    // 复制训练集数据
    for (int i = 0; i < *train_size; i++) {
        train_set[i] = dataset[i];
    }
    
    // 复制测试集数据
    for (int i = 0; i < *test_size; i++) {
        test_set[i] = dataset[*train_size + i];
    }
}

// 计算欧式距离
double calculate_distance(const double* point1, const double* point2) {
    double sum = 0.0;
    for (int i = 0; i < FEATURES; i++) {
        double diff = point1[i] - point2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// 4. KNN分类函数
int knn_classify(const IrisData* train_set, int train_size, const double* test_point, int k) {
    Distance distances[MAX_SAMPLES];
    
    // 计算测试点到所有训练点的距离
    for (int i = 0; i < train_size; i++) {
        distances[i].distance = calculate_distance(test_point, train_set[i].features);
        distances[i].species = train_set[i].species;
        distances[i].index = i;
    }
    
    // 按距离排序
    qsort(distances, train_size, sizeof(Distance), compare_distances);
    
    // 统计前K个邻居的类别
    int votes[4] = {0}; // 索引0不用，1-3对应三个物种
    
    for (int i = 0; i < k && i < train_size; i++) {
        int species = distances[i].species;
        if (species >= 1 && species <= 3) {
            votes[species]++;
        }
    }
    
    // 找出票数最多的类别
    int max_votes = 0;
    int predicted_species = 1;
    
    for (int i = 1; i <= 3; i++) {
        if (votes[i] > max_votes) {
            max_votes = votes[i];
            predicted_species = i;
        }
    }
    
    return predicted_species;
}

// 5. 准确度评估函数
double evaluate_accuracy(const IrisData* train_set, int train_size, const IrisData* test_set, int test_size, int k) {
    int correct = 0;
    
    for (int i = 0; i < test_size; i++) {
        int predicted = knn_classify(train_set, train_size, test_set[i].features, k);
        if (predicted == test_set[i].species) {
            correct++;
        }
    }
    
    return (double)correct / test_size;
}

// 辅助函数：打乱数组
void shuffle_array(IrisData* arr, int size) {
    srand(time(NULL));
    
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        
        // 交换arr[i]和arr[j]
        IrisData temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// 距离比较函数（用于qsort）
int compare_distances(const void* a, const void* b) {
    const Distance* da = (const Distance*)a;
    const Distance* db = (const Distance*)b;
    
    if (da->distance < db->distance) return -1;
    if (da->distance > db->distance) return 1;
    return 0;
}

// 物种名称转数字
int species_name_to_number(const char* name) {
    if (strstr(name, "setosa")) return 1;
    if (strstr(name, "versicolor")) return 2;
    if (strstr(name, "virginica")) return 3;
    return 1; // 默认值
}

// 数字转物种名称
const char* species_number_to_name(int number) {
    switch (number) {
        case 1: return "Iris-setosa     ";
        case 2: return "Iris-versicolor ";
        case 3: return "Iris-virginica  ";
        default: return "Unknown         ";
    }
}