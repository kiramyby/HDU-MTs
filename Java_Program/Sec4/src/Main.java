import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n=0;

        if (scanner.hasNext()) {
            n = scanner.nextInt();
        }

        RoomMate[] roomMate = new RoomMate[n];

        for (int i = 0; i < n; i++) {
            String name="", favor="";
            int age=0;

            if (scanner.hasNext()) {
                name = scanner.next();
            }
            if (scanner.hasNext()) {
                age = scanner.nextInt();
            }

            roomMate[i] = new RoomMate(name, favor, age);

        }
        for (int i = 0; i < n; i++) {
            roomMate[i].show();
        }
    }
}
