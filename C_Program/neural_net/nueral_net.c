#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define DATA_TRAIN 380
#define DATA_TEST 95  // 测试集大小，确保它足够大，至少大于10
#define FEATURES 13
#define OUTPUTS 1
#define NEURONS 40
#define TRAIN_CYCLES 19000
#define LR_OUTPUT 0.1    // 输出层学习率
#define LR_HIDDEN 0.2    // 隐含层学习率

// 数据结构
double train_input[DATA_TRAIN][FEATURES];
double train_output[DATA_TRAIN][OUTPUTS];
double test_input[DATA_TEST][FEATURES];
double test_output[DATA_TEST][OUTPUTS];

// 网络参数
double weights_hidden[NEURONS][FEATURES];
double weights_output[OUTPUTS][NEURONS];
double hidden_output[NEURONS];

// 归一化参数
double feature_max[FEATURES], feature_min[FEATURES];
double output_max[OUTPUTS], output_min[OUTPUTS];

// 性能指标
double mse, rmse;
double importance_scores_all[FEATURES]; // 用于存储所有特征的重要性得分

// 辅助函数声明
void shuffle_data(int* index, int n);

// 数据读取
void read_data() {
    FILE* fp = fopen("housing.txt", "r");
    if (!fp) {
        printf("无法打开 housing.txt 文件\n");
        exit(EXIT_FAILURE);
    }    // 跳过表头
    char header[256];
    fgets(header, sizeof(header), fp);
    
    // 临时存储文件前10个样本用于测试集
    double temp_first_10_input[10][FEATURES];
    double temp_first_10_output[10][OUTPUTS];
    
    // 确保DATA_TEST至少为10
    if (DATA_TEST < 10) {
        printf("错误：DATA_TEST必须至少为10\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < FEATURES; ++j) {
            if (fscanf(fp, "%lf", &temp_first_10_input[i][j]) != 1) {
                printf("读取前10个样本的特征时出错 (文件行 %d, 特征 %d)\n", i+2, j+1); // 表头和0索引补偿
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }
        if (fscanf(fp, "%lf", &temp_first_10_output[i][0]) != 1) {
            printf("读取前10个样本的标签时出错 (文件行 %d)\n", i+2);
            fclose(fp);
            exit(EXIT_FAILURE);        }
    }    
    
    // 读取训练集数据（从文件第10行到10+DATA_TRAIN-1行）
    for (int i = 0; i < DATA_TRAIN; ++i) {
        for (int j = 0; j < FEATURES; ++j) {
            if (fscanf(fp, "%lf", &train_input[i][j]) != 1) {
                printf("训练数据读取错误 (文件行 %d, 特征 %d)\n", i+10+2, j+1);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }
        if (fscanf(fp, "%lf", &train_output[i][0]) != 1) {
            printf("训练标签读取错误 (文件行 %d)\n", i+10+2);
            fclose(fp);
            exit(EXIT_FAILURE);        }
    }    
    
    // 填充测试集
    // 首先复制最初读取的10个样本
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < FEATURES; ++j) {
            test_input[i][j] = temp_first_10_input[i][j];
        }        test_output[i][0] = temp_first_10_output[i][0];
    }    
    
    // 然后读取剩余(DATA_TEST-10)个测试样本
    // 这些样本从文件索引(10+DATA_TRAIN)开始
    // 仅当DATA_TEST>10时执行此循环
    for (int i = 10; i < DATA_TEST; ++i) {
        for (int j = 0; j < FEATURES; ++j) {
            if (fscanf(fp, "%lf", &test_input[i][j]) != 1) {
                // 文件行计算：10(初始样本)+DATA_TRAIN(训练样本)+(i-10)(当前测试样本)+2(表头和0索引)
                printf("测试数据读取错误 (目标测试索引 %d, 文件行 %d, 特征 %d)\n", i, 10+DATA_TRAIN+(i-10)+2, j+1);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }
        if (fscanf(fp, "%lf", &test_output[i][0]) != 1) {
            printf("测试标签读取错误 (目标测试索引 %d, 文件行 %d)\n", i, 10+DATA_TRAIN+(i-10)+2);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
}

// 初始化网络
void init_network() {
    srand(time(NULL));
    
    // 隐含层权重初始化
    for (int i = 0; i < NEURONS; ++i) {
        for (int j = 0; j < FEATURES; ++j) {
            weights_hidden[i][j] = (double)rand() / RAND_MAX * 2 - 1; // 范围[-1, 1]
        }
    }

    // 输出层权重初始化
    for (int i = 0; i < OUTPUTS; ++i) {
        for (int j = 0; j < NEURONS; ++j) {
            weights_output[i][j] = (double)rand() / RAND_MAX * 2 - 1;
        }
    }
}

// 数据归一化
void normalize_data() {
    // 特征归一化
    for (int j = 0; j < FEATURES; ++j) {
        feature_min[j] = feature_max[j] = train_input[0][j];
        for (int i = 0; i < DATA_TRAIN; ++i) {
            if (train_input[i][j] < feature_min[j]) feature_min[j] = train_input[i][j];
            if (train_input[i][j] > feature_max[j]) feature_max[j] = train_input[i][j];
        }

        double range = feature_max[j] - feature_min[j];
        if (range == 0) range = 1; // 处理常数值特征
        
        for (int i = 0; i < DATA_TRAIN; ++i) {
            train_input[i][j] = (train_input[i][j] - feature_min[j]) / range;
        }
        for (int i = 0; i < DATA_TEST; ++i) {
            test_input[i][j] = (test_input[i][j] - feature_min[j]) / range;
        }
    }

    // 输出归一化
    output_min[0] = output_max[0] = train_output[0][0];
    for (int i = 0; i < DATA_TRAIN; ++i) {
        if (train_output[i][0] < output_min[0]) output_min[0] = train_output[i][0];
        if (train_output[i][0] > output_max[0]) output_max[0] = train_output[i][0];
    }
    double out_range = output_max[0] - output_min[0];
    for (int i = 0; i < DATA_TRAIN; ++i) {
        train_output[i][0] = (train_output[i][0] - output_min[0]) / out_range;
    }
}

// 前向传播
double forward_pass(const double* input) {
    // 隐含层计算
    for (int i = 0; i < NEURONS; ++i) {
        double sum = 0;
        for (int j = 0; j < FEATURES; ++j) {
            sum += weights_hidden[i][j] * input[j];
        }
        hidden_output[i] = 1.0 / (1.0 + exp(-sum));
    }

    // 输出层计算
    double output = 0;
    for (int j = 0; j < NEURONS; ++j) {
        output += weights_output[0][j] * hidden_output[j];
    }
    return 1.0 / (1.0 + exp(-output));
}

// 训练网络
void train() {
    int indices[DATA_TRAIN];
    for (int i = 0; i < DATA_TRAIN; ++i) indices[i] = i;
    
    for (int epoch = 1; epoch <= TRAIN_CYCLES; ++epoch) {
        shuffle_data(indices, DATA_TRAIN);
        mse = 0;

        for (int n = 0; n < DATA_TRAIN; ++n) {
            int i = indices[n];
            double pred_norm = forward_pass(train_input[i]);
            
            // 输出层误差计算
            double output_error = (train_output[i][0] - pred_norm) * pred_norm * (1 - pred_norm);
            
            // 更新输出层权重
            for (int j = 0; j < NEURONS; ++j) {
                weights_output[0][j] += LR_OUTPUT * output_error * hidden_output[j];
            }

            // 隐含层误差计算
            double hidden_errors[NEURONS];
            for (int j = 0; j < NEURONS; ++j) {
                hidden_errors[j] = output_error * weights_output[0][j] * hidden_output[j] * (1 - hidden_output[j]);
            }

            // 更新隐含层权重
            for (int j = 0; j < NEURONS; ++j) {
                for (int k = 0; k < FEATURES; ++k) {
                    weights_hidden[j][k] += LR_HIDDEN * hidden_errors[j] * train_input[i][k];
                }
            }

            // 计算MSE
            double pred = pred_norm * (output_max[0] - output_min[0]) + output_min[0];
            double actual = train_output[i][0] * (output_max[0] - output_min[0]) + output_min[0];
            mse += (pred - actual) * (pred - actual);
        }

        mse /= DATA_TRAIN;
        if (epoch % 1000 == 0) {
            printf("Epoch %d \tMSE: %.4f\n", epoch, mse);
        }
    }
}

// 特征重要性分析
void analyze_features() {
    double original_mse = mse;
    double importance_scores[FEATURES] = {0}; // 用于计算和排序的局部数组
    const int NUM_PERTURB = 50;

    for (int feat = 0; feat < FEATURES; ++feat) {
        double perturbed_mse = 0;
        
        for (int p = 0; p < NUM_PERTURB; ++p) {
            // 保存原始数据
            double backup[DATA_TRAIN];
            for (int i = 0; i < DATA_TRAIN; ++i) {
                backup[i] = train_input[i][feat];
                train_input[i][feat] += (rand() / (double)RAND_MAX - 0.5) * 0.1;
            }

            // 计算扰动后的MSE
            double current_mse = 0;
            for (int i = 0; i < DATA_TRAIN; ++i) {
                double pred = forward_pass(train_input[i]);
                pred = pred * (output_max[0] - output_min[0]) + output_min[0];
                double actual = train_output[i][0] * (output_max[0] - output_min[0]) + output_min[0];
                current_mse += (pred - actual) * (pred - actual);
            }
            perturbed_mse += current_mse / DATA_TRAIN;

            // 恢复数据
            for (int i = 0; i < DATA_TRAIN; ++i) {
                train_input[i][feat] = backup[i];
            }
        }
        importance_scores[feat] = fabs((original_mse - perturbed_mse / NUM_PERTURB) / original_mse);
    }

    // 全局存储所有特征的原始重要性得分
    for (int i = 0; i < FEATURES; ++i) {
        importance_scores_all[i] = importance_scores[i];
    }

    // 归一化 importance_scores_all 到 [0, 1] 区间
    double min_score = importance_scores_all[0];
    double max_score = importance_scores_all[0];
    for (int i = 1; i < FEATURES; ++i) {
        if (importance_scores_all[i] < min_score) min_score = importance_scores_all[i];
        if (importance_scores_all[i] > max_score) max_score = importance_scores_all[i];
    }

    if (max_score == min_score) {
        // 如果所有得分都相同
        for (int i = 0; i < FEATURES; ++i) {
            importance_scores_all[i] = (max_score == 0.0) ? 0.0 : 1.0;
        }
    } else {
        for (int i = 0; i < FEATURES; ++i) {
            importance_scores_all[i] = (importance_scores_all[i] - min_score) / (max_score - min_score);
        }
    }

    // 按重要性得分降序排序，并保持特征索引同步
    int feature_indices[FEATURES];
    for (int i = 0; i < FEATURES; ++i) {
        feature_indices[i] = i; // 初始化为原始索引 0 到 FEATURES-1
    }
    
    for (int i = 0; i < FEATURES; ++i) {
        for (int j = i + 1; j < FEATURES; ++j) {
            if (importance_scores[j] > importance_scores[i]) {
                // 交换得分
                double tmp_score = importance_scores[i];
                importance_scores[i] = importance_scores[j];
                importance_scores[j] = tmp_score;
                
                // 交换对应的索引
                int tmp_idx = feature_indices[i];
                feature_indices[i] = feature_indices[j];
                feature_indices[j] = tmp_idx;
            }
        }
    }

    printf("\n特征重要性得分 (降序排列):\n");
    for (int i = 0; i < FEATURES; ++i) {
        // 打印基于1的索引供用户查看，及其得分
        printf("特征 %d (原始索引 %d), 得分: %.4f, 归一化得分: %.4f\n", 
               feature_indices[i] + 1, 
               feature_indices[i],               importance_scores[i], // 局部排序的非归一化得分
               importance_scores_all[feature_indices[i]]); // 全局存储的归一化得分
    }
    printf("\n");
}

// 测试网络
void test() {
    rmse = 0;
    
    printf("\n实际值 vs 预测值（只显示前10个样本）:\n");
    for (int i = 0; i < DATA_TEST; ++i) {
        double pred_norm = forward_pass(test_input[i]);
        double prediction = pred_norm * (output_max[0] - output_min[0]) + output_min[0];
        double actual = test_output[i][0];
        
        rmse += (prediction - actual) * (prediction - actual);
        
        if (i < 10) {
            printf("样本%2d 实际: %7.2f  预测: %7.2f\n", i+1, actual, prediction);
        }
    }
    
    rmse = sqrt(rmse / DATA_TEST);
    printf("\nRMSE: %.4f\n", rmse);
}

// 工具函数：Fisher-Yates洗牌算法
void shuffle_data(int* index, int n) {
    for (int i = n-1; i > 0; --i) {
        int j = rand() % (i+1);
        int temp = index[i];
        index[i] = index[j];
        index[j] = temp;
    }
}

int main() {

    // 初始训练
    read_data();
    normalize_data();
    init_network();
    printf("\n--- 初始训练 (使用所有特征) ---\n");
    train();
    test();
    
    // 特征分析
    analyze_features();
    
    // 基于重要特征重新训练
    printf("\n--- 第二轮训练 (使用所有特征) ---\n");
    init_network();
    train();
    test();  // 调用更新的测试函数

    return 0;
}
