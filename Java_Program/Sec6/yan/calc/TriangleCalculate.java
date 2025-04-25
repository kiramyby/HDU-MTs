package yan.calc;

import yan.util.AreaCalculate;

public class TriangleCalculate extends AreaCalculate {
    private double a, b, c;
    private final String type = "Triangle";

    @Override
    public void setParameter() {
        System.out.println("设置三角形的参数...");
        System.out.print("请输入第一条边长: ");
        this.a = input.nextDouble();
        System.out.print("请输入第二条边长: ");
        this.b = input.nextDouble();
        System.out.print("请输入第三条边长: ");
        this.c = input.nextDouble();
        
        if (checkInfo()) {
            calculateArea();
        } else {
            System.out.println("三角形不合法，请重新设置参数");
            setParameter();
        }
    }

    @Override
    public void getArea() {
        System.out.println("Area: " + this.area);
    }

    @Override
    public void showInfo() {
        System.out.println("Type: " + type);
        System.out.println("Sides: a=" + a + ", b=" + b + ", c=" + c);
        System.out.println("Area: " + area);
    }
    
    @Override
    protected boolean checkInfo() {
        return (a + b > c) && (a + c > b) && (b + c > a);
    }
    
    @Override
    protected void calculateArea() {
        double p = (a + b + c) / 2;
        this.area = Math.sqrt(p * (p - a) * (p - b) * (p - c));
    }
}
