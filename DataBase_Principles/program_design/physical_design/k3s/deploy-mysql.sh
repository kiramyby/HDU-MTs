#!/bin/bash

# 创建数据存储目录
echo "创建数据存储目录..."
sudo mkdir -p /mnt/data
sudo chmod 777 /mnt/data

# 从原始SQL文件创建ConfigMap
echo "从hospital_management_system.sql创建初始化ConfigMap..."
cat > mysql-init-configmap.yaml << EOF
apiVersion: v1
kind: ConfigMap
metadata:
  name: mysql-init-script
data:
  hospital_management_system.sql: |-
EOF

# 添加前导语句
echo "    USE hospitaldb;" >> mysql-init-configmap.yaml
echo "" >> mysql-init-configmap.yaml

# 将SQL文件内容添加到ConfigMap
cat ../hospital_management_system.sql | sed 's/^/    /' >> mysql-init-configmap.yaml

# 应用Kubernetes配置
echo "部署MySQL..."
kubectl apply -f mysql-pv.yaml
kubectl apply -f mysql-secret.yaml
kubectl apply -f mysql-init-configmap.yaml
kubectl apply -f mysql-deployment.yaml

# 等待Pod就绪
echo "等待MySQL Pod就绪..."
kubectl wait --for=condition=ready pod -l app=mysql --timeout=300s

echo "MySQL数据库已成功部署！"
echo "可以使用以下命令连接到MySQL："
echo "kubectl run -it --rm --image=mysql:8.0 --restart=Never mysql-client -- mysql -h mysql -u root -p"
echo "密码: your-password"
