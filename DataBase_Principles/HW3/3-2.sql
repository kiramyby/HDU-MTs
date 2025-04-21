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
