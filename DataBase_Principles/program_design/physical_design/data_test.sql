-- 医院管理系统测试数据

-- 1. 检验仓库表（需要先插入，因为其他表有外键依赖）
INSERT INTO Warehouse (whNo, whName, whCapacity) VALUES
('WH001', '中央检验仓库', 150.50),
('WH002', '血液检验仓库', 80.25),
('WH003', '生化检验仓库', 120.75),
('WH004', '微生物仓库', 90.30),
('WH005', '病理仓库', 110.40);

-- 2. 药房表
INSERT INTO DrugStore (dsNo, dsName, dsCapacity) VALUES
('DS001', '中药房', 200.00),
('DS002', '西药房', 180.50),
('DS003', '急诊药房', 120.30),
('DS004', '儿科药房', 90.75),
('DS005', '肿瘤科药房', 150.25);

-- 3. 科室表
INSERT INTO Unit (unNo, unName, unResponsibility, unPosition) VALUES
('UN001', '内科', '内科疾病诊疗', '门诊楼2楼'),
('UN002', '外科', '外科手术治疗', '门诊楼3楼'),
('UN003', '儿科', '儿童疾病诊疗', '门诊楼1楼'),
('UN004', '妇产科', '妇科产科诊疗', '住院楼4楼'),
('UN005', '急诊科', '急诊抢救治疗', '急诊楼1楼'),
('UN006', '检验科', '医学检验服务', '医技楼2楼'),
('UN007', '放射科', '影像检查服务', '医技楼1楼'),
('UN008', '药剂科', '药品管理配发', '门诊楼地下1楼');

-- 4. 员工表
INSERT INTO Employee (empNo, empName, empStatus, empDutyStatus, empEntryYear, empJobPosition, empProfile) VALUES
('EMP01', '张伟', '在职', '在岗', '2020-03-15', '主任', '内科主任医师，从事内科临床工作15年'),
('EMP02', '李娜', '在职', '在岗', '2018-07-22', '副主任', '外科副主任医师，擅长胸外科手术'),
('EMP03', '王强', '在职', '在岗', '2019-01-10', '主治', '儿科主治医师，专业儿童呼吸系统疾病'),
('EMP04', '刘敏', '在职', '在岗', '2021-05-08', '住院', '妇产科住院医师，新入职员工'),
('EMP05', '陈明', '在职', '在岗', '2017-09-12', '主任', '急诊科主任，急救医学专家'),
('EMP06', '赵静', '在职', '在岗', '2019-11-20', '主治', '检验科主治医师，临床检验专业'),
('EMP07', '孙亮', '在职', '在岗', '2020-08-05', '主治', '检验科主治医师，生化检验专业'),
('EMP08', '周红', '在职', '在岗', '2018-12-15', '副主任', '药剂科副主任，临床药学专业'),
('EMP09', '吴磊', '在职', '在岗', '2021-02-28', '主治', '药剂科主治药师，中药学专业'),
('EMP10', '黄芳', '在职', '在岗', '2016-06-30', '主任', '人事部主任，负责人员管理'),
('EMP11', '马超', '在职', '在岗', '2020-04-18', '主治', '内科主治医师，心血管专业'),
('EMP12', '林雪', '在职', '在岗', '2019-10-12', '住院', '外科住院医师，普通外科'),
('EMP13', '郑涛', '在职', '在岗', '2018-03-25', '主治', '急诊科主治医师'),
('EMP14', '许玲', '在职', '在岗', '2021-01-15', '住院', '儿科住院医师'),
('EMP15', '钱伟', '在职', '在岗', '2019-07-08', '主治', '检验科主治医师，血液检验专业');

-- 5. 人事管理员表
INSERT INTO PersonnelAdministrator (paNo) VALUES
('EMP10');

