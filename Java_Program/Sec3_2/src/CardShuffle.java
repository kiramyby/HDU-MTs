import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.lang.String;

public class CardShuffle {
    int shuffle_num;
    int num;
    int[][] card;

    CardShuffle(int n, int times){
        card = new int[52][2];
        shuffle_num = times;
        num = n;

        for(int i = 0; i <52; i++){
            card[i][0] = i % 4;
            card[i][1] = i % 13;
        }

        for(int i = 0; i <times; i++){
            shuffle();
        }

    }

    public void shuffle(){
        List<int[]> list = new ArrayList<>();
        Collections.addAll(list, card);
        Collections.shuffle(list);
        card = list.toArray(new int[52][2]);
    }

    public void showCards(){
        for (int i = 0; i < num; i++){
            System.out.println(
                    "Card_info:  " +
                    getCardType(card[i][0]) +
                    " " + getCardValue(card[i][1])
            );
        }
    }

    private String getCardType(int type){
        return switch (type) {
            case 0 -> "红桃";
            case 1 -> "黑桃";
            case 2 -> "方片";
            case 3 -> "梅花";
            default -> "Unknown";
        };
    }

    private String getCardValue(int val){
        return switch (val) {
            case 0 -> "Ace";
            case 10 -> "Jack";
            case 11 -> "Queen";
            case 12 -> "King";
            default -> String.valueOf(val + 1);
        };
    }
}
