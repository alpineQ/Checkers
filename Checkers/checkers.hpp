#include <string>
#include <set>
#include <list>


struct Tile {
    bool is_empty;
    bool is_black;
    bool is_king;
    friend bool operator<(const Tile &left, const Tile &right);
    friend bool operator!=(const Tile &left, const Tile &right);
};

struct Position {
	char i;
	char j;
};

struct BoardSituation {
    Tile board[8][4];
    bool blackTurn;
    friend bool operator<(const BoardSituation &left, const BoardSituation &right);
	std::string toString();
};

void drawBoard(BoardSituation situation);
BoardSituation getDefaultBoard();
