/*15.针对习题10的四个表，用SQL语言完成以下各项操作：
(1)将20311班的全体学生的成绩置0值。
(2)删除2021级计算机软件的全体学生的选课记录。
(3)学生李勇已退学，从数据库中删除有关他的记录。
（4)对每个班，求学生的平均年龄，并把结果存入数据库*/

-- (1) 将20311班的全体学生的成绩置0值
UPDATE Grade
SET Gmark = 0
WHERE Sno IN (SELECT Sno FROM Student WHERE Clno = '20311');

-- (2) 删除2021级计算机软件的全体学生的选课记录
DELETE FROM Grade
WHERE Sno IN (
    SELECT s.Sno
    FROM Student s
    JOIN Class c ON s.Clno = c.Clno
    WHERE c.Inyear = '2021' AND c.Speciality = '软件工程'
);

-- (3) 学生李勇已退学，从数据库中删除有关他的记录
-- 先删除成绩记录
DELETE FROM Grade WHERE Sno = '2020101';
-- 更新班级表中的班长信息（如果李勇是班长）
UPDATE Class SET Monitor = NULL WHERE Monitor = '2020101';
-- 最后删除学生记录
DELETE FROM Student WHERE Sno = '2020101' AND Sname = '李勇';

-- (4) 对每个班，求学生的平均年龄，并把结果存入数据库
-- 创建存储平均年龄的表
CREATE TABLE ClassAvgAge (
    Clno CHAR(5) PRIMARY KEY,
    AvgAge NUMERIC(5,2)
);

-- 计算并插入平均年龄数据
INSERT INTO ClassAvgAge (Clno, AvgAge)
SELECT s.Clno, 
       AVG(EXTRACT(YEAR FROM CURRENT_DATE) - EXTRACT(YEAR FROM s.Sbirth)) AS AvgAge
FROM Student s
GROUP BY s.Clno;
