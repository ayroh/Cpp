#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "header.h"
using namespace std;

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
		Resize();											//						++++++++++++++++++++++++++++
											// filling class Cell
		for(int i = 0;i < size;++i){		// row
			for(int j = 0;j < size;++j){	// column
				hexCells[i][j].SetCharacter('.');
				hexCells[i][j].SetRow(i);
				hexCells[i][j].SetColumn(j + 97);
			}
		}
	}
	DisplayBoard();
	if(enemy == 1){
		while(GameOver == false){
			while(true){
				cout << "\nEnter your move Player 1  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'UNDO' for undo last move.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					ofstream save(filename);
					save << *this;											//						---------------------------
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');	
					cin >> filename;										//						---------------------------
					ifstream load(filename);
					load >> *this;											//						---------------------------
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
				else if(row == 0 && (column == 'u' || column == 'U')){				//						---------------------------
					cin.clear();
					cin.ignore(10,'\n');
					--(*this);
					DisplayBoard();
					break;
				}																	//						---------------------------
				temp.SetCharacter('x');
				temp.SetColumn(column);
				temp.SetRow(row);
				if(Play(temp) == true){	// returns GameOver
					temp.SetCharacter('x');
					FillSteps(temp);											//						++++++++++++++++++++++++++++
					break;
				}
			}
			turn = 2;
			
			while(true){
				cout << "\nEnter your move Player 2  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'UNDO' for undo last move.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					ofstream save(filename);
					save << *this;											//						---------------------------
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with LOAD, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;										//						---------------------------
					ifstream load(filename);
					load >> *this;											//						---------------------------
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
				else if(row == 0 && (column == 'u' || column == 'U')){				//					----------------------------
					cin.clear();	
					cin.ignore(10,'\n');
					--(*this);															
					DisplayBoard();
					break;
				}																	//					----------------------------
				temp.SetCharacter('o');
				temp.SetColumn(column);
				temp.SetRow(row);
				if(Play(temp) == true){	// returns GameOver
					temp.SetCharacter('o');
					FillSteps(temp);												//							+++++++++++++++++++++++++++
					break;
				}
			}
			turn = 1;
		}
	}
	else if(enemy == 2){
		while(GameOver == false){
			turn = 1;
			while(true){
				cout << "\nEnter your move Player 1  (C 5 etc.).\nEnter 'SAVE 'filename'' for save the game.\nEnter 'LOAD 'filename'' for load a game 		(if you play pvp load pvp, pve load pve).\nEnter 'UNDO' for undo last move.\nEnter 'QUIT' for back to menu\n\n";
				cin >> column >> row;
				if(row == 0 && (column == 's' || column == 'S')){
					cin.clear();			//	with SAVE, 'character' takes 'S' and 'number' takes 'A' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;							//									---------------------
					ofstream save(filename);
					save << *this;								//									---------------------
					continue;
				}
				else if(row == 0 && (column == 'l' || column == 'L')){
					cin.clear();			//	with SAVE, 'character' takes 'L' and 'number' takes 'O' after that i clear cin buffer until ' ' so other name will be my filename automatically
					cin.ignore(10,' ');
					cin >> filename;							//									-----------------------
					ifstream load(filename);
					load >> *this;								//									-----------------------
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
				else if(row == 0 && (column == 'u' || column == 'U')){//					------------------------
					cin.clear();
					cin.ignore(10,'\n');
					--(*this);
					turn = 3;
					(*this)--;
					ReadUndoBot();
					DisplayBoard();
					continue;
				}														//					------------------------
				temp.SetCharacter('x');
				temp.SetColumn(column);
				temp.SetRow(row);
				if(Play(temp) == true){	// returns GameOver
					temp.SetCharacter('x');
					FillSteps(temp);									//			++++++++++++++++++++++++++++++++
					break;
				}
			}
			turn = 2;
			Play();
			FillSteps(temp);											//						+++++++++++++++++++++++++
			AddBot();													//						+++++++++++++++++++++++++
		}
	}
	DisplayBoard();
}


