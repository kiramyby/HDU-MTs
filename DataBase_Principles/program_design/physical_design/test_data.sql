-- =============================================================================
-- 测试数据插入脚本
-- =============================================================================

USE HospitalManagement;

-- 插入人事管理员
INSERT INTO PersonnelAdministrator VALUES ('10005');

-- 插入人员管理关系
INSERT INTO Manage VALUES ('10005', '10001'), ('10005', '10002'), ('10005', '10003'), ('10005', '10004');

-- 插入检验器材
INSERT INTO InspectionEquipment VALUES 
('60001', '40001', '血液分析仪', 5),
('60002', '40001', '尿液分析仪', 3),
('60003', '40001', '生化分析仪', 2);

-- 插入药品
INSERT INTO Medicine VALUES 
('70001', '50001', '感冒药，用于治疗感冒症状', 100),
('70002', '50001', '退烧药，用于退烧', 80),
('70003', '50001', '消炎药，用于消炎', 60);

-- 插入更多测试数据
INSERT INTO Patient VALUES 
('20004', '测试患者1', '110101199001011111', '1990-01-01', '13800138004', '无'),
('20005', '测试患者2', '110101199002022222', '1990-02-02', '13800138005', '无');

-- 测试就诊记录
INSERT INTO MedicalRecord VALUES 
('80001', '20001', '10001', NOW(), '常规体检'),
('80002', '20002', '10001', NOW(), '感冒症状');

-- 测试检验记录
INSERT INTO InspectionRecord VALUES 
('90001', '10002', '80001', NOW(), '血常规正常'),
('90002', '10002', '80002', NOW(), '白细胞偏高');

-- 测试取药记录
INSERT INTO MedicationPickupRecord VALUES 
('11001', '80002', '10003', NOW());

-- 测试处方记录
INSERT INTO PrescriptionRecord VALUES 
('12001', '11001', '70001', '2盒', '每日3次，每次1片');

-- 测试账单
INSERT INTO Billing VALUES 
('13001', (SELECT chNo FROM CaseHistory WHERE mrNo = '80001' LIMIT 1), 150.00, '医保', NOW()),
('13002', (SELECT chNo FROM CaseHistory WHERE mrNo = '80002' LIMIT 1), 89.50, '支付宝', NOW());

-- =============================================================================
-- 数据验证查询
-- =============================================================================

-- 验证数据插入成功
SELECT '=== 员工信息 ===' AS info;
SELECT * FROM Employee;

SELECT '=== 病人信息 ===' AS info;
SELECT * FROM Patient;

SELECT '=== 就诊记录 ===' AS info;
SELECT * FROM MedicalRecord;

SELECT '=== 检验记录 ===' AS info;
SELECT * FROM InspectionRecord;

SELECT '=== 药品库存 ===' AS info;
SELECT * FROM Medicine;

SELECT '=== 账单信息 ===' AS info;
SELECT * FROM Billing;

SELECT '数据初始化完成！' AS status;
