#!/bin/bash

# 确认卸载
echo "警告: 这将删除MySQL数据库和所有存储的数据。"
read -p "确认卸载？ (y/N): " confirm

if [ "$confirm" != "y" ] && [ "$confirm" != "Y" ]; then
  echo "取消卸载"
  exit 0
fi

# 删除所有资源
echo "卸载MySQL资源..."
kubectl delete -f mysql-deployment.yaml
kubectl delete -f mysql-init-configmap.yaml
kubectl delete -f mysql-secret.yaml
kubectl delete -f mysql-pv.yaml

echo "MySQL成功卸载。"
echo "数据目录 /mnt/data 未被删除。如需删除，请手动执行: sudo rm -rf /mnt/data"
