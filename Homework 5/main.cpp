#include <iostream>
#include <fstream>
#include "HexVector.cpp"
#include "HexArray1D.cpp"
#include "HexAdapter.cpp"

using namespace std;



bool IsValid(AbstractHex **Hexes){
	int Xcount = 0, Ocount = 0;
	for(int k = 0;k < 4;++k){		
		Xcount = 0;
		Ocount = 0;
		if(*(Hexes + k) == 0)		// if game is empty doesnt look to that
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
		*(Hexes + k) = 0;
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
			
			cout << "\n\nthrowing exception.\n";
			try{
				e.lastMove();
			}catch(noLastMove){};
			
			
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

			cout << "--------------------------------\n";
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
				
			cout << "\n\nPress enter to end test.\n\n";
			getchar();
			getchar();
			
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
