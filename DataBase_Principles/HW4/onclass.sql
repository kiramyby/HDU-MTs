/*1. 创建储存过程实现传入用户名和密码，插入到admin表中*/

CREATE PROCEDURE insert_admin(
    IN username VARCHAR(50),
    IN loginpwd VARCHAR(50)
)
AS
BEGIN
    INSERT INTO admin (username, password)
    VALUES (username, loginpwd);
END;

/*2. 创建储存过程实现传入员工的编号，返回员工的名称和电话*/
CREATE PROCEDURE get_employee_info (
    IN eno INT
    OUT ename VARCHAR(32),
    OUT ephone VARCHAR(32)
)
AS
BEGIN
    SELECT ename, 
           ephone
    INTO ename, ephone
    FROM employee
    WHERE eno = eno;   
END$