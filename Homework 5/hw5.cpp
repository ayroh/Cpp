#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <exception>
using namespace std;

class noLastMove{
	public:
		noLastMove(){ cout << "There is no last move.\n"; };
};

class WrongIndex{
	public:
		WrongIndex(){ cout << "You entered wrong index.\n"; };
};


class AbstractHex{
	public:
		class Cell{
			public:
				Cell();
				Cell(char character, char column, int row);
				Cell(const Cell &CtrlC);
				Cell& SetCharacter(char c1);
				Cell& SetColumn(char c1);
				Cell& SetRow(int r1);
				inline char GetCharacter()const{	return character; }
				inline char GetColumn()const{	return column; }
				inline int GetRow()const{	return row; }
			private:
				char character;
				char column;
				int row;
		};
		AbstractHex();
		AbstractHex(string tempfilename);
		virtual void print() = 0;
		virtual void readFromFile() = 0;
		virtual void writeToFile()const = 0;
		virtual void setSize() = 0;
		virtual void reset() = 0;
		virtual Cell& Play() = 0;
		virtual bool Play(Cell& control) = 0;
		virtual bool isEnd() = 0;
		virtual char operator()(char c, int i) = 0;
		bool operator==(AbstractHex &other);
		Cell lastMove();
		int numberOfMoves(){  return NumberOfMoves;  };
		
		bool AnswerControl(Cell& control);
		void setFilename(string tempfilename){	strcpy(filename,tempfilename.c_str());	};
		void setSizeValue(int tempsize){size = tempsize;}
		int GetSize()const{ return size;	}
		inline int CharToint(char character)const{	return static_cast<int>(character) - 65; };
		
	protected:
		int size;
		int NumberOfMoves;
		char topchar, botchar;
		char filename[30];
		bool GameOver;
		bool IsItFirstMove,IsTopLineFinished, IsBotLineFinished;
		int enemy, turn;
		int topnumber, botnumber, direction;
		Cell LastMove, temp;
};

AbstractHex::AbstractHex()
	: size(0), turn(1), NumberOfMoves(0), IsItFirstMove(true), GameOver(false), IsTopLineFinished(false), IsBotLineFinished(false){
		temp.SetCharacter('o');
		LastMove.SetCharacter('.');
}

AbstractHex::AbstractHex(string tempfilename)			//	loads game
	:IsItFirstMove(false), GameOver(false), NumberOfMoves(0){
		temp.SetCharacter('o');
		LastMove.SetCharacter('x');
		strcpy(filename,tempfilename.c_str());
}
	
bool AbstractHex::AnswerControl(Cell& control){	
	if(control.GetColumn() > 96 && control.GetColumn() < 96 + GetSize() + 1)
		control.SetColumn(control.GetColumn() - 32);	 // in case player plays with lowercases
	
	if(control.GetRow() > GetSize() || control.GetRow() < 1 || control.GetColumn() > 65 + GetSize() - 1 || control.GetColumn() < 65)	 // if input is other than abcdefghijkl or ABCDEFGHIJKL or 1-12
		return false;
	else
		return true;
}


AbstractHex::Cell AbstractHex::lastMove(){
	if(LastMove.GetCharacter() == '.')
		throw noLastMove();
	return LastMove;
}

bool AbstractHex::operator==(AbstractHex &other){
	if(GetSize() == other.GetSize())
		for(int i = 0;i < GetSize();++i)
			for(int j = 1;j <= GetSize();++j)
				if((*this)(i + 65, j) != other(i + 65, j))	//	 controls cell by cell
					return false;
	return true;
}


AbstractHex::Cell& AbstractHex::Cell::SetCharacter(char c1){
	character = c1;
	return *this;
}
AbstractHex::Cell& AbstractHex::Cell::SetColumn(char c1){
	column = c1;
	return *this;
}
AbstractHex::Cell& AbstractHex::Cell::SetRow(int r1){
	row = r1;
	return *this;
}

AbstractHex::Cell::Cell()
{/* Left empty on purpose */}

AbstractHex::Cell::Cell(const Cell &CtrlC)
	:character(CtrlC.character), column(CtrlC.column), row(CtrlC.row)
{/* Left empty on purpose */}

AbstractHex::Cell::Cell(char character, char column, int row)
	:character(character), column(column), row(row)
{/* Left empty on purpose */}





class HexVector: public AbstractHex{
	public:
		HexVector();
		HexVector(string tempfilename);
		void print();
		void setSize();
		void writeToFile() const;
		void readFromFile();
		void reset();
		bool isEnd();
		Cell& Play();
		bool Play(Cell& control);
		virtual char operator()(char c, int i);
		
		void PlayGame();
		bool PointControl(Cell control)const;
		void FindSymbol(int x,int y, const char symbol);
		void BotPointPlace(bool IsItTop);
		int FindBestWayDirection(Cell& control);
	private:
		vector<vector<Cell> > Cells;
};

HexVector::HexVector(): AbstractHex(){	};
		
HexVector::HexVector(string tempfilename)
   :AbstractHex(tempfilename){
   	readFromFile();
}
void HexVector::print(){
	cout << "  ";
	for(int i = 0;i < GetSize();i++)
		cout << static_cast<char>(i + 65) << " ";
	cout << "\n";
	for(int i = 0;i < GetSize();i++){
		cout << i+1;
	for(int k = 0;k < i+1;k++){
		if(k == 9) continue;		// because of the two digit numbers
		cout << " ";
	}
	for(int j = 0;j < GetSize();j++)
		cout << Cells[i][j].GetCharacter() << ' ';
	cout << "\n";
	}	
}

void HexVector::setSize(){
	Cells.clear();
	Cells.resize(GetSize());
	for(int i = 0;i < GetSize();++i)
		Cells[i].resize(GetSize());
			
										// filling class Cell
	for(int i = 0;i < size;++i){		// row
		for(int j = 0;j < size;++j){	// column
			Cells[i][j].SetCharacter('.');
			Cells[i][j].SetRow(i);
			Cells[i][j].SetColumn(j + 97);
		}
	}
}

void HexVector::writeToFile() const{
	ofstream SaveFile(filename);
	int intbuffer;
				// Saving to a file by the order
	SaveFile << size << ' ' << enemy << ' ' << turn << ' ' << topnumber << ' ' << botnumber << ' ' << IsTopLineFinished << ' ' << IsBotLineFinished;
	if(enemy == 2)
	SaveFile << ' ' << topchar << ' ' << botchar << endl;
	else
	SaveFile << endl;
	for(int i = 0;i < GetSize();++i){
		for(int j = 0;j < GetSize();++j){
			intbuffer = static_cast<int>(Cells[i][j].GetCharacter());
			SaveFile << intbuffer << ' ';
		}
		SaveFile << endl;
	}
	cout << "Your game is saved to file: " << filename << endl;
	SaveFile.close();
}

void HexVector::readFromFile(){
	ifstream LoadFile(filename);
	int intbuffer;
	if(!LoadFile.is_open()){
		cerr << endl << "File couldn't open" << endl;
		exit(1);
	}
					//   Reading file by the order
	LoadFile >> size >> enemy >> turn >> topnumber >> botnumber >> IsTopLineFinished >> IsBotLineFinished;
	if(enemy == 2)
		LoadFile >> topchar >> botchar;

	setSize();
	for(int i = 0;i < GetSize();++i){
		for(int j = 0;j < GetSize();++j){
			LoadFile >> intbuffer;
			Cells[i][j].SetCharacter(static_cast<char>(intbuffer));
		}
	}
	LoadFile.close();
	cout << "\nNew board is:\n\n";
	print();
	LastMove.SetCharacter('x');
}

