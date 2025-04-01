public class CircleCalc {
    public CircleCalc() {
        // 输入半径
        System.out.println("请输入半径：");
        double radius = new java.util.Scanner(System.in).nextDouble();

        double area = Math.PI * radius * radius;
        double circle = 2 * Math.PI * radius;

        System.out.println("面积：" + area);
        System.out.println("周长：" + circle);
    }
}
