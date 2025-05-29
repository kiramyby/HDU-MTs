-- 医院管理系统视图测试文件

-- 视图1测试：病人就诊摘要视图
SELECT * FROM PatientMedicalSummaryView WHERE `病人编号` = 'P0001';
SELECT * FROM PatientMedicalSummaryView WHERE `患者类型` = '近期患者';

-- 视图2测试：可用药品清单视图  
SELECT * FROM AvailableMedicineView WHERE `库存状态` = '库存不足';
SELECT * FROM AvailableMedicineView ORDER BY `库存数量` ASC;

-- 视图3测试：医生科室信息视图
SELECT * FROM DoctorDepartmentView WHERE `医生类型` = '门诊医生';
SELECT * FROM DoctorDepartmentView WHERE `近30天接诊量` > 0;

-- 视图4测试：病人完整就医历史视图
SELECT * FROM PatientFullMedicalHistoryView WHERE `病人编号` = 'P0001';
SELECT * FROM PatientFullMedicalHistoryView WHERE `就诊类型` = '完整就诊';
