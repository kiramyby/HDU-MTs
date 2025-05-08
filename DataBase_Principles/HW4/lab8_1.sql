-- Work in PostgreSQL
/* 创建12~15题的触发器，并验证其正确性
12. 对于成绩管理数据库，为成绩管理数据库中的 Student 表创建一个触发器：当向Student表中插入或删除记录时，修改Class 表中相应班级的人数。     
13. 为 Class 表再建一个更新触发器：当更新班长学号时，检查新输入的学号是否为同一班级的学生学号，若不是，更新操作取消。
14. 创建一个反映学生选修课程总学分的视图，使用触发器实现：当选课成绩大于等于60分，才能获得该课程的学分。
15.为Student表创建一插入和更新触发器：当插入新的学生或者更新学生所在班级时，检查该班级的学生人数有没有超过40人，如果没有超过則插入或者更新成功，如果超过40人，则操作回滚。
*/
-- 先删除已存在的触发器(如果有)
-- DROP TRIGGER IF EXISTS update_class_number_trig ON Student;
-- DROP TRIGGER IF EXISTS check_monitor_trig ON Class;
-- DROP TRIGGER IF EXISTS update_credit_trig ON Grade;
-- DROP TRIGGER IF EXISTS check_class_size_trig ON Student;

-- DROP FUNCTION IF EXISTS update_class_number();
-- DROP FUNCTION IF EXISTS check_monitor();
-- DROP FUNCTION IF EXISTS update_credit();
-- DROP FUNCTION IF EXISTS check_class_size();

-- 12. 当向Student表中插入或删除记录时，修改Class表中相应班级的人数
CREATE OR REPLACE FUNCTION update_class_number() RETURNS TRIGGER AS $$
    BEGIN
        IF TG_OP = 'INSERT' THEN
            UPDATE Class SET Number = Number + 1 
            WHERE Clno = NEW.Clno;
            RETURN NEW;
        ELSIF TG_OP = 'DELETE' THEN
            UPDATE Class SET Number = Number - 1 
            WHERE Clno = OLD.Clno;
            RETURN OLD;
        ELSIF TG_OP = 'UPDATE' AND OLD.Clno <> NEW.Clno THEN
            UPDATE Class SET Number = Number - 1 
            WHERE Clno = OLD.Clno;
            UPDATE Class SET Number = Number + 1 
            WHERE Clno = NEW.Clno;
            RETURN NEW;
        END IF;
        RETURN NULL;
    END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER update_class_number_trig
AFTER INSERT OR DELETE OR UPDATE OF Clno ON Student
FOR EACH ROW
EXECUTE PROCEDURE update_class_number();

-- 13. 当更新班长学号时，检查新输入的学号是否为同一班级的学生学号
CREATE OR REPLACE FUNCTION check_monitor() RETURNS TRIGGER AS $$
DECLARE
    student_clno CHAR(5);
BEGIN
    -- 只有在更新Monitor且新Monitor不为空时检查
    IF NEW.Monitor IS NOT NULL AND (OLD.Monitor IS NULL OR OLD.Monitor <> NEW.Monitor) THEN
        -- 查找该学生是否存在且属于该班级
        SELECT Clno INTO student_clno FROM Student WHERE Sno = NEW.Monitor;
        
        IF student_clno IS NULL THEN
            RAISE EXCEPTION '学号 % 不存在', NEW.Monitor;
        ELSIF student_clno <> NEW.Clno THEN
            RAISE EXCEPTION '学号 % 不属于班级 %', NEW.Monitor, NEW.Clno;
        END IF;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_monitor_trig
BEFORE UPDATE OF Monitor ON Class
FOR EACH ROW
EXECUTE PROCEDURE check_monitor();

-- 14. 创建视图及触发器：当选课成绩大于等于60分，才能获得该课程的学分
-- 首先创建视图
CREATE OR REPLACE VIEW StudentCreditView AS
SELECT s.Sno, s.Sname, SUM(CASE WHEN g.Gmark >= 60 THEN c.Ccredit ELSE 0 END) AS TotalCredit
FROM Student s
LEFT JOIN Grade g ON s.Sno = g.Sno
LEFT JOIN Course c ON g.Cno = c.Cno
GROUP BY s.Sno, s.Sname;

-- 创建触发器函数，用于更新学分视图
CREATE OR REPLACE FUNCTION update_credit() RETURNS TRIGGER AS $$
BEGIN
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


-- 创建触发器，监听Grade表的变化
CREATE TRIGGER update_credit_trig
AFTER INSERT OR UPDATE OF Gmark ON Grade
FOR EACH ROW
EXECUTE PROCEDURE update_credit();

