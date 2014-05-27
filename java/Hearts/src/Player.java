import java.util.ArrayList;
import java.util.List;

public class Player {
  List<Card> hand;
  String name;
  int score;
  Game game;
  
  public Player (String n, Game g) {
    name = n;
    hand = new ArrayList<Card>();
    score = 0;
    game = g;
  }
  
  public boolean hasStarterCard() {
	for (Card card: hand) {
      if (card.isStarter()) {
    	return true;
      }
    }
    return false;
  }
  
  public Card lead(Trick t, boolean firstRound) throws RuntimeException {
	List<Card> legalCards = new ArrayList<Card>();

    if (firstRound) {
      for (Card card: hand) {
	    if (card.isStarter()) {
	      hand.remove(card);
	      return card;
	    }
	  }
	  throw new RuntimeException("Player started without starter card.");
	} else if (!game.trumpBroken) {
	  // Collect a subset of non-trump
	  for (Card card: hand) {
	    if (!card.isTrump()) {
          legalCards.add(card);
		}
	  }		
	}
	
	if (legalCards.size() == 0) {
	  legalCards = hand;
	}

	Card card = determineBestCard(t, legalCards);
	hand.remove(card);
	return card;
  }
  
  // Returns the best card given a subset of cards that are legal
  Card determineBestCard(Trick t, List<Card> cards) {
	// Stupid case
    return cards.get(0);
  }
  
  public Card follow(Trick t) {
	List<Card> legalCards = new ArrayList<Card>();
	
	for (Card card: hand) {
	  if (card.sameSuit(t.led())) {
	    legalCards.add(card);
	  }
	}

	if (legalCards.size() == 0) {
	  // Could not follow suit, so we can play any card in the hand
	  legalCards = hand;
	}

	Card card = determineBestCard(t, legalCards);
	hand.remove(card);
	return card;
  }
  
  public void receiveNewCard(Card c) {
    hand.add(c);
  }
  
  public String toString() {
	String out = getClass().getName() + " " + name + " : ";
	for (Card card: hand) {
      out += card + " ";
    }
    return out + " Score: " + score;
  }
}
