import java.io.*;
import java.util.Random;

class Buffer {
    private int value;
    private boolean isEmpty = true;

    public synchronized void put(int value) throws InterruptedException {
        while (!isEmpty) {
            try {
                System.out.println("Buffer is full, waiting...");
                wait(); // 缓冲区满，生产者等待
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        this.value = value;
        isEmpty = false;
        System.out.println("Produced: " + value);
        notify(); // 唤醒消费者
    }

    public synchronized int get() throws InterruptedException {
        while (isEmpty) {
            try {
                System.out.println("Buffer is empty, waiting...");
                wait(); // 缓冲区空，消费者等待
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        isEmpty = true;
        System.out.println("Consumed: " + value);
        notify(); // 唤醒生产者
        return value;
    }
}

public class ProducerConsumerExample {
    public static void main(String[] args) {
        Buffer buffer = new Buffer();

        Thread producer = new Thread(() -> {
            for (int i = 0; i < 5; i++) {
                try {
                    buffer.put(i);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                }
        });


        Thread consumer = new Thread(() -> {
            for (int i = 0; i < 5; i++) {
                try {
                    buffer.get();
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
        });

        try {
           FileWriter writer = new FileWriter("output.txt");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        try{
            BufferedWriter reader = new BufferedReader(writer);
            reader.write("Producer-Consumer Example Started\n");
            reader.close();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        producer.start();
        consumer.start();
        Random random = new Random();
    }
}
