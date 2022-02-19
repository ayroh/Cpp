#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

class Hex{
	
	public:
	Hex();				// i couldnt find any other reasonable constructors
	Hex(string tempfilename);
	void PlayGame();
	bool CompareGames(const Hex &other);
	inline static int NumberOfMarkedCells(){	return MarkedCells;		}
	inline int CurrentSize() const{		return size;	}

	private:
	class Cell{
		public:
		char character;
		char column;
		int row;
	};
	vector< vector<Cell> > hexCells;
	Cell temp;
	void DisplayBoard() const;
	bool AnswerControl(char &column,int &row);
	bool PointControl(char column,int row);
	void Play();
	bool Play(char &column, int &row);
	bool isGameFinished();
	void FindSymbol(int x,int y, const char symbol);
	int FindBestWayDirection();
	void BotPointPlace(char &column, int &row, bool IsItTop);
	void SaveGame()const;
	void LoadGame();
	int size, enemy, turn;
	int topnumber, botnumber, direction;
	char topchar, botchar;
	char filename[30];
	bool GameOver;
	bool IsItFirstMove,IsTopLineFinished, IsBotLineFinished;
	inline int CharToint(char character){	return static_cast<int>(character) - 65;	}
	static int MarkedCells;

	
};

int Hex::MarkedCells = 0;

int main(){
	string patates;
	int choice;
	int game1, game2;
	int sayz;
	cout << "\n\nEnter 0 for a new game, 1 for loading a game \n";
	cin >> choice;
	if(choice == 0){
		Hex game[5];
		while(true){
			cout << "	Choose your game (1 - 5).\n-1 for end.\n6 for compare games.\n7 for knowledge about marked cells.\n8 for size of a game.\n\n";
			cin >> choice;
			if(choice == -1)
				return 0;
			else if(choice == 6){
				cout << "Enter two games between 1-5.\n";
				cin >> game1 >> game2;
				if(game[game1 - 1].CompareGames(game[game2 -1]) == true)
					cout << "Game" << game1 << " has more marked cells than Game" << game2 << ".\n\n";
				else
					cout << "Game" << game2 << " has more marked cells than Game" << game1 << ".\n\n";
				continue;
			}
			else if(choice == 7){
				cout << "Number of marked cells are: " << Hex::NumberOfMarkedCells() << endl << endl;
				continue;
			}
			else if(choice == 8){
				cout << "Enter game (1 - 5).\n\n";
				cin >> sayz;
				--sayz;
				cout << "Size of Game " << sayz + 1 << " is: " << game[sayz].CurrentSize() << "x" << game[sayz].CurrentSize() << endl << endl;
				continue;
			}
			else if(choice < 1 || choice > 5){
				cerr << "Enter between 1-5\n\n";
				continue;
			}
			--choice;
			game[choice].PlayGame();
			cin.clear();
			cin.ignore(10000,'\n');
		}
	}
	else{
		cout << "Enter your filename.\n";
		cin >> patates;
		Hex gameload(patates);
		gameload.PlayGame();
	}
}