void HexVector::reset(){
	IsItFirstMove = true;
	turn = 1;
	NumberOfMoves = 0;
	GameOver = false;
	IsTopLineFinished = false;
	IsBotLineFinished = false;
	temp.SetCharacter('x');
	int tempsize;
	if(LastMove.GetCharacter() == '.'){
		while(true){		//	continues until user enters a number thats bigger than 5
			cout << "Enter the size of your table.(6 - infinity)\n";
			cin >> size;
			if(size < 6){
				cerr << "Enter a number that is bigger than 5.\n\n";
				continue;
			}
			break;
		}
	}	
	LastMove.SetCharacter('.');
	setSize();
											// filling class Cell
	for(int i = 0;i < GetSize();++i){		// row
		for(int j = 0;j < GetSize();++j){	// column
			Cells[i][j].SetCharacter('.');
			Cells[i][j].SetRow(i);
			Cells[i][j].SetColumn(j + 97);
		}
	}
}

bool HexVector::isEnd(){
	int i;
	char symbol;
	if(turn == 1){ 			 // Left-Right walls
		symbol = 'x';
		for(i = 0;i < GetSize();i++){			// if there is a symbol in the left column it starts to search for others
			if(Cells[i][0].GetCharacter() == symbol)
	        	FindSymbol(0,i,symbol);
		}
	}
	else if(turn == 2){		// top-bot walls
		symbol = 'o';
		for(i = 0;i < GetSize();i++){		// if there is a symbol in the bot row it starts to search for others
			if(Cells[0][i].GetCharacter() == symbol)
		        FindSymbol(i,0,symbol);
		}
	}
	return GameOver;
}

void HexVector::FindSymbol(int x,int y, const char symbol){
	Cells[y][x].SetCharacter(toupper(symbol));							// previous 'x' to 'X' to prevent jumping back.
	if(turn == 1 && x == GetSize() - 1){											// End of the functions if game doesnt end program lowercase them again.
		GameOver = true;
		return;
	}
	if(turn == 2 && y == size - 1){
		GameOver = true;
		return;
	}
						// Searches 6 different ways for lowercase symbols by the PlayerType and controls if numbers (x and y) are out of the array

	if(y - 1 > -1 && Cells[y - 1][x].GetCharacter() == symbol) FindSymbol(x,y-1,symbol);
	if(y - 1 > -1  && x + 1 < size && Cells[y - 1][x + 1].GetCharacter() == symbol) FindSymbol(x + 1,y - 1,symbol);
	if(x + 1 < size && Cells[y][x + 1].GetCharacter() == symbol) FindSymbol(x + 1,y,symbol);
	if(y + 1 < size && Cells[y + 1][x].GetCharacter() == symbol) FindSymbol(x,y + 1,symbol);
	if(y + 1 < size && x - 1 > -1 && Cells[y + 1][x - 1].GetCharacter() == symbol) FindSymbol(x - 1,y + 1,symbol);
	if(x - 1 > -1 && Cells[y][x - 1].GetCharacter() == symbol) FindSymbol(x - 1,y,symbol);

	if(GameOver == false)
		Cells[y][x].SetCharacter(symbol);
}

AbstractHex::Cell& HexVector::Play(){
	temp.SetCharacter('o');
	if(IsItFirstMove == true){
		IsItFirstMove = false;
		if(64 + size == temp.GetColumn())  // If at first move player selects the rightmost column
			temp.SetColumn(temp.GetColumn() - 1);
		else
			temp.SetColumn(temp.GetColumn() + 1);	// In first move program selects the 'right' of the player
		Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		topchar = botchar = temp.GetColumn();					// Program stores the rightmost, leftmost
		topnumber = botnumber = temp.GetRow();					// lowest and highest selection and continues from that point
		cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		if(temp.GetRow() == GetSize())
			IsTopLineFinished = true;
		else if(temp.GetRow() - 1 == 0)
			IsBotLineFinished = true;
	}
	else{
		if(temp.GetRow() > topnumber && IsTopLineFinished == false){			// if player selects higher temp.row from the topnumber, computer goes to down
			temp.SetRow(topnumber);
			temp.SetColumn(topchar);
			temp.SetCharacter('o');
			BotPointPlace(true);			// if there is a symbol in the top row program goes to bot
			cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		}
		else if(temp.GetRow() < botnumber && IsBotLineFinished == false){		// if lower from the botnumber goes to up
			temp.SetRow(botnumber);
			temp.SetColumn(botchar);
			temp.SetCharacter('o');
			BotPointPlace(false);		// if there is a symbol in the bot row program goes to top
			cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		}
		else{		// if between top and bot numbers program first tries to go to up, if there is already a symbol in the top row it goes to bottom
			if(IsTopLineFinished == false){
				temp.SetRow(topnumber);
				temp.SetColumn(topchar);
				temp.SetCharacter('o');
				BotPointPlace(true);
				cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
			}
			else{
				temp.SetRow(botnumber);
				temp.SetColumn(botchar);
				temp.SetCharacter('o');
				BotPointPlace(false);
				cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
			}
		}
	}
	if(isEnd() == true)
		cout << "\n\n\n        It's Over Anakin\n" << "        You lost against a bot that is not that smart \n" << "        Shame of you\n\n";
	++NumberOfMoves;
	return Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())];
}

bool HexVector::Play(Cell& control){
	
	if(AnswerControl(control) == false){
		cerr << "Enter a proper answer.\n";
		return false;
	}
		
	if(PointControl(control) == false){
		cerr << "That point is not empty.\n\n";
		return false;
	}
	if(turn == 1)
		Cells[control.GetRow() - 1][CharToint(control.GetColumn())].SetCharacter('x');
	else if(turn == 2)
		Cells[control.GetRow() - 1][CharToint(control.GetColumn())].SetCharacter('o');
	
	cout << "Player " << turn << " entered '" << control.GetColumn() << control.GetRow() << "' \n";

	if(isEnd() == true)
		cout << "\n\n\n        It's Over Anakin\n\n";
	
	if(enemy == 2){
		temp.SetCharacter('o');
		temp.SetColumn(control.GetColumn());
		temp.SetRow(control.GetRow());
	}
	++NumberOfMoves;
	return true;
}

char HexVector::operator()(char c, int i){
	if(c > 96 && c < 96 + GetSize() + 1)
		c -= 32;	 // in case player plays with lowercases
	if(c < 65 || c > 64 + GetSize() || i < 1 || i > GetSize())
		throw WrongIndex();
	return Cells[i - 1][CharToint(c)].GetCharacter();
}

