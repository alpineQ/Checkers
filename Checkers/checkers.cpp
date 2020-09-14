#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>
#include <algorithm>
#include "checkers.hpp"
using namespace std;

/*
  00  01  02  03
10  11  12  13
  20  21  22  23
30  31  32  33
  40  41  42  43
50  51  52  53
  60  61  62  63
70  71  72  73
*/
set<list<Position>> diagonals = 
{
	{{0,2}, {1,3}, {2,3}},
	{{0,1}, {1,2}, {2,2}, {3,3}, {4,3}},
	{{0,0}, {1,1}, {2,1}, {3,2}, {4,2}, {5,3}, {6,3}},
	{{1,0}, {2,0}, {3,1}, {4,1}, {5,2}, {6,2}, {7,3}},
    {{3,0}, {4,0}, {5,1}, {6,1}, {7,2}},
	{{5,0}, {6,0}, {7,1}},

	{{0,0}, {1,0}},
	{{0,1}, {1,1}, {2,0}, {3,0}},
	{{0,2}, {1,2}, {2,1}, {3,1}, {4,0}, {5,0}},
	{{0,3}, {1,3}, {2,2}, {3,2}, {4,1}, {5,1}, {6,0}, {7,0}},
	{{2,3}, {3,3}, {4,2}, {5,2}, {6,1}, {7,1}},
	{{4,3}, {5,3}, {6,2}, {7,2}},
	{{6,3}, {7,3}}
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

bool operator==(const Position &left, const Position &right) {
	return left.i == right.i && left.j == right.j;
}

bool operator<(const Position &left, const Position &right) {
	if (left.i != right.i)
		return left.i < right.i;
	return left.j < right.j;
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

BoardSituation moveChecker(BoardSituation situation, Position from, Position to) {
	situation.board[to.i][to.j] = situation.board[from.i][from.j];
	if (to.i == 7 && situation.blackTurn || to.i == 0 && !situation.blackTurn)
		situation.board[to.i][to.j].is_king = true;
	situation.board[from.i][from.j] = Tile{ true, false, false };
	return situation;
}

set<BoardSituation> checkCanMove(BoardSituation situation, char i, char j) {
	set<BoardSituation> possibleMoves;
	if (situation.board[i][j].is_king) {
		for (auto diagonal : diagonals) {
			auto pos = find(diagonal.begin(), diagonal.end(), Position{ i, j });
			if (pos == diagonal.end())
				continue;

			auto forwardPosition = next(pos);
			if (forwardPosition != diagonal.end() && situation.board[forwardPosition->i][forwardPosition->j].is_empty) {
				BoardSituation forwardSituation = moveChecker(situation, *pos, *forwardPosition);
				forwardSituation.blackTurn = !situation.blackTurn;
				possibleMoves.insert(forwardSituation);
			}

			if (pos == diagonal.begin())
				continue;
			auto backwardPosition = prev(pos);
			if (situation.board[backwardPosition->i][backwardPosition->j].is_empty) {
				BoardSituation backwardSituation = moveChecker(situation, *pos, *backwardPosition);
				backwardSituation.blackTurn = !situation.blackTurn;
				possibleMoves.insert(backwardSituation);
			}
		}
		return possibleMoves;
	}
	char direction = situation.blackTurn ? 1 : -1;
	for (auto diagonal : diagonals) {
		auto pos = find(diagonal.begin(), diagonal.end(), Position{ i, j });
		if (pos == diagonal.end() || direction == -1 && pos == diagonal.begin() || direction == 1 && next(pos) == diagonal.end())
			continue;
		auto newPosition = situation.blackTurn ? next(pos) : prev(pos);
		if (!situation.board[newPosition->i][newPosition->j].is_empty)
			continue;
		BoardSituation newSituation = moveChecker(situation, *pos, *newPosition);
		newSituation.blackTurn = !situation.blackTurn;
		possibleMoves.insert(newSituation);
	}
	return possibleMoves;
}

set<BoardSituation> checkCanEat(BoardSituation situation, char i, char j) {
	set<BoardSituation> possibleMoves;
	for (auto diagonal : diagonals) {
		auto pos = find(diagonal.begin(), diagonal.end(), Position{ i, j });
		if (pos == diagonal.end())
			continue;

		if (next(pos) != diagonal.end() && next(next(pos)) != diagonal.end() && !situation.board[next(pos)->i][next(pos)->j].is_empty &&
				situation.board[next(next(pos))->i][next(next(pos))->j].is_empty && situation.board[next(pos)->i][next(pos)->j].is_black != situation.blackTurn) {
			auto eatenPosition = next(pos);
			auto newPosition = next(eatenPosition);
			BoardSituation forwardSituation = moveChecker(situation, *pos, *newPosition);
			forwardSituation.board[eatenPosition->i][eatenPosition->j].is_empty = true;

			auto eatMoves = checkCanEat(forwardSituation, newPosition->i, newPosition->j);
			possibleMoves.insert(eatMoves.begin(), eatMoves.end());
			forwardSituation.blackTurn = !situation.blackTurn;
			possibleMoves.insert(forwardSituation);
		}

		if (pos != diagonal.begin() && prev(pos) != diagonal.begin() && !situation.board[prev(pos)->i][prev(pos)->j].is_empty &&
			situation.board[prev(prev(pos))->i][prev(prev(pos))->j].is_empty && situation.board[prev(pos)->i][prev(pos)->j].is_black != situation.blackTurn) {
			auto eatenPosition = prev(pos);
			auto newPosition = prev(eatenPosition);
			BoardSituation backwardSituation = moveChecker(situation, *pos, *newPosition);
			backwardSituation.board[eatenPosition->i][eatenPosition->j].is_empty = true;

			auto eatMoves = checkCanEat(backwardSituation, newPosition->i, newPosition->j);
			possibleMoves.insert(eatMoves.begin(), eatMoves.end());
			backwardSituation.blackTurn = !situation.blackTurn;
			possibleMoves.insert(backwardSituation);
		}
	}
	return possibleMoves;
}

set<BoardSituation> getPossibleMoves(BoardSituation situation) {
	set<BoardSituation> possibleMoves;
	// If can eat
	for (char i = 0; i < 8; ++i) {
		for (char j = 0; j < 4; ++j) {
			if (situation.board[i][j].is_empty || situation.board[i][j].is_black != situation.blackTurn)
				continue;
			set<BoardSituation> eatMoves = checkCanEat(situation, i, j);
			possibleMoves.insert(eatMoves.begin(), eatMoves.end());
		}
	}
	if (!possibleMoves.empty())
		return possibleMoves;
	// If can move
	for (char i = 0; i < 8; ++i) {
		char direction = situation.blackTurn ? 1 : -1;
		for (char j = 0; j < 4; ++j) {
			if (situation.board[i][j].is_empty || situation.board[i][j].is_black != situation.blackTurn || !situation.board[i][j].is_empty && !situation.board[i][j].is_king && (i + direction < 0 || i + direction >= 8))
				continue;
			set<BoardSituation> movePositions = checkCanMove(situation, i, j);
			possibleMoves.insert(movePositions.begin(), movePositions.end());
		}
	}
	return possibleMoves;
}

void drawBoard(BoardSituation situation) {
	cout << "  00112233" << endl;
    for (char i = 0; i < 8; ++i) {
		cout << int(i) << ' ';
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

void uploadToFile(std::string filename, set<BoardSituation> situations) {
	ofstream outputFile(filename);
	for (BoardSituation situation : situations)
		outputFile << situation.toString() << endl;
	outputFile.close();

}
BoardSituation loadFromFile(std::string filename) {
	BoardSituation situation;
	ifstream inputFile(filename);
	char letter;
	for (char i = 0; i < 8; ++i) {
		for (char j = 0; j < 4; ++j) {
			inputFile >> letter;
			switch (letter) {
			case 'e': situation.board[i][j] = Tile{ true, false, false }; break;
			case 'b': situation.board[i][j] = Tile{ false, true, false }; break;
			case 'w': situation.board[i][j] = Tile{ false, false, false }; break;
			case 'B': situation.board[i][j] = Tile{ false, true, true }; break;
			case 'W': situation.board[i][j] = Tile{ false, false, true }; break;
			default: break;
			}
		}
	}
	inputFile >> letter;
	inputFile >> letter;
	situation.blackTurn = (letter == 'b');
	return situation;
}