void Hex::PlayGame(){
	if(GameOver == true){
		cout << "This game already ended\n";
		return;
	}
	char column = ' ';
	int row = 0;
	if(size == 0){			//  this means user already loaded a game with constructor
		while(true){		//	continues until user enters a number thats bigger than 5
			cout << "Enter the size of your table.(6 - infinity)\n";
			cin >> size;
			if(size < 6){
				cerr << "Enter a number that is bigger than 5.\n\n";
				continue;
			}
			break;
		}
		
		while(true){						// computer or player
			cout << "Press '1' for playing with a user and '2' for computer.\n";
			cin >> enemy;
			if(enemy != 1 && enemy != 2){
				cerr << "Enter a proper answer.\n\n";
				continue;
			}
			break;
		}
		
		vector<Cell> vtemp(size);		// creating a 1D array then push_back it size times
		for(int i = 0;i < size;++i)
			vtemp[i].character = '.';
		for(int i = 0;i < size;++i)
			hexCells.push_back(vtemp);
			
											// filling class Cell
		for(int i = 0;i < size;++i){		// row
			for(int j = 0;j < size;++j){	// column
				hexCells[i][j].row = i;
				hexCells[i][j].column = j + 97;
			}
		}
	}
		DisplayBoard();
	if(enemy == 1){
		while(GameOver == false){
			while(true){
				cout << "\nEnter your move Player 1  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;
					SaveGame();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;
					LoadGame();
					if(enemy == 2){
						cerr << "You entered a game with pve\n";
						exit(1);
					}
					if(turn == 2)
						break;
					else
						continue;
				}
				else if(row == 0 && (column == 'q' || column == 'Q')){
					cin.clear();	//  quit to menu
					return;
				}
				if(Play(column,row) == true)    // returns GameOver
					break;
			}
			turn = 2;
			
			while(true){
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;
					SaveGame();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;
					LoadGame();
					if(enemy == 2){
						cerr << "You entered a game with pve\n";
						exit(1);
					}
					if(turn == 1)
						break;
					else
						continue;
				}
				else if(row == 0 && (column == 'q' || column == 'Q')){
					cin.clear();		// quit to menu
					return;
				}
				if(Play(column,row) == true)	// returns GameOver
					break;
			}
			turn = 1;
		}
	}
	else if(enemy == 2){
		while(GameOver == false){
			turn = 1;
			while(true){
				cout << "\nEnter your move Player 1  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;
					SaveGame();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with SAVE, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;
					LoadGame();
					if(enemy == 1){
						cerr << "You entered a game with pvp\n";
						exit(1);
					}
					continue;
				}
				else if(row == 0 && (column == 'q' || column == 'Q')){
					cin.clear();		// quit to menu
					return;
				}
				if(Play(column,row) == true)	// returns GameOver
					break;
			}
			turn = 2;
			Play();
			
		}
	}
	DisplayBoard();
}

void Hex::Play(){
	if(IsItFirstMove == true){
		IsItFirstMove = false;
		if(64 + size == temp.column)  // If at first move player selects the rightmost column
			--temp.column;
		else
			temp.column += 1;									// In first move program selects the 'right' of the player
		hexCells[temp.row - 1][CharToint(temp.column)].character = 'o';
		topchar = botchar = temp.column;					// Program stores the rightmost, leftmost
		topnumber = botnumber = temp.row;					// lowest and highest selection and continues from that point
		DisplayBoard();
		cout << "Computer entered '" << temp.column << temp.row << "' \n";
	}
	else{
		if(temp.row > topnumber && IsTopLineFinished == false){			// if player selects higher temp.row from the topnumber, computer goes to down
			BotPointPlace(topchar,topnumber, true);			// if there is a symbol in the top row program goes to bot
			DisplayBoard();
			cout << "Computer entered '" << topchar << topnumber << "' \n";
		}
		else if(temp.row < botnumber && IsBotLineFinished == false){		// if lower from the botnumber goes to up
			BotPointPlace(botchar, botnumber, false);		// if there is a symbol in the bot row program goes to top
			DisplayBoard();
			cout << "Computer entered '" << botchar << botnumber << "' \n";
		}
		else{		// if between top and bot numbers program first tries to go to up, if there is already a symbol in the top row it goes to bottom
			if(IsTopLineFinished == false){
				BotPointPlace(topchar, topnumber, true);
				DisplayBoard();
				cout << "Computer entered '" << topchar << topnumber << "' \n";
			}
			else{
				BotPointPlace(botchar, botnumber, false);
				DisplayBoard();
				cout << "Computer entered '" << botchar << botnumber << "' \n";
			}
		}
	}
	if(isGameFinished() == true)
		cout << "\n\n\n        It's Over Anakin\n" << "        You lost against a bot that is not that smart \n" << "        Shame of you\n\n";
	++MarkedCells;
}

bool Hex::Play(char &column, int &row){

	if(AnswerControl(column,row) == false){
		cerr << "Enter a proper answer.\n";
		return false;
	}
		
	if(PointControl(column,row) == false){
		cerr << "That point is not empty.\n\n";
		return false;
	}
	if(turn == 1)
		hexCells[row - 1][CharToint(column)].character = 'x';
	else if(turn == 2)
		hexCells[row - 1][CharToint(column)].character = 'o';
	
	cout << "Player " << turn << " entered '" << column << row << "' \n";

	if(isGameFinished() == true)
		cout << "\n\n\n        It's Over Anakin\n\n";
	
	if(enemy == 2){
		temp.column = column;
		temp.row = row;
	}
	else
		DisplayBoard();
	++MarkedCells;
	return true;
}

