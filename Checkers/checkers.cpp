#include <iostream>
#include <string>
#include <set>
#include "checkers.hpp"
using namespace std;

std::list<Position> diagonals[] = 
{
	{{0,0}, {1,1}, {2,1}, {3,2}, {4,2}, {5,3}, {6,3}},
	{{1,0}, {2,0}, {3,1}, {4,1}, {5,2}, {6,2}, {7,3}},
    {{3,0}, {4,0}, {5,1}, {6,1}, {7,2}},
	{{0,1}, {1,2}, {2,2}, {3,3}, {4,3}},
	{{5,0}, {6,0}, {7,1}},
	{{0,2}, {1,3}, {2,3}}
};

BoardSituation getDefaultBoard() {
    BoardSituation situation;
    for (char i = 0; i < 8; ++i) {
        for (char j = 0; j < 4; ++j) {
	    if (i < 3)
                situation.board[i][j] = Tile{false, true, false};
	    else if (i < 5)
                situation.board[i][j] = Tile{true, false, false};
	    else
                situation.board[i][j] = Tile{false, false, false};
	}
    }
    situation.blackTurn = false;
    return situation;
}

bool operator<(const Tile &left, const Tile &right) {
    if (left.is_empty != right.is_empty)
        return left.is_empty < right.is_empty;
    if (left.is_black != right.is_black)
        return left.is_black < right.is_black;
    return left.is_king < right.is_king;
}

bool operator!=(const Tile & left, const Tile & right)
{
	return left.is_black != right.is_black || left.is_empty != right.is_empty || left.is_king != right.is_king;
}

bool operator<(const BoardSituation &left, const BoardSituation &right) {
    if (left.blackTurn != right.blackTurn)
        return left.blackTurn < right.blackTurn;
    for (char i = 0; i < 8; ++i) {
        for (char j = 0; j < 4; ++j) {
			if (left.board[i][j] != right.board[i][j])
				return left.board[i][j] < right.board[i][j];
		}
    }
    return false;
}

string BoardSituation::toString() {
    string result;
    for (char i = 0; i < 8; ++i) {
        for (char j = 0; j < 4; ++j) {
			Tile tile = this->board[i][j];
			if (tile.is_empty)
				result += 'e';
			else if (!tile.is_king)
				result += tile.is_black?'b':'w';
			else
				result += tile.is_black?'B':'W';
            result += ' ';
		}
    }
    result[result.size()-1] = ',';
    result += ' ';
    result += this->blackTurn?'b':'w';
    return result;
}

set<BoardSituation> getPossibleMoves(BoardSituation situation) {
	for (char i = 0; i < 8; ++i) {
		char direction = (situation.blackTurn) ? 1 : -1;
		for (char j = 0; j < 4; ++j) {
			if (situation.board[i][j].is_empty || !situation.board[i][j].is_empty && !situation.board[i][j].is_king && (i + direction < 0 || i + direction >= 8))
				continue;
			// Walk by diagonals
		}
	}
}

void drawBoard(BoardSituation situation) {
    for (char i = 0; i < 8; ++i) {
		if (i % 2 == 0)
			cout << ' ';
        for (char j = 0; j < 4; ++j) {
			Tile tile = situation.board[i][j];
			if (tile.is_empty)
				cout << ' ';
			else if (!tile.is_king)
				cout << (tile.is_black?'b':'w');
			else
				cout << (tile.is_black?'B':'W');
            cout << ' ';
		}
		cout << endl;
    }
}
