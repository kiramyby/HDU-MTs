package yan.main;

import yan.calc.CircleCalculate;
import yan.calc.TriangleCalculate;

public class MainProcess {
    public static void main(String[] args) {
        CircleCalculate circle = new CircleCalculate();
        TriangleCalculate triangle = new TriangleCalculate();

        circle.setParameter();
        circle.showInfo();
        circle.getArea();

        System.out.println();

        triangle.setParameter();
        triangle.showInfo();
        triangle.getArea();
    }
}
