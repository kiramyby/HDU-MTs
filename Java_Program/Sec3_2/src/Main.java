public class Main {
    public static void main(String[] args) {
        CardShuffle cs = new CardShuffle(52,100);
        cs.shuffle();
        cs.showCards();
    }
}