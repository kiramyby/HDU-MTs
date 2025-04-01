public class Test2 {
    Chinese chinese = new Chinese("某",  "张", "China");
    American american = new American("Kiracoon", "XXX", "USA");
    public static void main(String[] args) {
        Test2 test = new Test2();
        test.chinese.showNameInNativeLanguage();
        test.chinese.showNameInNativeLanguage("母语书写的姓名： ");
        System.out.println();
        test.american.showNameInNativeLanguage();
        test.american.showNameInNativeLanguage("Name in native language: ");
    }
}
