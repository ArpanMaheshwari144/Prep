import java.util.*;

// --- Player: name, position ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Player sirf apna naam + current position track karta
class Player{
    String name;
    int position;

    Player(String name, int position) {
        this.name = name;
        this.position = position;
    }

    String getName() {
        return name;
    }

    void setName(String name) {
        this.name = name;
    }

    int getPosition() {
        return position;
    }

    void setPosition(int position) {
        this.position = position;
    }
}


// --- Snake: head (upar), tail (neeche) ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Snake sirf head/tail positions rakhta — pure data
class Snake{
    int head;
    int tail;

    Snake(int head, int tail) {
        this.head = head;
        this.tail = tail;
    }

    int getHead() {
        return head;
    }

    void setHead(int head) {
        this.head = head;
    }

    int getTail() {
        return tail;
    }

    void setTail(int tail) {
        this.tail = tail;
    }
}


// --- Ladder: start (neeche), end (upar) ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Ladder sirf start/end positions rakhta — pure data
class Ladder{
    int start;
    int end;

    Ladder(int start, int end) {
        this.start = start;
        this.end = end;
    }

    int getStart() {
        return start;
    }

    void setStart(int start) {
        this.start = start;
    }

    int getEnd() {
        return end;
    }

    void setEnd(int end) {
        this.end = end;
    }
}


// --- Dice: roll() → 1-6 ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Dice sirf roll karta, random number return
class Dice{
    int roll() {
        return (int)(Math.random() * 6) + 1;
    }
}


// --- Board: size, snakes, ladders ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Board sirf position mapping karta (snake head → tail, ladder start → end)
class Board{
    int size;
    List<Snake> snakes;
    List<Ladder> ladders;

    Board(int size, List<Snake> snakes, List<Ladder> ladders) {
        this.size = size;
        this.snakes = snakes;
        this.ladders = ladders;
    }

    int getSize() {
        return size;
    }

    void setSize(int size) {
        this.size = size;
    }

    List<Snake> getSnakes() {
        return snakes;
    }

    void setSnakes(List<Snake> snakes) {
        this.snakes = snakes;
    }

    List<Ladder> getLadders() {
        return ladders;
    }

    void setLadders(List<Ladder> ladders) {
        this.ladders = ladders;
    }

    int getNewPosition(int position){
        for(Snake it : snakes){
            if(it.head == position){
                return it.tail;
            }
        }

        for(Ladder it : ladders){
            if(it.start == position){
                return it.end;
            }
        }
        return position;
    }
    
}


// --- Game: players, board, dice, playGame() ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Game sirf turn-flow orchestrate karta — move decide Board/Dice se aata
//
// Composition = "has-a" relationship (strong ownership)
// Yahan kyu: Game HAS-A Players, Board, Dice — in ke bina Game exist nahi
//
// Facade Pattern = complex subsystem simple API ke piche
// Yahan kyu: Client sirf playGame() call kare, dice roll + board lookup + player update hidden
class Game{
    List<Player> players;
    Board board;
    Dice dice;

    Game(List<Player> players, Board board, Dice dice) {
        this.players = players;
        this.board = board;
        this.dice = dice;
    }

    List<Player> getPlayers() {
        return players;
    }

    void setPlayers(List<Player> players) {
        this.players = players;
    }

    Board getBoard() {
        return board;
    }

    void setBoard(Board board) {
        this.board = board;
    }

    Dice getDice() {
        return dice;
    }

    void setDice(Dice dice) {
        this.dice = dice;
    }

    void playGame() {
        int turn = 0;
        while (true) {
            Player current = players.get(turn % players.size());
            int roll = dice.roll();
            int newPos = current.getPosition() + roll;

            if (newPos > board.getSize()) {
                System.out.println(current.getName() + " rolled " + roll + " — can't move (over 100)");
                turn++;
                continue;
            }

            newPos = board.getNewPosition(newPos);
            current.setPosition(newPos);
            System.out.println(current.getName() + " rolled " + roll + " → position " + newPos);

            if (newPos == board.getSize()) {
                System.out.println(current.getName() + " WINS!");
                break;
            }
            turn++;
        }
    }
}

class Main {
    public static void main(String[] args) {
        List<Snake> snakes = new ArrayList<>();
        snakes.add(new Snake(36, 6));
        snakes.add(new Snake(62, 19));
        snakes.add(new Snake(88, 24));

        List<Ladder> ladders = new ArrayList<>();
        ladders.add(new Ladder(3, 22));
        ladders.add(new Ladder(17, 74));
        ladders.add(new Ladder(50, 91));

        Board board = new Board(100, snakes, ladders);
        Dice dice = new Dice();

        List<Player> players = new ArrayList<>();
        players.add(new Player("Arpan", 0));
        players.add(new Player("Claude", 0));

        Game game = new Game(players, board, dice);
        game.playGame();
    }
}