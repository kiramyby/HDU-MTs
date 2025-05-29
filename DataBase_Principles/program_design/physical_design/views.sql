-- ================================================================================
-- 医院管理系统视图定义文件
-- Hospital Management System Views
-- ================================================================================

-- 删除已存在的视图（如果存在）
DROP VIEW IF EXISTS PatientMedicalSummaryView;
DROP VIEW IF EXISTS AvailableMedicineView;
DROP VIEW IF EXISTS DoctorDepartmentView;
DROP VIEW IF EXISTS PatientFullMedicalHistoryView;

-- ================================================================================
-- 视图1: 病人就诊摘要视图
-- 功能：提供病人的基本信息和最近就诊摘要
-- 用途：快速查看病人概况，用于挂号、接诊等场景
-- ================================================================================
CREATE VIEW PatientMedicalSummaryView AS
SELECT 
    p.pNo AS '病人编号',
    p.pName AS '病人姓名',
    TIMESTAMPDIFF(YEAR, p.pbirth, CURDATE()) AS '年龄',
    p.pTelephone AS '联系电话',
    COUNT(mr.mrNo) AS '就诊次数',
    MAX(mr.mrTime) AS '最近就诊时间',
    (SELECT mrContent FROM MedicalRecord WHERE pNo = p.pNo ORDER BY mrTime DESC LIMIT 1) AS '最近诊断',
    (SELECT empName FROM Employee e 
     INNER JOIN OutPatientPhysician opp ON e.empNo = opp.oppNo 
     INNER JOIN MedicalRecord mr2 ON opp.oppNo = mr2.oppNo 
     WHERE mr2.pNo = p.pNo ORDER BY mr2.mrTime DESC LIMIT 1) AS '最近接诊医生',
    CASE 
        WHEN COUNT(mr.mrNo) = 0 THEN '首次就诊'
        WHEN MAX(mr.mrTime) >= DATE_SUB(CURDATE(), INTERVAL 30 DAY) THEN '近期患者'
        ELSE '历史患者'
    END AS '患者类型'
FROM Patient p
LEFT JOIN MedicalRecord mr ON p.pNo = mr.pNo
GROUP BY p.pNo, p.pName, p.pbirth, p.pTelephone;

-- ================================================================================
-- 视图2: 可用药品清单视图
-- 功能：显示当前有库存的药品信息，包括库存状态
-- 用途：药房管理、开药参考、库存监控
-- ================================================================================
CREATE VIEW AvailableMedicineView AS
SELECT 
    m.meNo AS '药品编号',
    m.mefunc AS '药品功能',
    m.meNum AS '库存数量',
    ds.dsName AS '所属药房',
    ds.dsCapacity AS '药房容量',
    CASE 
        WHEN m.meNum = 0 THEN '缺货'
        WHEN m.meNum < 10 THEN '库存不足'
        WHEN m.meNum < 20 THEN '库存正常'
        ELSE '库存充足'
    END AS '库存状态',
    CASE 
        WHEN m.meNum = 0 THEN '紧急'
        WHEN m.meNum < 10 THEN '高'
        WHEN m.meNum < 20 THEN '中'
        ELSE '低'
    END AS '补货优先级',
    -- 计算药房使用率
    ROUND((SELECT COUNT(*) FROM Medicine WHERE dsNo = ds.dsNo) * 100.0 / 
          NULLIF(ds.dsCapacity, 0), 2) AS '药房使用率(%)'
FROM Medicine m
INNER JOIN DrugStore ds ON m.dsNo = ds.dsNo
WHERE m.meNum >= 0  -- 包含所有药品，方便管理
ORDER BY m.meNum ASC, m.meNo;

