-- Test instances for HospitalDB Stored Procedures

-- --------------------------------------------------------------------------------
-- 存储过程测试实例
-- --------------------------------------------------------------------------------

-- 测试前的数据准备
-- 插入基础测试数据

-- 插入员工和科室数据
INSERT INTO Employee (empNo, empName, empStatus, empDutyStatus, empEntryYear, empJobPosition, empProfile) 
VALUES 
('E0001', '张医生', '在职', '在岗', '2020-01-01', '主治', '经验丰富的内科医生'),
('E0002', '李药师', '在职', '在岗', '2019-06-01', '药师', '专业药房管理人员'),
('E0003', '王检验师', '在职', '在岗', '2021-03-01', '技师', '专业检验技术人员');

INSERT INTO OutPatientDepartment (opNo) VALUES ('OP001');
INSERT INTO OutPatientPhysician (oppNo, opNo) VALUES ('E0001', 'OP001');
INSERT INTO Chemist (cNo) VALUES ('E0002');

INSERT INTO Warehouse (whNo, whName, whCapacity) VALUES ('WH001', '检验仓库1', 50.0);
INSERT INTO Laboratory (laNo, lwNo) VALUES ('LA001', 'WH001');
INSERT INTO MedicalTechnologist (mtNo, laNo) VALUES ('E0003', 'LA001');

-- 插入病人和药品数据
INSERT INTO Patient (pNo, pName, pID, pbirth, pTelephone) 
VALUES ('P0001', '王患者', '123456789012345678', '1990-01-01', '13800138000');

INSERT INTO DrugStore (dsNo, dsName, dsCapacity) VALUES ('DS001', '中心药房', 100.0);
INSERT INTO Medicine (meNo, dsNo, mefunc, meNum) VALUES ('M0001', 'DS001', '感冒药', 25);

-- 插入就诊和处方记录
INSERT INTO MedicalRecord (mrNo, pNo, oppNo, mrTime, mrContent) VALUES ('MR001', 'P0001', 'E0001', '2024-05-01', '患者感冒症状，建议休息');
INSERT INTO InspectionRecord (irNo, mtNo, mrNo, irTime, irResult) VALUES ('IR001', 'E0003', 'MR001', '2024-05-02', '血压偏高');
INSERT INTO MedicationPickupRecord (mprNo, mrNo, cNo, cTime) VALUES ('MPR01', 'MR001', 'E0002', '2024-05-01');
INSERT INTO PrescriptionRecord (prNo, mprNo, meNo, prNum, prMethod) VALUES ('PR001', 'MPR01', 'M0001', '3', '每日三次，每次一片');
INSERT INTO CaseHistory (chNo, irNo, mrNo, mprNo, chArchiveTime) VALUES ('CH001', 'IR001', 'MR001', 'MPR01', '2024-05-03');

-- --------------------------------------------------------------------------------
-- 存储过程1测试：病人就医历史查询 (sp_patient_medical_history)
-- --------------------------------------------------------------------------------

-- 演示实例：查询病人P0001的完整就医历史
CALL sp_patient_medical_history('P0001');

-- --------------------------------------------------------------------------------
-- 存储过程2测试：药品库存报告 (sp_medicine_stock_report)
-- --------------------------------------------------------------------------------

-- 演示实例：生成药品库存报告，使用默认阈值10
CALL sp_medicine_stock_report();

-- 演示实例：生成药品库存报告，使用自定义阈值20
CALL sp_medicine_stock_report(20);

-- --------------------------------------------------------------------------------
-- 存储过程3测试：医生工作量统计 (sp_doctor_workload_statistics)
-- --------------------------------------------------------------------------------

-- 演示实例：统计2024年5月医生工作量
CALL sp_doctor_workload_statistics('2024-05-01', '2024-05-31');

-- --------------------------------------------------------------------------------
-- 存储过程4测试：生成病人账单 (sp_generate_patient_bill)
-- --------------------------------------------------------------------------------

-- 演示实例：为病历CH001生成账单
CALL sp_generate_patient_bill('CH001');

-- 查看生成的账单记录
SELECT * FROM Billing WHERE chNo = 'CH001';

-- 备注：部分存储过程演示依赖特定数据状态，可能需要调整才能直接运行观察效果。