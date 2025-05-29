-- =============================================================================
-- 测试查询脚本
-- =============================================================================

USE HospitalManagement;

-- 测试1：基本查询功能
SELECT '=== 测试1：基本表查询 ===' AS test_name;
SELECT COUNT(*) AS employee_count FROM Employee;
SELECT COUNT(*) AS patient_count FROM Patient;
SELECT COUNT(*) AS medical_record_count FROM MedicalRecord;

-- 测试2：外键约束测试
SELECT '=== 测试2：外键关系验证 ===' AS test_name;
SELECT 
    e.empName AS doctor_name,
    mr.mrNo AS record_no,
    p.pName AS patient_name,
    mr.mrTime AS visit_time
FROM Employee e
JOIN OutPatientPhysician opp ON e.empNo = opp.oppNo
JOIN MedicalRecord mr ON opp.oppNo = mr.oppNo
JOIN Patient p ON mr.pNo = p.pNo;

-- 测试3：视图查询
SELECT '=== 测试3：视图查询 ===' AS test_name;
SELECT * FROM PatientMedicalView LIMIT 3;
SELECT * FROM MedicineInventoryView LIMIT 3;

-- 测试4：存储过程测试
SELECT '=== 测试4：存储过程测试 ===' AS test_name;
CALL RegisterPatientVisit('20010', '存储过程测试', '110101199901011234', '13900139001', '10001', '存储过程测试就诊', @new_mr_no);
SELECT @new_mr_no AS '新生成的就诊记录编号';

-- 测试5：触发器测试
SELECT '=== 测试5：触发器测试 ===' AS test_name;
-- 检查病历记录是否自动创建
SELECT COUNT(*) AS auto_created_case_history 
FROM CaseHistory 
WHERE mrNo = @new_mr_no;

-- 测试6：约束验证
SELECT '=== 测试6：约束验证 ===' AS test_name;
-- 尝试插入不符合约束的数据（这些应该会失败）
-- INSERT INTO Employee VALUES ('ABCDE', '错误工号', '在职', '在岗', '2020-01-01', 'MD01', '测试');

-- 测试7：安全性测试
SELECT '=== 测试7：用户权限检查 ===' AS test_name;
SELECT User, Host FROM mysql.user WHERE User LIKE '%doctor%' OR User LIKE '%admin%';

-- 测试8：索引性能测试
SELECT '=== 测试8：索引使用检查 ===' AS test_name;
EXPLAIN SELECT * FROM MedicalRecord WHERE pNo = '20001' AND mrTime > '2025-01-01';

-- 测试9：财务统计
SELECT '=== 测试9：财务统计 ===' AS test_name;
SELECT 
    biPayMethod AS payment_method,
    COUNT(*) AS transaction_count,
    SUM(biPay) AS total_amount,
    AVG(biPay) AS avg_amount
FROM Billing 
GROUP BY biPayMethod;

-- 测试10：复杂关联查询
SELECT '=== 测试10：复杂关联查询 ===' AS test_name;
SELECT 
    p.pName AS patient_name,
    mr.mrTime AS visit_time,
    ir.irResult AS inspection_result,
    m.meFunc AS prescribed_medicine,
    b.biPay AS total_cost
FROM Patient p
LEFT JOIN MedicalRecord mr ON p.pNo = mr.pNo
LEFT JOIN InspectionRecord ir ON mr.mrNo = ir.mrNo
LEFT JOIN CaseHistory ch ON mr.mrNo = ch.mrNo
LEFT JOIN MedicationPickupRecord mpr ON ch.mprNo = mpr.mprNo
LEFT JOIN PrescriptionRecord pr ON mpr.mprNo = pr.mprNo
LEFT JOIN Medicine m ON pr.meNo = m.meNo
LEFT JOIN Billing b ON ch.chNo = b.chNo
WHERE p.pNo IN ('20001', '20002')
ORDER BY mr.mrTime DESC;

SELECT '所有测试完成！' AS status;