void HexVector::PlayGame(){
	if(GameOver == true){
		cout << "This game already ended\n";
		return;
	}
	char column = ' ';
	int row = 0;
	int tempsize;
	if(GetSize() == 0){					//  this means user already has a game
		while(true){		//	continues until user enters a number thats bigger than 5
			cout << "Enter the size of your table.(6 - infinity)\n";
			cin >> tempsize;
			if(tempsize < 6){
				cerr << "Enter a number that is bigger than 5.\n\n";
				continue;
			}
			setSizeValue(tempsize);
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
		setSize();
											// filling class Cell
		for(int i = 0;i < GetSize();++i){		// row
			for(int j = 0;j < GetSize();++j){	// column
				Cells[i][j].SetCharacter('.');
				Cells[i][j].SetRow(i);
				Cells[i][j].SetColumn(j + 97);
			}
		}
	}
	if(enemy == 1){
		print();
		while(GameOver == false){
			while(turn == 1){
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										
					readFromFile();
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
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){	//	reset
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');	
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){// last move
					cin.clear();	//  quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){	//	number of moves
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetCharacter('x');
				temp.SetColumn(column);
				temp.SetRow(row);
				Play(temp);
				temp.SetCharacter('x');
				LastMove.SetCharacter('x');
				LastMove.SetColumn(column);
				LastMove.SetRow(row);
				break;
			}
			if(LastMove.GetCharacter() == '.')	//	for loaded game to go turn 1
				continue;
			if(GameOver == true)
				break;
			print();
			turn = 2;
			
			while(turn == 2){
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										
					readFromFile();
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
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');	
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){
					cin.clear();	
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetCharacter('o');
				temp.SetColumn(column);
				temp.SetRow(row);
				Play(temp);
					temp.SetCharacter('o');
					LastMove.SetCharacter('o');
					LastMove.SetColumn(column);
					LastMove.SetRow(row);
					break;
			}
			if(GameOver == true)
				break;
			turn = 1;
			print();
		}
	}
	else if(enemy == 2){
		while(GameOver == false){
			turn = 1;
			print();
			while(true){
				temp.SetCharacter('x');
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										//						---------------------------
					readFromFile();
					IsItFirstMove = false;
					if(enemy == 1){
						cerr << "You entered a game with pvp\n";
						exit(1);
					}
						continue;
				}
				else if(row == 0 && (column == 'q' || column == 'Q')){
					cin.clear();	// quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){
					cin.clear();	// reset game
					cin.clear();
					cin.ignore(10,'\n');
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){
					cin.clear();	// last move
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){
					cin.clear();	// number of moves
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetColumn(column);
				temp.SetRow(row);
				Play(temp);
				break;
			}
			if(temp.GetCharacter() == 'x')
				continue;
			if(GameOver == true)
				break;
			turn = 2;
			Play();
			LastMove.SetCharacter('o');
			LastMove.SetColumn(temp.GetColumn());
			LastMove.SetRow(temp.GetRow());
		}
	}
	print();
}

bool HexVector::PointControl(Cell control)const{
	if(Cells[control.GetRow() - 1][CharToint(control.GetColumn())].GetCharacter() == '.')
		return true;
	else
		return false;
}

void HexVector::BotPointPlace(bool IsItTop){
	             					// Places the next symbol by the IsItTop selection
	if(IsItTop == true){			// Function first tries to go straight up or down
		++topnumber;
		temp.SetRow(topnumber);					// then tries cross paths

									// at last computes the shortest way between right and left and goes that way
		if(PointControl(temp) == true){											// to down
			Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - 1, temp.GetRow())) == true && CharToint(temp.GetColumn()) - 1 > -1){	// cross
			temp.SetColumn(temp.GetColumn() - 1);
			--topchar;
			Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else{
			direction = FindBestWayDirection(temp);						// right or left			
			--topnumber;
			temp.SetRow(topnumber);

			if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + direction, temp.GetRow())) == true){
				temp.SetColumn(temp.GetColumn() + direction);
				topchar +=direction;
				Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
			else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - direction, temp.GetRow())) == true){
				temp.SetColumn(temp.GetColumn() - direction);
				topchar -= direction;
				Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
		}				
		if(temp.GetRow() == size)			// this means computer placed a symbol to last line
			IsTopLineFinished = true;
	}

	else if(IsItTop == false){
		temp.SetRow(temp.GetRow() - 1);
		--botnumber;
		
		if(PointControl(temp) == true)											// to up
			Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + 1, temp.GetRow())) == 1  && CharToint(temp.GetColumn()) + 1 < 12){	// cross
			++botchar;
			temp.SetColumn(botchar);
			Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else{
			direction = FindBestWayDirection(temp);						// right or left			
			++botnumber;
			temp.SetRow(botnumber);

			if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + direction, temp.GetRow())) == true){
				botchar += direction;
				temp.SetColumn(botchar);
				Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
			else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - direction, temp.GetRow())) == 1){
				botchar -= direction;
				temp.SetColumn(botchar);
				Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
		}
		if(temp.GetRow() == 0)			// this means computer placed a symbol to first line
			IsBotLineFinished = true;
	}
}

int HexVector::FindBestWayDirection(Cell& control){
	int right = 0,left = 0;						//  if not empty, computer computes the shortest way to go
		
	while(Cells[control.GetRow() - 1][CharToint(control.GetColumn()) + right].GetCharacter() == 'x') ++right;
	while(Cells[control.GetRow() - 1][CharToint(control.GetColumn()) - left].GetCharacter() == 'x') ++left;
	if(CharToint(control.GetColumn()) + right == GetSize())
		return -1;		// end of array
	else if(CharToint(control.GetColumn()) - left == -1) 
		return 1;	// end of array
	else if(right > left)
		return -1;
	else 
		return 1;
}





template<class T>
class HexAdapter: public AbstractHex{
	public:
		HexAdapter();
		HexAdapter(string tempfilename);
		void print();
		void setSize();
		void writeToFile() const;
		void readFromFile();
		void reset();
		bool isEnd();
		Cell& Play();
		bool Play(Cell& control);
		virtual char operator()(char c, int i);
		
		void PlayGame();
		bool PointControl(Cell control)const;
		void FindSymbol(int x,int y, const char symbol);
		void BotPointPlace(bool IsItTop);
		int FindBestWayDirection(Cell& control);
	private:
		T Cells;
};

template<class T>
HexAdapter<T>::HexAdapter(): AbstractHex(){ /* Intentionally Empty */ };

template<class T>
HexAdapter<T>::HexAdapter(string tempfilename)
   :AbstractHex(tempfilename){
   	readFromFile();
}


template<class T>
void HexAdapter<T>::print(){
	cout << "  ";
	for(int i = 0;i < GetSize();i++)
		cout << static_cast<char>(i + 65) << " ";
	cout << "\n";
	for(int i = 0;i < GetSize();i++){
		cout << i+1;
	for(int k = 0;k < i+1;k++){
		if(k == 9) continue;		// because of the two digit numbers
		cout << " ";
	}
	for(int j = 0;j < GetSize();j++)
		cout << Cells[i][j].GetCharacter() << ' ';
	cout << "\n";
	}
}
	
template <class T>
void HexAdapter<T>::setSize(){
	Cells.clear();
	Cells.resize(GetSize());
	for(int i = 0;i < GetSize();++i)
		Cells[i].resize(GetSize());
			
										// filling class Cell
	for(int i = 0;i < size;++i){		// row
		for(int j = 0;j < size;++j){	// column
			Cells[i][j].SetCharacter('.');
			Cells[i][j].SetRow(i);
			Cells[i][j].SetColumn(j + 97);
		}
	}
}

template<class T>
void HexAdapter<T>::writeToFile() const{
	ofstream SaveFile(filename);
	int intbuffer;
				// Saving to a file by the order
	SaveFile << size << ' ' << enemy << ' ' << turn << ' ' << topnumber << ' ' << botnumber << ' ' << IsTopLineFinished << ' ' << IsBotLineFinished;
	if(enemy == 2)
	SaveFile << ' ' << topchar << ' ' << botchar << endl;
	else
	SaveFile << endl;
	for(int i = 0;i < GetSize();++i){
		for(int j = 0;j < GetSize();++j){
			intbuffer = static_cast<int>(Cells[i][j].GetCharacter());
			SaveFile << intbuffer << ' ';
		}
		SaveFile << endl;
	}
	cout << "Your game is saved to file: " << filename << endl;
	SaveFile.close();
}

template<class T>
void HexAdapter<T>::readFromFile(){
	ifstream LoadFile(filename);
	int intbuffer;
	if(!LoadFile.is_open()){
		cerr << endl << "File couldn't open" << endl;
		exit(1);
	}
					//   Reading file by the order
	LoadFile >> size >> enemy >> turn >> topnumber >> botnumber >> IsTopLineFinished >> IsBotLineFinished;
	if(enemy == 2)
		LoadFile >> topchar >> botchar;

	setSize();
	for(int i = 0;i < GetSize();++i){
		for(int j = 0;j < GetSize();++j){
			LoadFile >> intbuffer;
			Cells[i][j].SetCharacter(static_cast<char>(intbuffer));
		}
	}
	LoadFile.close();
	cout << "\nNew board is:\n\n";
	print();
	LastMove.SetCharacter('x');
}

