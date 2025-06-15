-- 表一: 员工表（Employee）- 基础表，不依赖其他表
CREATE TABLE Employee (
    empNo CHAR(5) PRIMARY KEY COMMENT '员工工号',
    empName VARCHAR(10) NOT NULL COMMENT '姓名',
    empStatus VARCHAR(5) COMMENT '在职状态',
    empDutyStatus VARCHAR(5) COMMENT '在岗状态',
    empEntryYear DATE COMMENT '入职年份',
    empJobPosition CHAR(4) COMMENT '职级',
    empProfile TEXT COMMENT '个人简介'
);

-- 表二: 人事管理员表（PersonnelAdministrator）- 依赖Employee表
CREATE TABLE PersonnelAdministrator (
    paNo CHAR(5) PRIMARY KEY COMMENT '人事管理员工号',
    CONSTRAINT fk_personnelAdmin_employee FOREIGN KEY (paNo) REFERENCES Employee(empNo)
);

-- 表三: 人员管理（Manage）- 依赖PersonnelAdministrator和Employee表
CREATE TABLE Manage (
    paNo CHAR(5) COMMENT '人事管理员工号',
    empNo CHAR(5) COMMENT '员工工号',
    PRIMARY KEY (paNo, empNo),
    CONSTRAINT fk_manage_personnel FOREIGN KEY (paNo) REFERENCES PersonnelAdministrator(paNo),
    CONSTRAINT fk_manage_employee FOREIGN KEY (empNo) REFERENCES Employee(empNo)
);

-- 表四: 科室表（Unit）- 基础表，不依赖其他表
CREATE TABLE Unit (
    unNo CHAR(5) PRIMARY KEY COMMENT '科室编号',
    unName VARCHAR(10) NOT NULL COMMENT '科室名称',
    unResponsibility VARCHAR(30) COMMENT '科室职责',
    unPosition VARCHAR(50) COMMENT '科室位置'
);

-- 表八: 检验仓库表（Warehouse）- 基础表，移到这里因为被Laboratory引用
CREATE TABLE Warehouse (
    whNo CHAR(5) PRIMARY KEY COMMENT '检验仓库编号',
    whName VARCHAR(20) NOT NULL COMMENT '仓库名称',
    whCapacity DECIMAL(10,2) COMMENT '仓库容量（立方米）'
);

-- 表五: 检验科室表（Laboratory）- 依赖Warehouse表
CREATE TABLE Laboratory (
    laNo CHAR(5) PRIMARY KEY COMMENT '检验科室编号',
    lwNo CHAR(5) COMMENT '检验仓库编号',
    CONSTRAINT fk_laboratory_warehouse FOREIGN KEY (lwNo) REFERENCES Warehouse(whNo)
);

-- 表六: 检验医生表（MedicalTechnologist）- 依赖Employee和Laboratory表
CREATE TABLE MedicalTechnologist (
    mtNo CHAR(5) PRIMARY KEY COMMENT '检验医生工号',
    laNo CHAR(5) COMMENT '检验科室编号',
    CONSTRAINT fk_medicaltechnologist_employee FOREIGN KEY (mtNo) REFERENCES Employee(empNo),
    CONSTRAINT fk_medicaltechnologist_laboratory FOREIGN KEY (laNo) REFERENCES Laboratory(laNo)
);

-- 表七: 检验仓库管理（WarehouseManagement）- 依赖Warehouse和Employee表
CREATE TABLE WarehouseManagement (
    wmNo CHAR(5) COMMENT '检验仓库编号',
    wmDoctorNo CHAR(5) COMMENT '检验仓库管理医生工号',
    PRIMARY KEY (wmNo, wmDoctorNo),
    CONSTRAINT fk_warehousemanagement_warehouse FOREIGN KEY (wmNo) REFERENCES Warehouse(whNo),
    CONSTRAINT fk_warehousemanagement_employee FOREIGN KEY (wmDoctorNo) REFERENCES Employee(empNo)
);

-- 表九: 检验器材表（InspectionEquipment）- 依赖Warehouse表
CREATE TABLE InspectionEquipment (
    ieNo CHAR(5) PRIMARY KEY COMMENT '检验器材编号',
    wmNo CHAR(5) COMMENT '检验仓库编号',
    ieFunc TEXT COMMENT '器材功能',
    ieNumber INT NOT NULL DEFAULT 0 COMMENT '器材数量',
    CONSTRAINT fk_inspectionequipment_warehouse FOREIGN KEY (wmNo) REFERENCES Warehouse(whNo)
);

