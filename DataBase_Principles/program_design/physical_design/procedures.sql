-- ================================================================================
-- 医院管理系统存储过程定义文件
-- Hospital Management System Stored Procedures
-- ================================================================================

-- 删除已存在的存储过程（如果存在）
DROP PROCEDURE IF EXISTS sp_patient_medical_history;
DROP PROCEDURE IF EXISTS sp_medicine_stock_report;
DROP PROCEDURE IF EXISTS sp_doctor_workload_statistics;
DROP PROCEDURE IF EXISTS sp_generate_patient_bill;

-- ================================================================================
-- 存储过程1: 查询病人完整就医历史
-- 功能：根据病人编号查询其完整的就医历史，包括就诊记录、检验记录、处方记录等
-- 参数：病人编号
-- ================================================================================
DELIMITER $$
CREATE PROCEDURE sp_patient_medical_history(
    IN p_patient_no CHAR(5)
)
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    -- 验证病人是否存在
    IF NOT EXISTS (SELECT 1 FROM Patient WHERE pNo = p_patient_no) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '病人不存在';
    END IF;
    
    -- 查询病人基本信息
    SELECT 
        p.pNo AS '病人编号',
        p.pName AS '病人姓名',
        p.pID AS '身份证号',
        p.pbirth AS '出生日期',
        p.pTelephone AS '联系电话',
        TIMESTAMPDIFF(YEAR, p.pbirth, CURDATE()) AS '年龄'
    FROM Patient p
    WHERE p.pNo = p_patient_no;
    
    -- 查询就诊记录
    SELECT 
        mr.mrNo AS '就诊记录编号',
        mr.mrTime AS '就诊时间',
        mr.mrContent AS '诊断内容',
        e.empName AS '接诊医生',
        opd.opNo AS '门诊科室'
    FROM MedicalRecord mr
    LEFT JOIN OutPatientPhysician opp ON mr.oppNo = opp.oppNo
    LEFT JOIN Employee e ON opp.oppNo = e.empNo
    LEFT JOIN OutPatientDepartment opd ON opp.opNo = opd.opNo
    WHERE mr.pNo = p_patient_no
    ORDER BY mr.mrTime DESC;
    
    -- 查询检验记录
    SELECT 
        ir.irNo AS '检验记录编号',
        ir.irTime AS '检验时间',
        ir.irResult AS '检验结果',
        e.empName AS '检验医师',
        mr.mrNo AS '关联就诊记录'
    FROM InspectionRecord ir
    LEFT JOIN MedicalTechnologist mt ON ir.mtNo = mt.mtNo
    LEFT JOIN Employee e ON mt.mtNo = e.empNo
    LEFT JOIN MedicalRecord mr ON ir.mrNo = mr.mrNo
    WHERE mr.pNo = p_patient_no
    ORDER BY ir.irTime DESC;
    
    -- 查询处方和取药记录
    SELECT 
        pr.prNo AS '处方编号',
        mpr.cTime AS '取药时间',
        m.meNo AS '药品编号',
        m.mefunc AS '药品功能',
        pr.prNum AS '用量',
        pr.prMethod AS '用法',
        e.empName AS '药师',
        mr.mrNo AS '关联就诊记录'
    FROM PrescriptionRecord pr
    LEFT JOIN MedicationPickupRecord mpr ON pr.mprNo = mpr.mprNo
    LEFT JOIN Medicine m ON pr.meNo = m.meNo
    LEFT JOIN Chemist c ON mpr.cNo = c.cNo
    LEFT JOIN Employee e ON c.cNo = e.empNo
    LEFT JOIN MedicalRecord mr ON mpr.mrNo = mr.mrNo
    WHERE mr.pNo = p_patient_no
    ORDER BY mpr.cTime DESC;
    
END$$
DELIMITER ;

