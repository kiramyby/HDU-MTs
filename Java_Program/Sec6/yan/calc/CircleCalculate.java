package yan.calc;

import yan.util.AreaCalculate;

public class CircleCalculate extends AreaCalculate {
    private double radius;
    private double area;
    private String type = "Circle";

    @Override
    public void setParameter() {
        this.radius = 5.0;
        this.area = Math.PI * radius * radius;
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
}
