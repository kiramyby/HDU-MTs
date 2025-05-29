#!/bin/bash

echo "等待MySQL服务启动..."
sleep 30

echo "开始运行测试查询..."

# 连接MySQL并运行测试查询
mysql -h mysql -u root -proot123 HospitalManagement < /test_queries.sql

echo "测试完成！"

# 保持容器运行一段时间以便查看结果
sleep 60
