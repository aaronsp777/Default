import java.util.ArrayList;
import java.util.List;


public class Trick {
  List<Card> played;
  
  public Trick () {
    played = new ArrayList<Card>();
  }

  public boolean first() {
	return played.size() == 0;
  }
  
  public Card led() {
	return played.get(0);
  }
  
  public boolean canPlayerFollowSuit(Player p) {
	for (Card card: p.hand) {
	  if (card.sameSuit(led())) {
		return true;
	  }
	}
	return false;
  }
  
  // Given a list of players, plays them in order
  // Returns the person who took the trick
  public Player playTrick(List<Player> players, boolean firstRound) throws RuntimeException {
	Player trickTaker = players.get(0);
	Card highestCard = trickTaker.lead(this, firstRound);
	played.add(highestCard);
	
    for (int p = 1; p < players.size(); p++) {
      Player player = players.get(p);
      boolean playerCouldFollowSuit = canPlayerFollowSuit(player);
      Card card = player.follow(this);

      if (card.isGreater(highestCard) &&
    	  card.sameSuit(highestCard)) {
        trickTaker = player;
        highestCard = card;
      }
      
      played.add(card);
      if (card.isTrump()) {
        player.game.trumpBroken = true;
      }
      
      if (playerCouldFollowSuit && !card.sameSuit(led())) {
        throw new RuntimeException(
            "Player " + player.name + " did not follow suit: " + toString());
      }
    }
    trickTaker.score += score();
    return trickTaker;
  }

  private int score() {
	int score = 0;
	for (Card card: played) {
	  if (card.isTrump()) {
	    score++;
	  }
	  if (card.isQueenOfSpades()) {
		score+=13;
	  }
	}
    return score;
  }
  
  public String toString() {
	String out = "Trick:";
	for (Card card: played) {
      out += " " + card;
    };
    return out;
  }
}
