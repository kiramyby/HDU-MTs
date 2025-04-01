public class BinaryCalc {
    public BinaryCalc() {
        System.out.println(Integer.toBinaryString(8));
        System.out.println(Integer.toBinaryString(-8));
        System.out.println(Integer.toBinaryString((byte)(-8 >> 2)));
        System.out.println(Integer.toBinaryString((byte)(-8 >>> 2)));
        System.out.println(Integer.toBinaryString((byte)(8 >> 2)));
        System.out.println(Integer.toBinaryString((byte)(8 >>> 2)));
        System.out.println(-8 >>> 2);
        System.out.println(-8 >> 2);
        System.out.println(8 >>> 2);
        System.out.println(8 >> 2);
        System.out.println((byte)(-8 >>> 2));
        System.out.println((byte)(-8 >> 2));
        System.out.println((byte)(8 >>> 2));
        System.out.println((byte)(8 >> 2));
    }
}
