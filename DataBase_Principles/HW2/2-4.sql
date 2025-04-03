SELECT Cno FROM Grade;
SELECT * FROM Student WHERE Clno = '20311' and Ssex = '女';
SELECT (Sname,Ssex, Sbirth) FROM Student WHERE Clno = '20311' or Clno = '20312';
SELECT * FROM Student WHERE Sname LIKE '李%';
SELECT COUNT(*) FROM Student WHERE Clno = (
    SELECT Clno FROM Student WHERE Sname = '李勇'
);
SELECT AVERAGE(Gmark),MIN(Gmark),MAX(Gmark) FROM Grade WHERE Cno = (
    SELECT Cno FROM Course WHERE Cname = '操作系统'
);
SELECT COUNT(*) FROM Grade Group BY Sno;
SELECT COUNT(*) FROM Grade Group BY Sno HAVING Cno = (
    SELECT Cno FROM Course WHERE Cname = '操作系统'
);
SELECT Sname FROM Student WHERE Sno LIKE '2020%' and NOT IN (
    SELECT Sno FROM Grade;
)
SELECT Cname FROM Course WHERE Cpno = NULL or ( Cno IN (
    SELECT Cpno FROM Course;
));