template<class T>
void HexAdapter<T>::reset(){
	IsItFirstMove = true;
	turn = 1;
	NumberOfMoves = 0;
	GameOver = false;
	IsTopLineFinished = false;
	IsBotLineFinished = false;
	temp.SetCharacter('x');
	int tempsize;
	if(LastMove.GetCharacter() == '.'){
		while(true){		//	continues until user enters a number thats bigger than 5
			cout << "Enter the size of your table.(6 - infinity)\n";
			cin >> size;
			if(size < 6){
				cerr << "Enter a number that is bigger than 5.\n\n";
				continue;
			}
			break;
		}
	}
	LastMove.SetCharacter('.');
	setSize();
											// filling class Cell
	for(int i = 0;i < GetSize();++i){		// row
		for(int j = 0;j < GetSize();++j){	// column
			Cells[i][j].SetCharacter('.');
			Cells[i][j].SetRow(i);
			Cells[i][j].SetColumn(j + 97);
		}
	}
}

template<class T>
bool HexAdapter<T>::isEnd(){
	int i;
	char symbol;
	if(turn == 1){ 			 // Left-Right walls
		symbol = 'x';
		for(i = 0;i < GetSize();i++){			// if there is a symbol in the left column it starts to search for others
			if(Cells[i][0].GetCharacter() == symbol)
		        FindSymbol(0,i,symbol);
		}
	}
	else if(turn == 2){		// top-bot walls
		symbol = 'o';
		for(i = 0;i < GetSize();i++){		// if there is a symbol in the bot row it starts to search for others
			if(Cells[0][i].GetCharacter() == symbol)
		        FindSymbol(i,0,symbol);
		}
	}
	return GameOver;
}

template<class T>
AbstractHex::Cell& HexAdapter<T>::Play(){
	temp.SetCharacter('o');
	if(IsItFirstMove == true){
		IsItFirstMove = false;
		if(64 + size == temp.GetColumn())  // If at first move player selects the rightmost column
			temp.SetColumn(temp.GetColumn() - 1);
		else
			temp.SetColumn(temp.GetColumn() + 1);	// In first move program selects the 'right' of the player
		Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		topchar = botchar = temp.GetColumn();					// Program stores the rightmost, leftmost
		topnumber = botnumber = temp.GetRow();					// lowest and highest selection and continues from that point
		cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		if(temp.GetRow() == GetSize())
			IsTopLineFinished = true;
		else if(temp.GetRow() - 1 == 0)
			IsBotLineFinished = true;
	}
	else{
		if(temp.GetRow() > topnumber && IsTopLineFinished == false){			// if player selects higher temp.row from the topnumber, computer goes to down
			temp.SetRow(topnumber);
			temp.SetColumn(topchar);
			temp.SetCharacter('o');
			BotPointPlace(true);			// if there is a symbol in the top row program goes to bot
			cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		}
		else if(temp.GetRow() < botnumber && IsBotLineFinished == false){		// if lower from the botnumber goes to up
			temp.SetRow(botnumber);
			temp.SetColumn(botchar);
			temp.SetCharacter('o');
			BotPointPlace(false);		// if there is a symbol in the bot row program goes to top
			cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		}
		else{		// if between top and bot numbers program first tries to go to up, if there is already a symbol in the top row it goes to bottom
			if(IsTopLineFinished == false){
				temp.SetRow(topnumber);
				temp.SetColumn(topchar);
				temp.SetCharacter('o');
				BotPointPlace(true);
				cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
			}
			else{
				temp.SetRow(botnumber);
				temp.SetColumn(botchar);
				temp.SetCharacter('o');
				BotPointPlace(false);
				cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
			}
		}
	}
	if(isEnd() == true)
		cout << "\n\n\n        It's Over Anakin\n" << "        You lost against a bot that is not that smart \n" << "        Shame of you\n\n";
	++NumberOfMoves;
	return Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())];
}

template<class T>
bool HexAdapter<T>::Play(Cell& control){

	if(AnswerControl(control) == false){
		cerr << "Enter a proper answer.\n";
		return false;
	}
		
	if(PointControl(control) == false){
		cerr << "That point is not empty.\n\n";
		return false;
	}
	if(turn == 1)
		Cells[control.GetRow() - 1][CharToint(control.GetColumn())].SetCharacter('x');
	else if(turn == 2)
		Cells[control.GetRow() - 1][CharToint(control.GetColumn())].SetCharacter('o');
	
	cout << "Player " << turn << " entered '" << control.GetColumn() << control.GetRow() << "' \n";

	if(isEnd() == true)
		cout << "\n\n\n        It's Over Anakin\n\n";
	
	if(enemy == 2){
		temp.SetCharacter('o');
		temp.SetColumn(control.GetColumn());
		temp.SetRow(control.GetRow());
	}
	++NumberOfMoves;
	return true;
}
		
template<class T>
char HexAdapter<T>::operator()(char c, int i){
	if(c > 96 && c < 96 + GetSize() + 1)
		c -= 32;	 // in case player plays with lowercases
	if(c < 65 || c > 64 + GetSize() || i < 1 || i > GetSize())
		throw WrongIndex();
	return Cells[i - 1][CharToint(c)].GetCharacter();
}

