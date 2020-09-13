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

std::list<Position> diagonals[] {
	{{0,0}, {1,1}, {2,1}, {3,2}, {4,2}, {5,3}, {6,3}},
	{{1,0}, {2,0}, {3,1}, {4,1}, {5,2}, {6,2}, {7,3}},      {{3,0}, {4,0}, {5,1}, {6,1}, {7,2}},
	{{0,1}, {1,2}, {2,2}, {3,3}, {4,3}},
	{{5,0}, {6,0}, {7,1}},
	{{0,2}, {1,3}, {2,3}}
};

struct BoardSituation {
    Tile board[8][4];
    bool blackTurn;
    friend bool operator<(const BoardSituation &left, const BoardSituation &right);
	std::string toString();
};

void drawBoard(BoardSituation situation);
BoardSituation getDefaultBoard();
