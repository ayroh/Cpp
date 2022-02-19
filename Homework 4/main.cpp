#include <iostream>
#include <cstring>
#include "header.h"
using namespace std;

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
			cout << "	Choose your game (1 - 5).\n-1 for end.\n6 for compare games(with == operator).\n7 for knowledge about marked cells.\n8 for size of a game.\n9 for display a board.\n\n";
			cin >> choice;
			if(choice == -1)
				return 0;
			else if(choice == 6){
				cout << "Enter two games between 1-5.\n";
				cin >> game1 >> game2;
				if(game[game1 - 1] == game[game2 -1])
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
				cout << "Size of Game " << sayz + 1 << " is: " << game[sayz].GetSize() << "x" << game[sayz].GetSize() << endl << endl;
				continue;
			}
			else if(choice == 9){
				cout << "Enter game (1 - 5).\n\n";
				cin >> sayz;
				cout << game[sayz-1];
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

