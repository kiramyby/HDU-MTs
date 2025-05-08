/* 创建17~19题的储存过程，并执行该储存过程以验证其正确性
创建20题的用户自定义函数，并调用该函数以验证其正确性
17. 对于第3章习题10中的成绩管理数据库，创建一个存储过程：根据学生学号查询该学生所有选修课的成绩，学号作为参数输入。
18.对于成绩管理数据库，创建一个存储过程，输入的参数是班级名，判断该班级中是否已有学生存在。若有，存储过程返回1；若没有，存储过程返回0。
19. 对于成绩管理数据库，创建一个带输出参数的存储过程：根据学生学号查询该学生的姓名和所学专业。
20. 将习题17～习题19的存储过程，分别改为用户自定义函数来实现。
*/

-- 创建17~19题的储存过程

-- 17. 根据学生学号查询该学生所有选修课的成绩
CREATE OR REPLACE PROCEDURE get_student_grades(IN student_id CHAR(7))
LANGUAGE plpgsql
AS $$
DECLARE
    temp_record RECORD;
BEGIN
    FOR temp_record IN 
        SELECT c.Cno, c.Cname, g.Gmark
        FROM Grade g
        JOIN Course c ON g.Cno = c.Cno
        WHERE g.Sno = student_id
        ORDER BY c.Cno
    LOOP
        RAISE NOTICE '课程号: %, 课程名: %, 成绩: %', 
            temp_record.Cno, temp_record.Cname, temp_record.Gmark;
    END LOOP;
END;
$$;

-- 18. 判断班级中是否已有学生存在
CREATE OR REPLACE PROCEDURE check_class_has_students(
    IN class_id CHAR(5),
    OUT result INTEGER
)
LANGUAGE plpgsql
AS $$
DECLARE
    student_count INTEGER;
BEGIN
    -- 查询该班级的学生数量
    SELECT COUNT(*) INTO student_count
    FROM Student
    WHERE Clno = class_id;
    
    -- 判断是否有学生
    IF student_count > 0 THEN
        result := 1;  -- 有学生存在
    ELSE
        result := 0;  -- 没有学生存在
    END IF;
END;
$$;

-- 19. 根据学生学号查询该学生的姓名和所学专业
CREATE OR REPLACE PROCEDURE get_student_info(
    IN student_id CHAR(7),
    OUT student_name VARCHAR(20),
    OUT student_speciality VARCHAR(20)
)
LANGUAGE plpgsql
AS $$
BEGIN
    -- 查询学生姓名和专业
    SELECT s.Sname, c.Speciality
    INTO student_name, student_speciality
    FROM Student s
    JOIN Class c ON s.Clno = c.Clno
    WHERE s.Sno = student_id;
END;
$$;

-- 20. 将习题17～习题19的存储过程改为用户自定义函数

-- 改写17题：根据学生学号查询该学生所有选修课的成绩
CREATE OR REPLACE FUNCTION get_student_grades_func(student_id CHAR(7))
RETURNS TABLE (
    course_id CHAR(3),
    course_name VARCHAR(20),
    grade NUMERIC(4,1)
)
LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
    SELECT c.Cno, c.Cname, g.Gmark
    FROM Grade g
    JOIN Course c ON g.Cno = c.Cno
    WHERE g.Sno = student_id
    ORDER BY c.Cno;
END;
$$;

-- 改写18题：判断班级中是否已有学生存在
CREATE OR REPLACE FUNCTION check_class_has_students_func(class_id CHAR(5))
RETURNS INTEGER
LANGUAGE plpgsql
AS $$
DECLARE
    student_count INTEGER;
BEGIN
    -- 查询该班级的学生数量
    SELECT COUNT(*) INTO student_count
    FROM Student
    WHERE Clno = class_id;
    
    -- 判断是否有学生
    IF student_count > 0 THEN
        RETURN 1;  -- 有学生存在
    ELSE
        RETURN 0;  -- 没有学生存在
    END IF;
END;
$$;

-- 改写19题：根据学生学号查询该学生的姓名和所学专业
CREATE OR REPLACE FUNCTION get_student_info_func(student_id CHAR(7))
RETURNS TABLE (
    student_name VARCHAR(20),
    speciality VARCHAR(20)
)
LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
    SELECT s.Sname, c.Speciality
    FROM Student s
    JOIN Class c ON s.Clno = c.Clno
    WHERE s.Sno = student_id;
END;
$$;

-- 测试存储过程和函数

-- 测试17题存储过程：查询学生成绩
CALL get_student_grades('2202001');

-- 测试18题存储过程：判断班级是否有学生
DO $$
DECLARE
    has_students INTEGER;
BEGIN
    CALL check_class_has_students('20311', has_students);
    RAISE NOTICE '班级20311是否有学生(1=有,0=没有): %', has_students;
    
    CALL check_class_has_students('20999', has_students); -- 不存在的班级
    RAISE NOTICE '班级20999是否有学生(1=有,0=没有): %', has_students;
END $$;

-- 测试19题存储过程：获取学生信息
DO $$
DECLARE
    student_name VARCHAR(20);
    student_speciality VARCHAR(20);
BEGIN
    CALL get_student_info('2020101', student_name, student_speciality);
    RAISE NOTICE '学生姓名: %, 专业: %', student_name, student_speciality;
END $$;

-- 测试20题函数：查询学生成绩
SELECT * FROM get_student_grades_func('2020101');

-- 测试20题函数：判断班级是否有学生
SELECT check_class_has_students_func('20311') AS has_students;
SELECT check_class_has_students_func('20999') AS has_students; -- 不存在的班级

-- 测试20题函数：获取学生信息
SELECT * FROM get_student_info_func('2020101');

