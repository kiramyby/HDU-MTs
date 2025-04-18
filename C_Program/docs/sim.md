# 学生信息管理系统(SIM)

## 系统架构
- 用户界面层
  - Windows控制台
  - 菜单驱动交互
  - GotoXY坐标定位
  - Window界面绘制
- 认证授权层
  - 注册/登录/登出
  - RBAC模型
  - 权限检查函数
    - IsAdmin
    - IsTeacher
    - IsStudent
- 业务逻辑层
  - CRUD操作
    - Create
    - Read
    - Update
    - Delete
  - 查询排序
    - queryStudent
    - sortGrades
  - 成绩管理
    - createGrade
    - updateGrade
- 数据持久层
  - 文本文件存储
    - store/目录结构
    - CSV格式
  - 数据校验
    - check.c
  - 动态加载
    - loadStudents
    - loadCourses
- 日志系统
  - LogSystemActivity
  - 按日存储
  - 管理员查看

## 数据结构
- Student结构体
  - id
  - name
  - grades链表
  - next指针
- Course结构体
  - code
  - name
  - teacher关联
  - grades链表
- Grade结构体
  - student_id
  - course_code
  - score
  - 双向链表
    - next_student
    - next_course
- UserRecord
  - username
  - password
  - userType
  - 权限位掩码

## 核心功能
- 用户管理
  - 多角色注册
  - 安全登录
  - 权限继承
- 学生管理
  - 增删改查
  - 成绩关联
  - 信息加密
- 课程管理
  - 课程分配
  - 教师绑定
  - 容量控制
- 成绩管理
  - 双维度录入
    - 学生维度
    - 课程维度
  - 统计排序
  - 修改审计

## 文件存储
- 数据文件
  - students.txt
    - ID,Name
  - courses.txt
    - Code,Name
  - grades.txt
    - SID,Code,Score
- 用户文件
  - [ID].user
    - 用户名
    - 密码
    - 类型
  - 映射文件
    - username.map
- 日志文件
  - [date].log
    - 时间戳
    - 用户行为
    - 操作详情

## 权限控制
- 角色类型
  - 管理员(0x10)
    - 全权限
  - 教师(0x20)
    - 课程级权限
  - 学生(0x30)
    - 个人数据
  - 访客(-1)
    - 只读受限
- 控制机制
  - 功能级拦截
  - 数据级过滤
  - 教师课程绑定
    - HasAccessToCourse
  - 学生数据隔离

## 设计特点
- 链表应用
  - 内存管理
  - 双向遍历
- 模块化
  - 分层架构
  - 接口分离
- 持久化
  - 文本存储
  - 动态加载
- 安全设计
  - 密码加密
  - 操作日志
  - 数据校验
  