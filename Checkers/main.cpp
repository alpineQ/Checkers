#include <iostream>
#include "checkers.hpp"
using namespace std;

int main() {
	BoardSituation situation = getDefaultBoard();
	drawBoard(situation);
	cout << situation.toString() << endl;
	return 0;
}