-- 15. 检查班级人数是否超过40人
CREATE OR REPLACE FUNCTION check_class_size() RETURNS TRIGGER AS $$
DECLARE
    current_number INTEGER;
BEGIN
    -- 获取更新后班级的当前人数
    SELECT Number INTO current_number FROM Class WHERE Clno = NEW.Clno;
    
    -- 如果是插入新学生或者学生转班
    IF (TG_OP = 'INSERT' OR (TG_OP = 'UPDATE' AND OLD.Clno <> NEW.Clno)) THEN
        -- 检查加入该学生后班级人数是否超过40
        IF current_number > 40 THEN
            RAISE EXCEPTION '班级 % 人数已达到40人上限，无法添加新学生', NEW.Clno;
        END IF;
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_class_size_trig
BEFORE INSERT OR UPDATE OF Clno ON Student
FOR EACH ROW
EXECUTE PROCEDURE check_class_size();

-- 测试触发器

-- 清空测试数据
DELETE FROM Grade;
DELETE FROM Student;
DELETE FROM Course;
DELETE FROM Class;

-- 插入测试数据
-- 班级数据
INSERT INTO Class (Clno, Speciality, Inyear, Number, Monitor) 
    VALUES ('CS001', '计算机科学', '2022', 30, NULL);
INSERT INTO Class (Clno, Speciality, Inyear, Number, Monitor) 
    VALUES ('CS002', '软件工程', '2022', 38, NULL);

-- 学生数据
INSERT INTO Student (Sno, Sname, Ssex, Sbirth, Clno)
    VALUES ('2201001', '张三', '男', '2000-01-01', 'CS001');
INSERT INTO Student (Sno, Sname, Ssex, Sbirth, Clno)
    VALUES ('2201002', '李四', '女', '2000-05-15', 'CS001');
INSERT INTO Student (Sno, Sname, Ssex, Sbirth, Clno)
    VALUES ('2202001', '王五', '男', '2001-03-10', 'CS002');

-- 课程数据
INSERT INTO Course (Cno, Cname, Ccredit, Cpno)
    VALUES ('C01', '数据库原理', 4, NULL);
INSERT INTO Course (Cno, Cname, Ccredit, Cpno)
    VALUES ('C02', '数据结构', 5, NULL);
INSERT INTO Course (Cno, Cname, Ccredit, Cpno)
    VALUES ('C03', '高级数据库', 3, 'C01');

-- 成绩数据
INSERT INTO Grade (Sno, Cno, Gmark)
    VALUES ('2201001', 'C01', 85);
INSERT INTO Grade (Sno, Cno, Gmark)
    VALUES ('2201001', 'C02', 55);
INSERT INTO Grade (Sno, Cno, Gmark)
    VALUES ('2201002', 'C01', 92);
INSERT INTO Grade (Sno, Cno, Gmark)
    VALUES ('2202001', 'C01', 78);

-- 测试1: 验证触发器12 - 学生插入和删除影响班级人数
SELECT Clno, Number FROM Class ORDER BY Clno;
-- 删除一个学生
DELETE FROM Student WHERE Sno = '2201001';
-- 查看班级人数变化
SELECT Clno, Number FROM Class ORDER BY Clno;
-- 添加一个新学生
INSERT INTO Student (Sno, Sname, Ssex, Sbirth, Clno)
    VALUES ('2201003', '赵六', '男', '2000-11-11', 'CS001');
-- 查看班级人数变化
SELECT Clno, Number FROM Class ORDER BY Clno;

-- 测试2: 验证触发器13 - 更新班长学号
-- 将张三设为CS001班长
UPDATE Class SET Monitor = '2201002' WHERE Clno = 'CS001';
SELECT Clno, Monitor FROM Class WHERE Clno = 'CS001';
-- 尝试将非本班级的学生设为班长(应该失败)
UPDATE Class SET Monitor = '2202001' WHERE Clno = 'CS001';

-- 测试3: 验证触发器14 - 学分视图
SELECT * FROM StudentCreditView ORDER BY Sno;
-- 修改一个不及格成绩为及格
UPDATE Grade SET Gmark = 65 WHERE Sno = '2201001' AND Cno = 'C02';
-- 再次查看学分视图
SELECT * FROM StudentCreditView ORDER BY Sno;

-- 测试4: 验证触发器15 - 班级人数上限
-- 添加一个学生到人数接近上限的班级
INSERT INTO Student (Sno, Sname, Ssex, Sbirth, Clno)
    VALUES ('2202002', '刘七', '女', '2001-02-20', 'CS002');
-- 尝试再添加一个学生到人数超过上限的班级(应该失败)
INSERT INTO Student (Sno, Sname, Ssex, Sbirth, Clno)
    VALUES ('2202003', '孙八', '男', '2001-04-25', 'CS002');