void Hex::BotPointPlace(bool IsItTop){
	             					// Places the next symbol by the IsItTop selection
	if(IsItTop == true){			// Function first tries to go straight up or down
		++topnumber;
		temp.SetRow(topnumber);					// then tries cross paths

									// at last computes the shortest way between right and left and goes that way
		if(PointControl(temp) == true){											// to down
			hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - 1, temp.GetRow())) == true && CharToint(temp.GetColumn()) - 1 > -1){	// cross
			temp.SetColumn(temp.GetColumn() - 1);
			--topchar;
			hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else{
			direction = FindBestWayDirection(temp);						// right or left			
			--topnumber;
			temp.SetRow(topnumber);

			if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + direction, temp.GetRow())) == true){
				temp.SetColumn(temp.GetColumn() + direction);
				topchar +=direction;
				hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
			else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - direction, temp.GetRow())) == true){
				temp.SetColumn(temp.GetColumn() - direction);
				topchar -= direction;
				hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
		}				
		if(temp.GetRow() == size)			// this means computer placed a symbol to last line
			IsTopLineFinished = true;
	}

	else if(IsItTop == false){
		temp.SetRow(temp.GetRow() - 1);
		--botnumber;
		
		if(PointControl(temp) == true)											// to up
			hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + 1, temp.GetRow())) == 1  && CharToint(temp.GetColumn()) + 1 < 12){	// cross
			++botchar;
			temp.SetColumn(botchar);
			hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		}
		else{
			direction = FindBestWayDirection(temp);						// right or left			
			++botnumber;
			temp.SetRow(botnumber);

			if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() + direction, temp.GetRow())) == true){
				botchar += direction;
				temp.SetColumn(botchar);
				hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
			else if(PointControl(Cell(temp.GetCharacter(), temp.GetColumn() - direction, temp.GetRow())) == 1){
				botchar -= direction;
				temp.SetColumn(botchar);
				hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
			}
		}
		if(temp.GetRow() == 0)			// this means computer placed a symbol to first line
			IsBotLineFinished = true;
	}
}


Hex::Cell& Hex::Play(){
	temp.SetCharacter('o');
	if(IsItFirstMove == true){
		IsItFirstMove = false;
		if(64 + size == temp.GetColumn())  // If at first move player selects the rightmost column
			temp.SetColumn(temp.GetColumn() - 1);
		else
			temp.SetColumn(temp.GetColumn() + 1);	// In first move program selects the 'right' of the player
		hexCells[temp.GetRow() - 1][CharToint(temp.GetColumn())].SetCharacter('o');
		topchar = botchar = temp.GetColumn();					// Program stores the rightmost, leftmost
		topnumber = botnumber = temp.GetRow();					// lowest and highest selection and continues from that point
		DisplayBoard();
		cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
	}
	else{
		if(temp.GetRow() > topnumber && IsTopLineFinished == false){			// if player selects higher temp.row from the topnumber, computer goes to down
			temp.SetRow(topnumber);
			temp.SetColumn(topchar);
			temp.SetCharacter('o');
			BotPointPlace(true);			// if there is a symbol in the top row program goes to bot
			DisplayBoard();
			cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		}
		else if(temp.GetRow() < botnumber && IsBotLineFinished == false){		// if lower from the botnumber goes to up
			temp.SetRow(botnumber);
			temp.SetColumn(botchar);
			temp.SetCharacter('o');
			BotPointPlace(false);		// if there is a symbol in the bot row program goes to top
			DisplayBoard();
			cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
		}
		else{		// if between top and bot numbers program first tries to go to up, if there is already a symbol in the top row it goes to bottom
			if(IsTopLineFinished == false){
				temp.SetRow(topnumber);
				temp.SetColumn(topchar);
				temp.SetCharacter('o');
				BotPointPlace(true);
				DisplayBoard();
				cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
			}
			else{
				temp.SetRow(botnumber);
				temp.SetColumn(botchar);
				temp.SetCharacter('o');
				BotPointPlace(false);
				DisplayBoard();
				cout << "Computer entered '" << temp.GetColumn() << temp.GetRow() << "' \n";
			}
		}
	}
	if(isGameFinished() == true)
		cout << "\n\n\n        It's Over Anakin\n" << "        You lost against a bot that is not that smart \n" << "        Shame of you\n\n";
	++MarkedCells;
	return hexCells[temp.GetRow()][temp.GetColumn()];											//						+++++++++++++++++++++++++++
}

