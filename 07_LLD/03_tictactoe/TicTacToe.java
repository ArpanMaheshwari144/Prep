import java.util.*;

// --- Player: name, symbol (X/O) ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Player sirf apna naam aur symbol rakhta, koi game logic nahi
class Player {
    String name;
    String symbol;

    Player(String name, String symbol) {
        this.name = name;
        this.symbol = symbol;
    }

    String getName() {
        return name;
    }

    void setName(String name) {
        this.name = name;
    }

    String getSymbol() {
        return symbol;
    }

    void setSymbol(String symbol) {
        this.symbol = symbol;
    }
}

// --- Board: 3x3 grid ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Board sirf grid state manage karta (place, checkWin, isFull, print)
class Board {
    String[][] grid = new String[3][3];

    Board() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grid[i][j] = " ";
            }
        }
    }

    void placeMove(int row, int col, String symbol) {
        grid[row][col] = symbol;
    }

    boolean checkWin(String symbol) {
        // row check
        for (int i = 0; i < 3; i++) {
            if (grid[i][0].equals(symbol) && grid[i][1].equals(symbol) && grid[i][2].equals(symbol)) {
                return true;
            }
        }

        for (int j = 0; j < 3; j++) {
            if (grid[0][j].equals(symbol) && grid[1][j].equals(symbol) && grid[2][j].equals(symbol)) {
                return true;
            }
        }

        if (grid[0][0].equals(symbol) && grid[1][1].equals(symbol) && grid[2][2].equals(symbol)) {
            return true;
        }
        if (grid[0][2].equals(symbol) && grid[1][1].equals(symbol) && grid[2][0].equals(symbol)) {
            return true;
        }

        return false;
    }

    boolean isFull() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[i][j].equals(" ")) {
                    return false;
                }
            }
        }
        return true;
    }

    void printBoard() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                System.out.print(grid[i][j]);
            }
            System.out.println();
        }
    }
}

// --- Game: 2 players, board, turn manage ---
// SRP = har class ka ek hi kaam
// Yahan kyu: Game sirf turn-flow orchestrate karta, actual win check Board karta
//
// Composition = "has-a" relationship
// Yahan kyu: Game HAS-A Board aur 2 Players — ownership clear
//
// Facade Pattern = complex internals ko simple API ke piche chhupa
// Yahan kyu: Client sirf playGame() call kare, Board/Player coordination hide
class Game {
    Player player1;
    Player player2;
    Board board;

    Game(Player player1, Player player2) {
        this.player1 = player1;
        this.player2 = player2;
        this.board = new Board();
    }

    void playGame() {
        Player current = player1;
        // simulate moves
        int[][] moves = { { 0, 0 }, { 1, 1 }, { 0, 1 }, { 2, 2 }, { 0, 2 } }; // X wins row 0

        for (int[] move : moves) {
            board.placeMove(move[0], move[1], current.getSymbol());
            board.printBoard();
            System.out.println();

            if (board.checkWin(current.getSymbol())) {
                System.out.println(current.getName() + " WINS!");
                return;
            }
            if (board.isFull()) {
                System.out.println("DRAW!");
                return;
            }
            current = (current == player1) ? player2 : player1;
        }
    }
}

class Main {
    public static void main(String[] args) {
        Player p1 = new Player("Arpan", "X");
        Player p2 = new Player("Claude", "O");
        Game game = new Game(p1, p2);
        game.playGame();
    }
}
