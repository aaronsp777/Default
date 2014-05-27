
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Card {

  public enum Suit {
    H, D, S, C,
  }

  Suit suit;
  int val;  // 2-10, J=11, Q=12, K=13, A=14

  public Card (Suit s, int v) {
    suit = s;
    val = v;
  }

  boolean isStarter() {
    return (suit == Suit.C) && (val == 2);  
  }
  
  boolean isTrump() {
    return suit == Suit.H;
  }

  boolean isGreater(Card other) {
    return val > other.val;
  }

  boolean sameSuit(Card other) {
    return suit == other.suit;
  }

  boolean isQueenOfSpades() {
    return (suit == Suit.S) && (val == 11);
  }

  public String valString() {
    switch (val) {
      case 11 : return "J";
      case 12 : return "Q";
      case 13 : return "K";
      case 14 : return "A";
    }
    return "" + val;
  }

  public String toString() {
    return valString() + suit;
  }

  public static List<Card> newDeck() {
    List<Card> deck = new ArrayList<Card>();
    for (int i=2; i<=14; i++) {
      deck.add(new Card(Suit.H, i));
      deck.add(new Card(Suit.D, i));
      deck.add(new Card(Suit.S, i));
      deck.add(new Card(Suit.C, i));
    }
    return deck;
  }
  
  public static void shuffle(List<Card> cards) {
	Random rand = new Random();
	for (int i = cards.size(); i > 0; i --) {
	  int n = rand.nextInt(i);
	  Card card = cards.remove(n);
	  cards.add(card);
	}
  }
}