bool Hex::Play(Cell& control){/*                            Now i get Cell as parameter						*/

	if(AnswerControl(control) == false){
		cerr << "Enter a proper answer.\n";
		return false;
	}
		
	if(PointControl(control) == false){
		cerr << "That point is not empty.\n\n";
		return false;
	}
	if(turn == 1)
		hexCells[control.GetRow() - 1][CharToint(control.GetColumn())].SetCharacter('x');
	else if(turn == 2)
		hexCells[control.GetRow() - 1][CharToint(control.GetColumn())].SetCharacter('o');
	
	cout << "Player " << GetTurn() << " entered '" << control.GetColumn() << control.GetRow() << "' \n";

	if(isGameFinished() == true)
		cout << "\n\n\n        It's Over Anakin\n\n";
	
	if(enemy == 2){
		temp.SetCharacter('o');
		temp.SetColumn(control.GetColumn());
		temp.SetRow(control.GetRow());
	}
	else
		DisplayBoard();
	++MarkedCells;
	return true;
}

void Hex::DisplayBoard() const{
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
			cout << hexCells[i][j].GetCharacter() << ' ';
		cout << "\n";
	}
}


bool Hex::AnswerControl(Cell& control){		/*                            Now i get Cell as parameter						*/
	if(control.GetColumn() > 96 && control.GetColumn() < 96 + GetSize() + 1)
		control.SetColumn(control.GetColumn() - 32);	 // in case player plays with lowercases
	
	if(control.GetRow() > GetSize() || control.GetRow() < 1 || control.GetColumn() > 65 + GetSize() - 1 || control.GetColumn() < 65)	 // if input is other than abcdefghijkl or ABCDEFGHIJKL or 1-12
		return false;
	else
		return true;
}

bool Hex::PointControl(Cell control)const{/*                            Now i get Cell as parameter						*/
	if(hexCells[control.GetRow() - 1][CharToint(control.GetColumn())].GetCharacter() == '.')
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
			if(hexCells[i][0].GetCharacter() == symbol) break;
		}
		if(i == size) return false;
		FindSymbol(0,i,symbol);
	}
	else if(turn == 2){		// top-bot walls
		symbol = 'o';
		for(i = 0;i < size;i++){		// if there is a symbol in the bot row it starts to search for others
			if(hexCells[0][i].GetCharacter() == symbol) break;
		}
		if(i == size) return false;
		FindSymbol(i,0,symbol);
	}
	
	return GameOver;
}

void Hex::FindSymbol(int x,int y, const char symbol){						// for player1 searches for 'x', jumps to that 'x' and uppercase the 
	hexCells[y][x].SetCharacter(toupper(symbol));							// previous 'x' to 'X' to prevent jumping back.
	if(turn == 1 && x == size - 1){											// End of the functions if game doesnt end program lowercase them again.
		GameOver = true;
		return;
	}
	if(turn == 2 && y == size - 1){
		GameOver = true;
		return;
	}
						// Searches 6 different ways for lowercase symbols by the PlayerType and controls if numbers (x and y) are out of the array

	if(y - 1 > -1 && hexCells[y - 1][x].GetCharacter() == symbol) FindSymbol(x,y-1,symbol);
	if(y - 1 > -1  && x + 1 < size && hexCells[y - 1][x + 1].GetCharacter() == symbol) FindSymbol(x + 1,y - 1,symbol);
	if(x + 1 < size && hexCells[y][x + 1].GetCharacter() == symbol) FindSymbol(x + 1,y,symbol);
	if(y + 1 < size && hexCells[y + 1][x].GetCharacter() == symbol) FindSymbol(x,y + 1,symbol);
	if(y + 1 < size && x - 1 > -1 && hexCells[y + 1][x - 1].GetCharacter() == symbol) FindSymbol(x - 1,y + 1,symbol);
	if(x - 1 > -1 && hexCells[y][x - 1].GetCharacter() == symbol) FindSymbol(x - 1,y,symbol);

	if(GameOver == false)
		hexCells[y][x].SetCharacter(symbol);	//  If game doesnt end, program lowercases for next time
}