-- ================================================================================
-- 视图3: 医生科室信息视图
-- 功能：整合医生信息和其所属科室信息
-- 用途：人事管理、排班安排、科室统计
-- ================================================================================
CREATE VIEW DoctorDepartmentView AS
-- 门诊医生信息
SELECT 
    e.empNo AS '工号',
    e.empName AS '医生姓名',
    e.empJobPosition AS '职级',
    e.empStatus AS '在职状态',
    e.empDutyStatus AS '在岗状态',
    '门诊医生' AS '医生类型',
    opd.opNo AS '科室编号',
    '门诊科室' AS '科室类型',
    -- 统计最近30天工作量
    (SELECT COUNT(*) FROM MedicalRecord mr 
     WHERE mr.oppNo = e.empNo 
     AND mr.mrTime >= DATE_SUB(CURDATE(), INTERVAL 30 DAY)) AS '近30天接诊量',
    YEAR(CURDATE()) - YEAR(e.empEntryYear) AS '工作年限'
FROM Employee e
INNER JOIN OutPatientPhysician opp ON e.empNo = opp.oppNo
LEFT JOIN OutPatientDepartment opd ON opp.opNo = opd.opNo

UNION ALL

-- 检验医生信息
SELECT 
    e.empNo AS '工号',
    e.empName AS '医生姓名',
    e.empJobPosition AS '职级',
    e.empStatus AS '在职状态',
    e.empDutyStatus AS '在岗状态',
    '检验医生' AS '医生类型',
    l.laNo AS '科室编号',
    '检验科室' AS '科室类型',
    -- 统计最近30天工作量
    (SELECT COUNT(*) FROM InspectionRecord ir 
     WHERE ir.mtNo = e.empNo 
     AND ir.irTime >= DATE_SUB(CURDATE(), INTERVAL 30 DAY)) AS '近30天检验量',
    YEAR(CURDATE()) - YEAR(e.empEntryYear) AS '工作年限'
FROM Employee e
INNER JOIN MedicalTechnologist mt ON e.empNo = mt.mtNo
LEFT JOIN Laboratory l ON mt.laNo = l.laNo

UNION ALL

-- 药房医师信息
SELECT 
    e.empNo AS '工号',
    e.empName AS '医生姓名',
    e.empJobPosition AS '职级',
    e.empStatus AS '在职状态',
    e.empDutyStatus AS '在岗状态',
    '药房医师' AS '医生类型',
    ds.dsNo AS '科室编号',
    '药房' AS '科室类型',
    -- 统计最近30天工作量
    (SELECT COUNT(*) FROM MedicationPickupRecord mpr 
     WHERE mpr.cNo = e.empNo 
     AND mpr.cTime >= DATE_SUB(CURDATE(), INTERVAL 30 DAY)) AS '近30天配药量',
    YEAR(CURDATE()) - YEAR(e.empEntryYear) AS '工作年限'
FROM Employee e
INNER JOIN Chemist c ON e.empNo = c.cNo
LEFT JOIN PharmacyManage pm ON c.cNo = pm.cNo
LEFT JOIN DrugStore ds ON pm.dsNo = ds.dsNo;

-- ================================================================================
-- 视图4: 病人完整就医历史视图
-- 功能：提供病人的完整就医历史记录，包括就诊、检验、用药等信息
-- 用途：医生诊断参考、病历管理、医疗质量分析
-- ================================================================================
CREATE VIEW PatientFullMedicalHistoryView AS
SELECT 
    p.pNo AS '病人编号',
    p.pName AS '病人姓名',
    mr.mrNo AS '就诊记录编号',
    mr.mrTime AS '就诊时间',
    mr.mrContent AS '诊断内容',
    e1.empName AS '接诊医生',
    -- 检验信息
    ir.irNo AS '检验记录编号',
    ir.irTime AS '检验时间',
    ir.irResult AS '检验结果',
    e2.empName AS '检验医师',
    -- 用药信息
    mpr.mprNo AS '取药记录编号',
    mpr.cTime AS '取药时间',
    e3.empName AS '药师',
    -- 处方详情
    GROUP_CONCAT(
        CONCAT(m.mefunc, '(', pr.prNum, ')', pr.prMethod) 
        SEPARATOR '; '
    ) AS '处方详情',
    -- 病历信息
    ch.chNo AS '病历编号',
    ch.chArchiveTime AS '归档时间',
    -- 费用信息
    b.biNo AS '账单编号',
    b.biPay AS '费用金额',
    b.biPayMethod AS '支付方式',
    b.biPayTime AS '支付时间',
    -- 就诊分类
    CASE 
        WHEN ir.irNo IS NOT NULL AND mpr.mprNo IS NOT NULL THEN '完整就诊'
        WHEN ir.irNo IS NOT NULL THEN '检验就诊'
        WHEN mpr.mprNo IS NOT NULL THEN '取药就诊'
        ELSE '简单咨询'
    END AS '就诊类型'
