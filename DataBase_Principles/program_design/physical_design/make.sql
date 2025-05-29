-- =============================================================================
-- 医院管理系统 SQL 实现
-- =============================================================================

-- 7 系统SQL实现

-- =============================================================================
-- 7.1 完整性设计
-- =============================================================================
/*
完整性设计说明：
1. 实体完整性：所有表都有主键，确保每行记录唯一标识
2. 参照完整性：通过外键约束确保数据一致性
3. 域完整性：通过数据类型、CHECK约束确保数据有效性
4. 用户自定义完整性：通过触发器实现业务规则

主要约束：
- 员工工号、病人编号等采用5位数字字符串格式
- 身份证号必须为18位
- 联系电话必须为11位数字
- 数量字段不能为负数
- 时间字段有默认值
*/

-- =============================================================================
-- 7.2 表的创建（含数据完整性实现）
-- =============================================================================

-- 创建数据库
CREATE DATABASE IF NOT EXISTS HospitalManagement 
CHARACTER SET utf8mb4 
COLLATE utf8mb4_unicode_ci;

USE HospitalManagement;

-- 表一：员工表（Employee）
CREATE TABLE Employee (
    empNo CHAR(5) PRIMARY KEY,
    empName VARCHAR(10) NOT NULL,
    empStatus VARCHAR(5),
    empDutyStatus VARCHAR(5),
    empEntryYear DATE,
    empJobPosition CHAR(4),
    empProfile TEXT,
    
    CONSTRAINT chk_emp_no CHECK (empNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT chk_emp_status CHECK (empStatus IN ('在职', '离职', '休假')),
    CONSTRAINT chk_duty_status CHECK (empDutyStatus IN ('在岗', '离岗', '出差'))
);

-- 表二：人事管理员表（PersonnelAdministrator）
CREATE TABLE PersonnelAdministrator (
    paNo CHAR(5) PRIMARY KEY,
    
    CONSTRAINT chk_pa_no CHECK (paNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT fk_pa_emp FOREIGN KEY (paNo) REFERENCES Employee(empNo) ON DELETE CASCADE
);

-- 表三：人员管理（Manage）
CREATE TABLE Manage (
    paNo CHAR(5),
    empNo CHAR(5),
    
    PRIMARY KEY (paNo, empNo),
    CONSTRAINT fk_manage_pa FOREIGN KEY (paNo) REFERENCES PersonnelAdministrator(paNo) ON DELETE CASCADE,
    CONSTRAINT fk_manage_emp FOREIGN KEY (empNo) REFERENCES Employee(empNo) ON DELETE CASCADE
);

-- 表四：科室表（Unit）
CREATE TABLE Unit (
    unNo CHAR(5) PRIMARY KEY,
    unName VARCHAR(10) NOT NULL,
    unResponsibility VARCHAR(30),
    unPosition VARCHAR(50),
    
    CONSTRAINT chk_unit_no CHECK (unNo REGEXP '^[0-9]{5}$')
);

-- 表八：检验仓库表（Warehouse）- 需要先创建，因为有外键依赖
CREATE TABLE Warehouse (
    whNo CHAR(5) PRIMARY KEY,
    whName VARCHAR(10) NOT NULL,
    whCapacity DECIMAL(10,2) DEFAULT 0,
    
    CONSTRAINT chk_wh_no CHECK (whNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT chk_wh_capacity CHECK (whCapacity >= 0)
);

-- 表五：检验科室表（Laboratory）
CREATE TABLE Laboratory (
    laNo CHAR(5) PRIMARY KEY,
    lwNo CHAR(5),
    
    CONSTRAINT chk_lab_no CHECK (laNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT fk_lab_unit FOREIGN KEY (laNo) REFERENCES Unit(unNo) ON DELETE CASCADE,
    CONSTRAINT fk_lab_warehouse FOREIGN KEY (lwNo) REFERENCES Warehouse(whNo) ON DELETE SET NULL
);

-- 表六：检验医生表（MedicalTechnologist）
CREATE TABLE MedicalTechnologist (
    mtNo CHAR(5) PRIMARY KEY,
    laNo CHAR(5),
    
    CONSTRAINT fk_mt_emp FOREIGN KEY (mtNo) REFERENCES Employee(empNo) ON DELETE CASCADE,
    CONSTRAINT fk_mt_lab FOREIGN KEY (laNo) REFERENCES Laboratory(laNo) ON DELETE SET NULL
);

-- 表七：检验仓库管理（WarehouseManagement）
CREATE TABLE WarehouseManagement (
    wmNo CHAR(5),
    mtNo CHAR(5),
    
    PRIMARY KEY (wmNo, mtNo),
    CONSTRAINT fk_wm_warehouse FOREIGN KEY (wmNo) REFERENCES Warehouse(whNo) ON DELETE CASCADE,
    CONSTRAINT fk_wm_mt FOREIGN KEY (mtNo) REFERENCES MedicalTechnologist(mtNo) ON DELETE CASCADE
);

-- 表九：检验器材表（InspectionEquipment）
CREATE TABLE InspectionEquipment (
    ieNo CHAR(5) PRIMARY KEY,
    wmNo CHAR(5),
    ieFunc TEXT,
    ieNumber INT NOT NULL DEFAULT 0,
    
    CONSTRAINT chk_ie_no CHECK (ieNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT chk_ie_number CHECK (ieNumber >= 0),
    CONSTRAINT fk_ie_warehouse FOREIGN KEY (wmNo) REFERENCES Warehouse(whNo) ON DELETE SET NULL
);

-- 表十一：门诊科室表（OutPatientDepartment）
CREATE TABLE OutPatientDepartment (
    opNo CHAR(5) PRIMARY KEY,
    
    CONSTRAINT fk_op_unit FOREIGN KEY (opNo) REFERENCES Unit(unNo) ON DELETE CASCADE
);

-- 表十二：门诊医生表（OutPatientPhysician）
CREATE TABLE OutPatientPhysician (
    oppNo CHAR(5) PRIMARY KEY,
    opNo CHAR(5),
    
    CONSTRAINT fk_opp_emp FOREIGN KEY (oppNo) REFERENCES Employee(empNo) ON DELETE CASCADE,
    CONSTRAINT fk_opp_op FOREIGN KEY (opNo) REFERENCES OutPatientDepartment(opNo) ON DELETE SET NULL
);

-- 表十三：病人表（Patient）
CREATE TABLE Patient (
    pNo CHAR(5) PRIMARY KEY,
    pName VARCHAR(10) NOT NULL,
    pID CHAR(18) NOT NULL UNIQUE,
    pBirth DATE,
    pTelephone CHAR(11),
    pDependents TEXT,
    
    CONSTRAINT chk_p_no CHECK (pNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT chk_p_id CHECK (pID REGEXP '^[0-9]{17}[0-9X]$'),
    CONSTRAINT chk_p_phone CHECK (pTelephone REGEXP '^[0-9]{11}$')
);

-- 表十四：就诊记录表（MedicalRecord）
CREATE TABLE MedicalRecord (
    mrNo CHAR(5) PRIMARY KEY,
    pNo CHAR(5),
    oppNo CHAR(5),
    mrTime DATETIME DEFAULT CURRENT_TIMESTAMP,
    mrContent TEXT,
    
    CONSTRAINT chk_mr_no CHECK (mrNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT fk_mr_patient FOREIGN KEY (pNo) REFERENCES Patient(pNo) ON DELETE CASCADE,
    CONSTRAINT fk_mr_physician FOREIGN KEY (oppNo) REFERENCES OutPatientPhysician(oppNo) ON DELETE SET NULL
);

-- 表十：检验记录表（InspectionRecord）
CREATE TABLE InspectionRecord (
    irNo CHAR(5) PRIMARY KEY,
    mtNo CHAR(5),
    mrNo CHAR(5),
    irTime DATETIME DEFAULT CURRENT_TIMESTAMP,
    irResult TEXT,
    
    CONSTRAINT chk_ir_no CHECK (irNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT fk_ir_mt FOREIGN KEY (mtNo) REFERENCES MedicalTechnologist(mtNo) ON DELETE SET NULL,
    CONSTRAINT fk_ir_mr FOREIGN KEY (mrNo) REFERENCES MedicalRecord(mrNo) ON DELETE CASCADE
);

-- 表十七：药房医师表（Chemist）
CREATE TABLE Chemist (
    cNo CHAR(5) PRIMARY KEY,
    
    CONSTRAINT fk_chemist_emp FOREIGN KEY (cNo) REFERENCES Employee(empNo) ON DELETE CASCADE
);

-- 表十八：取药记录表（MedicationPickupRecord）
CREATE TABLE MedicationPickupRecord (
    mprNo CHAR(5) PRIMARY KEY,
    mrNo CHAR(5),
    cNo CHAR(5),
    cTime DATETIME DEFAULT CURRENT_TIMESTAMP,
    
    CONSTRAINT chk_mpr_no CHECK (mprNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT fk_mpr_mr FOREIGN KEY (mrNo) REFERENCES MedicalRecord(mrNo) ON DELETE CASCADE,
    CONSTRAINT fk_mpr_chemist FOREIGN KEY (cNo) REFERENCES Chemist(cNo) ON DELETE SET NULL
);

-- 表十五：病历表（CaseHistory）
CREATE TABLE CaseHistory (
    chNo CHAR(5) PRIMARY KEY,
    irNo CHAR(5),
    mrNo CHAR(5),
    mprNo CHAR(5),
    chArchiveTime DATETIME DEFAULT CURRENT_TIMESTAMP,
    
    CONSTRAINT chk_ch_no CHECK (chNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT fk_ch_ir FOREIGN KEY (irNo) REFERENCES InspectionRecord(irNo) ON DELETE SET NULL,
    CONSTRAINT fk_ch_mr FOREIGN KEY (mrNo) REFERENCES MedicalRecord(mrNo) ON DELETE CASCADE,
    CONSTRAINT fk_ch_mpr FOREIGN KEY (mprNo) REFERENCES MedicationPickupRecord(mprNo) ON DELETE SET NULL
);

-- 表十六：账单表（Billing）
CREATE TABLE Billing (
    biNo CHAR(5) PRIMARY KEY,
    chNo CHAR(5),
    biPay DECIMAL(10,2) NOT NULL,
    biPayMethod VARCHAR(20),
    biPayTime DATETIME DEFAULT CURRENT_TIMESTAMP,
    
    CONSTRAINT chk_bi_no CHECK (biNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT chk_bi_pay CHECK (biPay >= 0),
    CONSTRAINT chk_pay_method CHECK (biPayMethod IN ('现金', '银行卡', '支付宝', '微信', '医保')),
    CONSTRAINT fk_billing_ch FOREIGN KEY (chNo) REFERENCES CaseHistory(chNo) ON DELETE CASCADE
);

-- 表十九：药房表（DrugStore）
CREATE TABLE DrugStore (
    dsNo CHAR(5) PRIMARY KEY,
    dsName VARCHAR(20),
    dsCapacity DECIMAL(10,2) DEFAULT 0,
    
    CONSTRAINT chk_ds_no CHECK (dsNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT chk_ds_capacity CHECK (dsCapacity >= 0)
);

-- 表二十：药房管理（PharmacyManage）
CREATE TABLE PharmacyManage (
    cNo CHAR(5),
    dsNo CHAR(5),
    
    PRIMARY KEY (cNo, dsNo),
    CONSTRAINT fk_pm_chemist FOREIGN KEY (cNo) REFERENCES Chemist(cNo) ON DELETE CASCADE,
    CONSTRAINT fk_pm_drugstore FOREIGN KEY (dsNo) REFERENCES DrugStore(dsNo) ON DELETE CASCADE
);

-- 表二十一：药品表（Medicine）
CREATE TABLE Medicine (
    meNo CHAR(5) PRIMARY KEY,
    dsNo CHAR(5),
    meFunc TEXT,
    meNum INT NOT NULL DEFAULT 0,
    
    CONSTRAINT chk_me_no CHECK (meNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT chk_me_num CHECK (meNum >= 0),
    CONSTRAINT fk_medicine_ds FOREIGN KEY (dsNo) REFERENCES DrugStore(dsNo) ON DELETE SET NULL
);

-- 表二十二：处方记录表（PrescriptionRecord）
CREATE TABLE PrescriptionRecord (
    prNo CHAR(5) PRIMARY KEY,
    mprNo CHAR(5),
    meNo CHAR(5),
    prNum VARCHAR(10),
    prMethod TEXT,
    
    CONSTRAINT chk_pr_no CHECK (prNo REGEXP '^[0-9]{5}$'),
    CONSTRAINT fk_pr_mpr FOREIGN KEY (mprNo) REFERENCES MedicationPickupRecord(mprNo) ON DELETE CASCADE,
    CONSTRAINT fk_pr_medicine FOREIGN KEY (meNo) REFERENCES Medicine(meNo) ON DELETE CASCADE
);

-- 创建索引（基于设计文档）
CREATE INDEX idx_medicalrecord_pno ON MedicalRecord(pNo);
CREATE INDEX idx_medicalrecord_oppno ON MedicalRecord(oppNo);
CREATE INDEX idx_medicalrecord_mrtime ON MedicalRecord(mrTime);
CREATE INDEX idx_medicalrecord_pno_mrtime ON MedicalRecord(pNo, mrTime);

-- =============================================================================
-- 7.3 安全性设计
-- =============================================================================

-- 创建角色
CREATE ROLE 'hospital_admin';
CREATE ROLE 'doctor';
CREATE ROLE 'nurse';
CREATE ROLE 'pharmacist';
CREATE ROLE 'lab_technician';
CREATE ROLE 'finance_staff';

-- 管理员权限（全部权限）
GRANT ALL PRIVILEGES ON HospitalManagement.* TO 'hospital_admin';

-- 医生权限
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.Patient TO 'doctor';
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.MedicalRecord TO 'doctor';
GRANT SELECT ON HospitalManagement.InspectionRecord TO 'doctor';
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.MedicationPickupRecord TO 'doctor';
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.PrescriptionRecord TO 'doctor';
GRANT SELECT ON HospitalManagement.Medicine TO 'doctor';

-- 护士权限
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.Patient TO 'nurse';
GRANT SELECT ON HospitalManagement.MedicalRecord TO 'nurse';
GRANT SELECT, UPDATE ON HospitalManagement.InspectionRecord TO 'nurse';

-- 药师权限
GRANT SELECT ON HospitalManagement.MedicationPickupRecord TO 'pharmacist';
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.PrescriptionRecord TO 'pharmacist';
GRANT SELECT, UPDATE ON HospitalManagement.Medicine TO 'pharmacist';
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.DrugStore TO 'pharmacist';

-- 检验技师权限
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.InspectionRecord TO 'lab_technician';
GRANT SELECT, UPDATE ON HospitalManagement.InspectionEquipment TO 'lab_technician';
GRANT SELECT ON HospitalManagement.MedicalRecord TO 'lab_technician';

-- 财务人员权限
GRANT SELECT, INSERT, UPDATE ON HospitalManagement.Billing TO 'finance_staff';
GRANT SELECT ON HospitalManagement.CaseHistory TO 'finance_staff';
GRANT SELECT ON HospitalManagement.Patient TO 'finance_staff';

-- 创建用户并分配角色
CREATE USER 'admin'@'localhost' IDENTIFIED BY 'admin123';
CREATE USER 'doctor1'@'localhost' IDENTIFIED BY 'doc123';
CREATE USER 'nurse1'@'localhost' IDENTIFIED BY 'nurse123';
CREATE USER 'pharmacist1'@'localhost' IDENTIFIED BY 'pharm123';
CREATE USER 'lab_tech1'@'localhost' IDENTIFIED BY 'lab123';
CREATE USER 'finance1'@'localhost' IDENTIFIED BY 'fin123';

-- 分配角色
GRANT 'hospital_admin' TO 'admin'@'localhost';
GRANT 'doctor' TO 'doctor1'@'localhost';
GRANT 'nurse' TO 'nurse1'@'localhost';
GRANT 'pharmacist' TO 'pharmacist1'@'localhost';
GRANT 'lab_technician' TO 'lab_tech1'@'localhost';
GRANT 'finance_staff' TO 'finance1'@'localhost';

-- 激活角色
SET DEFAULT ROLE ALL TO 'admin'@'localhost';
SET DEFAULT ROLE ALL TO 'doctor1'@'localhost';
SET DEFAULT ROLE ALL TO 'nurse1'@'localhost';
SET DEFAULT ROLE ALL TO 'pharmacist1'@'localhost';
SET DEFAULT ROLE ALL TO 'lab_tech1'@'localhost';
SET DEFAULT ROLE ALL TO 'finance1'@'localhost';

-- =============================================================================
-- 7.4 视图的创建
-- =============================================================================

-- 视图1：病人就诊详细信息视图
CREATE VIEW PatientMedicalView AS
SELECT 
    p.pNo,
    p.pName,
    p.pID,
    mr.mrNo,
    mr.mrTime,
    mr.mrContent,
    opp.oppNo AS doctorNo,
    e.empName AS doctorName,
    u.unName AS departmentName
FROM Patient p
JOIN MedicalRecord mr ON p.pNo = mr.pNo
JOIN OutPatientPhysician opp ON mr.oppNo = opp.oppNo
JOIN Employee e ON opp.oppNo = e.empNo
JOIN OutPatientDepartment opd ON opp.opNo = opd.opNo
JOIN Unit u ON opd.opNo = u.unNo;

-- 视图2：检验结果汇总视图
CREATE VIEW InspectionSummaryView AS
SELECT 
    p.pNo,
    p.pName,
    mr.mrNo,
    ir.irNo,
    ir.irTime,
    ir.irResult,
    mt.mtNo AS technicianNo,
    e.empName AS technicianName,
    lab.laNo AS labNo,
    u.unName AS labName
FROM Patient p
JOIN MedicalRecord mr ON p.pNo = mr.pNo
JOIN InspectionRecord ir ON mr.mrNo = ir.mrNo
JOIN MedicalTechnologist mt ON ir.mtNo = mt.mtNo
JOIN Employee e ON mt.mtNo = e.empNo
JOIN Laboratory lab ON mt.laNo = lab.laNo
JOIN Unit u ON lab.laNo = u.unNo;

-- 视图3：药品库存管理视图
CREATE VIEW MedicineInventoryView AS
SELECT 
    ds.dsNo,
    ds.dsName,
    m.meNo,
    m.meFunc,
    m.meNum,
    CASE 
        WHEN m.meNum < 10 THEN '库存不足'
        WHEN m.meNum < 50 THEN '库存偏低'
        ELSE '库存充足'
    END AS stockStatus
FROM DrugStore ds
JOIN Medicine m ON ds.dsNo = m.dsNo;

-- 视图4：财务统计视图
CREATE VIEW FinancialSummaryView AS
SELECT 
    p.pNo,
    p.pName,
    ch.chNo,
    b.biNo,
    b.biPay,
    b.biPayMethod,
    b.biPayTime,
    DATE_FORMAT(b.biPayTime, '%Y-%m') AS payMonth
FROM Patient p
JOIN MedicalRecord mr ON p.pNo = mr.pNo
JOIN CaseHistory ch ON mr.mrNo = ch.mrNo
JOIN Billing b ON ch.chNo = b.chNo;

-- 视图5：员工科室分布视图
CREATE VIEW EmployeeDepartmentView AS
SELECT 
    e.empNo,
    e.empName,
    e.empJobPosition,
    CASE 
        WHEN pa.paNo IS NOT NULL THEN '人事管理员'
        WHEN opp.oppNo IS NOT NULL THEN '门诊医生'
        WHEN mt.mtNo IS NOT NULL THEN '检验医生'
        WHEN c.cNo IS NOT NULL THEN '药房医师'
        ELSE '其他'
    END AS jobType,
    COALESCE(u1.unName, u2.unName, '未分配') AS departmentName
FROM Employee e
LEFT JOIN PersonnelAdministrator pa ON e.empNo = pa.paNo
LEFT JOIN OutPatientPhysician opp ON e.empNo = opp.oppNo
LEFT JOIN OutPatientDepartment opd ON opp.opNo = opd.opNo
LEFT JOIN Unit u1 ON opd.opNo = u1.unNo
LEFT JOIN MedicalTechnologist mt ON e.empNo = mt.mtNo
LEFT JOIN Laboratory lab ON mt.laNo = lab.laNo
LEFT JOIN Unit u2 ON lab.laNo = u2.unNo
LEFT JOIN Chemist c ON e.empNo = c.cNo;

-- =============================================================================
-- 7.5 存储过程的创建
-- =============================================================================

DELIMITER //

-- 存储过程1：病人就诊流程
CREATE PROCEDURE RegisterPatientVisit(
    IN p_pNo CHAR(5),
    IN p_pName VARCHAR(10),
    IN p_pID CHAR(18),
    IN p_pTelephone CHAR(11),
    IN p_oppNo CHAR(5),
    IN p_mrContent TEXT,
    OUT p_mrNo CHAR(5)
);
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    START TRANSACTION;
    
    -- 检查病人是否存在，不存在则创建
    IF NOT EXISTS (SELECT 1 FROM Patient WHERE pNo = p_pNo) THEN
        INSERT INTO Patient (pNo, pName, pID, pTelephone)
        VALUES (p_pNo, p_pName, p_pID, p_pTelephone);
    END IF;
    
    -- 生成就诊记录编号
    SELECT LPAD(IFNULL(MAX(CAST(mrNo AS UNSIGNED)), 0) + 1, 5, '0') 
    INTO p_mrNo FROM MedicalRecord;
    
    -- 插入就诊记录
    INSERT INTO MedicalRecord (mrNo, pNo, oppNo, mrTime, mrContent)
    VALUES (p_mrNo, p_pNo, p_oppNo, NOW(), p_mrContent);
    
    COMMIT;
END //

-- 存储过程2：检验流程
CREATE PROCEDURE ProcessInspection(
    IN p_mrNo CHAR(5),
    IN p_mtNo CHAR(5),
    IN p_irResult TEXT,
    OUT p_irNo CHAR(5)
);
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    START TRANSACTION;
    
    -- 生成检验记录编号
    SELECT LPAD(IFNULL(MAX(CAST(irNo AS UNSIGNED)), 0) + 1, 5, '0') 
    INTO p_irNo FROM InspectionRecord;
    
    -- 插入检验记录
    INSERT INTO InspectionRecord (irNo, mtNo, mrNo, irTime, irResult)
    VALUES (p_irNo, p_mtNo, p_mrNo, NOW(), p_irResult);
    
    COMMIT;
END //

-- 存储过程3：药品出库
CREATE PROCEDURE DispenseMedicine(
    IN p_meNo CHAR(5),
    IN p_quantity INT
);
BEGIN
    DECLARE current_stock INT;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    START TRANSACTION;
    
    -- 检查库存
    SELECT meNum INTO current_stock FROM Medicine WHERE meNo = p_meNo;
    
    IF current_stock < p_quantity THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '库存不足';
    END IF;
    
    -- 更新库存
    UPDATE Medicine 
    SET meNum = meNum - p_quantity 
    WHERE meNo = p_meNo;
    
    COMMIT;
END //

-- 存储过程4：生成月度财务报告
CREATE PROCEDURE GenerateMonthlyFinancialReport(
    IN p_year INT,
    IN p_month INT
);
BEGIN
    SELECT 
        DATE_FORMAT(biPayTime, '%Y-%m-%d') AS payDate,
        COUNT(*) AS transactionCount,
        SUM(biPay) AS totalAmount,
        biPayMethod,
        AVG(biPay) AS avgAmount
    FROM Billing 
    WHERE YEAR(biPayTime) = p_year 
        AND MONTH(biPayTime) = p_month
    GROUP BY DATE_FORMAT(biPayTime, '%Y-%m-%d'), biPayMethod
    ORDER BY payDate, biPayMethod;
END //

-- 存储过程5：批量更新药品库存
CREATE PROCEDURE BatchUpdateMedicineStock(
    IN p_dsNo CHAR(5),
    IN p_threshold INT
);
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE v_meNo CHAR(5);
    DECLARE v_meNum INT;
    DECLARE cur CURSOR FOR 
        SELECT meNo, meNum FROM Medicine WHERE dsNo = p_dsNo AND meNum < p_threshold;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
    
    OPEN cur;
    
    read_loop: LOOP
        FETCH cur INTO v_meNo, v_meNum;
        IF done THEN
            LEAVE read_loop;
        END IF;
        
        -- 模拟补充库存到阈值的2倍
        UPDATE Medicine 
        SET meNum = p_threshold * 2 
        WHERE meNo = v_meNo;
        
    END LOOP;
    
    CLOSE cur;
END //

DELIMITER ;

-- =============================================================================
-- 7.6 触发器的创建
-- =============================================================================

DELIMITER //

-- 触发器1：自动创建病历记录
CREATE TRIGGER after_medical_record_insert
AFTER INSERT ON MedicalRecord
FOR EACH ROW
BEGIN
    DECLARE v_chNo CHAR(5);
    
    -- 生成病历编号
    SELECT LPAD(IFNULL(MAX(CAST(chNo AS UNSIGNED)), 0) + 1, 5, '0') 
    INTO v_chNo FROM CaseHistory;
    
    -- 自动创建病历记录
    INSERT INTO CaseHistory (chNo, mrNo, chArchiveTime)
    VALUES (v_chNo, NEW.mrNo, NOW());
END //

-- 触发器2：药品库存警告
CREATE TRIGGER medicine_stock_warning
AFTER UPDATE ON Medicine
FOR EACH ROW
BEGIN
    IF NEW.meNum < 10 AND OLD.meNum >= 10 THEN
        INSERT INTO StockWarning (meNo, warningMessage, warningTime)
        VALUES (NEW.meNo, CONCAT('药品 ', NEW.meNo, ' 库存不足，当前库存：', NEW.meNum), NOW());
    END IF;
END //

-- 首先创建库存警告表
CREATE TABLE IF NOT EXISTS StockWarning (
    id INT AUTO_INCREMENT PRIMARY KEY,
    meNo CHAR(5),
    warningMessage TEXT,
    warningTime DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- 触发器3：防止删除有关联记录的病人
CREATE TRIGGER before_patient_delete
BEFORE DELETE ON Patient
FOR EACH ROW
BEGIN
    DECLARE record_count INT;
    
    SELECT COUNT(*) INTO record_count 
    FROM MedicalRecord 
    WHERE pNo = OLD.pNo;
    
    IF record_count > 0 THEN
        SIGNAL SQLSTATE '45000' 
        SET MESSAGE_TEXT = '无法删除有就诊记录的病人';
    END IF;
END //

-- 触发器4：自动更新员工状态
CREATE TRIGGER update_employee_status
BEFORE UPDATE ON Employee
FOR EACH ROW
BEGIN
    -- 如果职级发生变化，记录变更时间
    IF OLD.empJobPosition != NEW.empJobPosition THEN
        SET NEW.empEntryYear = CURDATE();
    END IF;
END //

-- 触发器5：检验器材数量变更记录
CREATE TRIGGER equipment_quantity_log
AFTER UPDATE ON InspectionEquipment
FOR EACH ROW
BEGIN
    IF OLD.ieNumber != NEW.ieNumber THEN
        INSERT INTO EquipmentLog (ieNo, oldQuantity, newQuantity, changeTime, changeType)
        VALUES (
            NEW.ieNo, 
            OLD.ieNumber, 
            NEW.ieNumber, 
            NOW(),
            CASE 
                WHEN NEW.ieNumber > OLD.ieNumber THEN '入库'
                ELSE '出库'
            END
        );
    END IF;
END //

-- 创建器材变更日志表
CREATE TABLE IF NOT EXISTS EquipmentLog (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ieNo CHAR(5),
    oldQuantity INT,
    newQuantity INT,
    changeTime DATETIME,
    changeType VARCHAR(10)
);

DELIMITER ;

-- =============================================================================
-- 示例数据插入（用于演示）
-- =============================================================================

-- 插入示例员工数据
INSERT INTO Employee VALUES 
('10001', '张医生', '在职', '在岗', '2020-01-01', 'MD01', '资深内科医生'),
('10002', '李技师', '在职', '在岗', '2021-01-01', 'MT01', '检验科技师'),
('10003', '王药师', '在职', '在岗', '2019-01-01', 'PH01', '主管药师'),
('10004', '赵护士', '在职', '在岗', '2022-01-01', 'RN01', '主管护师'),
('10005', '刘管理', '在职', '在岗', '2018-01-01', 'AD01', '人事主管');

-- 插入科室数据
INSERT INTO Unit VALUES 
('30001', '内科', '内科疾病诊治', '1号楼2层'),
('30002', '检验科', '医学检验', '2号楼1层'),
('30003', '药房', '药品管理', '1号楼1层');

-- 插入门诊科室
INSERT INTO OutPatientDepartment VALUES ('30001');

-- 插入检验科室
INSERT INTO Laboratory VALUES ('30002', '40001');

-- 插入仓库
INSERT INTO Warehouse VALUES ('40001', '检验仓库A', 100.0);

-- 插入药房
INSERT INTO DrugStore VALUES ('50001', '中心药房', 200.0);

-- 插入医生
INSERT INTO OutPatientPhysician VALUES ('10001', '30001');
INSERT INTO MedicalTechnologist VALUES ('10002', '30002');
INSERT INTO Chemist VALUES ('10003');

-- 插入管理关系
INSERT INTO PharmacyManage VALUES ('10003', '50001');

-- 插入示例病人
INSERT INTO Patient VALUES 
('20001', '患者甲', '110101199001011234', '1990-01-01', '13800138001', '父亲：患者父'),
('20002', '患者乙', '110101199002022345', '1990-02-02', '13800138002', '母亲：患者母');

-- =============================================================================
-- 演示示例
-- =============================================================================

-- 演示存储过程：病人就诊
CALL RegisterPatientVisit('20003', '患者丙', '110101199003033456', '13800138003', '10001', '头痛，发热', @new_mrNo);
SELECT @new_mrNo AS '新生成的就诊记录编号';

-- 演示视图查询
SELECT * FROM PatientMedicalView LIMIT 5;

-- 演示财务报告
CALL GenerateMonthlyFinancialReport(2025, 5);

-- 查看创建的表
SHOW TABLES;

-- 查看索引
SHOW INDEX FROM MedicalRecord;

-- =============================================================================
-- 性能优化建议
-- =============================================================================
/*
1. 定期分析表结构：ANALYZE TABLE table_name;
2. 定期优化表：OPTIMIZE TABLE table_name;
3. 监控慢查询日志
4. 根据查询模式调整索引策略
5. 考虑分区表对大数据量表进行分区
6. 定期备份数据库
*/

-- 结束脚本
SELECT '医院管理系统数据库创建完成！' AS Status;