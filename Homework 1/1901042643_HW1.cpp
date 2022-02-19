#include <iostream>
#include <string>
using namespace std;

void DisplayBoard(char board[12][12],int Area);
int PointControl(char board[12][12],char character,int number);
void PointPlace(char board[12][12],char character,int number,char symbol);
int IsGameFinished(char board[12][12],int PlayerType,int Area);
int FindSymbol(char board[12][12],char Symbol,int Area,int PlayerType,int x,int y);
int FindBestWayDirection(char board[12][12],char character,int number,char Symbol);		 // '-1' for left, '1' for right
void BotPointPlace(char board[12][12],char *character,int *number,bool IsItTop,int Area);

bool GameOver = false,IsTopLineFinished = false,IsBotLineFinished = false;


int main(){
	int Area,Enemy,topnumber,botnumber,direction;
	char topchar,botchar;
	bool IsItFirstMove = true;


	while(true){						// table size
		cout << "Enter the Area of your table.(6 - 12)\n";
		cin >> Area;
		if(Area > 12 || Area < 6){
			cerr << "Enter between 6 and 12.\n\n";
			continue;
		}
		break;
	}
	char board[12][12];
	for(int i = 0;i < Area;i++)			// filling the board with '.'
		for(int j = 0;j < Area;j++)
			board[i][j] = '.';
	while(true){						// computer or player
		cout << "Press '1' for playing with a user and '2' for computer.\n";
		cin >> Enemy;
		if(Enemy != 1 && Enemy != 2){
			cerr << "Enter a proper answer.\n\n";
			continue;
		}
		break;
	}
	DisplayBoard(board,Area);
	int number;
	char character;
	if(Enemy == 1){						// Enemy = Player
		while(true){
			while(true){
				cout << "\nEnter your move Player 1.(C5 etc.)\n";
				cin >> character >> number;
				if(character > 96 && character < 109) character -= 32;	 // in case player plays with lowercases
				if(number > Area || number < 1 || character > 65 + Area - 1 || character < 65){	 // if input is other than abcdefghijkl or ABCDEFGHIJKL or 1-12
					cerr << "Enter a proper answer.\n";
					continue;
				}
				if(PointControl(board,character,number) == 0){
					cerr << "That point is not empty.\n\n";
					continue;
				}
				PointPlace(board,character,number,'x');
				break;
			}
			DisplayBoard(board,Area);
			if(IsGameFinished(board,1,Area) == 1) break;
			while(true){
				cout << "\nEnter your move Player 2.(C5 etc.)\n";
				cin >> character >> number;
				if(character > 96 && character < 109) character -= 32;
				if(number > Area || number < 1 || character > 65 + Area - 1 || character < 65){
					cerr << "Enter a proper answer.\n";
					continue;
				}
				if(PointControl(board,character,number) == 0){
					cerr << "That point is not empty.\n\n";
					continue;
				}
				PointPlace(board,character,number,'o');	
				break;
			}
			DisplayBoard(board,Area);
			if(IsGameFinished(board,2,Area) == 1) break;
		}
	}
	else if(Enemy == 2){				// Enemy = Computer
		while(true){
			while(true){
				cout << "\nEnter your move Player 1.(C5 etc.)\n";
				cin >> character >> number;
				if(character > 96 && character < 109) character -= 32; // in case player plays with lowercases
				if(number > Area || number < 1 || character > 65 + Area - 1 || character < 65){ // if input is other than abcdefghijkl or ABCDEFGHIJKL or 1-12
					cerr << "Enter a proper answer.\n";
					continue;
				}
				if(PointControl(board,character,number) == 0){
					cerr << "That point is not empty.\n\n";
					continue;
				}
				PointPlace(board,character,number,'x');
				break;
			}
			if(IsGameFinished(board,1,Area) == 1) break;

			if(IsItFirstMove == true){
				IsItFirstMove = false;
				if(64 + Area == character || 96 + Area == character)  // If at first move player selects the rightmost character
					character -= 1;
				else
					character += 1;									// In first move program selects the 'right' of the player
				PointPlace(board,character,number,'o');
				topchar = botchar = character;					// Program stores the rightmost, leftmost
				topnumber = botnumber = number;					// lowest and highest selection and continues from that point
			}
			else{
				if(number > topnumber && IsTopLineFinished == false)			// if player selects higher number from the topnumber, computer goes to down
					BotPointPlace(board,&topchar,&topnumber,true,Area);			// if there is a symbol in the top row program goes to bot
				else if(number < botnumber && IsBotLineFinished == false)		// if lower from the botnumber goes to up
					BotPointPlace(board,&botchar,&botnumber,false,Area);		// if there is a symbol in the bot row program goes to top
				else{		// if between top and bot numbers program first tries to go to up, if there is already a symbol in the top row it goes to bottom
					if(IsTopLineFinished == false)
						BotPointPlace(board,&topchar,&topnumber,true,Area); 
					else
						BotPointPlace(board,&botchar,&botnumber,false,Area);
				}
			}
			DisplayBoard(board,Area);
			if(IsGameFinished(board,2,Area) == 1) break;
		}

	}
	cout << "\n\n\n        It's Over Anakin\n\n";
	DisplayBoard(board,Area);
}

