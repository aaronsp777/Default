import java.util.ArrayList;
import java.util.List;


public class SmartPlayer extends Player {

  public SmartPlayer(String n, Game g) {
	super(n, g);
  }

  public List<Card> above(List<Card> cards, Card card) {
	List<Card> matched = new ArrayList<Card>();
	for (Card c: cards) {
	  if (c.isGreater(card)) {
	    matched.add(c);
	  }
	}
    return matched;
  }

  public List<Card> below(List<Card> cards, Card card) {
	List<Card> matched = new ArrayList<Card>();
	for (Card c: cards) {
	  if (card.isGreater(c)) {
	    matched.add(c);
	  }
	}
    return matched;
  }
 
  public List<Card> matching(List<Card> cards, Card card) {
	List<Card> matched = new ArrayList<Card>();
	for (Card c: cards) {
	  if (card.sameSuit(c)) {
	    matched.add(c);
	  }
	}
    return matched;
  }
  
  public Card lowest(List<Card> cards) {
    Card low = cards.get(0);
    for (Card card: cards) {
      if (low.isGreater(card)) {
        low = card;
      }
    }
    return low;
  }
  
  public Card highest(List<Card> cards) {
    Card high = cards.get(0);
    for (Card card: cards) {
      if (card.isGreater(high)) {
        high = card;
      }
    }
    return high;
  }


  Card determineBestCard(Trick t, List<Card> cards) {
	// If we are leading, play the lowest card we have (no cards played yet)
	if (t.played.size() == 0) {
	  return lowest(cards);
	}
	// If we following suit, play the highest card under the lead
	if (cards.get(0).sameSuit(t.led())) {
	  Card highestInSuit = highest(matching(t.played, t.led()));
	  List<Card> below = below(cards, highestInSuit);
	  if (below.size() > 0) {
	    return highest(below);
	  }
	  
	  // TODO: consider if there are more players that are likely to go over as well
	 //  return highest(cards);
	 return lowest(cards);
	}
	// If we are throwing off, play the highest card
	return highest(cards);
  }
}
