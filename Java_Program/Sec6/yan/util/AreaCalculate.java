package yan.util;

public abstract class AreaCalculate implements Calculate {
    // 保持抽象方法与接口一致，确保可以被MainProcess访问
    @Override
    public abstract void setParameter();
    
    @Override
    public abstract void getArea();
    
    @Override
    public abstract void showInfo();
    
    // 可以添加一些受保护的工具方法，供calc包中的子类使用
    protected double calculateCircleArea(double radius) {
        return Math.PI * radius * radius;
    }
    
    protected double calculateTriangleArea(double a, double b, double c) {
        double p = (a + b + c) / 2;
        return Math.sqrt(p * (p - a) * (p - b) * (p - c));
    }
}
