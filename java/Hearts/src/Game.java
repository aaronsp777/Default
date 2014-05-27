import java.util.ArrayList;
import java.util.List;

public class Game {
  List<Player> players;
  boolean trumpBroken;
  
  public Game () {
	trumpBroken = false;
	players = new ArrayList<Player>();
    players.add(new SmartPlayer("One", this));
    players.add(new SmartPlayer("Two", this));
    players.add(new SmartPlayer("Three", this));
    players.add(new Player("Four", this));
  }
	
  void deal() {
    List<Card> deck = Card.newDeck();
    Card.shuffle(deck);
    int nextPlayer = 0;
    for (Card card: deck) {
      players.get(nextPlayer).receiveNewCard(card);
      nextPlayer++;
      if (nextPlayer >= players.size()) {
    	nextPlayer = 0;
      }
    }
  }

  public void dumpPlayers() {
	for (Player player: players) {
	  System.out.println(player);
	}  
  }
  
  public void start() {
	Player nextLeader = players.get(0);
	for (Player player: players) {
      if (player.hasStarterCard()) {
	    nextLeader = player;
	  }
	}
	boolean firstRound = true;
	while (nextLeader.hand.size() > 0) {
	  // Reorder players so that next player is leader
	  while (nextLeader != players.get(0)) {
	    players.add(players.remove(0));
	  }
	  System.out.println("");
	  dumpPlayers();
	  Trick t = new Trick();
	  nextLeader = t.playTrick(players, firstRound);
	  firstRound = false;
		
	  System.out.println(t + " Taken by " + nextLeader.name);
	}
  }
  
  public static void main(String[] args) {
	Game game = new Game();
	game.deal();
    game.start();
  }

}