-- ================================================================================
-- 存储过程2: 药品库存报告
-- 功能：生成药品库存报告，包括库存充足、库存不足、缺货等状态
-- 参数：库存阈值（可选，默认为10）
-- ================================================================================
DELIMITER $$
CREATE PROCEDURE sp_medicine_stock_report(
    IN p_threshold INT
)
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;

    -- Set default for p_threshold if NULL
    IF p_threshold IS NULL THEN
        SET p_threshold = 10;
    END IF;
    
    -- 库存充足的药品（数量 >= 阈值）
    SELECT 
        '库存充足' AS '库存状态',
        m.meNo AS '药品编号',
        m.mefunc AS '药品功能',
        m.meNum AS '当前库存',
        ds.dsName AS '所在药房',
        CASE 
            WHEN m.meNum >= p_threshold * 2 THEN '充足'
            WHEN m.meNum >= p_threshold THEN '正常'
        END AS '库存等级'
    FROM Medicine m
    LEFT JOIN DrugStore ds ON m.dsNo = ds.dsNo
    WHERE m.meNum >= p_threshold
    ORDER BY m.meNum DESC;
    
    -- 库存不足的药品（0 < 数量 < 阈值）
    SELECT 
        '库存不足' AS '库存状态',
        m.meNo AS '药品编号',
        m.mefunc AS '药品功能',
        m.meNum AS '当前库存',
        ds.dsName AS '所在药房',
        (p_threshold - m.meNum) AS '需补充数量'
    FROM Medicine m
    LEFT JOIN DrugStore ds ON m.dsNo = ds.dsNo
    WHERE m.meNum > 0 AND m.meNum < p_threshold
    ORDER BY m.meNum ASC;
    
    -- 缺货的药品（数量 = 0）
    SELECT 
        '缺货' AS '库存状态',
        m.meNo AS '药品编号',
        m.mefunc AS '药品功能',
        m.meNum AS '当前库存',
        ds.dsName AS '所在药房',
        p_threshold AS '建议补充数量'
    FROM Medicine m
    LEFT JOIN DrugStore ds ON m.dsNo = ds.dsNo
    WHERE m.meNum = 0
    ORDER BY m.meNo;
    
    -- 库存统计汇总
    SELECT 
        COUNT(CASE WHEN m.meNum >= p_threshold THEN 1 END) AS '库存充足数量',
        COUNT(CASE WHEN m.meNum > 0 AND m.meNum < p_threshold THEN 1 END) AS '库存不足数量',
        COUNT(CASE WHEN m.meNum = 0 THEN 1 END) AS '缺货数量',
        COUNT(*) AS '药品总数',
        ROUND(COUNT(CASE WHEN m.meNum >= p_threshold THEN 1 END) * 100.0 / COUNT(*), 2) AS '库存充足率(%)'
    FROM Medicine m;
    
END$$
DELIMITER ;

