package yan.calc;

import yan.util.AreaCalculate;

public class TriangleCalculate extends AreaCalculate {
    private double a, b, c;
    private double area;
    private String type = "Triangle";

    @Override
    public void setParameter() {
        System.out.println("Setting triangle parameters...");
        this.a = 3.0;
        this.b = 4.0;
        this.c = 5.0;
        
        double p = (a + b + c) / 2;
        this.area = Math.sqrt(p * (p - a) * (p - b) * (p - c));
    }

    @Override
    public void getArea() {
        System.out.println("Calculating area of Triangle...");
        System.out.println("Area: " + this.area);
    }

    @Override
    public void showInfo() {
        System.out.println("Type: " + type);
        System.out.println("Sides: a=" + a + ", b=" + b + ", c=" + c);
        System.out.println("Area: " + area);
    }
}