template<class T>
void HexAdapter<T>::PlayGame(){
	if(GameOver == true){
		cout << "This game already ended\n";
		return;
	}
	char column = ' ';
	int row = 0;
	int tempsize;
	if(GetSize() == 0){			//  this means user already loaded a game with constructor
		while(true){		//	continues until user enters a number thats bigger than 5
			cout << "Enter the size of your table.(6 - infinity)\n";
			cin >> tempsize;
			if(tempsize < 6){
				cerr << "Enter a number that is bigger than 5.\n\n";
				continue;
			}
			setSizeValue(tempsize);
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
		setSize();
											// filling class Cell
		for(int i = 0;i < GetSize();++i){		// row
			for(int j = 0;j < GetSize();++j){	// column
				Cells[i][j].SetCharacter('.');
				Cells[i][j].SetRow(i);
				Cells[i][j].SetColumn(j + 97);
			}
		}
	}
	if(enemy == 1){
		print();
		while(GameOver == false){
			while(turn == 1){
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										
					readFromFile();
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
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){
					cin.clear();	//	reset game
					cin.clear();
					cin.ignore(10,'\n');	
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){
					cin.clear();	//  last move
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){
					cin.clear();	// number of moves
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetCharacter('x');
				temp.SetColumn(column);
				temp.SetRow(row);
				if(Play(temp) == true){	// returns GameOver
					temp.SetCharacter('x');
					LastMove.SetCharacter('x');
					LastMove.SetColumn(column);
					LastMove.SetRow(row);
					break;
				}
			}
			if(LastMove.GetCharacter() == '.')
				continue;
			if(GameOver == true)
				break;
			print();
			turn = 2;
			
			while(turn == 2){
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										//						---------------------------
					readFromFile();
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
					cin.clear();		// quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');	
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){
					cin.clear();	//  quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetCharacter('o');
				temp.SetColumn(column);
				temp.SetRow(row);
				if(Play(temp) == true){
					temp.SetCharacter('o');
					LastMove.SetCharacter('o');
					LastMove.SetColumn(column);
					LastMove.SetRow(row);
					break;
				}
			}
			if(GameOver == true)
				break;
			turn = 1;
			print();
		}
	}
	else if(enemy == 2){
		while(GameOver == false){
			turn = 1;
			print();
			while(true){
				temp.SetCharacter('x');
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										//						---------------------------
					readFromFile();
					IsItFirstMove = false;
					if(enemy == 1){
						cerr << "You entered a game with pvp\n";
						exit(1);
					}
						continue;
				}
				else if(row == 0 && (column == 'q' || column == 'Q')){
					cin.clear();		// quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){
					cin.clear();	//  quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetColumn(column);
				temp.SetRow(row);
				Play(temp);
				break;
			}
			if(temp.GetCharacter() == 'x')
				continue;
			if(GameOver == true)
				break;
			turn = 2;
			Play();
			LastMove.SetCharacter('o');
			LastMove.SetColumn(temp.GetColumn());
			LastMove.SetRow(temp.GetRow());
		}
	}
	print();
}

template<class T>
bool HexAdapter<T>::PointControl(Cell control)const{
	if(Cells[control.GetRow() - 1][CharToint(control.GetColumn())].GetCharacter() == '.')
		return true;
	else
		return false;
}
		
template<class T>
void HexAdapter<T>::FindSymbol(int x,int y, const char symbol){
	Cells[y][x].SetCharacter(toupper(symbol));							// previous 'x' to 'X' to prevent jumping back.
	if(turn == 1 && x == GetSize() - 1){											// End of the functions if game doesnt end program lowercase them again.
		GameOver = true;
		return;
	}
	if(turn == 2 && y == GetSize() - 1){
		GameOver = true;
		return;
	}
						// Searches 6 different ways for lowercase symbols by the PlayerType and controls if numbers (x and y) are out of the array

	if(y - 1 > -1 && Cells[y - 1][x].GetCharacter() == symbol) FindSymbol(x,y-1,symbol);
	if(y - 1 > -1  && x + 1 < size && Cells[y - 1][x + 1].GetCharacter() == symbol) FindSymbol(x + 1,y - 1,symbol);
	if(x + 1 < size && Cells[y][x + 1].GetCharacter() == symbol) FindSymbol(x + 1,y,symbol);
	if(y + 1 < size && Cells[y + 1][x].GetCharacter() == symbol) FindSymbol(x,y + 1,symbol);
	if(y + 1 < size && x - 1 > -1 && Cells[y + 1][x - 1].GetCharacter() == symbol) FindSymbol(x - 1,y + 1,symbol);
	if(x - 1 > -1 && Cells[y][x - 1].GetCharacter() == symbol) FindSymbol(x - 1,y,symbol);

	if(GameOver == false)
		Cells[y][x].SetCharacter(symbol);	//  If game doesnt end, program lowercases for next time}
}

template<class T>
void HexAdapter<T>::BotPointPlace(bool IsItTop){
	             					// Places the next symbol by the IsItTop selection
	if(IsItTop == true){			// Function first tries to go straight up or down
		++topnumber;
		temp.SetRow(topnumber);					// then tries cross paths

									// at last computes the shortest way between right and left and goes that way
		if(PointControl(temp) == true){											// to down
			Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - 1, temp.GetRow())) == true && CharToint(temp.GetColumn()) - 1 > -1){	// cross
			temp.SetColumn(temp.GetColumn() - 1);
			--topchar;
			Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else{
			direction = FindBestWayDirection(temp);						// right or left			
			--topnumber;
			temp.SetRow(topnumber);

			if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + direction, temp.GetRow())) == true){
				temp.SetColumn(temp.GetColumn() + direction);
				topchar +=direction;
				Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
			else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - direction, temp.GetRow())) == true){
				temp.SetColumn(temp.GetColumn() - direction);
				topchar -= direction;
				Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
		}				
		if(temp.GetRow() == size)			// this means computer placed a symbol to last line
			IsTopLineFinished = true;
	}

	else if(IsItTop == false){
		temp.SetRow(temp.GetRow() - 1);
		--botnumber;
		
		if(PointControl(temp) == true)											// to up
			Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + 1, temp.GetRow())) == 1  && CharToint(temp.GetColumn()) + 1 < 12){	// cross
			++botchar;
			temp.SetColumn(botchar);
			Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else{
			direction = FindBestWayDirection(temp);						// right or left			
			++botnumber;
			temp.SetRow(botnumber);

			if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + direction, temp.GetRow())) == true){
				botchar += direction;
				temp.SetColumn(botchar);
				Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
			else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - direction, temp.GetRow())) == 1){
				botchar -= direction;
				temp.SetColumn(botchar);
				Cells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
		}
		if(temp.GetRow() == 0)			// this means computer placed a symbol to first line
			IsBotLineFinished = true;
	}
}

template<class T>
int HexAdapter<T>::FindBestWayDirection(Cell& control){
	int right = 0,left = 0;						//  is not empty, computer computes the shortest way to go
	
	while(Cells[control.GetRow() - 1][CharToint(control.GetColumn()) - left].GetCharacter() == 'x') {
	++left;
	if(control.GetColumn()-left == 'a' || control.GetColumn()-left == 'A'){		// dont know why but deque cant go left of board while vector can
		++left;
		break;
	}
	}
	while(Cells[control.GetRow() - 1][CharToint(control.GetColumn()) + right].GetCharacter() == 'x') ++right;
	
	if(CharToint(control.GetColumn()) + right == GetSize())
		return -1;		// end of array
	else if(CharToint(control.GetColumn()) - left == -1) 
		return 1;	// end of array
	else if(right > left)
		return -1;
	else 
		return 1;
}
		






class HexArray1D: public AbstractHex{
	public:
		HexArray1D();
		~HexArray1D();
		HexArray1D(string tempfilename);
		HexArray1D(const HexArray1D &other);
		void print();
		void setSize();
		void writeToFile() const;
		void readFromFile();
		void reset();
		bool isEnd();
		Cell& Play();
		bool Play(Cell& control);
		virtual char operator()(char c, int i);
		
		void PlayGame();
		bool PointControl(Cell control)const;
		void FindSymbol(int x,int y, const char symbol);
		void BotPointPlace(bool IsItTop);
		int FindBestWayDirection(Cell& control);

		
	private:
		Cell *Cells;
};


HexArray1D::HexArray1D()
	:AbstractHex(), Cells(nullptr)
		{ /* Left blank on purpose */ };

HexArray1D::~HexArray1D(){
	delete [] Cells;
}

HexArray1D::HexArray1D(string tempfilename)
   :AbstractHex(tempfilename){
   	readFromFile();
}

HexArray1D::HexArray1D(const HexArray1D &other)
	:AbstractHex(other){
	int size;
		size = other.size;
		NumberOfMoves = other.NumberOfMoves;
		topchar = other.topchar; 
		botchar = other.botchar;
		GameOver = other.GameOver;
		IsItFirstMove = other.IsItFirstMove;
		IsTopLineFinished = other.IsTopLineFinished;
		IsBotLineFinished = other.IsBotLineFinished;
		enemy = other.enemy;
		turn = other.turn;
		topnumber = other.topnumber;
		botnumber = other.botnumber;
		direction = other.direction;
		LastMove = other.LastMove;
		temp = other.temp;
		setSize();
		for(int i = 0;i < GetSize();++i)
			for(int j = 0;j < GetSize();++j)
					Cells[i * GetSize() + j] = other.Cells[i * GetSize() + j];
		
}

void HexArray1D::print(){
	cout << "  ";
	for(int i = 0;i < GetSize();i++)
		cout << static_cast<char>(i + 65) << " ";
	cout << "\n";
	for(int i = 0;i < GetSize();i++){
		cout << i+1;
		for(int k = 0;k < i+1;k++){
			if(k == 9) continue;		// because of the two digit numbers
			cout << " ";
		}
		for(int j = 0;j < GetSize();j++)
			cout << Cells[i * GetSize() + j].GetCharacter() << ' ';
		cout << "\n";
	}
}