void DisplayBoard(char board[12][12],int Area){		// prints the board
	string FirstLine = "ABCDEFGHIJKL";
	cout << "  ";
	for(int i = 0;i < Area;i++) cout << FirstLine[i] << " ";
	cout << "\n";
	for(int i = 0;i < Area;i++){
		cout << i+1;
		for(int k = 0;k < i+1;k++){
			if(k == 9) continue;		// because of the two digit numbers
			cout << " ";
	}
		for(int j = 0;j < Area;j++) cout << board[i][j] << " ";
		cout << "\n";
	}
}

int PointControl(char board[12][12],char character,int number){
	int column = character - 65;		// char to int from ASCII
	if(board[number - 1][column] == '.') return 1;
	else return 0;	
}

void PointPlace(char board[12][12],char character,int number,char symbol){
	int column = character - 65;		// char to int from ASCII
	board[number - 1][column] = symbol;
}

int IsGameFinished(char board[12][12],int PlayerType,int Area){		
	int i;
	if(PlayerType == 1){ 			 // Left-Right walls
		for(i = 0;i < Area;i++){			// if there is a symbol in the left column it starts to search for others
			if(board[i][0] == 'x') break;
		}
		if(i == Area) return 0;
		FindSymbol(board,'x',Area,PlayerType,0,i);
	}
	else if(PlayerType == 2){		// top-bot walls
		for(i = 0;i < Area;i++){		// if there is a symbol in the bot row it starts to search for others
			if(board[0][i] == 'o') break;
		}
		if(i == Area) return 0;
		FindSymbol(board,'o',Area,PlayerType,i,0);
	}
	if(GameOver == true) return 1;
	else return 0;
}


