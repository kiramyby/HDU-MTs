import java.util.Arrays;

public class FindMaxFromArray {

    double[] myList;
    private final double[] sortedList;          // avoid Arrays.sort method change myList

    public FindMaxFromArray(int n) {
        myList = new double[n];
        sortedList = new double[n];

        for (int i = 0; i < n; i++) {
            myList[i] = Math.random()*100;
            sortedList[i] = myList[i];
        }
        Arrays.sort(sortedList);    // Sort list for MAX value, Ascending
    }

    public void getRandomArray() {
        for (double v : myList) {
            System.out.printf("%.2f ", v);
        }
        System.out.println();
    }

    public void getMaxNumber() {
        System.out.printf("MAX = %.2f", sortedList[myList.length - 1]);
    }
}