char HexArray1D::operator()(char c, int i){
	if(c > 96 && c < 96 + GetSize() + 1)
		c -= 32;	 // in case player plays with lowercases
	if(c < 65 || c > 64 + GetSize() || i < 1 || i > GetSize())
		throw WrongIndex();	
	return Cells[(i - 1) * GetSize() + CharToint(c)].GetCharacter();
}

void HexArray1D::reset(){
		IsItFirstMove = true;
		turn = 1;
		NumberOfMoves = 0;
		GameOver = false;
		IsTopLineFinished = false;
		IsBotLineFinished = false;
		temp.SetCharacter('x');
		int tempsize;
		if(LastMove.GetCharacter() == '.'){
			while(true){		//	continues until user enters a number thats bigger than 5
				cout << "Enter the size of your table.(6 - infinity)\n";
				cin >> size;
				if(size < 6){
					cerr << "Enter a number that is bigger than 5.\n\n";
					continue;
				}
				break;
			}
		}
		LastMove.SetCharacter('.');
		setSize();
}

bool HexArray1D::PointControl(Cell control)const{/*                            Now i get Cell as parameter						*/
	if(Cells[(control.GetRow() - 1) * GetSize() + CharToint(control.GetColumn())].GetCharacter() == '.')
		return true;
	else
		return false;	
}

void HexArray1D::setSize(){
	free(Cells);
	Cells = (Cell*)malloc(GetSize() * GetSize() * sizeof(Cell));
	for(int i = 0;i < GetSize();++i){	
		for(int j = 0;j < GetSize();++j){	
														
			Cells[i * GetSize() + j].SetCharacter('.');  // filling class Cell 
			Cells[i * GetSize() + j].SetRow(i);			 // row
			Cells[i * GetSize() + j].SetColumn(j + 97);  // column
		}
	}
}


void HexArray1D::writeToFile()const{
	ofstream SaveFile(filename);
	int intbuffer;
					// Saving to a file by the order
	SaveFile << size << ' ' << enemy << ' ' << turn << ' ' << topnumber << ' ' << botnumber << ' ' << IsTopLineFinished << ' ' << IsBotLineFinished;
	if(enemy == 2)
		SaveFile << ' ' << topchar << ' ' << botchar << endl;
	else
		SaveFile << endl;
	for(int i = 0;i < GetSize();++i){
		for(int j = 0;j < GetSize();++j){
			intbuffer = static_cast<int>(Cells[i * GetSize() + j].GetCharacter());
			SaveFile << intbuffer << ' ';
		}
		SaveFile << endl;
	}
	cout << "Your game is saved to file: " << filename << endl;
	SaveFile.close();
}

void HexArray1D::readFromFile(){
	ifstream LoadFile(filename);
	int intbuffer;
	if(!LoadFile.is_open()){
		cerr << endl << "File couldn't open" << endl;
		exit(1);
	}
					//   Reading file by the order
	LoadFile >> size >> enemy >> turn >> topnumber >> botnumber >> IsTopLineFinished >> IsBotLineFinished;
	if(enemy == 2)
		LoadFile >> topchar >> botchar;


	setSize();
	for(int i = 0;i < GetSize();++i){
		for(int j = 0;j < GetSize();++j){
			LoadFile >> intbuffer;
			Cells[i * GetSize() + j].SetCharacter(static_cast<char>(intbuffer));
		}
	}
	LoadFile.close();
	cout << "\nNew board is:\n\n";
	print();
	LastMove.SetCharacter('x');
}

void HexArray1D::PlayGame(){
	if(GameOver == true){
		cout << "This game already ended\n";
		return;
	}
	char column = ' ';
	int row = 0;
	int tempsize;
	if(GetSize() == 0){			//  this means user already loaded a game with constructor
		while(true){		//	continues until user enters a number thats bigger than 5
			cout << "Enter the size of your table.(6 - infinity)\n";
			cin >> tempsize;
			if(tempsize < 6){
				cerr << "Enter a number that is bigger than 5.\n\n";
				continue;
			}
			setSizeValue(tempsize);
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
		setSize();
	
	}
	if(enemy == 1){
		print();
		while(GameOver == false){
			while(turn == 1){
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										//						---------------------------
					readFromFile();
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
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');	
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){
					cin.clear();	//  quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetCharacter('x');
				temp.SetColumn(column);
				temp.SetRow(row);
				Play(temp);
				temp.SetCharacter('x');
				LastMove.SetCharacter('x');
				LastMove.SetColumn(column);
				LastMove.SetRow(row);
				break;
			}
			print();
				
			if(LastMove.GetCharacter() == '.')
				continue;
			turn = 2;
			
			while(turn == 2){
				if(GameOver)
					break;
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										//						---------------------------
					readFromFile();
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
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');	
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){
					cin.clear();	//  quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetCharacter('o');
				temp.SetColumn(column);
				temp.SetRow(row);
				Play(temp);
					temp.SetCharacter('o');
					LastMove.SetCharacter('o');
					LastMove.SetColumn(column);
					LastMove.SetRow(row);
					break;
				
			}
			turn = 1;
			print();
		}
	}
	else if(enemy == 2){
		while(GameOver == false){
			turn = 1;
			print();
			while(true){
				temp.SetCharacter('x');
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'RESET' for reset the game.\nEnter 'MOVE' for the last move\nEnter 'NUMBER' for number of moves in this board.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					writeToFile();
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										//						---------------------------
					readFromFile();
					IsItFirstMove = false;
					if(enemy == 1){
						cerr << "You entered a game with pvp\n";
						exit(1);
					}
						continue;
				}
				else if(row == 0 && (column == 'q' || column == 'Q')){
					cin.clear();		// quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					return;
				}
				else if(row == 0 && (column == 'r' || column == 'R')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					reset();
					break;
				}
				else if(row == 0 && (column == 'm' || column == 'M')){
					cin.clear();	//  quit to menu
					cin.clear();
					cin.ignore(10,'\n');
					try{
					cout << "\nLast move:"
						 << "\nColumn: " << lastMove().GetColumn()
						 << "\nRow: " << lastMove().GetRow()
						 << "\nCharacter: " << lastMove().GetCharacter()
						 << endl << endl;
					}
					catch(noLastMove){ /* Intentionally Empty */ };
					continue;
				}
				else if(row == 0 && (column == 'n' || column == 'N')){
					cin.clear();
					cin.clear();
					cin.ignore(10,'\n');
					cout << "\nNumber of moves in this board: " << numberOfMoves() << endl;
					continue;
				}
				temp.SetColumn(column);
				temp.SetRow(row);
				
				Play(temp);
				
				break;
			}
			if(temp.GetCharacter() == 'x')
				continue;
			turn = 2;
			Play();
			LastMove.SetCharacter('o');
			LastMove.SetColumn(temp.GetColumn());
			LastMove.SetRow(temp.GetRow());
		}
	}
}