void Hex::DisplayBoard() const{
	cout << "  ";
	for(int i = 0;i < size;i++)
		cout << static_cast<char>(i + 65) << " ";
	cout << "\n";
	for(int i = 0;i < size;i++){
		cout << i+1;
		for(int k = 0;k < i+1;k++){
			if(k == 9) continue;		// because of the two digit numbers
			cout << " ";
		}
		for(int j = 0;j < size;j++)
			cout << hexCells[i][j].character << " ";
		cout << "\n";
	}
}


bool Hex::AnswerControl(char &column,int &row){
	if(column > 96 && column < 96 + size + 1)
		column -= 32;	 // in case player plays with lowercases
	
	if(row > size || row < 1 || column > 65 + size - 1 || column < 65)	 // if input is other than abcdefghijkl or ABCDEFGHIJKL or 1-12
		return false;
	else
		return true;
}

bool Hex::PointControl(char column,int row){
	if(hexCells[row - 1][CharToint(column)].character == '.')
		return true;
	else
		return false;	
}


bool Hex::isGameFinished(){
	int i;
	char symbol;
	if(turn == 1){ 			 // Left-Right walls
		symbol = 'x';
		for(i = 0;i < size;i++){			// if there is a symbol in the left column it starts to search for others
			if(hexCells[i][0].character == symbol) break;
		}
		if(i == size) return false;
		FindSymbol(0,i,symbol);
	}
	else if(turn == 2){		// top-bot walls
		symbol = 'o';
		for(i = 0;i < size;i++){		// if there is a symbol in the bot row it starts to search for others
			if(hexCells[0][i].character == symbol) break;
		}
		if(i == size) return false;
		FindSymbol(i,0,symbol);
	}
	
	return GameOver;
}

void Hex::FindSymbol(int x,int y, const char symbol){						// for player1 searches for 'x', jumps to that 'x' and uppercase the 
	hexCells[y][x].character = toupper(symbol);								// previous 'x' to 'X' to prevent jumping back.
	if(turn == 1 && x == size - 1){											// End of the functions if game doesnt end program lowercase them again.
		GameOver = true;
		return;
	}
	if(turn == 2 && y == size - 1){
		GameOver = true;
		return;
	}
						// Searches 6 different ways for lowercase symbols by the PlayerType and controls if numbers (x and y) are out of the array

	if(y - 1 > -1 && hexCells[y - 1][x].character == symbol) FindSymbol(x,y-1,symbol);
	if(y - 1 > -1  && x + 1 < size && hexCells[y - 1][x + 1].character == symbol) FindSymbol(x + 1,y - 1,symbol);
	if(x + 1 < size && hexCells[y][x + 1].character == symbol) FindSymbol(x + 1,y,symbol);
	if(y + 1 < size && hexCells[y + 1][x].character == symbol) FindSymbol(x,y + 1,symbol);
	if(y + 1 < size && x - 1 > -1 && hexCells[y + 1][x - 1].character == symbol) FindSymbol(x - 1,y + 1,symbol);
	if(x - 1 > -1 && hexCells[y][x - 1].character == symbol) FindSymbol(x - 1,y,symbol);

	if(GameOver == false)
		hexCells[y][x].character = symbol;	//  If game doesnt end, program lowercases for next time
}

void Hex::BotPointPlace(char &column, int &row, bool IsItTop){
	             					// Places the next symbol by the IsItTop selection
	if(IsItTop == true){			// Function first tries to go straight up or down
		row += 1;					// then tries cross paths
									// at last computes the shortest way between right and left and goes that way
		if(PointControl(column, row) == true)											// to down
			hexCells[row - 1][CharToint(column)].character = 'o';
		else if(PointControl(column - 1, row) == true && CharToint(column) - 1 > -1){	// cross
			--column;
			hexCells[row - 1][CharToint(column)].character = 'o';
		}
		else{
			direction = FindBestWayDirection();						// right or left			
			row -= 1;
			if(PointControl(column + direction, row) == true){
				column += direction;
				hexCells[row - 1][CharToint(column)].character = 'o';
			}
			else if(PointControl(column - direction, row) == true){
				column -= direction;
				hexCells[row - 1][CharToint(column)].character = 'o';
			}
		}				
		if(row == size)			// this means computer placed a symbol to last line
			IsTopLineFinished = true;
	}

	else if(IsItTop == false){
		row -= 1;
		
		if(PointControl(column,row) == 1)											// to up
			hexCells[row - 1][CharToint(column)].character = 'o';
		else if(PointControl(column + 1,row) == 1  && CharToint(column) + 1 < 12){	// cross
			++column;
			hexCells[row - 1][CharToint(column)].character = 'o';
		}
		else{
			direction = FindBestWayDirection();						// right or left
			row += 1;
			if(PointControl(column + direction,row) == 1){
				column += direction;
				hexCells[row - 1][CharToint(column)].character = 'o';
			}
			else if(PointControl(column - direction,row) == 1){
				column -= direction;
				hexCells[row - 1][CharToint(column)].character = 'o';
			}
		}
		if(row == 0)			// this means computer placed a symbol to first line
			IsBotLineFinished = true;
	}
}

