-- Work in PostgreSQL
/*16.完成以下对视图的操作：
(1)建立20312班选修了1号课程的学生视图Stu_20312_1。
(2)建立20312班选修了1号课程并且成绩不及格的学生视图Stu_20312_2。
(3）建立视图Stu_age，由学生学号、姓名和年龄组成。
(4)查询2000年以后出生的学生姓名。
(5)查询20312班选修了1号课程并且成绩不及格的学生的学号、姓名和年龄。
(6）查询选课数超过两门的学生的平均成绩和选课门数。
(7）查询软件工程专业中比计算机科学与技术专业所有学生年龄小的学生学号、姓名和年龄。
(8)查询所有课程的平均成绩和不及格人数的百分比（不合格率），输出课程号、课程名、平均成绩和不及格人数的百分比。*/

-- (1)建立20312班选修了1号课程的学生视图Stu_20312_1。
CREATE VIEW Stu_20312_1 AS
SELECT s.* 
FROM Student s 
JOIN Grade g ON s.Sno = g.Sno
WHERE s.Clno = '20312' AND g.Cno = '1';

-- (2)建立20312班选修了1号课程并且成绩不及格的学生视图Stu_20312_2。
CREATE VIEW Stu_20312_2 AS
SELECT s.* 
FROM Student s 
JOIN Grade g ON s.Sno = g.Sno
WHERE s.Clno = '20312' AND g.Cno = '1' AND g.Gmark < 60;

-- (3）建立视图Stu_age，由学生学号、姓名和年龄组成。
CREATE VIEW Stu_age AS
SELECT Sno, Sname, EXTRACT(YEAR FROM AGE(CURRENT_DATE, Sbirth)) AS age
FROM Student;

-- (4)查询2000年以后出生的学生姓名。
SELECT Sname
FROM Student
WHERE Sbirth > '2000-12-31';

-- (5)查询20312班选修了1号课程并且成绩不及格的学生的学号、姓名和年龄。
SELECT a.Sno, a.Sname, a.age
FROM Stu_age a
JOIN Student s ON a.Sno = s.Sno
JOIN Grade g ON s.Sno = g.Sno
WHERE s.Clno = '20312' AND g.Cno = '1' AND g.Gmark < 60;

-- (6)查询选课数超过两门的学生的平均成绩和选课门数。
SELECT g.Sno, AVG(g.Gmark) AS average_score, COUNT(g.Cno) AS course_count
FROM Grade g
GROUP BY g.Sno
HAVING COUNT(g.Cno) > 2;

-- (7)查询软件工程专业中比计算机科学与技术专业所有学生年龄小的学生学号、姓名和年龄。
SELECT a.Sno, a.Sname, a.age
FROM Stu_age a
JOIN Student s ON a.Sno = s.Sno
JOIN Class c ON s.Clno = c.Clno
WHERE c.Speciality = '软件工程' 
AND a.age < ALL (
    SELECT a2.age
    FROM Stu_age a2
    JOIN Student s2 ON a2.Sno = s2.Sno
    JOIN Class c2 ON s2.Clno = c2.Clno
    WHERE c2.Speciality = '计算机科学与技术'
);

-- (8)查询所有课程的平均成绩和不及格人数的百分比（不合格率），输出课程号、课程名、平均成绩和不及格人数的百分比。
SELECT c.Cno, c.Cname, 
       AVG(g.Gmark) AS average_score,
       100.0 * COUNT(CASE WHEN g.Gmark < 60 THEN 1 END) / COUNT(*) AS failure_rate
FROM Course c
LEFT JOIN Grade g ON c.Cno = g.Cno
GROUP BY c.Cno, c.Cname;

-- EX 创建一个名为Class_grade的视图，用于反映每个班级的所有选修课的平均成绩，并对其进行更新操作
CREATE VIEW Class_grade AS
SELECT s.Clno, AVG(g.Gmark) AS avg_grade
FROM Student s
JOIN Grade g ON s.Sno = g.Sno
GROUP BY s.Clno;
-- 更新操作示例
UPDATE Class_grade
SET avg_grade = 85.0
WHERE Clno = '20311';