-- 表十一: 门诊科室表（OutPatientDepartment）- 基础表，不依赖其他表
CREATE TABLE OutPatientDepartment (
    opNo CHAR(5) PRIMARY KEY COMMENT '门诊科室编号'
);

-- 表十二: 门诊医生表（OutPatientPhysician）- 依赖Employee和OutPatientDepartment表
CREATE TABLE OutPatientPhysician (
    oppNo CHAR(5) PRIMARY KEY COMMENT '门诊医生工号',
    opNo CHAR(5) COMMENT '门诊科室编号',
    CONSTRAINT fk_outpatientphysician_employee FOREIGN KEY (oppNo) REFERENCES Employee(empNo),
    CONSTRAINT fk_outpatientphysician_department FOREIGN KEY (opNo) REFERENCES OutPatientDepartment(opNo)
);

-- 表十三: 病人表（Patient）- 基础表，不依赖其他表
CREATE TABLE Patient (
    pNo CHAR(5) PRIMARY KEY COMMENT '病人编号',
    pName VARCHAR(10) NOT NULL COMMENT '姓名',
    pID CHAR(18) NOT NULL COMMENT '身份证号',
    pbirth DATE COMMENT '生日',
    pTelephone CHAR(13) COMMENT '联系电话',
    pDependents TEXT COMMENT '家属'
);

-- 表十四: 就诊记录表（MedicalRecord）- 依赖Patient和OutPatientPhysician表
CREATE TABLE MedicalRecord (
    mrNo CHAR(5) PRIMARY KEY COMMENT '就诊记录编号',
    pNo CHAR(5) COMMENT '病人编号',
    oppNo CHAR(5) COMMENT '接诊医生工号',
    mrTime DATE COMMENT '就诊时间',
    mrContent TEXT COMMENT '诊断内容',
    CONSTRAINT fk_medicalrecord_patient FOREIGN KEY (pNo) REFERENCES Patient(pNo),
    CONSTRAINT fk_medicalrecord_physician FOREIGN KEY (oppNo) REFERENCES OutPatientPhysician(oppNo)
);

-- 表十七: 药房医师表（Chemist）- 依赖Employee表
CREATE TABLE Chemist (
    cNo CHAR(5) PRIMARY KEY COMMENT '药房医师工号',
    CONSTRAINT fk_chemist_employee FOREIGN KEY (cNo) REFERENCES Employee(empNo)
);

-- 表十八: 取药记录表（MedicationPickupRecord）- 依赖MedicalRecord和Chemist表
CREATE TABLE MedicationPickupRecord (
    mprNo CHAR(5) PRIMARY KEY COMMENT '取药记录编号',
    mrNo CHAR(5) COMMENT '就诊记录编号',
    cNo CHAR(5) COMMENT '药房医师工号',
    cTime DATE COMMENT '取药时间',
    CONSTRAINT fk_medicationpickup_medicalrecord FOREIGN KEY (mrNo) REFERENCES MedicalRecord(mrNo),
    CONSTRAINT fk_medicationpickup_chemist FOREIGN KEY (cNo) REFERENCES Chemist(cNo)
);

-- 表十: 检验记录表（InspectionRecord）- 依赖MedicalTechnologist和MedicalRecord表
CREATE TABLE InspectionRecord (
    irNo CHAR(5) PRIMARY KEY COMMENT '检验记录编号',
    mtNo CHAR(5) COMMENT '检验医师工号',
    mrNo CHAR(5) COMMENT '就诊记录编号',
    irTime DATE DEFAULT (CURRENT_DATE) COMMENT '检验时间',
    irResult TEXT COMMENT '检验结果',
    CONSTRAINT fk_inspectionrecord_medicaltechnologist FOREIGN KEY (mtNo) REFERENCES MedicalTechnologist(mtNo),
    CONSTRAINT fk_inspectionrecord_medicalrecord FOREIGN KEY (mrNo) REFERENCES MedicalRecord(mrNo)
);