int FindSymbol(char board[12][12],char Symbol,int Area,int PlayerType,int x,int y){	// for player1 searches for 'x', jumps to that 'x' and uppercase the 
	board[y][x] = toupper(Symbol);													// previous 'x' to 'X' to prevent jumping back.
	if(PlayerType == 1 && x == Area - 1){											// End of the functions if game doesnt end program lowercase them again.
		GameOver = true;
		return 0;
	}
	if(PlayerType == 2 && y == Area - 1){
		GameOver = true;
		return 0;
	}
						// Searches 8 different ways for lowercase symbols by the PlayerType and controls if numbers (x and y) are out of the array

	if(board[y - 1][x] == Symbol && y - 1 > -1) FindSymbol(board,Symbol,Area,PlayerType,x,y-1);
	if(board[y - 1][x + 1] == Symbol && y - 1 > -1  && x + 1 < Area) FindSymbol(board,Symbol,Area,PlayerType,x + 1,y - 1);
	if(board[y][x + 1] == Symbol && x + 1 < Area) FindSymbol(board,Symbol,Area,PlayerType,x + 1,y);
	if(board[y + 1][x + 1] == Symbol && y + 1 < Area && x + 1 < Area) FindSymbol(board,Symbol,Area,PlayerType,x + 1,y + 1);
	if(board[y + 1][x] == Symbol && y + 1 < Area) FindSymbol(board,Symbol,Area,PlayerType,x,y + 1);
	if(board[y + 1][x - 1] == Symbol && y + 1 < Area && x - 1 > -1) FindSymbol(board,Symbol,Area,PlayerType,x - 1,y + 1);
	if(board[y][x - 1] == Symbol && x - 1 > -1) FindSymbol(board,Symbol,Area,PlayerType,x - 1,y);
	if(board[y - 1][x - 1] == Symbol && y - 1 > -1 && x - 1 > -1) FindSymbol(board,Symbol,Area,PlayerType,x - 1,y - 1);

	if(GameOver == false) board[y][x] = Symbol;	//  If game doesnt end, program lowercases for next time
}

int FindBestWayDirection(char board[12][12],char character,int number,char Symbol){	//	for computer, if top-topleft-topright or bot-botleft-botright
	int right = 0,left = 0;															//  is not empty, computer computes the shortest way to go
	int column = character - 65;		// char to int from ASCII
		
	while(board[number - 1][column + right] == Symbol) ++right;
	while(board[number - 1][column - left] == Symbol) ++left;
	if(column + right == 12) return -1;		// end of array
	else if(column - left == -1) return 1;	// end of array
	else if(right > left) return -1;
	else return 1;
}

void BotPointPlace(char board[12][12],char *character,int *number,bool IsItTop,int Area){
	int direction;					// Places the next symbol by the IsItTop selection
	if(IsItTop == true){			// Function first tries to go straight up or down
		*number += 1;				// then tries cross paths
									// at last computes the shortest way between right and left and goes that way
		if(PointControl(board,*character,*number) == 1)											// to down
			PointPlace(board,*character,*number,'o');
		else if(PointControl(board,*character + 1,*number) == 1 && *character - 65 + 1 < 12){	// cross
			*character += 1;
			PointPlace(board,*character,*number,'o');
		}
		else if(PointControl(board,*character - 1,*number) == 1 && *character - 65 - 1 > -1){	// cross
			*character -= 1;
			PointPlace(board,*character,*number,'o');
		}
		else{
			direction = FindBestWayDirection(board,*character,*number,'x');						// right or left			
			*number -= 1;
			if(PointControl(board,*character + direction,*number) == 1){
				*character += direction;
				PointPlace(board,*character,*number,'o');
			}
			else if(PointControl(board,*character - direction,*number) == 1){
				*character -= direction;
				PointPlace(board,*character,*number,'o');	
			}
		}				
		if(*number == Area)			// this means computer placed a symbol to last line
			IsTopLineFinished = true;
	}

	else if(IsItTop == false){
		*number -= 1;
		
		if(PointControl(board,*character,*number) == 1)											// to up
			PointPlace(board,*character,*number,'o');
		else if(PointControl(board,*character + 1,*number) == 1  && *character - 65 + 1 < 12){	// cross
			*character += 1;
			PointPlace(board,*character,*number,'o');
		}
		else if(PointControl(board,*character - 1,*number) == 1  && *character - 65 - 1 > -1){	// cross
			*character -= 1;
			PointPlace(board,*character,*number,'o');
		}
		else{
			direction = FindBestWayDirection(board,*character,*number,'x');						// right or left
			*number += 1;
			if(PointControl(board,*character + direction,*number) == 1){
				*character += direction;
				PointPlace(board,*character,*number,'o');
			}
			else if(PointControl(board,*character - direction,*number) == 1){
				*character -= direction;
				PointPlace(board,*character,*number,'o');	
			}
		}
		if(*number == 0)			// this means computer placed a symbol to first line
			IsBotLineFinished = true;
	}
}
