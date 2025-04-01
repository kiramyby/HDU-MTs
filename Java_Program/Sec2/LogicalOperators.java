public class LogicalOperators {
    public LogicalOperators(){
        boolean result;

        result = 0 <= 100 && 200 <= 100;
        System.out.println("0 <= 100 && 200 <= 100: " + result);

        result = 20 <= 25 && !(40>15) || 20 >= 25;
        System.out.println("20 <= 25 && !(40>15) || 20 >= 25: " + result);

    }
}
