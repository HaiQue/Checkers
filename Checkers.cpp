#include <iostream>
#include <string>
#include "CheckerBoard.h"
#include "Game.h"

using namespace std;

int main();

//---------------------------------------------------------------------------------
// Pranesimas 
void CheckerBoard::displayTitle(void) {
	cout << endl << "\t\t\t\t      Saskes" << endl;
}

//---------------------------------------------------------------------------------
// Pradine lenta apibrezimas
void CheckerBoard::initBoard(void) {
	m_rPieces = 0;
	m_bPieces = 0;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if ((row + col) % 2 != 0 && row >= 5) {
				m_board[row][col] = "r";
				m_rPieces++;
				if (row == 3 || row == 4) {
					m_board[row][col] = "";
				}
			}
			else if ((row + col) % 2 != 0 && row <= 2) {
				m_board[row][col] = "b";
				m_bPieces++;
				if (row >= 3) {
					m_board[row][col] = "";
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
// Parodome lenta su zymejimas stulpeliu ir eiluciu
void CheckerBoard::displayBoard(void) {
	system("cls");
	displayTitle();
	cout << "\n";
	displayScore();
	cout << "\n\n";

	cout << "\t\t     0     1     2     3     4     5     6     7" << endl;

	for (int row = 0; row < 8; row++) {
		cout << "\t\t  -------------------------------------------------" << endl; 
		cout << "\t\t" << row << " |";
		for (int col = 0; col < 8; col++) {
			if (m_board[row][col].empty()) {
				cout << "  " << "   |";
			}
			else {
				cout << "  " << m_board[row][col] << "  |";
			}

		}
		cout << endl;
	}
	cout << "\t\t  -------------------------------------------------" << endl;
	
}

//---------------------------------------------------------------------------------
// Parodome rezultata (kiek liko lentoje gyvu figuru)
void CheckerBoard::displayScore() {
	cout << "\n";
	cout << "\t\t\tPlayer r: " << m_rPieces;
	cout << "\t\tPlayer b: " << m_bPieces;
}

//---------------------------------------------------------------------------------
// Atnaujiname rezultata
void CheckerBoard::updateScore(string& player, int jumpOption) {
	if (jumpOption <= 4) {
		if (player == "r" || player == "R") {
			m_bPieces--;
		}
		else if (player == "b" || player == "B") {
			m_rPieces--;
		}
	}
	else if (jumpOption > 4 && jumpOption <= 8) {
		if (player == "r" || player == "R") {
			m_bPieces -= 2;
		}
		else if (player == "b" || player == "B") {
			m_rPieces -= 2;
		}
	}
}

//---------------------------------------------------------------------------------
// Graziname stringa viduje specifinio kvadrato 
string& CheckerBoard::getSquare(int row, int col) {
	return m_board[row][col];
}

//---------------------------------------------------------------------------------
// Atnaujiname lenta (ivykus veiksmui)
void CheckerBoard::updateBoard(int row, int col, string& square) {
	m_board[row][col] = square;
	checkForKing();
}

//---------------------------------------------------------------------------------
// Isvalome lentos kvadrato vieta
void CheckerBoard::clearSquare(int row, int col) {
	m_board[row][col] = "";
}

//---------------------------------------------------------------------------------
// Patikriname ar nera jokiu karaliu kai lenta yra atnaujinama
void CheckerBoard::checkForKing(void) {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (row == 0) {
				if (m_board[row][col] == "r") {
					m_board[row][col] = "R";
				}
			}
			else if (row == 7) {
				if (m_board[row][col] == "b") {
					m_board[row][col] = "B";
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
// Patikriname ar nera zaidimo laimetojo
bool CheckerBoard::checkWinner(void) {
	if (m_rPieces == 0 || m_bPieces == 0) {
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------------
// Graziname kiek liko "r" tipo figuru 
const int CheckerBoard::getRPieces(void) {
	return m_rPieces;
}

//---------------------------------------------------------------------------------
// Graziname kiek liko "b" tipo figuru 
const int CheckerBoard::getBPieces(void) {
	return m_bPieces;
}

//---------------------------------------------------------------------------------
// Parodome zaidima 
void Game::displayGame(void) {
	m_checkerBoard.initBoard();
	m_checkerBoard.displayBoard();
	m_player = "r";
	gameLoop(m_checkerBoard);
}

//---------------------------------------------------------------------------------
// Pagrindinis zaidimo ciklas kur vyksta visas veiksmas
void Game::gameLoop(CheckerBoard& checkerBoard) {
	m_winner = checkerBoard.checkWinner();
	while (!m_winner) {
		cout << "\n\tTurn: " << m_player << "\n\n";
		cout << "\t" << m_player << " Iveskite zaidimo figuros kuria noretumet pasirinkti stulpeli ir eilute: ";
		cin >> m_row;
		cin >> m_col;

		string square = checkerBoard.getSquare(m_row, m_col);
		if (square == m_player) {
			cout << "\n\t Kuriame stulpeli ir eiluteje noretumet perkelti sia figura: ";
			cin >> m_rowMove;
			cin >> m_colMove;
			if (m_player == "b") {
				if ((m_col - m_colMove == 1 || m_col - m_colMove == -1) && m_row - m_rowMove == -1) {
					string squareMove;
					squareMove = checkerBoard.getSquare(m_rowMove, m_colMove);
					if (squareMove.empty()) {
						checkerBoard.clearSquare(m_row, m_col);
						checkerBoard.updateBoard(m_rowMove, m_colMove, m_player);
						switchPlayer();
						checkerBoard.displayBoard();
					}
					else {
						if (squareMove == m_player) {
							cout << "\tNegalimas veiksmas. Pasirinktas kvadratas nera tuscias. Pasirinkite kita vieta." << endl;
							gameLoop(checkerBoard);
						}
					}
				}
				// viengubo jump veiksmas 
				else if ((m_col - m_colMove == 2 || m_col - m_colMove == -2) && m_row - m_rowMove == -2) {
					bool jumpMove = checkForJump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
					if (jumpMove) {
						jump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
						switchPlayer();
					}
				}
				// dvigubo jump veiksmas
				else if ((m_col - m_colMove == 4 || m_col - m_colMove == -4) && m_row - m_rowMove == -4) {
					bool jumpMove = checkForJump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
					if (jumpMove) {
						jump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
						switchPlayer();
					}
				}
				else {
					cout << "\tNegalimas veiksmas. Negalima perkelti figura i ta vieta. Bandykit dar karta." << endl;
				}
			}
			else if (m_player == "r") {
				if ((m_col - m_colMove == 1 || m_col - m_colMove == -1) && m_row - m_rowMove == 1) {
					string squareMove;
					squareMove = checkerBoard.getSquare(m_rowMove, m_colMove);
					if (squareMove.empty()) {
						checkerBoard.clearSquare(m_row, m_col);
						checkerBoard.updateBoard(m_rowMove, m_colMove, m_player);
						switchPlayer();
						checkerBoard.displayBoard();
					}
					else {
						if (squareMove == m_player) {
							cout << "\tNegalimas veiksmas. Negalima perkelti figura i ta vieta. Bandykit dar karta." << endl;
							gameLoop(checkerBoard);
						}
					}
				}
				// Viengubo jump veiksmas
				else if ((m_col - m_colMove == 2 || m_col - m_colMove == -2) && m_row - m_rowMove == 2) {
					bool jumpMove = checkForJump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
					if (jumpMove) {
						jump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
						switchPlayer();
					}
				}
				// Dvigubo jump veiksmas
				else if ((m_col - m_colMove == 4 || m_col - m_colMove == -4) && m_row - m_rowMove == 4) {
					bool jumpMove = checkForJump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
					if (jumpMove) {
						jump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
						switchPlayer();
					}
				}
				else {
					cout << "\tNegalimas veiksmas. Negalima perkelti figura i ta vieta. Bandykit dar karta." << endl;
				}
			}
		}
		// Patikrinti ar nera karaliaus figuru 
		else if ((m_player == "r" && square == "R") || (m_player == "b" && square == "B")) {
			cout << "\nKuri stulpeli ir eilute noretum sia figura patraukti: ";
			cin >> m_rowMove;
			cin >> m_colMove;
			if ((m_col - m_colMove == 1 || m_col - m_colMove == -1) && (m_row - m_rowMove == 1 || m_row - m_rowMove == -1)) {
				string squareMove;
				squareMove = checkerBoard.getSquare(m_rowMove, m_colMove);
				if (squareMove.empty()) {
					checkerBoard.clearSquare(m_row, m_col);
					checkerBoard.updateBoard(m_rowMove, m_colMove, square);
					switchPlayer();
					checkerBoard.displayBoard();
				}
				else {
					if (squareMove == m_player) {
						cout << "\tNegalimas veiksmas. Pasirinkimo vieta nera tuscia. Pasirinkite kita vieta." << endl;
						gameLoop(checkerBoard);
					}
				}
			}
			// Patikrinti del viengubo jump veiksmo
			else if ((m_col - m_colMove == 2 || m_col - m_colMove == -2) && (m_row - m_rowMove == 2 || m_row - m_rowMove == -2)) {
				bool jumpMove = checkForJump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
				if (jumpMove) {
					jump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
					switchPlayer();
				}
			}
			// Patikrinti del dvigubo jump veiksmo
			else if ((m_col - m_colMove == 4 || m_col - m_colMove == -4) && (m_row - m_rowMove == 4 || m_row - m_rowMove == -4)) {
				bool jumpMove = checkForJump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
				if (jumpMove) {
					jump(m_row, m_col, m_rowMove, m_colMove, checkerBoard);
					switchPlayer();
				}
			}
		}
		else {
			cout << "\n Nerasta zaidimo figura. Pasirinkite kita vieta.\n\n";
			gameLoop(checkerBoard);
		}
	}

	string answer;
	if (checkerBoard.getRPieces() == 0) {
		cout << "\n\tZaidejas b yra laimetojas!\n\n" << endl;
		cout << "\nZaisti dar karta? (y/n)" << endl;
		cin >> answer;
		if (answer == "y") {
			main();
		}
		else {
			exit(0);
		}
	}
	else if (checkerBoard.getBPieces() == 0) {
		cout << "\n\tZaidejas r yra laimetojas!\n\n" << endl;
		cout << "\tZaisti dar karta? (y/n)";
		cin >> answer;
		if (answer == "y") {
			main();
		}
		else {
			exit(0);
		}
	}
}

//---------------------------------------------------------------------------------
// Pakeisti zaideja
void Game::switchPlayer(void) {
	if (m_player == "r") {
		m_player = "b";
	}
	else {
		m_player = "r";
	}
}

//---------------------------------------------------------------------------------
// Patikrinti del legalaus jump veiksmo 
bool Game::checkForJump(int row, int col, int rowMove, int colMove, CheckerBoard& checkerBoard) {
	m_jump = false;
	// šuolis iš kaires i dešine
	if (rowMove - row == 2 && colMove - col == 2) {
		string square = checkerBoard.getSquare((rowMove - 1), (colMove - 1));		// Patikriname vieta i kuria soksime 
		string jumpSquare = checkerBoard.getSquare(rowMove, colMove);				// Patikriname vieta is kurios sokti 
		if (!(square.empty())) {
			if (square != m_player && jumpSquare.empty()) {
				m_jump = true;
				m_jumpOption = 1;
			}
		}
	}
	// šuolis aukštyn iš dešines i kaire
	else if (row - rowMove == 2 && col - colMove == 2) {
		string square = checkerBoard.getSquare((rowMove + 1), (colMove + 1));		// Patikriname vieta i kuria soksime
		string jumpSquare = checkerBoard.getSquare(rowMove, colMove);				// Patikriname vieta is kurios sokti
		if (!(square.empty())) {
			if (square != m_player && jumpSquare.empty()) {
				m_jump = true;
				m_jumpOption = 2;
			}
		}
	}
	// šuolis iš kaires i dešine
	else if (row - rowMove == 2 && colMove - col == 2) {
		string square = checkerBoard.getSquare((rowMove + 1), (colMove - 1));		// Patikriname vieta i kuria soksime
		string jumpSquare = checkerBoard.getSquare(rowMove, colMove);				// Patikriname vieta is kurios sokti
		if (!(square.empty())) {
			if (square != m_player && jumpSquare.empty()) {
				m_jump = true;
				m_jumpOption = 3;
			}
		}
	}
	// šuolis žemyn iš dešines i kaire
	else if (rowMove - row == 2 && col - colMove == 2) {
		string square = checkerBoard.getSquare((rowMove - 1), (colMove + 1));		// Patikriname vieta i kuria soksime
		string jumpSquare = checkerBoard.getSquare(rowMove, colMove);				// Patikriname vieta is kurios sokti
		if (!(square.empty())) {
			if (square != m_player && jumpSquare.empty()) {
				m_jump = true;
				m_jumpOption = 4;
			}
		}
	}
	// Patikrinimas del dvibubu šuoliu
	// dvigubas šuolis iš kaires i dešine
	else if (rowMove - row == 4 && colMove - col == 4) {
		string square = checkerBoard.getSquare((rowMove - 3), (colMove - 3));		// Patikriname vieta i kuria soksime
		string jumpSquare = checkerBoard.getSquare((rowMove - 2), (colMove - 2));	// Patikriname vieta is kurios sokti 
		if (!(square.empty())) {
			if (square != m_player && jumpSquare.empty()) {
				square = checkerBoard.getSquare((rowMove - 1), (colMove - 1));		// Patikriname vieta i kuria soksime
				jumpSquare = checkerBoard.getSquare(rowMove, colMove);				// Patikriname vieta is kurios sokti 
				if (!(square.empty())) {
					if (square != m_player && jumpSquare.empty()) {
						m_jump = true;
						m_jumpOption = 5;
					}
				}
			}
		}
	}
	// dvigubas šuolis aukštyn iš dešines i kaire
	else if (row - rowMove == 4 && col - colMove == 4) {
		string square = checkerBoard.getSquare((rowMove + 3), (colMove + 3));		// Patikriname vieta i kuria soksime
		string jumpSquare = checkerBoard.getSquare((rowMove + 2), (colMove + 2));	// Patikriname vieta is kurios sokti
		if (!(square.empty())) {
			if (square != m_player && jumpSquare.empty()) {
				square = checkerBoard.getSquare((rowMove + 1), (colMove + 1));		// Patikriname vieta i kuria soksime
				jumpSquare = checkerBoard.getSquare(rowMove, colMove);				// Patikriname vieta is kurios sokti
				if (!(square.empty())) {
					if (square != m_player && jumpSquare.empty()) {
						m_jump = true;
						m_jumpOption = 6;
					}
				}
			}
		}
	}
	// dvigubas šuolis iš kaires i dešine
	else if (row - rowMove == 4 && colMove - col == 4) {
		string square = checkerBoard.getSquare((rowMove + 3), (colMove - 3));		// Patikriname vieta i kuria soksime
		string jumpSquare = checkerBoard.getSquare((rowMove + 2), (colMove - 2));	// Patikriname vieta is kurios sokti
		if (!(square.empty())) {
			if (square != m_player && jumpSquare.empty()) {
				square = checkerBoard.getSquare((rowMove + 1), (colMove - 1));		// Patikriname vieta i kuria soksime
				jumpSquare = checkerBoard.getSquare(rowMove, colMove);				// Patikriname vieta is kurios sokti 
				if (!(square.empty())) {
					if (square != m_player && jumpSquare.empty()) {
						m_jump = true;
						m_jumpOption = 7;
					}
				}
			}
		}
	}
	// dvigubas šuolis žemyn iš dešines i kaire
	else if (rowMove - row == 4 && col - colMove == 4) {
		string square = checkerBoard.getSquare((rowMove - 3), (colMove + 3));		// Patikriname vieta i kuria soksime
		string jumpSquare = checkerBoard.getSquare((rowMove - 2), (colMove + 2));	// Patikriname vieta is kurios sokti
		if (!(square.empty())) {
			if (square != m_player && jumpSquare.empty()) {
				square = checkerBoard.getSquare((rowMove - 1), (colMove + 1));		// Patikriname vieta i kuria soksime
				jumpSquare = checkerBoard.getSquare(rowMove, colMove);				// Patikriname vieta is kurios sokti
				if (!(square.empty())) {
					if (square != m_player && jumpSquare.empty()) {
						m_jump = true;
						m_jumpOption = 8;
					}
				}
			}
		}
	}

	return m_jump;
}

//---------------------------------------------------------------------------------
// Padaryti jump veiksma
void Game::jump(int row, int col, int rowMove, int colMove, CheckerBoard& checkerBoard) {
	int jumpOption = getJumpMove();
	string square = checkerBoard.getSquare(row, col);
	if (square == "R" && m_player == "r") {
		m_player == "R";
	}
	else if (square == "B" && m_player == "b") {
		m_player == "B";
	}

	switch (jumpOption) {
	case 1:
		checkerBoard.updateBoard(rowMove, colMove, checkerBoard.getSquare(row, col));
		checkerBoard.clearSquare(row, col);
		checkerBoard.clearSquare((rowMove - 1), (colMove - 1));
		checkerBoard.updateScore(m_player, jumpOption);
		checkerBoard.displayBoard();
		break;
	case 2:
		checkerBoard.updateBoard(rowMove, colMove, checkerBoard.getSquare(row, col));
		checkerBoard.clearSquare(row, col);
		checkerBoard.clearSquare((rowMove + 1), (colMove + 1));
		checkerBoard.updateScore(m_player, jumpOption);
		checkerBoard.displayBoard();
		break;
	case 3:
		checkerBoard.updateBoard(rowMove, colMove, checkerBoard.getSquare(row, col));
		checkerBoard.clearSquare(row, col);
		checkerBoard.clearSquare((rowMove + 1), (colMove - 1));
		checkerBoard.updateScore(m_player, jumpOption);
		checkerBoard.displayBoard();
		break;
	case 4:
		checkerBoard.updateBoard(rowMove, colMove, checkerBoard.getSquare(row, col));
		checkerBoard.clearSquare(row, col);
		checkerBoard.clearSquare((rowMove - 1), (colMove + 1));
		checkerBoard.updateScore(m_player, jumpOption);
		checkerBoard.displayBoard();
		break;
	case 5:
		checkerBoard.updateBoard(rowMove, colMove, checkerBoard.getSquare(row, col));
		checkerBoard.clearSquare(row, col);
		checkerBoard.clearSquare((rowMove - 3), (colMove - 3));
		checkerBoard.clearSquare((rowMove - 1), (colMove - 1));
		checkerBoard.updateScore(m_player, jumpOption);
		checkerBoard.displayBoard();
		break;
	case 6:
		checkerBoard.updateBoard(rowMove, colMove, checkerBoard.getSquare(row, col));
		checkerBoard.clearSquare(row, col);
		checkerBoard.clearSquare((rowMove + 3), (colMove + 3));
		checkerBoard.clearSquare((rowMove + 1), (colMove + 1));
		checkerBoard.updateScore(m_player, jumpOption);
		checkerBoard.displayBoard();
		break;
	case 7:
		checkerBoard.updateBoard(rowMove, colMove, checkerBoard.getSquare(row, col));
		checkerBoard.clearSquare(row, col);
		checkerBoard.clearSquare((rowMove + 3), (colMove - 3));
		checkerBoard.clearSquare((rowMove + 1), (colMove - 1));
		checkerBoard.updateScore(m_player, jumpOption);
		checkerBoard.displayBoard();
		break;
	case 8:
		checkerBoard.updateBoard(rowMove, colMove, checkerBoard.getSquare(row, col));
		checkerBoard.clearSquare(row, col);
		checkerBoard.clearSquare((rowMove - 3), (colMove + 3));
		checkerBoard.clearSquare((rowMove - 1), (colMove + 1));
		checkerBoard.updateScore(m_player, jumpOption);
		checkerBoard.displayBoard();
		break;
	default:
		break;
	}

	switchPlayer();
	gameLoop(checkerBoard);
}

//---------------------------------------------------------------------------------
// Grazinti jump pasirinkima
int Game::getJumpMove(void) {
	return m_jumpOption;
}

int main()
{
	Game checkers;
	checkers.displayGame();
	return 0;
}
