-- Work in PostgreSQL

CREATE TABLE Student (
    Sno CHAR(7) PRIMARY KEY,
        Sname VARCHAR(20) NOT NULL,
        Ssex CHAR(2) NOT NULL,
        Sbirth DATE,
        Clno CHAR(5) NOT NULL
);

CREATE TABLE Course (
        Cno CHAR(3) PRIMARY KEY,
        Cname VARCHAR(20) NOT NULL,
        Ccredit SMALLINT,
        Cpno CHAR(3)
);

CREATE TABLE Class (
        Clno CHAR(5) PRIMARY KEY,
        Speciality VARCHAR(20) NOT NULL,
        Inyear CHAR(4) NOT NULL,
        Number INTEGER,
        Monitor CHAR(7)
);

CREATE TABLE Grade (
        Sno CHAR(7) NOT NULL,
        Cno CHAR(3) NOT NULL,
        Gmark NUMERIC(4, 1),
        PRIMARY KEY (Sno, Cno)
);

INSERT INTO Class (Clno, Speciality, Inyear, Number, Monitor) VALUES
('20311', '软件工程', '2020', 35, '2020101'),
('20312', '计算机科学与技术', '2020', 38, '2020103'),
('21311', '软件工程', '2021', 40, '2021103');

INSERT INTO Student (Sno, Sname, Ssex, Sbirth, Clno) VALUES
('2020101', '李勇', '男', '2002-08-09', '20311'),
('2020102', '刘诗晨', '女', '2003-04-01', '20311'),
('2020103', '王一鸣', '男', '2002-12-25', '20312'),
('2020104', '张婷婷', '女', '2002-10-01', '20312'),
('2021101', '李勇敏', '女', '2003-11-11', '21311'),
('2021102', '贾向东', '男', '2003-12-12', '21311'),
('2021103', '陈宝玉', '男', '2004-05-01', '21311'),
('2021104', '张逸凡', '男', '2005-01-01', '21311');

INSERT INTO Course (Cno, Cname, Ccredit, Cpno) VALUES
('1', '数据库系统原理', 4, '5'),                
('2', '计算机系统结构', 3, '8'),                 
('3', '数字电路设计', 2, NULL),                
('4', '操作系统', 4, '8'),                     
('5', '数据结构', 4, '7'),                      
('6', '软件工程', 2, '1'),                      
('7', 'C语言', 4, NULL),                       
('8', '计算机组成原理', 4, '3');

INSERT INTO Grade (Sno, Cno, Gmark) VALUES
('2020101', '1', 92),
('2020101', '3', 88),
('2020101', '5', 86),
('2020102', '1', 78),
('2020102', '6', 55),
('2020103', '3', 65),
('2020103', '6', 78),
('2020103', '5', 66),
('2020104', '1', 54),
('2020104', '6', 83),
('2021101', '2', 70),
('2021101', '4', 65),
('2021102', '2', 80),
('2021102', '4', 90),
('2020103', '1', 83),
('2020103', '2', 76),
('2020103', '4', 56),
('2020103', '7', 88);

/*13.针对习题10的四个表，用SELECT的嵌套查询完成以下各项查询：
（1）找出与李勇在同一个班级的学生信息。
（2）找出所有与学生李勇有相同选修课程的学生信息。
(3)找出年龄介于学生李勇和25岁之间的学生信息（已知李勇的年龄小于25岁）。
(4)找出选修了课程操作系统的学生学号和姓名。
(5)找出所有没有选修1号课程的学生姓名。
(6)找出每个学生超过他选修课程的平均成绩的学生号和课程号。
(7）找出选修了全部课程的学生姓名。
(提示：可找出这样的学生，没有一门课程是他不选修的。)
(8)找出数据库系统原理成绩高于平均分的所有学生学号、姓名成绩。
(9）找出每个班级课程数据库系统原理成绩高于本班数据库系统原理平均分的所有学
生的学号、姓名、成绩。
(10)找出至少选修了"2020101"号学生选修的全部课程的学生学号。
14.针对习题10的四个表，用SQL语言完成以下各项查询：
(1)'查询选修了3号课程的学生学号及其成绩，并按成绩的降序排列己减薄的意惠
(2)查询全体学生信息，要求查询结果按班级号升序排列，同一班级学生按年龄降序
排列。
(3)查询每个课程号及相应的选课人数。
(4）查询选修了三门以上课程的学生学号。
(5)查询至少选修1号和2号课程的学生的学号和姓名
（6）查询每门课程成绩前三名的学生学号、姓名、课程号和成绩。
(7）查询每个学生学号、姓名和选修课程的总学分，并按总学分降序排序。
*/

-- 13.
-- (1) 找出与李勇在同一个班级的学生信息。
SELECT * FROM Student
WHERE Clno = (SELECT Clno FROM Student WHERE Sname = '李勇');

-- (2) 找出所有与学生李勇有相同选修课程的学生信息。
SELECT DISTINCT S.*
FROM Student S
WHERE S.Sno IN (
    SELECT G.Sno 
    FROM Grade G
    WHERE G.Cno IN (
        SELECT G2.Cno
        FROM Grade G2
        WHERE G2.Sno = (SELECT Sno FROM Student WHERE Sname = '李勇')
    )
    AND S.Sno != (SELECT Sno FROM Student WHERE Sname = '李勇')
);