-- ================================================================================
-- 存储过程3: 医生工作量统计
-- 功能：统计指定时间段内医生的工作量，包括接诊数量、开药次数等
-- 参数：开始日期、结束日期（可选，默认为当月）
-- ================================================================================
DELIMITER $$
CREATE PROCEDURE sp_doctor_workload_statistics(
    IN p_start_date DATE,
    IN p_end_date DATE
)
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    -- 设置默认日期范围为当月
    IF p_start_date IS NULL THEN
        SET p_start_date = DATE_FORMAT(CURDATE(), '%Y-%m-01');
    END IF;
    
    IF p_end_date IS NULL THEN
        SET p_end_date = LAST_DAY(CURDATE());
    END IF;
    
    -- 门诊医生工作量统计
    SELECT 
        '门诊医生' AS '医生类型',
        e.empNo AS '工号',
        e.empName AS '医生姓名',
        e.empJobPosition AS '职级',
        opd.opNo AS '科室',
        COUNT(mr.mrNo) AS '接诊次数',
        COUNT(DISTINCT mr.pNo) AS '接诊病人数',
        COALESCE(mpr_count.prescription_count, 0) AS '开药次数',
        ROUND(COUNT(mr.mrNo) / DATEDIFF(p_end_date, p_start_date), 2) AS '日均接诊量'
    FROM Employee e
    INNER JOIN OutPatientPhysician opp ON e.empNo = opp.oppNo
    LEFT JOIN OutPatientDepartment opd ON opp.opNo = opd.opNo
    LEFT JOIN MedicalRecord mr ON opp.oppNo = mr.oppNo 
        AND mr.mrTime BETWEEN p_start_date AND p_end_date
    LEFT JOIN (
        SELECT 
            mr.oppNo,
            COUNT(DISTINCT mpr.mprNo) AS prescription_count
        FROM MedicalRecord mr
        INNER JOIN MedicationPickupRecord mpr ON mr.mrNo = mpr.mrNo
        WHERE mr.mrTime BETWEEN p_start_date AND p_end_date
        GROUP BY mr.oppNo
    ) mpr_count ON opp.oppNo = mpr_count.oppNo
    GROUP BY e.empNo, e.empName, e.empJobPosition, opd.opNo
    HAVING COUNT(mr.mrNo) > 0
    ORDER BY COUNT(mr.mrNo) DESC;
    
    -- 检验医生工作量统计
    SELECT 
        '检验医生' AS '医生类型',
        e.empNo AS '工号',
        e.empName AS '医生姓名',
        e.empJobPosition AS '职级',
        l.laNo AS '检验科室',
        COUNT(ir.irNo) AS '检验次数',
        COUNT(DISTINCT mr.pNo) AS '检验病人数',
        ROUND(COUNT(ir.irNo) / DATEDIFF(p_end_date, p_start_date), 2) AS '日均检验量'
    FROM Employee e
    INNER JOIN MedicalTechnologist mt ON e.empNo = mt.mtNo
    LEFT JOIN Laboratory l ON mt.laNo = l.laNo
    LEFT JOIN InspectionRecord ir ON mt.mtNo = ir.mtNo 
        AND ir.irTime BETWEEN p_start_date AND p_end_date
    LEFT JOIN MedicalRecord mr ON ir.mrNo = mr.mrNo
    GROUP BY e.empNo, e.empName, e.empJobPosition, l.laNo
    HAVING COUNT(ir.irNo) > 0
    ORDER BY COUNT(ir.irNo) DESC;
    
    -- 药房医师工作量统计
    SELECT 
        '药房医师' AS '医生类型',
        e.empNo AS '工号',
        e.empName AS '医生姓名',
        e.empJobPosition AS '职级',
        GROUP_CONCAT(DISTINCT ds.dsName) AS '管理药房',
        COUNT(mpr.mprNo) AS '配药次数',
        COUNT(DISTINCT mr.pNo) AS '服务病人数',
        COUNT(DISTINCT pr.prNo) AS '处方数量',
        ROUND(COUNT(mpr.mprNo) / DATEDIFF(p_end_date, p_start_date), 2) AS '日均配药量'
    FROM Employee e
    INNER JOIN Chemist c ON e.empNo = c.cNo
    LEFT JOIN PharmacyManage pm ON c.cNo = pm.cNo
    LEFT JOIN DrugStore ds ON pm.dsNo = ds.dsNo
    LEFT JOIN MedicationPickupRecord mpr ON c.cNo = mpr.cNo 
        AND mpr.cTime BETWEEN p_start_date AND p_end_date
    LEFT JOIN MedicalRecord mr ON mpr.mrNo = mr.mrNo
    LEFT JOIN PrescriptionRecord pr ON mpr.mprNo = pr.mprNo
    GROUP BY e.empNo, e.empName, e.empJobPosition
    HAVING COUNT(mpr.mprNo) > 0
    ORDER BY COUNT(mpr.mprNo) DESC;
    
END$$
DELIMITER ;