-- 6. 人员管理关系表
INSERT INTO Manage (paNo, empNo) VALUES
('EMP10', 'EMP01'),
('EMP10', 'EMP02'),
('EMP10', 'EMP03'),
('EMP10', 'EMP04'),
('EMP10', 'EMP05'),
('EMP10', 'EMP06'),
('EMP10', 'EMP07'),
('EMP10', 'EMP08'),
('EMP10', 'EMP09'),
('EMP10', 'EMP11'),
('EMP10', 'EMP12'),
('EMP10', 'EMP13'),
('EMP10', 'EMP14'),
('EMP10', 'EMP15');

-- 7. 检验科室表
INSERT INTO Laboratory (laNo, lwNo) VALUES
('LAB01', 'WH001'),
('LAB02', 'WH002'),
('LAB03', 'WH003'),
('LAB04', 'WH004'),
('LAB05', 'WH005');

-- 8. 检验医生表
INSERT INTO MedicalTechnologist (mtNo, laNo) VALUES
('EMP06', 'LAB01'),
('EMP07', 'LAB03'),
('EMP15', 'LAB02');

-- 9. 检验仓库管理表
INSERT INTO WarehouseManagement (wmNo, wmDoctorNo) VALUES
('WH001', 'EMP06'),
('WH002', 'EMP15'),
('WH003', 'EMP07'),
('WH004', 'EMP06'),
('WH005', 'EMP15');

-- 10. 检验器材表
INSERT INTO InspectionEquipment (ieNo, wmNo, ieFunc, ieNumber) VALUES
('IE001', 'WH001', '全自动血细胞分析仪，用于血常规检测', 2),
('IE002', 'WH001', '尿液分析仪，用于尿常规检测', 3),
('IE003', 'WH002', '血凝分析仪，用于凝血功能检测', 1),
('IE004', 'WH002', '血气分析仪，用于血气酸碱度检测', 2),
('IE005', 'WH003', '全自动生化分析仪，用于生化指标检测', 2),
('IE006', 'WH003', '酶标仪，用于免疫检测', 1),
('IE007', 'WH004', '细菌培养箱，用于细菌培养', 3),
('IE008', 'WH004', '显微镜，用于病原体观察', 5),
('IE009', 'WH005', '切片机，用于病理切片制作', 2),
('IE010', 'WH005', '染色机，用于病理染色', 1);

-- 11. 门诊科室表
INSERT INTO OutPatientDepartment (opNo) VALUES
('OP001'),
('OP002'),
('OP003'),
('OP004'),
('OP005');

-- 12. 门诊医生表
INSERT INTO OutPatientPhysician (oppNo, opNo) VALUES
('EMP01', 'OP001'),  -- 内科
('EMP11', 'OP001'),  -- 内科
('EMP02', 'OP002'),  -- 外科
('EMP12', 'OP002'),  -- 外科
('EMP03', 'OP003'),  -- 儿科
('EMP14', 'OP003'),  -- 儿科
('EMP04', 'OP004'),  -- 妇产科
('EMP05', 'OP005'),  -- 急诊科
('EMP13', 'OP005');  -- 急诊科

-- 13. 药房医师表
INSERT INTO Chemist (cNo) VALUES
('EMP08'),
('EMP09');

-- 14. 药房管理表
INSERT INTO PharmacyManage (cNo, dsNo) VALUES
('EMP08', 'DS001'),
('EMP08', 'DS002'),
('EMP08', 'DS003'),
('EMP09', 'DS001'),
('EMP09', 'DS004'),
('EMP09', 'DS005');