-- 表十五: 病历表（CaseHistory）- 依赖InspectionRecord、MedicalRecord和MedicationPickupRecord表
CREATE TABLE CaseHistory (
    chNo CHAR(5) PRIMARY KEY COMMENT '病历编号',
    irNo CHAR(5) COMMENT '检验记录编号',
    mrNo CHAR(5) COMMENT '就诊记录编号',
    mprNo CHAR(5) COMMENT '取药记录编号',
    chArchiveTime DATE COMMENT '归档时间',
    CONSTRAINT fk_casehistory_inspectionrecord FOREIGN KEY (irNo) REFERENCES InspectionRecord(irNo),
    CONSTRAINT fk_casehistory_medicalrecord FOREIGN KEY (mrNo) REFERENCES MedicalRecord(mrNo),
    CONSTRAINT fk_casehistory_medicationpickup FOREIGN KEY (mprNo) REFERENCES MedicationPickupRecord(mprNo)
);

-- 表十六: 账单表（Billing）- 依赖CaseHistory表
CREATE TABLE Billing (
    biNo CHAR(5) PRIMARY KEY COMMENT '账单编号',
    chNo CHAR(5) COMMENT '病历编号',
    biPay DECIMAL(10,2) NOT NULL COMMENT '金额',
    biPayMethod VARCHAR(20) COMMENT '支付方式',
    biPayTime DATE COMMENT '支付日期',
    CONSTRAINT fk_billing_casehistory FOREIGN KEY (chNo) REFERENCES CaseHistory(chNo)
);

-- 表十九: 药房表（DrugStore）- 基础表，不依赖其他表
CREATE TABLE DrugStore (
    dsNo CHAR(5) PRIMARY KEY COMMENT '药房编号',
    dsName VARCHAR(20) COMMENT '药房名称',
    dsCapacity DECIMAL(10,2) COMMENT '药房容量（立方米）'
);

-- 表二十: 药房管理（PharmacyManage）- 依赖Chemist和DrugStore表
CREATE TABLE PharmacyManage (
    cNo CHAR(5) COMMENT '药房医师工号',
    dsNo CHAR(5) COMMENT '药房编号',
    PRIMARY KEY (cNo, dsNo),
    CONSTRAINT fk_pharmacymanage_chemist FOREIGN KEY (cNo) REFERENCES Chemist(cNo),
    CONSTRAINT fk_pharmacymanage_drugstore FOREIGN KEY (dsNo) REFERENCES DrugStore(dsNo)
);

-- 表二十一: 药品表（Medicine）- 依赖DrugStore表
CREATE TABLE Medicine (
    meNo CHAR(5) PRIMARY KEY COMMENT '药品编号',
    dsNo CHAR(5) COMMENT '药房编号',
    mefunc TEXT COMMENT '药品功能',
    meNum INT NOT NULL DEFAULT 0 COMMENT '药品数量',
    CONSTRAINT fk_medicine_drugstore FOREIGN KEY (dsNo) REFERENCES DrugStore(dsNo)
);

-- 表二十二: 处方记录表（PrescriptionRecord）- 依赖MedicationPickupRecord和Medicine表
CREATE TABLE PrescriptionRecord (
    prNo CHAR(5) PRIMARY KEY COMMENT '处方记录编号',
    mprNo CHAR(5) COMMENT '取药记录编号',
    meNo CHAR(5) COMMENT '药品编号',
    prNum VARCHAR(10) COMMENT '用量',
    prMethod TEXT COMMENT '用法',
    CONSTRAINT fk_prescriptionrecord_medicationpickup FOREIGN KEY (mprNo) REFERENCES MedicationPickupRecord(mprNo),
    CONSTRAINT fk_prescriptionrecord_medicine FOREIGN KEY (meNo) REFERENCES Medicine(meNo)
);

-- 创建索引以优化查询性能
-- 根据设计文档，仅对高频查询字段创建索引

-- 就诊记录表的索引
-- 病人编号索引 - 频繁与病人表进行连接查询
CREATE INDEX idx_medicalrecord_pno ON MedicalRecord(pNo);

-- 门诊医生工号索引 - 频繁与门诊医生表进行连接查询
CREATE INDEX idx_medicalrecord_oppno ON MedicalRecord(oppNo);

-- 就诊时间索引 - 按时间查询就诊记录
CREATE INDEX idx_medicalrecord_mrtime ON MedicalRecord(mrTime);

-- 组合索引 - 按病人编号和时间范围查询，覆盖pNo过滤和mrTime排序
CREATE INDEX idx_medicalrecord_pno_mrtime ON MedicalRecord(pNo, mrTime);
