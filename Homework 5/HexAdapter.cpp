#include "HexAdapter.h"


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