FROM Patient p
LEFT JOIN MedicalRecord mr ON p.pNo = mr.pNo
LEFT JOIN Employee e1 ON mr.oppNo = e1.empNo
LEFT JOIN CaseHistory ch ON mr.mrNo = ch.mrNo
LEFT JOIN InspectionRecord ir ON ch.irNo = ir.irNo
LEFT JOIN Employee e2 ON ir.mtNo = e2.empNo
LEFT JOIN MedicationPickupRecord mpr ON ch.mprNo = mpr.mprNo
LEFT JOIN Employee e3 ON mpr.cNo = e3.empNo
LEFT JOIN PrescriptionRecord pr ON mpr.mprNo = pr.mprNo
LEFT JOIN Medicine m ON pr.meNo = m.meNo
LEFT JOIN Billing b ON ch.chNo = b.chNo
GROUP BY p.pNo, p.pName, mr.mrNo, mr.mrTime, mr.mrContent, e1.empName,
         ir.irNo, ir.irTime, ir.irResult, e2.empName,
         mpr.mprNo, mpr.cTime, e3.empName,
         ch.chNo, ch.chArchiveTime,
         b.biNo, b.biPay, b.biPayMethod, b.biPayTime
ORDER BY p.pNo, mr.mrTime DESC;

-- ================================================================================
-- 视图信息查询
-- ================================================================================

-- 查看所有视图
SELECT 
    TABLE_NAME as '视图名称',
    VIEW_DEFINITION as '视图定义',
    CHECK_OPTION as '检查选项',
    IS_UPDATABLE as '是否可更新'
FROM INFORMATION_SCHEMA.VIEWS 
WHERE TABLE_SCHEMA = DATABASE()
ORDER BY TABLE_NAME;

-- ================================================================================
-- 视图设计说明
-- ================================================================================
/*
1. 视图1 (PatientMedicalSummaryView):
   - 业务场景：病人基础信息查询和就诊概况
   - 功能：快速了解病人的基本情况和就诊历史摘要
   - 用途：挂号、接诊、病人管理
   - 业务价值：提高接诊效率，快速了解病人情况

2. 视图2 (AvailableMedicineView):
   - 业务场景：药品库存管理和查询
   - 功能：显示药品库存状态和补货优先级
   - 用途：药房管理、开药参考、采购决策
   - 业务价值：优化库存管理，避免缺药情况

3. 视图3 (DoctorDepartmentView):
   - 业务场景：医生信息管理和科室统计
   - 功能：整合不同类型医生的信息和工作量
   - 用途：人事管理、排班安排、绩效评估
   - 业务价值：支持人力资源管理和工作量分析

4. 视图4 (PatientFullMedicalHistoryView):
   - 业务场景：完整病历查询和医疗质量分析
   - 功能：提供病人的完整就医历史和费用信息
   - 用途：诊断参考、病历管理、质量控制
   - 业务价值：支持医生诊断决策，提高医疗质量

这四个视图覆盖了医院管理系统的核心查询需求：
- 病人管理（基础信息和完整历史）
- 药品管理（库存状态和可用性）
- 人力资源管理（医生信息和工作量）
- 医疗质量管理（完整医疗记录）
*/
