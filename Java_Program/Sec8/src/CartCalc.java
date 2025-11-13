import java.io.*;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.LinkedList;
import java.util.Queue;

class Item {
    String username;
    String time;
    double total;

    public Item(String[] data) throws IllegalArgumentException {
        // 获取当前日期
        time = LocalDate.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd"));
        // 创建条目
        try{
            this.username = data[0];
            double price = Double.parseDouble(data[2]);
            int quantity = Integer.parseInt(data[3]);
            this.total = price * quantity;
        }catch (IllegalArgumentException e){
            throw new IllegalArgumentException("Invalid data format: " + e.getMessage());
        }
    }

    @Override
    public String toString() {
        return username + ',' + time + ',' + String.format("%.2f", total);
    }
}

public class CartCalc {
    public static void main(String[] args) {
        String readFilePath = "./src/cart.txt";
        String writeFilePath = "./src/book.txt";

        // 创建队列保存条目
        Queue<Item> bookQueue = new LinkedList<>();
        int size = 0;

        // 读取记录并转换为条目
        try{
            BufferedReader br = new BufferedReader(new FileReader(readFilePath));
            String line;
            while ((line = br.readLine()) != null) {
                Item item = new Item(line.split(","));
                bookQueue.add(item);
                size += 1;
            }
        }catch (IOException e) {
            e.printStackTrace();
        }

        // 写入条目
        try{
            BufferedWriter bw = new BufferedWriter(new FileWriter(writeFilePath));
            while (!bookQueue.isEmpty()) {
                Item item = bookQueue.poll();
                bw.write(item.toString());
                bw.newLine();
            }
            bw.close();
        }catch (IOException e) {
            e.printStackTrace();
        }
        // 运行结束提示
        System.out.println("已写入 共"+ size +"条 条目到 " + writeFilePath);

    }
}