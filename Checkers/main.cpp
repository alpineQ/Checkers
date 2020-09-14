#include <iostream>
#include <time.h>
#include "checkers.hpp"
using namespace std;

void game() {
	srand((unsigned)time(NULL));
	BoardSituation situation = getDefaultBoard();
	auto moves = getPossibleMoves(situation);
	list<BoardSituation> gameTurns = { situation };
	while (!moves.empty() && gameTurns.size() < 120) {
		auto it = moves.begin();
		advance(it, rand() % moves.size());
		situation = *it;
		gameTurns.push_back(situation);
		moves = getPossibleMoves(situation);
	}
	char ans = '?';
	auto turn = gameTurns.begin();
	while (ans != 'q' && turn != gameTurns.end()) {
		drawBoard(*turn);
		ans = getchar();
		system("cls");
		switch (ans) {
		case 'n': turn++; break;
		case 'b': turn--; break;
		case 'c': uploadToFile("inhere.txt", {*turn}); break;
		default: break;
		}
	}
}

void test() {
	BoardSituation situation = loadFromFile("inhere.txt");
	auto moves = getPossibleMoves(situation);
	for (auto newSituation : moves) {
		drawBoard(situation);
		cout << moves.size() << (situation.blackTurn?" Black":" White") << endl;
		drawBoard(newSituation);
		getchar();
		system("cls");
	}

}

int main() {
	game();
	return 0;
}