-- ================================================================================
-- 存储过程4: 生成病人账单
-- 功能：根据病历编号生成详细账单，包括诊疗费、检验费、药品费等
-- 参数：病历编号、基础诊疗费（可选，默认50元）
-- ================================================================================
DELIMITER $$
CREATE PROCEDURE sp_generate_patient_bill(
    IN p_case_history_no CHAR(5),
    IN p_base_consultation_fee DECIMAL(10,2)
)
BEGIN
    DECLARE v_total_amount DECIMAL(10,2) DEFAULT 0;
    DECLARE v_consultation_fee DECIMAL(10,2) DEFAULT 0;
    DECLARE v_inspection_fee DECIMAL(10,2) DEFAULT 0;
    DECLARE v_medicine_fee DECIMAL(10,2) DEFAULT 0;
    DECLARE v_patient_name VARCHAR(10);
    DECLARE v_medical_record_no CHAR(5);
    
    -- Set default for p_base_consultation_fee if NULL
    IF p_base_consultation_fee IS NULL THEN
        SET p_base_consultation_fee = 50.00;
    END IF;
    
    -- 验证病历是否存在
    IF NOT EXISTS (SELECT 1 FROM CaseHistory WHERE chNo = p_case_history_no) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = '病历不存在';
    END IF;
    
    -- 获取病历相关信息
    SELECT mr.mrNo, p.pName
    INTO v_medical_record_no, v_patient_name
    FROM CaseHistory ch
    INNER JOIN MedicalRecord mr ON ch.mrNo = mr.mrNo
    INNER JOIN Patient p ON mr.pNo = p.pNo
    WHERE ch.chNo = p_case_history_no;
    
    -- 计算诊疗费
    SET v_consultation_fee = p_base_consultation_fee;
    
    -- 计算检验费（假设每项检验50元）
    SELECT COALESCE(COUNT(ir.irNo) * 50.00, 0)
    INTO v_inspection_fee
    FROM CaseHistory ch
    LEFT JOIN InspectionRecord ir ON ch.irNo = ir.irNo
    WHERE ch.chNo = p_case_history_no;
    
    -- 计算药品费（假设每种药品基础价格20元，按用量计算）
    SELECT COALESCE(SUM(CAST(pr.prNum AS UNSIGNED) * 20.00), 0)
    INTO v_medicine_fee
    FROM CaseHistory ch
    LEFT JOIN MedicationPickupRecord mpr ON ch.mprNo = mpr.mprNo
    LEFT JOIN PrescriptionRecord pr ON mpr.mprNo = pr.mprNo
    WHERE ch.chNo = p_case_history_no;
    
    -- 计算总金额
    SET v_total_amount = v_consultation_fee + v_inspection_fee + v_medicine_fee;
    
    -- 显示账单详情
    SELECT 
        p_case_history_no AS '病历编号',
        v_patient_name AS '病人姓名',
        v_medical_record_no AS '就诊记录编号',
        CURDATE() AS '账单日期',
        v_consultation_fee AS '诊疗费(元)',
        v_inspection_fee AS '检验费(元)',
        v_medicine_fee AS '药品费(元)',
        v_total_amount AS '总金额(元)';
    
    -- 显示费用明细
    -- 诊疗明细
    SELECT 
        '诊疗费' AS '费用类型',
        mr.mrContent AS '项目描述',
        1 AS '数量',
        p_base_consultation_fee AS '单价(元)',
        p_base_consultation_fee AS '小计(元)'
    FROM CaseHistory ch
    INNER JOIN MedicalRecord mr ON ch.mrNo = mr.mrNo
    WHERE ch.chNo = p_case_history_no
    
    UNION ALL
    
    -- 检验费明细
    SELECT 
        '检验费' AS '费用类型',
        CONCAT('检验项目 - ', COALESCE(ir.irResult, '待出结果')) AS '项目描述',
        1 AS '数量',
        50.00 AS '单价(元)',
        50.00 AS '小计(元)'
    FROM CaseHistory ch
    LEFT JOIN InspectionRecord ir ON ch.irNo = ir.irNo
    WHERE ch.chNo = p_case_history_no AND ir.irNo IS NOT NULL
    
    UNION ALL
    
    -- 药品费明细
    SELECT 
        '药品费' AS '费用类型',
        m.mefunc AS '项目描述',
        CAST(pr.prNum AS UNSIGNED) AS '数量',
        20.00 AS '单价(元)',
        CAST(pr.prNum AS UNSIGNED) * 20.00 AS '小计(元)'
    FROM CaseHistory ch
    LEFT JOIN MedicationPickupRecord mpr ON ch.mprNo = mpr.mprNo
    LEFT JOIN PrescriptionRecord pr ON mpr.mprNo = pr.mprNo
    LEFT JOIN Medicine m ON pr.meNo = m.meNo
    WHERE ch.chNo = p_case_history_no AND pr.prNo IS NOT NULL;
    
    -- 如果账单表中还没有记录，自动插入
    IF NOT EXISTS (SELECT 1 FROM Billing WHERE chNo = p_case_history_no) THEN
        INSERT INTO Billing (biNo, chNo, biPay, biPayMethod, biPayTime)
        VALUES (
            CONCAT('BI', LPAD((SELECT COALESCE(MAX(CAST(SUBSTRING(biNo, 3) AS UNSIGNED)), 0) + 1 FROM Billing), 3, '0')),
            p_case_history_no,
            v_total_amount,
            '待支付',
            NULL
        );
        
        SELECT CONCAT('账单 BI', LPAD((SELECT COALESCE(MAX(CAST(SUBSTRING(biNo, 3) AS UNSIGNED)), 0) FROM Billing), 3, '0'), ' 已生成') AS '提示信息';
    END IF;
    
