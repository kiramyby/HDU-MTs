# MySQL数据库部署指南 (k3s)

本指南将帮助您在k3s集群上部署MySQL数据库，并使用hospital_management_system.sql初始化数据库。

## 前提条件

- 已安装并配置k3s集群
- 已安装kubectl并配置为连接到k3s集群
- 宿主机上有足够的权限创建/mnt/data目录

## 部署步骤

### 方式一：使用部署脚本（推荐）

使用提供的脚本一键部署MySQL数据库：

```bash
# 添加执行权限
chmod +x deploy-mysql.sh

# 执行部署脚本
./deploy-mysql.sh
```

该脚本将自动：
1. 创建持久卷存储
2. 从hospital_management_system.sql生成初始化ConfigMap
3. 部署MySQL数据库及相关资源

### 方式二：手动部署

### 1. 创建持久卷和持久卷声明

```bash
# 在宿主机上创建数据目录
sudo mkdir -p /mnt/data
# 确保k3s可以访问该目录
sudo chmod 777 /mnt/data

# 应用PV和PVC配置
kubectl apply -f mysql-pv.yaml
```

### 2. 创建MySQL密钥

```bash
kubectl apply -f mysql-secret.yaml
```

注意：默认密码为"your-password"，生产环境请修改mysql-secret.yaml中的密码值。密码需要使用base64编码：
```bash
echo -n "your-new-password" | base64
```

### 3. 创建数据库初始化脚本ConfigMap

```bash
# 从hospital_management_system.sql创建ConfigMap
kubectl create configmap mysql-init-script --from-file=hospital_management_system.sql=../hospital_management_system.sql
```

或者应用准备好的ConfigMap文件：
```bash
kubectl apply -f mysql-init-configmap.yaml
```

### 4. 部署MySQL数据库

```bash
kubectl apply -f mysql-deployment.yaml
```

### 5. 验证部署

```bash
# 查看Pod状态
kubectl get pods -l app=mysql

# 查看Service状态
kubectl get svc mysql

# 连接到MySQL实例
kubectl run -it --rm --image=mysql:8.0 --restart=Never mysql-client -- mysql -h mysql -u root -p
# 输入密码 "your-password"
```

## 访问数据库

MySQL服务暴露在集群内部。如需从集群外部访问，可以使用端口转发：

```bash
kubectl port-forward svc/mysql 3306:3306
```

然后，您可以使用本地MySQL客户端连接到localhost:3306。

## 卸载

如果需要卸载MySQL数据库，请运行：

```bash
kubectl delete -f mysql-deployment.yaml
kubectl delete -f mysql-init-configmap.yaml
kubectl delete -f mysql-secret.yaml
kubectl delete -f mysql-pv.yaml
```

注意：删除PV将导致存储的数据丢失。如需保留数据，请在删除前备份。
