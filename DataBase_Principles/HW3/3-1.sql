-- Work in PostgreSQL
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
