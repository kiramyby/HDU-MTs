-- ================================================================================
-- 医院管理系统触发器定义文件
-- Hospital Management System Triggers
-- ================================================================================

-- 删除已存在的触发器（如果存在）
DROP TRIGGER IF EXISTS tr_check_medicine_stock;
DROP TRIGGER IF EXISTS tr_set_medical_record_time;
DROP TRIGGER IF EXISTS tr_deduct_medicine_stock;
DROP TRIGGER IF EXISTS tr_set_case_history_archive_time;

-- 创建库存警告表（支持触发器1）
DROP TABLE IF EXISTS StockAlert;
CREATE TABLE StockAlert (
    alertId INT AUTO_INCREMENT PRIMARY KEY COMMENT '警告ID',
    alertTime DATETIME COMMENT '警告时间',
    meNo CHAR(5) COMMENT '药品编号',
    currentStock INT COMMENT '当前库存',
    alertMessage TEXT COMMENT '警告信息',
    CONSTRAINT fk_stockalert_medicine FOREIGN KEY (meNo) REFERENCES Medicine(meNo)
);

-- ================================================================================
-- 触发器1: 药品库存检查触发器
-- 功能：当药品数量更新时，检查库存是否低于安全阈值（10），如果是则记录警告
-- 触发时机：药品表更新后
-- ================================================================================
DELIMITER $$
CREATE TRIGGER tr_check_medicine_stock
AFTER UPDATE ON Medicine
FOR EACH ROW
BEGIN
    -- 当药品数量低于安全库存时，插入警告记录到库存警告表
    IF NEW.meNum < 10 AND NEW.meNum != OLD.meNum THEN
        INSERT INTO StockAlert (alertTime, meNo, currentStock, alertMessage)
        VALUES (NOW(), NEW.meNo, NEW.meNum, 
                CONCAT('药品 ', NEW.meNo, ' 库存不足，当前数量: ', NEW.meNum));
    END IF;
END$$
DELIMITER ;

-- ================================================================================
-- 触发器2: 就诊记录时间自动设置触发器
-- 功能：当插入就诊记录时，如果没有指定时间，自动设置为当前日期
-- 触发时机：就诊记录插入前
-- ================================================================================
DELIMITER $$
CREATE TRIGGER tr_set_medical_record_time
BEFORE INSERT ON MedicalRecord
FOR EACH ROW
BEGIN
    -- 如果就诊时间为空，设置为当前日期
    IF NEW.mrTime IS NULL THEN
        SET NEW.mrTime = CURDATE();
    END IF;
END$$
DELIMITER ;

-- ================================================================================
-- 触发器3: 处方记录药品数量扣减触发器
-- 功能：当插入处方记录时，自动从药品库存中扣减相应数量
-- 触发时机：处方记录插入后
-- ================================================================================
DELIMITER $$
CREATE TRIGGER tr_deduct_medicine_stock
AFTER INSERT ON PrescriptionRecord
FOR EACH ROW
BEGIN
    DECLARE medicine_count INT;
    DECLARE prescribed_amount INT;
    
    -- 获取当前药品库存
    SELECT meNum INTO medicine_count 
    FROM Medicine 
    WHERE meNo = NEW.meNo;
    
    -- 将处方用量转换为数字（假设prNum格式为数字）
    SET prescribed_amount = CAST(NEW.prNum AS UNSIGNED);
    
    -- 检查库存是否充足
    IF medicine_count >= prescribed_amount THEN
        -- 扣减药品数量
        UPDATE Medicine 
        SET meNum = meNum - prescribed_amount 
        WHERE meNo = NEW.meNo;
    ELSE
        -- 库存不足时抛出错误
        SIGNAL SQLSTATE '45000' 
        SET MESSAGE_TEXT = '药品库存不足，无法完成处方';
    END IF;
END$$
DELIMITER ;

-- ================================================================================
-- 触发器4: 病历归档时间自动设置触发器
-- 功能：当插入病历记录时，自动设置归档时间为当前日期
-- 触发时机：病历记录插入前
-- ================================================================================
DELIMITER $$
CREATE TRIGGER tr_set_case_history_archive_time
BEFORE INSERT ON CaseHistory
FOR EACH ROW
BEGIN
    -- 如果归档时间为空，设置为当前日期
    IF NEW.chArchiveTime IS NULL THEN
        SET NEW.chArchiveTime = CURDATE();
    END IF;
END$$
DELIMITER ;

-- ================================================================================
-- 触发器信息查询
-- ================================================================================

-- 查看所有触发器
SELECT 
    TRIGGER_NAME as '触发器名称',
    EVENT_MANIPULATION as '触发事件',
    EVENT_OBJECT_TABLE as '作用表',
    ACTION_TIMING as '触发时机',
    ACTION_STATEMENT as '触发语句'
FROM INFORMATION_SCHEMA.TRIGGERS 
WHERE TRIGGER_SCHEMA = DATABASE()
ORDER BY EVENT_OBJECT_TABLE, ACTION_TIMING;

-- ================================================================================
-- 触发器设计说明
-- ================================================================================
/*
1. 触发器1 (tr_check_medicine_stock):
   - 业务场景：药品库存管理
   - 功能：监控药品库存，当数量低于10时自动记录警告
   - 触发时机：药品表更新后
   - 业务价值：确保药品库存管理，及时补货，避免缺药情况

2. 触发器2 (tr_set_medical_record_time):
   - 业务场景：就诊记录管理
   - 功能：自动设置就诊记录的时间为当前日期
   - 触发时机：就诊记录插入前
   - 业务价值：确保数据完整性，简化数据录入，避免时间缺失

3. 触发器3 (tr_deduct_medicine_stock):
   - 业务场景：处方开具和库存管理
   - 功能：处方开具时自动扣减药品库存
   - 触发时机：处方记录插入后
   - 业务价值：实时库存管理，防止超量开药，保证库存准确性

4. 触发器4 (tr_set_case_history_archive_time):
   - 业务场景：病历归档管理
   - 功能：自动设置病历归档时间为当前日期
   - 触发时机：病历记录插入前
   - 业务价值：确保病历管理的时效性和完整性，规范归档流程

这四个触发器覆盖了医院管理系统的核心业务流程：
- 库存管理（药品监控和自动扣减）
- 数据完整性（时间自动填充）
- 业务流程自动化（归档时间设置）
- 异常处理（库存不足警告）
*/
