-- 创建数据库
CREATE DATABASE IF NOT EXISTS CompanyManagement;
USE CompanyManagement;

-- 1. 员工信息表 (Employee)
CREATE TABLE Employee (
    EID CHAR(10) NOT NULL,
    EName VARCHAR(20) NOT NULL,
    Gender CHAR(1) NOT NULL,
    Performance DECIMAL(8,2) NULL,

    PRIMARY KEY (EID),
    CONSTRAINT chk_gender CHECK (Gender IN ('0', '1')),
    CONSTRAINT chk_performance CHECK (Performance >= 0)
);

-- 2. 商店信息表 (Shop)、
CREATE TABLE Shop (
    ShopID CHAR(8) NOT NULL,
    ShopName VARCHAR(50) NOT NULL,
    Address CHAR(100) NULL,

    PRIMARY KEY (ShopID)
);

-- 3. 商品信息表 (Product)
CREATE TABLE Product (
    ProductID CHAR(8) NOT NULL,
    ProductName VARCHAR(50) NOT NULL,
    Spec VARCHAR(20) NULL,
    UnitPrice DECIMAL(8,2) NOT NULL,

    PRIMARY KEY (ProductID),
    CONSTRAINT chk_unit_price CHECK (UnitPrice > 0)
);

-- 4. 聘用信息表 (Employment)
CREATE TABLE Employment (
    EID CHAR(10) NOT NULL,
    ShopID CHAR(8) NOT NULL,
    HireDate DATE NOT NULL,
    Salary DECIMAL(10,2) NOT NULL,

    PRIMARY KEY (EID, ShopID),
    CONSTRAINT fk_employment_employee FOREIGN KEY (EID) REFERENCES Employee(EID) 
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_employment_shop FOREIGN KEY (ShopID) REFERENCES Shop(ShopID) 
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT chk_salary CHECK (Salary > 0)
);

-- 5. 销售信息表 (Sales)
CREATE TABLE Sales (
    ShopID CHAR(8) NOT NULL,
    ProductID CHAR(8) NOT NULL,
    SalesQty INT NOT NULL,

    PRIMARY KEY (ShopID, ProductID),
    CONSTRAINT fk_sales_shop FOREIGN KEY (ShopID) REFERENCES Shop(ShopID) 
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_sales_product FOREIGN KEY (ProductID) REFERENCES Product(ProductID) 
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT chk_sales_qty CHECK (SalesQty >= 0)
);


-- 6. 创建索引以提高查询性能
-- 员工表索引
CREATE INDEX idx_employee_name ON Employee(EName);
CREATE INDEX idx_employee_performance ON Employee(Performance);

-- 商店表索引
CREATE INDEX idx_shop_name ON Shop(ShopName);

-- 商品表索引
CREATE INDEX idx_product_name ON Product(ProductName);
CREATE INDEX idx_product_price ON Product(UnitPrice);

-- 聘用信息表索引
CREATE INDEX idx_employment_hire_date ON Employment(HireDate);
CREATE INDEX idx_employment_salary ON Employment(Salary);

-- 销售信息表索引
CREATE INDEX idx_sales_qty ON Sales(SalesQty);

-- 7. 创建视图用于数据查询
-- 员工详细信息视图
CREATE VIEW v_employee_details AS
SELECT 
    e.EID,
    e.EName,
    CASE e.Gender WHEN '1' THEN '男' ELSE '女' END AS Gender,
    e.Performance,
    s.ShopName,
    emp.Salary,
    emp.HireDate
FROM Employee e
LEFT JOIN Employment emp ON e.EID = emp.EID
LEFT JOIN Shop s ON emp.ShopID = s.ShopID;

-- 销售汇总视图
CREATE VIEW v_sales_summary AS
SELECT 
    s.ShopName,
    p.ProductName,
    p.UnitPrice,
    sal.SalesQty,
    (p.UnitPrice * sal.SalesQty) AS TotalSales
FROM Sales sal
JOIN Shop s ON sal.ShopID = s.ShopID
JOIN Product p ON sal.ProductID = p.ProductID;


-- 8. 用户角色和权限管理
-- 创建管理员角色
CREATE ROLE 'admin_role';
GRANT ALL PRIVILEGES ON CompanyManagement.* TO 'admin_role';

-- 创建人力资源角色
CREATE ROLE 'hr_role';
GRANT SELECT, INSERT, UPDATE ON CompanyManagement.Employee TO 'hr_role';
GRANT SELECT, INSERT, UPDATE ON CompanyManagement.Employment TO 'hr_role';
GRANT SELECT ON CompanyManagement.Shop TO 'hr_role';
GRANT SELECT ON CompanyManagement.v_employee_details TO 'hr_role';

-- 创建销售人员角色
CREATE ROLE 'sales_role';
GRANT SELECT ON CompanyManagement.Sales TO 'sales_role';
GRANT SELECT ON CompanyManagement.Product TO 'sales_role';
GRANT SELECT ON CompanyManagement.Shop TO 'sales_role';
GRANT SELECT ON CompanyManagement.v_sales_summary TO 'sales_role';

-- 创建运营管理角色
CREATE ROLE 'ops_role';
GRANT SELECT, INSERT, UPDATE ON CompanyManagement.Shop TO 'ops_role';
GRANT SELECT, INSERT, UPDATE ON CompanyManagement.Product TO 'ops_role';
GRANT SELECT ON CompanyManagement.Sales TO 'ops_role';


-- 9. 存储过程和函数
-- 添加员工的存储过程
DELIMITER //
CREATE PROCEDURE sp_add_employee(
    IN p_eid CHAR(10),
    IN p_name VARCHAR(20),
    IN p_gender CHAR(1),
    IN p_performance DECIMAL(8,2)
)
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    START TRANSACTION;
    
    INSERT INTO Employee (EID, EName, Gender, Performance)
    VALUES (p_eid, p_name, p_gender, p_performance);
    
    COMMIT;
END //
DELIMITER ;

-- 计算员工总薪资的函数
DELIMITER //
CREATE FUNCTION fn_calculate_total_salary(p_eid CHAR(10))
RETURNS DECIMAL(10,2)
READS SQL DATA
DETERMINISTIC
BEGIN
    DECLARE total_salary DECIMAL(10,2) DEFAULT 0;
    
    SELECT SUM(Salary) INTO total_salary
    FROM Employment
    WHERE EID = p_eid;
    
    RETURN IFNULL(total_salary, 0);
END //
DELIMITER ;


-- 10. 触发器，用于审计和数据完整性
-- 创建审计日志表
CREATE TABLE audit_log (
    log_id INT AUTO_INCREMENT PRIMARY KEY,
    table_name VARCHAR(50),
    operation VARCHAR(10),
    old_values JSON,
    new_values JSON,
    user_name VARCHAR(50),
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 员工表变更记录触发器
DELIMITER //
CREATE TRIGGER tr_employee_audit
AFTER UPDATE ON Employee
FOR EACH ROW
BEGIN
    INSERT INTO audit_log (table_name, operation, old_values, new_values, user_name)
    VALUES (
        'Employee',
        'UPDATE',
        JSON_OBJECT('EID', OLD.EID, 'EName', OLD.EName, 'Gender', OLD.Gender, 'Performance', OLD.Performance),
        JSON_OBJECT('EID', NEW.EID, 'EName', NEW.EName, 'Gender', NEW.Gender, 'Performance', NEW.Performance),
        USER()
    );
END //
DELIMITER ;

COMMIT;