-- 15. 药品表
INSERT INTO Medicine (meNo, dsNo, mefunc, meNum) VALUES
-- 中药房药品
('MED01', 'DS001', '清热解毒，用于感冒发热', 100),
('MED02', 'DS001', '健脾益气，用于脾胃虚弱', 80),
('MED03', 'DS001', '活血化瘀，用于跌打损伤', 60),
-- 西药房药品
('MED04', 'DS002', '解热镇痛，用于发热头痛', 200),
('MED05', 'DS002', '抗生素，用于细菌感染', 150),
('MED06', 'DS002', '降压药，用于高血压治疗', 120),
('MED07', 'DS002', '胰岛素，用于糖尿病治疗', 80),
-- 急诊药房药品
('MED08', 'DS003', '急救药物，用于心脏急救', 50),
('MED09', 'DS003', '止血药，用于外伤止血', 60),
-- 儿科药房药品
('MED10', 'DS004', '儿童感冒糖浆，用于小儿感冒', 100),
('MED11', 'DS004', '儿童退烧药，用于小儿发热', 80),
-- 肿瘤科药房药品
('MED12', 'DS005', '化疗药物A，用于肿瘤治疗', 30),
('MED13', 'DS005', '化疗药物B，用于肿瘤治疗', 25);

-- 16. 病人表
INSERT INTO Patient (pNo, pName, pID, pbirth, pTelephone, pDependents) VALUES
('PAT01', '李明', '330102199001011234', '1990-01-01', '13812345678', '配偶：王丽，电话：13987654321'),
('PAT02', '张华', '330103198503152345', '1985-03-15', '13823456789', '父亲：张伟，电话：13876543210'),
('PAT03', '王芳', '330104197812203456', '1978-12-20', '13834567890', '丈夫：李强，电话：13765432109'),
('PAT04', '刘小明', '330105201005104567', '2010-05-10', '13845678901', '母亲：刘敏，电话：13654321098'),
('PAT05', '陈雅', '330106199207256789', '1992-07-25', '13856789012', '男友：张斌，电话：13543210987'),
('PAT06', '赵强', '330107196811308901', '1968-11-30', '13867890123', '妻子：李娟，电话：13432109876'),
('PAT07', '孙丽', '330108200208159012', '2002-08-15', '13878901234', '父亲：孙伟，电话：13321098765'),
('PAT08', '周建国', '330109195506203456', '1955-06-20', '13889012345', '儿子：周明，电话：13210987654'),
('PAT09', '吴美娜', '330110198909128901', '1989-09-12', '13890123456', '母亲：吴红，电话：13109876543'),
('PAT10', '郑天', '330111199512024567', '1995-12-02', '13901234567', '女友：林雪，电话：13098765432');

-- 17. 就诊记录表
INSERT INTO MedicalRecord (mrNo, pNo, oppNo, mrTime, mrContent) VALUES
('MR001', 'PAT01', 'EMP01', '2025-05-20', '患者主诉发热3天，伴有咳嗽，体温38.5°C。初步诊断：上呼吸道感染'),
('MR002', 'PAT02', 'EMP02', '2025-05-21', '患者右腿外伤，伤口长约5cm，需要清创缝合。诊断：右腿外伤'),
('MR003', 'PAT03', 'EMP04', '2025-05-22', '孕妇产前检查，孕36周，胎位正常，各项指标正常'),
('MR004', 'PAT04', 'EMP03', '2025-05-23', '患儿发热2天，体温39°C，咽部红肿。诊断：急性咽炎'),
('MR005', 'PAT05', 'EMP11', '2025-05-24', '患者胸闷气短1周，心电图显示心律不齐。诊断：心律不齐'),
('MR006', 'PAT06', 'EMP01', '2025-05-25', '高血压复查，血压150/90mmHg，需调整用药'),
('MR007', 'PAT07', 'EMP14', '2025-05-26', '青少年体检，发现轻度贫血，建议营养调理'),
('MR008', 'PAT08', 'EMP05', '2025-05-27', '急诊就诊，突发胸痛2小时，排除心肌梗死'),
('MR009', 'PAT09', 'EMP11', '2025-05-28', '体检发现血糖偏高，建议进一步检查'),
('MR010', 'PAT10', 'EMP12', '2025-05-29', '腹痛3天，右下腹压痛明显，疑似阑尾炎');