-- (3) 找出年龄介于学生李勇和25岁之间的学生信息（已知李勇的年龄小于25岁）。
SELECT *
FROM Student
WHERE Sbirth BETWEEN (
    CURRENT_DATE - INTERVAL '25 year'
) AND (
    SELECT Sbirth FROM Student WHERE Sname = '李勇'
);

-- (4) 找出选修了课程操作系统的学生学号和姓名。
SELECT S.Sno, S.Sname
FROM Student S
WHERE S.Sno IN (
    SELECT G.Sno
    FROM Grade G
    WHERE G.Cno = (
        SELECT C.Cno
        FROM Course C
        WHERE C.Cname = '操作系统'
    )
);

-- (5) 找出所有没有选修1号课程的学生姓名。
SELECT S.Sname
FROM Student S
WHERE S.Sno NOT IN (
    SELECT G.Sno
    FROM Grade G
    WHERE G.Cno = '1'
);

-- (6) 找出每个学生超过他选修课程的平均成绩的学生号和课程号。
SELECT G.Sno, G.Cno
FROM Grade G
WHERE G.Gmark > (
    SELECT AVG(G2.Gmark)
    FROM Grade G2
    WHERE G2.Sno = G.Sno
);

-- (7) 找出选修了全部课程的学生姓名。
SELECT S.Sname
FROM Student S
WHERE NOT EXISTS (
    SELECT C.Cno
    FROM Course C
    WHERE NOT EXISTS (
        SELECT G.Cno
        FROM Grade G
        WHERE G.Sno = S.Sno AND G.Cno = C.Cno
    )
);

-- (8) 找出数据库系统原理成绩高于平均分的所有学生学号、姓名成绩。
SELECT S.Sno, S.Sname, G.Gmark
FROM Student S, Grade G
WHERE S.Sno = G.Sno
AND G.Cno = '1'
AND G.Gmark > (
    SELECT AVG(G2.Gmark)
    FROM Grade G2
    WHERE G2.Cno = '1'
);

-- (9) 找出每个班级课程数据库系统原理成绩高于本班数据库系统原理平均分的所有学生的学号、姓名、成绩。
SELECT S.Sno, S.Sname, G.Gmark
FROM Student S, Grade G
WHERE S.Sno = G.Sno
AND G.Cno = '1'
AND G.Gmark > (
    SELECT AVG(G2.Gmark)
    FROM Grade G2
    JOIN Student S2 ON G2.Sno = S2.Sno
    WHERE G2.Cno = '1' AND S2.Clno = S.Clno
);

-- (10) 找出至少选修了"2020101"号学生选修的全部课程的学生学号。
SELECT DISTINCT S.Sno
FROM Student S
WHERE NOT EXISTS (
    SELECT G1.Cno
    FROM Grade G1
    WHERE G1.Sno = '2020101'
    AND NOT EXISTS (
        SELECT G2.Cno
        FROM Grade G2
        WHERE G2.Sno = S.Sno AND G2.Cno = G1.Cno
    )
) AND S.Sno != '2020101';

-- 14.
-- (1) 查询选修了3号课程的学生学号及其成绩，并按成绩的降序排列
SELECT G.Sno, G.Gmark
FROM Grade G
WHERE G.Cno = '3'
ORDER BY G.Gmark DESC;

-- (2) 查询全体学生信息，要求查询结果按班级号升序排列，同一班级学生按年龄降序排列。
SELECT *
FROM Student
ORDER BY Clno ASC, Sbirth ASC;

-- (3) 查询每个课程号及相应的选课人数。
SELECT G.Cno, COUNT(G.Sno) AS StudentsCount
FROM Grade G
GROUP BY G.Cno;

-- (4) 查询选修了三门以上课程的学生学号。
SELECT G.Sno
FROM Grade G
GROUP BY G.Sno
HAVING COUNT(G.Cno) > 3;

-- (5) 查询至少选修1号和2号课程的学生的学号和姓名
SELECT S.Sno, S.Sname
FROM Student S
WHERE S.Sno IN (
    SELECT G1.Sno
    FROM Grade G1
    WHERE G1.Cno = '1'
) AND S.Sno IN (
    SELECT G2.Sno
    FROM Grade G2
    WHERE G2.Cno = '2'
);

-- (6) 查询每门课程成绩前三名的学生学号、姓名、课程号和成绩。
SELECT S.Sno, S.Sname, G1.Cno, G1.Gmark
FROM Student S, Grade G1
WHERE S.Sno = G1.Sno
AND (
    SELECT COUNT(*)
    FROM Grade G2
    WHERE G2.Cno = G1.Cno AND G2.Gmark > G1.Gmark
) < 3
ORDER BY G1.Cno, G1.Gmark DESC;

-- (7) 查询每个学生学号、姓名和选修课程的总学分，并按总学分降序排序。
SELECT S.Sno, S.Sname, SUM(C.Ccredit) AS TotalCredits
FROM Student S
JOIN Grade G ON S.Sno = G.Sno
JOIN Course C ON G.Cno = C.Cno
GROUP BY S.Sno, S.Sname
ORDER BY TotalCredits DESC;
