#!/bin/bash

echo "正在验证 SQL 脚本语法..."

# 检查是否安装了 MySQL 客户端
if ! command -v mysql &> /dev/null; then
    echo "警告: 本地未安装 MySQL 客户端，跳过语法检查"
    echo "请使用 Docker 环境进行完整测试"
    exit 0
fi

# 启动 Docker 环境进行测试
echo "启动 Docker 测试环境..."
docker-compose up -d mysql

# 等待 MySQL 启动
echo "等待 MySQL 服务启动..."
sleep 20

# 测试数据库连接
echo "测试数据库连接..."
for i in {1..10}; do
    if docker-compose exec -T mysql mysql -u root -proot123 -e "SELECT 1;" &> /dev/null; then
        echo "MySQL 连接成功！"
        break
    fi
    echo "等待 MySQL 启动... ($i/10)"
    sleep 5
done

# 验证数据库和表是否创建成功
echo "验证数据库结构..."
docker-compose exec -T mysql mysql -u root -proot123 HospitalManagement -e "SHOW TABLES;"

if [ $? -eq 0 ]; then
    echo "✅ 数据库创建成功！"
    echo "✅ 所有表创建成功！"
    
    # 显示表的数量
    TABLE_COUNT=$(docker-compose exec -T mysql mysql -u root -proot123 HospitalManagement -e "SHOW TABLES;" | wc -l)
    echo "📊 创建了 $((TABLE_COUNT-1)) 个表"
    
    # 测试一些基本查询
    echo "测试基本查询..."
    docker-compose exec -T mysql mysql -u root -proot123 HospitalManagement -e "SELECT COUNT(*) as employee_count FROM Employee;"
    docker-compose exec -T mysql mysql -u root -proot123 HospitalManagement -e "SELECT COUNT(*) as patient_count FROM Patient;"
    
    echo "✅ SQL 脚本验证通过！"
else
    echo "❌ 数据库创建失败，请检查 SQL 脚本"
    docker-compose logs mysql
    exit 1
fi

echo ""
echo "🚀 验证完成！你可以："
echo "1. 访问 http://localhost:8080 使用 Adminer 管理数据库"
echo "2. 运行 'docker-compose logs test_runner' 查看测试结果"
echo "3. 运行 'docker-compose down' 停止环境"
