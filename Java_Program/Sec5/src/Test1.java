public class Test1 {
    Undergraduate underStu = new Undergraduate("Kira", "123456", "CS", "20040908", "辅导员");
    Graduate graduateStu = new Graduate("Kira", "123456", "CS", "20040908", "导师");

    public static void main(String[] args) {
        Test1 test = new Test1();
        test.underStu.getInfo();
        test.graduateStu.getInfo();
    }
}