AbstractHex::Cell& HexArray1D::Play(){
	temp.SetCharacter('o');
	if(IsItFirstMove == true){
		IsItFirstMove = false;
		if(64 + size == temp.GetColumn())  // If at first move player selects the rightmost column
			temp.SetColumn(temp.GetColumn() - 1);
		else
			temp.SetColumn(temp.GetColumn() + 1);	// In first move program selects the 'right' of the player
		Cells[(temp.GetRow() - 1) * GetSize()+ CharToint(temp.GetColumn())].SetCharacter('o');
		topchar = botchar = temp.GetColumn();					// Program stores the rightmost, leftmost
		topnumber = botnumber = temp.GetRow();					// lowest and highest selection and continues from that point
		//print();
		cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
	}
	else{
		if(temp.GetRow() > topnumber && IsTopLineFinished == false){			// if player selects higher temp.row from the topnumber, computer goes to down
			temp.SetRow(topnumber);
			temp.SetColumn(topchar);
			temp.SetCharacter('o');
			BotPointPlace(true);			// if there is a symbol in the top row program goes to bot
			//print();
			cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		}
		else if(temp.GetRow() < botnumber && IsBotLineFinished == false){		// if lower from the botnumber goes to up
			temp.SetRow(botnumber);
			temp.SetColumn(botchar);
			temp.SetCharacter('o');
			BotPointPlace(false);		// if there is a symbol in the bot row program goes to top
			//print();
			cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		}
		else{		// if between top and bot numbers program first tries to go to up, if there is already a symbol in the top row it goes to bottom
			if(IsTopLineFinished == false){
				temp.SetRow(topnumber);
				temp.SetColumn(topchar);
				temp.SetCharacter('o');
				BotPointPlace(true);
				//print();
				cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
			}
			else{
				temp.SetRow(botnumber);
				temp.SetColumn(botchar);
				temp.SetCharacter('o');
				BotPointPlace(false);
				//print();
				cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
			}
		}
	}
	if(isEnd() == true){
		cout << "\n\n\n        It's Over Anakin\n" << "        You lost against a bot that is not that smart \n" << "        Shame of you\n\n";
		print();
    }
	++NumberOfMoves;
	return Cells[temp.GetRow() * GetSize() + temp.GetColumn()];											//						+++++++++++++++++++++++++++
}

bool HexArray1D::Play(Cell& control){
	if(AnswerControl(control) == false){
		cerr << "Enter a proper answer.\n";
		print();
		return false;
	}
		
	if(PointControl(control) == false){
		cerr << "That point is not empty.\n\n";
		print();
		return false;
	}
	if(turn == 1)
		Cells[(control.GetRow() - 1) * GetSize() + CharToint(control.GetColumn())].SetCharacter('x');
	else if(turn == 2)
		Cells[(control.GetRow() - 1) * GetSize() + CharToint(control.GetColumn())].SetCharacter('o');
	
	cout << "Player " << turn << " entered '" << control.GetColumn() << control.GetRow() << "' \n";

	if(isEnd() == true)
		cout << "\n\n\n        It's Over Anakin\n\n";
	
	if(enemy == 2){
		temp.SetCharacter('o');
		temp.SetColumn(control.GetColumn());
		temp.SetRow(control.GetRow());
	}
	++NumberOfMoves;
	return true;
}

void HexArray1D::BotPointPlace(bool IsItTop){
	             					// Places the next symbol by the IsItTop selection
	if(IsItTop == true){			// Function first tries to go straight up or down
		++topnumber;
		temp.SetRow(topnumber);					// then tries cross paths

									// at last computes the shortest way between right and left and goes that way
		if(PointControl(temp) == true){											// to down
			Cells[(temp.GetRow() - 1) * GetSize() + CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - 1, temp.GetRow())) == true && CharToint(temp.GetColumn()) - 1 > -1){	// cross
			temp.SetColumn(temp.GetColumn() - 1);
			--topchar;
			Cells[(temp.GetRow() - 1) * GetSize() + CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else{
			direction = FindBestWayDirection(temp);						// right or left			
			--topnumber;
			temp.SetRow(topnumber);

			if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + direction, temp.GetRow())) == true){
				temp.SetColumn(temp.GetColumn() + direction);
				topchar +=direction;
				Cells[(temp.GetRow() - 1) * GetSize() + CharToint(temp.GetColumn())].SetCharacter('o');
			}
			else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - direction, temp.GetRow())) == true){
				temp.SetColumn(temp.GetColumn() - direction);
				topchar -= direction;
				Cells[(temp.GetRow() - 1) * GetSize() + CharToint(temp.GetColumn())].SetCharacter('o');
			}
		}				
		if(temp.GetRow() == size)			// this means computer placed a symbol to last line
			IsTopLineFinished = true;
	}

	else if(IsItTop == false){
		temp.SetRow(temp.GetRow() - 1);
		--botnumber;
		
		if(PointControl(temp) == true)											// to up
			Cells[(temp.GetRow() - 1) * GetSize() + CharToint(temp.GetColumn())].SetCharacter('o');
		else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + 1, temp.GetRow())) == 1  && CharToint(temp.GetColumn()) + 1 < 12){	// cross
			++botchar;
			temp.SetColumn(botchar);
			Cells[(temp.GetRow() - 1) * GetSize() + CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else{
			direction = FindBestWayDirection(temp);						// right or left			
			++botnumber;
			temp.SetRow(botnumber);

			if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + direction, temp.GetRow())) == true){
				botchar += direction;
				temp.SetColumn(botchar);
				Cells[(temp.GetRow() - 1) * GetSize() + CharToint(temp.GetColumn())].SetCharacter('o');
			}
			else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - direction, temp.GetRow())) == 1){
				botchar -= direction;
				temp.SetColumn(botchar);
				Cells[(temp.GetRow() - 1) * GetSize() + CharToint(temp.GetColumn())].SetCharacter('o');
			}
		}
		if(temp.GetRow() == 0)			// this means computer placed a symbol to first line
			IsBotLineFinished = true;
	}
}

int HexArray1D::FindBestWayDirection(Cell& control){	//	for computer, if top-topleft-topright or bot-botleft-botright
	int right = 0,left = 0;						//  is not empty, computer computes the shortest way to go
	
		
	while(Cells[(control.GetRow() - 1) * GetSize() + CharToint(control.GetColumn()) + right].GetCharacter() == 'x') ++right;
	while(Cells[(control.GetRow() - 1) * GetSize() + CharToint(control.GetColumn()) - left].GetCharacter() == 'x') ++left;
	if(CharToint(control.GetColumn()) + right == GetSize())
		return -1;		// end of array
	else if(CharToint(control.GetColumn()) - left == -1) 
		return 1;	// end of array
	else if(right > left)
		return -1;
	else 
		return 1;
}

bool HexArray1D::isEnd(){
	int i;
	char symbol;
	if(turn == 1){ 			 // Left-Right walls
		symbol = 'x';
		for(i = 0;i < size;i++){			// if there is a symbol in the left column it starts to search for others
			if(Cells[i * GetSize()].GetCharacter() == symbol)
				FindSymbol(0,i,symbol);
		}
		
	}
	else if(turn == 2){		// top-bot walls
		symbol = 'o';
		for(i = 0;i < GetSize();i++){		// if there is a symbol in the bot row it starts to search for others
			if(Cells[i].GetCharacter() == symbol)
				FindSymbol(i,0,symbol);
		}
	}
	
	return GameOver;
}

void HexArray1D::FindSymbol(int x,int y, const char symbol){						// for player1 searches for 'x', jumps to that 'x' and uppercase the 
	Cells[y * GetSize() + x].SetCharacter(toupper(symbol));							// previous 'x' to 'X' to prevent jumping back.
	if(turn == 1 && x == GetSize() - 1){											// End of the functions if game doesnt end program lowercase them again.
		GameOver = true;
		return;
	}
	if(turn == 2 && y == size - 1){
		GameOver = true;
		return;
	}
						// Searches 6 different ways for lowercase symbols by the PlayerType and controls if numbers (x and y) are out of the array

	if(y - 1 > -1 && Cells[(y - 1) * GetSize() + x].GetCharacter() == symbol) FindSymbol(x,y-1,symbol);
	if(y - 1 > -1  && x + 1 < size && Cells[(y - 1) * GetSize() + x + 1].GetCharacter() == symbol) FindSymbol(x + 1,y - 1,symbol);
	if(x + 1 < size && Cells[y * GetSize() + x + 1].GetCharacter() == symbol) FindSymbol(x + 1,y,symbol);
	if(y + 1 < size && Cells[(y + 1) * GetSize() + x].GetCharacter() == symbol) FindSymbol(x,y + 1,symbol);
	if(y + 1 < size && x - 1 > -1 && Cells[(y + 1) * GetSize() + x - 1].GetCharacter() == symbol) FindSymbol(x - 1,y + 1,symbol);
	if(x - 1 > -1 && Cells[y * GetSize() + x - 1].GetCharacter() == symbol) FindSymbol(x - 1,y,symbol);

	if(GameOver == false)
		Cells[y * GetSize() + x].SetCharacter(symbol);	//  If game doesnt end, program lowercases for next time
}