int Hex::FindBestWayDirection(Cell& control){	//	for computer, if top-topleft-topright or bot-botleft-botright
	int right = 0,left = 0;						//  is not empty, computer computes the shortest way to go
	
		
	while(hexCells[control.GetRow() - 1][CharToint(control.GetColumn()) + right].GetCharacter() == 'x') ++right;
	while(hexCells[control.GetRow() - 1][CharToint(control.GetColumn()) - left].GetCharacter() == 'x') ++left;
	if(CharToint(control.GetColumn()) + right == 12)
		return -1;		// end of array
	else if(CharToint(control.GetColumn()) - left == -1) 
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
				if(hexCells[i][j].GetCharacter() == '.')
					++marked1;
				if(other.hexCells[i][j].GetCharacter() == '.')
					++marked2;
			}
		}
	}
	else{
		for(int i = 0;i < size;++i){
			for(int j = 0; j < size;++j){
				if(hexCells[i][j].GetCharacter() == '.')
					++marked1;
			}
		}
		for(int i = 0;i < other.size;++i){
			for(int j = 0; j < other.size;++j){
				if(other.hexCells[i][j].GetCharacter() == '.')
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
	: size(0), turn(1), IsItFirstMove(true), GameOver(false), IsTopLineFinished(false), IsBotLineFinished(false), used(0), capacity(0){
		rowsteps = nullptr;
		hexCells = nullptr;
		steps = nullptr;
		temp.SetCharacter('o');
	}



Hex::Hex(string tempfilename)			//	loads game
	: size(0), turn(1), IsItFirstMove(false), GameOver(false), IsTopLineFinished(false), IsBotLineFinished(false){
		temp.SetCharacter('o');
		rowsteps = nullptr;
		hexCells = nullptr;
		steps = nullptr;
		strcpy(filename,tempfilename.c_str());
		LoadGame();
	}



void Hex::SaveGame()const{
	ofstream SaveFile(filename);
	int intbuffer;
					// Saving to a file by the order
	SaveFile << size << ' ' << enemy << ' ' << turn << ' ' << topnumber << ' ' << botnumber << ' ' << IsTopLineFinished << ' ' << IsBotLineFinished;
	if(enemy == 2)
		SaveFile << ' ' << topchar << ' ' << botchar << endl;
	else
		SaveFile << endl;
	for(int i = 0;i < size;++i){
		for(int j = 0;j < size;++j){
			intbuffer = static_cast<int>(hexCells[i][j].GetCharacter());
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


	Resize();
	for(int i = 0;i < size;++i){
		for(int j = 0;j < size;++j){
			LoadFile >> intbuffer;
			hexCells[i][j].SetCharacter(static_cast<char>(intbuffer));
		}
	}
	LoadFile.close();
	cout << endl << "New board is:\n\n";
	DisplayBoard();
}



/*										   	I renewed all above as OOPP, like setters and getters.											 */
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
/*													From this point all functions are new.					 								 */




Hex::Hex(const Hex &CtrlC){			// copy constructor
		rowsteps = nullptr;
		hexCells = nullptr;
		steps = new Cell;		// for temporary
		temp.SetCharacter(CtrlC.temp.GetCharacter());
		temp.SetColumn(CtrlC.temp.GetColumn());
		temp.SetRow(CtrlC.temp.GetRow());
		enemy = CtrlC.enemy;
		turn = CtrlC.turn;
		topnumber = CtrlC.topnumber;
		botnumber = CtrlC.botnumber;
		direction = CtrlC.direction;
		topchar = CtrlC.topchar;
		botchar = CtrlC.botchar;
		GameOver = CtrlC.GameOver;
		IsItFirstMove = CtrlC.IsItFirstMove;
		IsTopLineFinished = CtrlC.IsTopLineFinished;
		IsBotLineFinished = CtrlC.IsBotLineFinished;
		used = CtrlC.used;
		capacity = CtrlC.capacity;
		size = CtrlC.size;
		Resize();
		for(int i = 0; i < size;++i)
			for(int j = 0;j < size;++j)
				hexCells[i][j] = CtrlC.hexCells[i][j];
		AddBot();
		used = 0;
		FillSteps();
		used = CtrlC.used;
}

bool Hex::operator==(const Hex& control){			// same as comparegames() but operator way
	int marked1 = 0, marked2 = 0;

	if(GetSize() == control.GetSize()){
		for(int i = 0;i < GetSize();++i){
			for(int j = 0; j < GetSize();++j){
				if(hexCells[i][j].GetCharacter() != '.')
					++marked1;
				if(control.hexCells[i][j].GetCharacter() != '.')
					++marked2;
			}
		}
	}
	else{
		for(int i = 0;i < GetSize();++i){
			for(int j = 0; j < GetSize();++j){
				if(hexCells[i][j].GetCharacter() != '.')
					++marked1;
			}
		}
		for(int i = 0;i < control.GetSize();++i){
			for(int j = 0; j < control.GetSize();++j){
				if(control.hexCells[i][j].GetCharacter() != '.')
					++marked2;
			}
		}
	}
	if(marked1 > marked2)
		return true;
	else
		return false;
}

ostream& operator<<(ostream& print, const Hex& control){		// i already have displayboard()const;
	control.DisplayBoard();
	return print;
}

void operator<<(ofstream& print,const Hex& control){		// same as SaveGame() system
	int intbuffer;
					// Saving to a file by the order
	print << control.size << ' ' << control.enemy << ' ' << control.turn << ' ' << control.topnumber << ' ' << control.botnumber << ' ' << control.IsTopLineFinished << ' ' << control.IsBotLineFinished;
	if(control.enemy == 2)
		print << ' ' << control.topchar << ' ' << control.botchar << endl;
	else
		print << endl;
	for(int i = 0;i < control.size;++i){
		for(int j = 0;j < control.size;++j){
			intbuffer = static_cast<int>(control.hexCells[i][j].GetCharacter());
			print << intbuffer << ' ';
		}
		print << endl;
	}
	cout << "Your game is saved to file: " << control.filename << endl;
}

void operator>>(ifstream& scan, Hex& control){				// same as LoadGame() system
	int intbuffer;
	if(!scan.is_open()){
		cerr << endl << "File couldn't open" << endl;
		exit(1);
	}
					//   Reading file by the order
	scan >> control.size >> control.enemy >> control.turn >> control.topnumber >> control.botnumber >> control.IsTopLineFinished >> control.IsBotLineFinished;
	if(control.enemy == 2)
		scan >> control.topchar >> control.botchar;


	control.Resize();
	for(int i = 0;i < control.size;++i){
		for(int j = 0;j < control.size;++j){
			scan >> intbuffer;
			control.hexCells[i][j].SetCharacter(static_cast<char>(intbuffer));
		}
	}
	scan.close();
	cout << endl << "New board is:\n\n";
	control.DisplayBoard();
}

Hex& Hex::operator--(){		// undo last play, erase last play from steps, if pve undo last variables like topchar
	used-=1;
	hexCells[steps[used].GetRow() - 1][CharToint(steps[used].GetColumn())].SetCharacter('.');
	FillSteps();
	if(enemy == 2 && turn == 3){
		ResizeBot();
		turn = 1;
	}
	return *this;
}

Hex& Hex::operator--(int){	// same
	used-=1;
	hexCells[steps[used].GetRow() - 1][CharToint(steps[used].GetColumn())].SetCharacter('.');
	FillSteps();
	if(enemy == 2 && turn == 3){
		ResizeBot();
		turn = 1;
		
	}
	return *this;
}

Hex::~Hex(){				// destructor
	for(int i = 0;i < GetSize();++i)
		delete [] hexCells[i];
	delete [] hexCells;
	delete [] steps;
	for(int i = 0;i < capacity;++i){
		delete [] rowsteps[i];
		delete [] columnsteps[i];
		delete [] linesteps[i];
	}
	delete [] rowsteps;
	delete [] columnsteps;
	delete [] linesteps;
}

void Hex::Resize(){
	if(hexCells != nullptr){  // ifnot create , else resize
		for(int i = 0;i <size;++i)
			delete [] hexCells[i];
		delete [] hexCells;
	}

	hexCells = new Cell*[size];
	for(int i = 0;i < size;++i)
		hexCells[i] = new Cell[size];
}

void Hex::ResizeBot(){
	if(rowsteps == nullptr){		// ifnot create , else resize and fill
		rowsteps = new int*[capacity];
		columnsteps = new char*[capacity];
		linesteps = new bool*[capacity];

		for(int i = 0;i < capacity;++i){
			rowsteps[i] = new int[2];
			columnsteps[i] = new char[2];
			linesteps[i] = new bool[2];
		}		
	}
	else{
		int **temp1 = new int*[used];
		char **temp2 = new char*[used];
		bool **temp3 = new bool*[used];


		for(int i = 0;i < used;++i){
			temp1[i] = new int[2];
			temp2[i] = new char[2];
			temp3[i] = new bool[2];
		}
		for(int i = 0;i < used;++i){
			for(int j = 0;j < 2;++j){
				temp1[i][j] = rowsteps[i][j];
				temp2[i][j] = columnsteps[i][j];
				temp3[i][j] = linesteps[i][j];
			}
		}

		for(int i = 0;i < capacity - 15;++i){
			delete [] rowsteps[i];
			delete [] columnsteps[i];
			delete [] linesteps[i];
		}
		delete [] rowsteps;
		delete [] columnsteps;
		delete [] linesteps;

		rowsteps = new int*[capacity];
		columnsteps = new char*[capacity];
		linesteps = new bool*[capacity];

		for(int i = 0;i < capacity;++i){
			rowsteps[i] = new int[2];
			columnsteps[i] = new char[2];
			linesteps[i] = new bool[2];
		}

		for(int i = 0;i < used;++i){
			for(int j = 0;j < 2;++j){
				rowsteps[i][j] = temp1[i][j];
				columnsteps[i][j] = temp2[i][j];
				linesteps[i][j] = temp3[i][j];
			}
		}

		for(int i = 0;i < used;++i){
			delete [] temp1[i];
			delete [] temp2[i];
			delete [] temp3[i];
		}
		delete [] temp1;
		delete [] temp2;
		delete [] temp3;
	}
}

void Hex::AddBot(){	//	ifnot create by sending to Resizebot(), else save steps
	if(rowsteps == nullptr)
		ResizeBot();
	else{
		rowsteps[used - 1][0] = botnumber;
		rowsteps[used - 1][1] = topnumber;
		columnsteps[used - 1][0] = botchar;
		columnsteps[used - 1][1] = topchar;
		linesteps[used - 1][0] = IsBotLineFinished;
		linesteps[used - 1][1] = IsTopLineFinished;
	}
}
void Hex::ReadUndoBot(){		// reads after undo for bot
	botnumber = rowsteps[used - 1][0];
	topnumber = rowsteps[used - 1][1];
	botchar = columnsteps[used - 1][0];
	topchar = columnsteps[used - 1][1];
	IsBotLineFinished = linesteps[used - 1][0];
	IsTopLineFinished = linesteps[used - 1][1];
}

void Hex::FillSteps(){		// adds capacity to steps if needed
	Cell *temporary = new Cell[capacity];
	for(int i = 0;i < used;++i){
		temporary[i].SetCharacter(steps[i].GetCharacter());
		temporary[i].SetColumn(steps[i].GetColumn());
		temporary[i].SetRow(steps[i].GetRow());
	}
	delete [] steps;
	steps = new Cell[capacity];
	for(int i = 0;i < used;++i){
		steps[i].SetCharacter(temporary[i].GetCharacter());
		steps[i].SetColumn(temporary[i].GetColumn());
		steps[i].SetRow(temporary[i].GetRow());
	}
	delete [] temporary;
}

void Hex::FillSteps(const Cell& newstep){	//ifnot creates , if needed creates steps and also creates steps for bot , and fills last cell
	if(steps == nullptr){
		capacity += 15;
		steps = new Cell[capacity];
	}
	else if(used == capacity){
		Cell *temporary = new Cell[capacity];
		for(int i = 0;i < used;++i){
			temporary[i].SetCharacter(steps[i].GetCharacter());
			temporary[i].SetColumn(steps[i].GetColumn());
			temporary[i].SetRow(steps[i].GetRow());
		}
		delete [] steps;
		capacity += 15;
		if(enemy == 2)
			ResizeBot();
		steps = new Cell[capacity];
		for(int i = 0;i < used;++i){
			steps[i].SetCharacter(temporary[i].GetCharacter());
			steps[i].SetColumn(temporary[i].GetColumn());
			steps[i].SetRow(temporary[i].GetRow());
		}
		delete [] temporary;

	}
	steps[used].SetCharacter(newstep.GetCharacter());
	steps[used].SetColumn(newstep.GetColumn());
	steps[used].SetRow(newstep.GetRow());
	++used;
}

Hex::Cell& Hex::Cell::SetCharacter(char c1){
	character = c1;
	return *this;
}
Hex::Cell& Hex::Cell::SetColumn(char c1){
	column = c1;
	return *this;
}

Hex::Cell& Hex::Cell::SetRow(int r1){
	row = r1;
	return *this;
}

Hex::Cell::Cell()
{/* Left empty on purpose */}

Hex::Cell::Cell(const Cell &CtrlC)
	:character(CtrlC.character), column(CtrlC.column), row(CtrlC.row)
{/* Left empty on purpose */}

Hex::Cell::Cell(char character, char column, int row)
	:character(character), column(column), row(row)
{/* Left empty on purpose */}