END$$
DELIMITER ;

-- ================================================================================
-- 存储过程信息查询
-- ================================================================================

-- 查看所有存储过程
SELECT 
    ROUTINE_NAME as '存储过程名称',
    ROUTINE_TYPE as '类型',
    CREATED as '创建时间',
    LAST_ALTERED as '最后修改时间',
    ROUTINE_COMMENT as '注释'
FROM INFORMATION_SCHEMA.ROUTINES 
WHERE ROUTINE_SCHEMA = DATABASE() AND ROUTINE_TYPE = 'PROCEDURE'
ORDER BY ROUTINE_NAME;

-- ================================================================================
-- 存储过程设计说明
-- ================================================================================
/*
1. 存储过程1 (sp_patient_medical_history):
   - 业务场景：病人就医历史查询
   - 功能：根据病人编号查询完整就医历史
   - 参数：病人编号
   - 业务价值：提供完整的病人医疗档案，支持医生诊断决策

2. 存储过程2 (sp_medicine_stock_report):
   - 业务场景：药品库存管理
   - 功能：生成详细的药品库存报告
   - 参数：库存阈值（可选）
   - 业务价值：帮助药房管理人员及时了解库存状况，制定采购计划

3. 存储过程3 (sp_doctor_workload_statistics):
   - 业务场景：医生工作量统计和绩效评估
   - 功能：统计医生在指定时间段的工作量
   - 参数：开始日期、结束日期（可选）
   - 业务价值：支持医院管理层进行人力资源配置和绩效评估

4. 存储过程4 (sp_generate_patient_bill):
   - 业务场景：病人费用结算
   - 功能：根据病历生成详细账单
   - 参数：病历编号、基础诊疗费（可选）
   - 业务价值：自动化费用计算，提高结算效率和准确性

这四个存储过程覆盖了医院管理系统的核心业务场景：
- 医疗档案管理（病人历史查询）
- 库存管理（药品库存报告）
- 人力资源管理（医生工作量统计）
- 财务管理（账单生成）
*/