-- 18. 检验记录表
INSERT INTO InspectionRecord (irNo, mtNo, mrNo, irTime, irResult) VALUES
('IR001', 'EMP06', 'MR001', '2025-05-20', '血常规：白细胞计数10.2×10^9/L（偏高），中性粒细胞77%'),
('IR002', 'EMP15', 'MR002', '2025-05-21', '血常规：各项指标正常，凝血功能正常'),
('IR003', 'EMP07', 'MR003', '2025-05-22', '肝肾功能：ALT 25U/L，AST 30U/L，肌酐65μmol/L，均在正常范围'),
('IR004', 'EMP06', 'MR004', '2025-05-23', '血常规：白细胞计数12.5×10^9/L（明显偏高），提示细菌感染'),
('IR005', 'EMP07', 'MR005', '2025-05-24', '心肌酶谱：CK-MB 15U/L，cTnI 0.02ng/mL，均在正常范围'),
('IR006', 'EMP07', 'MR006', '2025-05-25', '生化全套：血糖5.8mmol/L，总胆固醇5.2mmol/L，甘油三酯1.8mmol/L'),
('IR007', 'EMP15', 'MR007', '2025-05-26', '血常规：血红蛋白95g/L（偏低），红细胞计数3.8×10^12/L'),
('IR008', 'EMP06', 'MR008', '2025-05-27', 'D-二聚体：0.3mg/L（正常），心肌标志物阴性'),
('IR009', 'EMP07', 'MR009', '2025-05-28', '空腹血糖：7.2mmol/L（偏高），糖化血红蛋白6.8%'),
('IR010', 'EMP06', 'MR010', '2025-05-29', '血常规：白细胞计数15.2×10^9/L（显著升高），中性粒细胞85%');

-- 19. 取药记录表
INSERT INTO MedicationPickupRecord (mprNo, mrNo, cNo, cTime) VALUES
('MPR01', 'MR001', 'EMP08', '2025-05-20'),
('MPR02', 'MR002', 'EMP09', '2025-05-21'),
('MPR03', 'MR003', 'EMP08', '2025-05-22'),
('MPR04', 'MR004', 'EMP09', '2025-05-23'),
('MPR05', 'MR005', 'EMP08', '2025-05-24'),
('MPR06', 'MR006', 'EMP08', '2025-05-25'),
('MPR07', 'MR007', 'EMP09', '2025-05-26'),
('MPR08', 'MR008', 'EMP08', '2025-05-27'),
('MPR09', 'MR009', 'EMP08', '2025-05-28'),
('MPR10', 'MR010', 'EMP08', '2025-05-29');

-- 20. 处方记录表
INSERT INTO PrescriptionRecord (prNo, mprNo, meNo, prNum, prMethod) VALUES
-- 李明的处方（上呼吸道感染）
('PR001', 'MPR01', 'MED04', '1片×3次', '每日3次，饭后服用，连续服用3天'),
('PR002', 'MPR01', 'MED05', '2粒×2次', '每日2次，空腹服用，连续服用5天'),
-- 张华的处方（外伤）
('PR003', 'MPR02', 'MED05', '1粒×3次', '每日3次，餐后服用，预防感染，连续服用7天'),
('PR004', 'MPR02', 'MED09', '外用', '伤口清洁后外敷，每日换药1次'),
-- 王芳的处方（孕妇产前）
('PR005', 'MPR03', 'MED02', '1剂×2次', '每日2次，温水冲服，健脾益气'),
-- 刘小明的处方（儿童急性咽炎）
('PR006', 'MPR04', 'MED10', '10ml×3次', '每日3次，饭后服用，连续服用3天'),
('PR007', 'MPR04', 'MED11', '5ml', '发热时服用，体温超过38.5°C时使用'),
-- 陈雅的处方（心律不齐）
('PR008', 'MPR05', 'MED06', '1片×1次', '每日1次，早晨空腹服用，长期服用'),
-- 赵强的处方（高血压）
('PR009', 'MPR06', 'MED06', '1片×2次', '每日2次，早晚各1次，饭后服用'),
-- 孙丽的处方（贫血）
('PR010', 'MPR07', 'MED02', '1剂×2次', '每日2次，温水冲服，补气养血'),
-- 周建国的处方（胸痛急诊）
('PR011', 'MPR08', 'MED08', '必要时', '胸痛发作时舌下含服，15分钟后可重复'),
-- 吴美娜的处方（血糖高）
('PR012', 'MPR09', 'MED07', '10单位×2次', '每日2次，餐前30分钟皮下注射'),
-- 郑天的处方（疑似阑尾炎）
('PR013', 'MPR10', 'MED05', '2粒×3次', '每日3次，饭后服用，预防感染');

