package yan.calc;

import yan.util.AreaCalculate;

public class CircleCalculate extends AreaCalculate {
    private double radius;
    private final String type = "Circle";

    @Override
    public void setParameter() {
        System.out.print("请输入圆的半径: ");
        this.radius = input.nextDouble();
        if (checkInfo()) {
            calculateArea();
        } else {
            System.out.println("半径必须大于0，请重新设置参数");
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
        System.out.println("Radius: " + radius);
        System.out.println("Area: " + area);
    }
    
    @Override
    protected boolean checkInfo() {
        return radius > 0;
    }
    
    @Override
    protected void calculateArea() {
        this.area = Math.PI * radius * radius;
    }
}
