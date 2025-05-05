/* 完成第17、18题，并验证其安全机制的正确性
17. 针对成绩管理数据库中的表，（假设 DBMS 中已经存在张勇、李勇、李勇敏三个用户）完成以下操作：
（1）用户张勇对 Student 表和 Course表有 SELECT 权限。
（2） 将对表 Student 的 INSERT 和DELETE 权限授予用户张勇，并允许他再把此权限授予其他用户。
（3） 将查询 Course 表和修改属性 Credit 的权限授予用户李勇。
（4） 授予用户李勇敏对Student表的所有权限（读、插、删、改），并具有给其他用户授权的权限。
（5） 撤销（1）中对用户张勇所授予的所有权限。
（6） 撤销（2）中对用户张勇所授予的所有权限。
（7） 将创建表、创建存储过程的权限授予用户张勇和用户李勇。
18. 角色和用户的联系与区别是什么？试用角色和用户管理实现以下两个案例的权限管理：
（1）计算机科学与技术专业负责人角色拥有对计算机科学与技术专业学生每门课程选课人数、平均成绩、最高成绩、最低成绩的查询权限，却不能查看其他专业的课程成绩情况。
（2）分别授予用户张老师、李老师计算机科学与技术专业负责人的权限。
*/

-- 17.
-- 首先确保这三个用户存在（实际操作中如用户已存在可省略）
-- CREATE USER 张勇 IDENTIFIED BY 'password';
-- CREATE USER 李勇 IDENTIFIED BY 'password';
-- CREATE USER 李勇敏 IDENTIFIED BY 'password';

-- （1）用户张勇对 Student 表和 Course表有 SELECT 权限。
GRANT SELECT ON Student TO 张勇;
GRANT SELECT ON Course TO 张勇;

-- （2） 将对表 Student 的 INSERT 和DELETE 权限授予用户张勇，并允许他再把此权限授予其他用户。
GRANT INSERT, DELETE ON Student TO 张勇 WITH GRANT OPTION;

-- （3） 将查询 Course 表和修改属性 Credit 的权限授予用户李勇。
GRANT SELECT ON Course TO 李勇;
GRANT UPDATE (Credit) ON Course TO 李勇;

-- （4） 授予用户李勇敏对Student表的所有权限（读、插、删、改），并具有给其他用户授权的权限。
GRANT ALL PRIVILEGES ON Student TO 李勇敏 WITH GRANT OPTION;

-- （5） 撤销（1）中对用户张勇所授予的所有权限。
REVOKE SELECT ON Student FROM 张勇;
REVOKE SELECT ON Course FROM 张勇;

-- （6） 撤销（2）中对用户张勇所授予的所有权限。
REVOKE INSERT, DELETE ON Student FROM 张勇;

-- （7） 将创建表、创建存储过程的权限授予用户张勇和用户李勇。
GRANT CREATE TABLE, CREATE PROCEDURE TO 张勇, 李勇;

-- 权限验证（可以用下面的语句验证权限设置是否成功）
-- 连接张勇用户并尝试操作
-- CONNECT 张勇/password;
-- SELECT * FROM Student; -- 测试是否已撤销权限，应该失败
-- CREATE TABLE Test(id INT); -- 测试是否有创建表权限

-- 连接李勇用户并尝试操作
-- CONNECT 李勇/password;
-- SELECT * FROM Course; -- 应该成功
-- UPDATE Course SET Credit = 5 WHERE CourseID = 'C001'; -- 应该成功
-- UPDATE Course SET CourseName = 'New Name' WHERE CourseID = 'C001'; -- 应该失败，因为只有权限修改Credit

-- 连接李勇敏用户并尝试操作
-- CONNECT 李勇敏/password;
-- SELECT * FROM Student; -- 应该成功
-- INSERT INTO Student VALUES(...); -- 应该成功
-- UPDATE Student SET ... WHERE ...; -- 应该成功
-- DELETE FROM Student WHERE ...; -- 应该成功
-- GRANT SELECT ON Student TO 测试用户; -- 应该成功，因为有WITH GRANT OPTION

-- 题目18实现

-- 角色和用户的联系与区别:
-- 1. 联系：都是数据库安全机制中的主体，都可以被授予权限
-- 2. 区别：
--    a. 角色是权限的集合，可以被授予给用户或其他角色
--    b. 用户是可以登录到数据库的实体账户
--    c. 角色便于管理大量用户的权限，一次性将一组权限分配给多个用户
--    d. 对角色权限的修改会影响所有被授予该角色的用户

-- （1）创建计算机科学与技术专业负责人角色
CREATE ROLE CS_Director;

-- 为该角色授予查询计算机科学与技术专业学生课程统计信息的权限
-- 假设有SC(StudentID, CourseID, Grade)表存储学生成绩
-- 假设Student表中有Major字段表示学生专业
GRANT SELECT ON Student TO CS_Director;
GRANT SELECT ON Course TO CS_Director;
GRANT SELECT ON SC TO CS_Director;

-- 创建视图以限制只能查看计算机科学与技术专业学生的课程成绩统计
CREATE VIEW CS_Course_Stats AS
SELECT c.CourseID, c.CourseName, 
       COUNT(*) AS StudentCount,
       AVG(sc.Grade) AS AvgGrade,
       MAX(sc.Grade) AS MaxGrade,
       MIN(sc.Grade) AS MinGrade
FROM SC sc
JOIN Student s ON sc.StudentID = s.StudentID
JOIN Course c ON sc.CourseID = c.CourseID
WHERE s.Major = '计算机科学与技术'
GROUP BY c.CourseID, c.CourseName;

-- 授予查询视图的权限给角色
GRANT SELECT ON CS_Course_Stats TO CS_Director;

-- 撤销对基础表的直接查询权限，只允许通过视图访问
REVOKE SELECT ON SC FROM CS_Director;

-- （2）授予用户张老师、李老师计算机科学与技术专业负责人的权限
-- 首先创建用户（如果需要）
-- CREATE USER 张老师 IDENTIFIED BY 'password';
-- CREATE USER 李老师 IDENTIFIED BY 'password';

-- 将角色授予用户
GRANT CS_Director TO 张老师, 李老师;

-- 验证授权
-- 用户登录后查询
-- CONNECT 张老师/password;
-- SELECT * FROM CS_Course_Stats; -- 应该成功
-- SELECT * FROM SC; -- 应该失败，因为已撤销直接查询权限

-- CONNECT 李老师/password;
-- SELECT * FROM CS_Course_Stats; -- 应该成功
-- SELECT * FROM SC; -- 应该失败

