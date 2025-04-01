public class OddGenerator {
    public OddGenerator() {
        for (int i = 1; i <= 1000; i++) {
            if (i % 2 != 0) {
                System.out.println(i);
            }
        }
    }
}
