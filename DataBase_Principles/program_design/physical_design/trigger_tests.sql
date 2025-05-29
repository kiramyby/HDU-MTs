-- Test instances for HospitalDB Triggers

-- --------------------------------------------------------------------------------
-- 触发器测试实例
-- --------------------------------------------------------------------------------

-- 测试前的数据准备
-- 首先插入一些基础测试数据

-- 插入员工数据
INSERT INTO Employee (empNo, empName, empStatus, empDutyStatus, empEntryYear, empJobPosition, empProfile) 
VALUES 
('E0001', '张医生', '在职', '在岗', '2020-01-01', '主治', '经验丰富的内科医生'),
('E0002', '李药师', '在职', '在岗', '2019-06-01', '药师', '专业药房管理人员');

-- 插入门诊科室和医生
INSERT INTO OutPatientDepartment (opNo) VALUES ('OP001');
INSERT INTO OutPatientPhysician (oppNo, opNo) VALUES ('E0001', 'OP001');

-- 插入药房医师
INSERT INTO Chemist (cNo) VALUES ('E0002');

-- 插入病人
INSERT INTO Patient (pNo, pName, pID, pbirth, pTelephone) 
VALUES ('P0001', '王患者', '123456789012345678', '1990-01-01', '13800138000');

-- 插入药房和药品
INSERT INTO DrugStore (dsNo, dsName, dsCapacity) VALUES ('DS001', '中心药房', 100.0);
INSERT INTO Medicine (meNo, dsNo, mefunc, meNum) 
VALUES 
('M0001', 'DS001', '感冒药', 15),
('M0002', 'DS001', '止痛药', 8);

-- --------------------------------------------------------------------------------
-- 触发器1测试：药品库存检查触发器 (tr_check_medicine_stock)
-- --------------------------------------------------------------------------------

-- 测试1.1：更新药品数量到低库存（应该触发警告）
UPDATE Medicine SET meNum = 5 WHERE meNo = 'M0001';

-- 查看警告记录
SELECT * FROM StockAlert WHERE meNo = 'M0001';

-- 测试1.2：更新药品数量到安全库存以上（不应该触发警告）
UPDATE Medicine SET meNum = 20 WHERE meNo = 'M0001';

-- --------------------------------------------------------------------------------
-- 触发器2测试：就诊记录时间自动设置触发器 (tr_set_medical_record_time)
-- --------------------------------------------------------------------------------

-- 测试2.1：插入就诊记录，不指定时间（应该自动设置为当前日期）
INSERT INTO MedicalRecord (mrNo, pNo, oppNo, mrContent) 
VALUES ('MR001', 'P0001', 'E0001', '患者感冒症状，建议休息');

-- 查看自动设置的时间
SELECT mrNo, mrTime, mrContent FROM MedicalRecord WHERE mrNo = 'MR001';

-- 测试2.2：插入就诊记录，指定时间（应该保持指定时间）
INSERT INTO MedicalRecord (mrNo, pNo, oppNo, mrTime, mrContent) 
VALUES ('MR002', 'P0001', 'E0001', '2024-01-15', '复查情况良好');

-- 查看指定的时间
SELECT mrNo, mrTime, mrContent FROM MedicalRecord WHERE mrNo = 'MR002';

-- --------------------------------------------------------------------------------
-- 触发器3测试：处方记录药品数量扣减触发器 (tr_deduct_medicine_stock)
-- --------------------------------------------------------------------------------

-- 先插入取药记录
INSERT INTO MedicationPickupRecord (mprNo, mrNo, cNo, cTime) 
VALUES ('MPR01', 'MR001', 'E0002', CURDATE());

-- 测试3.1：开具处方，数量在库存范围内（应该成功扣减库存）
-- 查看开药前的库存
SELECT meNo, meNum FROM Medicine WHERE meNo = 'M0001';

-- 开具处方：3盒感冒药
INSERT INTO PrescriptionRecord (prNo, mprNo, meNo, prNum, prMethod) 
VALUES ('PR001', 'MPR01', 'M0001', '3', '每日三次，每次一片');

-- 查看开药后的库存（应该从20减少到17）
SELECT meNo, meNum FROM Medicine WHERE meNo = 'M0001';

-- 测试3.2：开具处方，数量超过库存（应该失败）
-- 尝试开具20盒止痛药（库存只有8盒）
-- INSERT INTO PrescriptionRecord (prNo, mprNo, meNo, prNum, prMethod) 
-- VALUES ('PR002', 'MPR01', 'M0002', '20', '按需服用');
-- 此语句会失败并显示错误信息

-- --------------------------------------------------------------------------------
-- 触发器4测试：病历归档时间自动设置触发器 (tr_set_case_history_archive_time)
-- --------------------------------------------------------------------------------

-- 测试4.1：插入病历记录，不指定归档时间（应该自动设置为当前日期）
INSERT INTO CaseHistory (chNo, mrNo, mprNo) 
VALUES ('CH001', 'MR001', 'MPR01');

-- 查看自动设置的归档时间
SELECT chNo, chArchiveTime FROM CaseHistory WHERE chNo = 'CH001';

-- 测试4.2：插入病历记录，指定归档时间（应该保持指定时间）
INSERT INTO CaseHistory (chNo, mrNo, mprNo, chArchiveTime) 
VALUES ('CH002', 'MR002', 'MPR01', '2024-02-01');

-- 查看指定的归档时间
SELECT chNo, chArchiveTime FROM CaseHistory WHERE chNo = 'CH002';

-- --------------------------------------------------------------------------------
-- 清理测试数据（可选）
-- --------------------------------------------------------------------------------

-- 删除测试数据的语句（注释状态，根据需要执行）
/*
DELETE FROM StockAlert;
DELETE FROM CaseHistory;
DELETE FROM PrescriptionRecord;
DELETE FROM MedicationPickupRecord;
DELETE FROM MedicalRecord;
DELETE FROM Medicine;
DELETE FROM DrugStore;
DELETE FROM Patient;
DELETE FROM OutPatientPhysician;
DELETE FROM OutPatientDepartment;
DELETE FROM Chemist;
DELETE FROM Employee;
*/

-- 查看所有触发器
SHOW TRIGGERS;
