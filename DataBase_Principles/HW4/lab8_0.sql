-- Work in PostgreSQL
--11. 使用SQL语句完成对表结构的完整性约束
CREATE TABLE Student (
    Sno CHAR(7) PRIMARY KEY,
    Sname VARCHAR(20) NOT NULL,
    Ssex CHAR(2) NOT NULL DEFAULT '男',
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

-- 添加约束逻辑
-- Student表约束
ALTER TABLE Student
    ADD CONSTRAINT chk_student_ssex CHECK (Ssex IN ('男', '女')),
    ADD CONSTRAINT chk_student_sbirth CHECK (Sbirth < CURRENT_DATE),
    ADD CONSTRAINT fk_student_class FOREIGN KEY (Clno) 
        REFERENCES Class(Clno) ON UPDATE CASCADE;

-- Course表约束
ALTER TABLE Course
    ADD CONSTRAINT chk_course_credit CHECK (Ccredit IN (1, 2, 3, 4, 5, 6)),
    ADD CONSTRAINT fk_course_cpno FOREIGN KEY (Cpno) REFERENCES Course(Cno);

-- Class表约束
ALTER TABLE Class
    ADD CONSTRAINT chk_class_number CHECK (Number > 25 AND Number < 50),
    ADD CONSTRAINT fk_class_monitor FOREIGN KEY (Monitor) REFERENCES Student(Sno);

-- Grade表约束
ALTER TABLE Grade
    ADD CONSTRAINT chk_grade_gmark CHECK (Gmark > 0 AND Gmark < 100),
    ADD CONSTRAINT fk_grade_student FOREIGN KEY (Sno) 
        REFERENCES Student(Sno) ON UPDATE CASCADE ON DELETE CASCADE,
    ADD CONSTRAINT fk_grade_course FOREIGN KEY (Cno) 
        REFERENCES Course(Cno) ON UPDATE CASCADE ON DELETE CASCADE;