bool IsValid(AbstractHex **Hexes){
	int Xcount = 0, Ocount = 0;
	for(int k = 0;k < 4;++k){		
		Xcount = 0;
		Ocount = 0;
		if(*(Hexes + k) == nullptr)		// if game is empty doesnt look to that
			continue;
		for(int i = 65;i < 65 + (*(Hexes + k))->GetSize();++i){		// controls if there is any other char than 'x','o' or ','
			for(int j = 1;j <= (*(Hexes + k))->GetSize();++j){
				if((*(Hexes + k))->operator()(i,j) == 'x' || (*(Hexes + k))->operator()(i,j) == 'X')
					++Xcount;
				else if((*(Hexes + k))->operator()(i,j) == 'o' || (*(Hexes + k))->operator()(i,j) == 'O')
					++Ocount;
				else if((*(Hexes + k))->operator()(i,j) != '.')
					return false;
			}
		}
			if(Xcount != Ocount && Xcount + 1 != Ocount && Xcount != Ocount + 1) // control if there is one that has more 'x's or 'o's (1 difference is okey since player 1 can win before player 2 plays)
				return false;
	}
	return true;
}



int main(){

	int game1, game2;
	int sayz;
	int choice = 1, i = 0;
	
	AbstractHex **Hexes = new AbstractHex*[4];
	for(int k = 0;k < 4;++k)
		*(Hexes + k) = nullptr;
	HexVector *a;
			a = new HexVector;
	HexArray1D *b;
			b = new HexArray1D;
	HexAdapter <vector<vector<AbstractHex::Cell> > > *c;
			c = new HexAdapter <vector<vector<AbstractHex::Cell> > >;
	HexAdapter <deque<deque<AbstractHex::Cell> > > *d;
			d = new HexAdapter <deque<deque<AbstractHex::Cell> > >;
	
	while(choice != -1){
		cout << "0 for driver code.\n\n" 
			 << "Choose your game type:\n"
			 <<	"1 -> HexVector\n"
			 << "2 -> HexArray1D\n"
			 << "3 -> HexAdapter<vector>\n"
			 << "4 -> HexAdapter<deque>\n"
			 << "        OR\n"
			 << "5 -> IsValid function\n"
			 << "6 -> Compare games.\n"
			 << "-1 -> End program.\n\n";
		cin >> choice;
		if(choice == 0){
			cout << "--------------------------------\n";
			cout << "          HexVector \n\n" ;
			cout << "readFromFile() and print():";
			HexVector e("patates.txt");
			
			cout << "\nValue of A2 with operator(): A2 ->" <<e('A',2);
			
			cout << "\n\nreset()\n";			
			e.reset();
			e.print();
			
			cout << "\n\nwriteToFile file1.txt\n";
			e.setFilename("file1.txt");
			e.writeToFile();
			
			cout << "\n\nsetSize()\n";
			e.setSizeValue(12);
			e.setSize();
			e.print();
			
			cout << "\n\nValue of IsEnd(): " << e.isEnd();
			cout << "\n\nPress enter to next test.\n\n";
			getchar();
			getchar();
			  
			cout << "--------------------------------\n";
			cout << "          HexArray1D \n\n" ;
			cout << "readFromFile() and print():";
			HexArray1D *f = new HexArray1D("patates.txt");
			
			cout << "\nValue of A2 with operator(): B2 ->" <<f->operator()('b',2);
			
			cout << "\n\nreset()\n";			
			f->reset();
			f->print();
			
			cout << "\n\nwriteToFile file2.txt\n";
			f->setFilename("file2.txt");
			f->writeToFile();
			
			cout << "\n\nsetSize()\n";
			f->setSizeValue(13);
			f->setSize();
			f->print();
			
			cout << "\n\nValue of IsEnd(): " << f->isEnd();
			cout << "\n\nPress enter to next test.\n\n";
			getchar();
			getchar();
		  
			cout << "--------------------------------\n";
			cout << "          HexAdapter->vector \n\n" ;
			cout << "readFromFile() and print():";
			HexAdapter <vector<vector<AbstractHex::Cell> > > g("patates.txt");
			
			cout << "\nValue of A2 with operator(): C2 ->" <<g('c',2);
			
			cout << "\n\nreset()\n";			
			g.reset();
			g.print();
			
			cout << "\n\nwriteToFile file3.txt\n";
			g.setFilename("file3.txt");
			g.writeToFile();
			
			cout << "\n\nsetSize()\n";
			g.setSizeValue(14);
			g.setSize();
			g.print();
			
			cout << "\n\nValue of IsEnd(): " << g.isEnd();
			cout << "\n\nPress enter to next test.\n\n";
			getchar();
			getchar();
			
			cout << "          HexAdapter->deque \n\n" ;
			cout << "readFromFile() and print():";
			HexAdapter <deque<deque<AbstractHex::Cell> > > h("patates.txt");
			
			cout << "\nValue of A2 with operator(): C3 ->" <<h('c',3);
			
			cout << "\n\nreset()\n";			
			h.reset();
			h.print();
			
			cout << "\n\nwriteToFile file4.txt\n";
			h.setFilename("file4.txt");
			h.writeToFile();
			
			cout << "\n\nsetSize()\n";
			h.setSizeValue(15);
			h.setSize();
			h.print();
			
			cout << "\n\nValue of IsEnd(): " << h.isEnd();
			cout << "\n--------------------------------\n";
			
			AbstractHex **HexesDriver = new AbstractHex*[4];
			*(HexesDriver) = dynamic_cast<AbstractHex*>(&e);
			*(HexesDriver + 1) = dynamic_cast<AbstractHex*>(f);
			*(HexesDriver + 2) = dynamic_cast<AbstractHex*>(&g);
			*(HexesDriver + 3) = dynamic_cast<AbstractHex*>(&h);
			cout << "\n\nIsValid() of driver codes: " << IsValid(HexesDriver);
			
			cout << "\n\nCompare of games with ==:\n1-3 -> ";
			if(Hexes[0] == Hexes[2])
				cout << "Games are equal.\n\n";
			else
				cout << "Games are not equal.\n\n";
				
			getchar();
			getchar();
			cout << "\n\nPress enter to end test.\n\n";
			
		}
		else if(choice == 1){
			a->PlayGame();
			*Hexes = dynamic_cast<AbstractHex*>(a);
		}
		else if(choice == 2){
			b->PlayGame();
			*(Hexes + 1) = dynamic_cast<AbstractHex*>(b);
		}
		else if(choice == 3){
			c->PlayGame();
			*(Hexes + 2) = dynamic_cast<AbstractHex*>(c);
		}
		else if(choice == 4){
			d->PlayGame();
			*(Hexes + 3) = dynamic_cast<AbstractHex*>(d);
		}
		else if(choice == 5){
			if(IsValid(Hexes))
				cout << "\nYes, all entered games are valid.\n\n";
			else
				cout << "\nNo, there is a not valid game.\n\n";
		}
		else if(choice == 6){
			cout << "Enter two games between 1-4 according to menu game type numbers.\n";
			cin >> game1 >> game2;
			if(*Hexes[game1 - 1] == *Hexes[game2 -1])
				cout << "Games are equal.\n\n";
			else
				cout << "Games are not equal.\n\n";
			continue;
		}
		else if((choice < 1 || choice > 4) && choice != -1){
			cerr << "Enter between 1-5\n\n";
		}
	}
}	