-- 21. 病历表
INSERT INTO CaseHistory (chNo, irNo, mrNo, mprNo, chArchiveTime) VALUES
('CH001', 'IR001', 'MR001', 'MPR01', '2025-05-21'),
('CH002', 'IR002', 'MR002', 'MPR02', '2025-05-22'),
('CH003', 'IR003', 'MR003', 'MPR03', '2025-05-23'),
('CH004', 'IR004', 'MR004', 'MPR04', '2025-05-24'),
('CH005', 'IR005', 'MR005', 'MPR05', '2025-05-25'),
('CH006', 'IR006', 'MR006', 'MPR06', '2025-05-26'),
('CH007', 'IR007', 'MR007', 'MPR07', '2025-05-27'),
('CH008', 'IR008', 'MR008', 'MPR08', '2025-05-28'),
('CH009', 'IR009', 'MR009', 'MPR09', '2025-05-29'),
('CH010', 'IR010', 'MR010', 'MPR10', '2025-05-29');

-- 22. 账单表
INSERT INTO Billing (biNo, chNo, biPay, biPayMethod, biPayTime) VALUES
('BI001', 'CH001', 85.50, '微信支付', '2025-05-20'),
('BI002', 'CH002', 156.80, '支付宝', '2025-05-21'),
('BI003', 'CH003', 234.20, '现金', '2025-05-22'),
('BI004', 'CH004', 76.30, '医保卡', '2025-05-23'),
('BI005', 'CH005', 198.90, '银行卡', '2025-05-24'),
('BI006', 'CH006', 145.60, '医保卡', '2025-05-25'),
('BI007', 'CH007', 89.40, '微信支付', '2025-05-26'),
('BI008', 'CH008', 267.80, '银行卡', '2025-05-27'),
('BI009', 'CH009', 312.50, '医保卡', '2025-05-28'),
('BI010', 'CH010', 423.70, '支付宝', '2025-05-29');

-- ============================================
-- 数据统计信息
-- ============================================

-- 查看插入的数据统计
SELECT '员工总数' AS 统计项, COUNT(*) AS 数量 FROM Employee
UNION ALL
SELECT '病人总数', COUNT(*) FROM Patient
UNION ALL
SELECT '就诊记录总数', COUNT(*) FROM MedicalRecord
UNION ALL
SELECT '检验记录总数', COUNT(*) FROM InspectionRecord
UNION ALL
SELECT '取药记录总数', COUNT(*) FROM MedicationPickupRecord
UNION ALL
SELECT '处方记录总数', COUNT(*) FROM PrescriptionRecord
UNION ALL
SELECT '病历总数', COUNT(*) FROM CaseHistory
UNION ALL
SELECT '账单总数', COUNT(*) FROM Billing
UNION ALL
SELECT '药品种类', COUNT(*) FROM Medicine
UNION ALL
SELECT '检验器材种类', COUNT(*) FROM InspectionEquipment;

-- 提示信息
SELECT '测试数据插入完成！' AS 状态, 
       '本次共插入10名病人，10条就诊记录，10条检验记录，13种药品，10种检验器材等完整数据' AS 详情;