int Hex::FindBestWayDirection(){	//	for computer, if top-topleft-topright or bot-botleft-botright
	int right = 0,left = 0;			//  is not empty, computer computes the shortest way to go
	
		
	while(hexCells[temp.row - 1][CharToint(temp.column) + right].character == 'x') ++right;
	while(hexCells[temp.row - 1][CharToint(temp.column) - left].character == 'x') ++left;
	if(CharToint(temp.column) + right == 12)
		return -1;		// end of array
	else if(CharToint(temp.column) - left == -1) 
		return 1;	// end of array
	else if(right > left)
		return -1;
	else 
		return 1;
}



bool Hex::CompareGames(const Hex &other){		// if sizes are equal tries same time, if not calculates separetely then returns
	int marked1 = 0, marked2 = 0;

	if(size == other.size){
		for(int i = 0;i < size;++i){
			for(int j = 0; j < size;++j){
				if(hexCells[i][j].character == '.')
					++marked1;
				if(other.hexCells[i][j].character == '.')
					++marked2;
			}
		}
	}
	else{
		for(int i = 0;i < size;++i){
			for(int j = 0; j < size;++j){
				if(hexCells[i][j].character == '.')
					++marked1;
			}
		}
		for(int i = 0;i < other.size;++i){
			for(int j = 0; j < other.size;++j){
				if(other.hexCells[i][j].character == '.')
					++marked2;
			}
		}
	}
	if(marked1 > marked2)
		return 1;
	else
		return 0;
}



Hex::Hex()
	: size(0), turn(1), IsItFirstMove(true), GameOver(false), IsTopLineFinished(false), IsBotLineFinished(false){
		temp.character = 'o';
	}

Hex::Hex(string tempfilename)			//	loads game
	: size(0), turn(1), IsItFirstMove(false), GameOver(false), IsTopLineFinished(false), IsBotLineFinished(false){
		temp.character = 'o';
		strcpy(filename,tempfilename.c_str());
		LoadGame();
	}


void Hex::SaveGame()const{
	int intbuffer;
	ofstream SaveFile(filename);
					// Saving to a file by the order
	cout << size<< "\n\n";
	SaveFile << size << ' ' << enemy << ' ' << turn << ' ' << topnumber << ' ' << botnumber << ' ' << IsTopLineFinished << ' ' << IsBotLineFinished;
	if(enemy == 2)
		SaveFile << ' ' << topchar << ' ' << botchar << endl;
	else
		SaveFile << endl;
	for(int i = 0;i < size;++i){
		for(int j = 0;j < size;++j){
			intbuffer = static_cast<int>(hexCells[i][j].character);
			SaveFile << intbuffer << ' ';
		}
		SaveFile << endl;
	}
	cout << "Your game is saved to file: " << filename << endl;
	SaveFile.close();
}


void Hex::LoadGame(){
	int intbuffer;
	ifstream LoadFile(filename);
	if(!LoadFile.is_open()){
		cerr << endl << "File couldn't open" << endl;
		exit(1);
	}
					//   Reading file by the order
	LoadFile >> size >> enemy >> turn >> topnumber >> botnumber >> IsTopLineFinished >> IsBotLineFinished;
	if(enemy == 2)
		LoadFile >> topchar >> botchar;


	hexCells.resize(size * size);
	for(int i = 0;i < size;++i){
		hexCells[i].resize(size);

		for(int j = 0;j < size;++j){
			LoadFile >> intbuffer;
			hexCells[i][j].character = static_cast<char>(intbuffer);
		}
	}
	LoadFile.close();
	cout << endl << "New board is:\n\n";
	DisplayBoard();
}





