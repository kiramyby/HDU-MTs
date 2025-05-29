# 医院管理系统 Docker 环境

这个 Docker Compose 配置提供了一个完整的医院管理系统测试环境。

## 包含的服务

1. **MySQL 8.0** - 主数据库服务
   - 端口: 3306
   - 根用户密码: root123
   - 应用用户: hospital_user / hospital123

2. **Adminer** - Web 数据库管理界面
   - 端口: 8080
   - 访问地址: http://localhost:8080

3. **测试运行器** - 自动执行测试查询

## 快速开始

### 1. 启动环境
```bash
docker-compose up -d
```

### 2. 查看日志
```bash
# 查看所有服务日志
docker-compose logs -f

# 查看特定服务日志
docker-compose logs -f mysql
docker-compose logs -f test_runner
```

### 3. 访问数据库

#### 通过 Adminer Web 界面
1. 打开浏览器访问 http://localhost:8080
2. 登录信息：
   - 系统: MySQL
   - 服务器: mysql
   - 用户名: root
   - 密码: root123
   - 数据库: HospitalManagement

#### 通过命令行
```bash
# 连接到 MySQL 容器
docker-compose exec mysql mysql -u root -proot123 HospitalManagement

# 或者从本地连接（如果安装了 MySQL 客户端）
mysql -h localhost -P 3306 -u root -proot123 HospitalManagement
```

## 文件说明

- `make.sql` - 主数据库创建脚本（自动执行）
- `test_data.sql` - 测试数据插入脚本（自动执行）
- `test_queries.sql` - 测试查询脚本
- `run_tests.sh` - 测试执行脚本

## 测试内容

测试脚本会验证以下功能：
1. 基本表查询
2. 外键关系验证
3. 视图查询
4. 存储过程执行
5. 触发器功能
6. 约束验证
7. 用户权限检查
8. 索引性能
9. 财务统计
10. 复杂关联查询

## 清理环境

```bash
# 停止并删除容器
docker-compose down

# 同时删除数据卷（注意：会删除所有数据）
docker-compose down -v
```

## 故障排除

### MySQL 连接问题
如果遇到连接问题，请检查：
1. 端口 3306 是否被占用
2. 容器是否正常启动：`docker-compose ps`
3. MySQL 日志：`docker-compose logs mysql`

### 权限问题
如果遇到权限错误：
```bash
# 重新设置文件权限
chmod +x run_tests.sh
```

### 数据重置
如果需要重新初始化数据：
```bash
docker-compose down -v
docker-compose up -d
```
