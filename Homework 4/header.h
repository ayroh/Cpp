#ifndef HEX_H
#define HEX_H
#include <iostream>
#include <vector>
using namespace std;

enum celltype{	user1 = 'x', user2 = 'o', computer = 'o', empty = '.'	};

class Hex{

	public:
	explicit Hex();				
	Hex(const Hex &CtrlC);
	Hex(string tempfilename);
	~Hex();

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

	void PlayGame();
	bool operator==(const Hex& control);
	friend void operator>>(ifstream& scan, Hex& control);
	friend void operator<<(ofstream& print,const Hex& control);
	friend ostream& operator<<(ostream& print, const Hex& control);
	Hex& operator--();
	Hex& operator--(int);
	inline static int NumberOfMarkedCells(){	return MarkedCells;	}
	inline int GetSize()const{	return size; }
	inline int GetTurn()const{	return turn; }
	

	private:

	Cell **hexCells;
	Cell *steps,temp;
	int **rowsteps;
	char **columnsteps;
	bool **linesteps;
	Cell& Play();
	void Resize();
	void FillSteps();
	void FillSteps(const Cell& newstep);
	void AddBot();
	void ReadUndoBot();
	void ResizeBot();
	bool CompareGames(const Hex &other);
	
	bool Play(Cell &control);
	bool AnswerControl(Cell& control);
	bool PointControl(Cell control)const;
	bool isGameFinished();
	void FindSymbol(int x,int y, const char symbol);
	void DisplayBoard() const;
	void BotPointPlace(bool IsItTop);
	void SaveGame()const;
	void LoadGame();
	int FindBestWayDirection(Cell &control);
	int size, enemy, turn;
	int topnumber, botnumber, direction;
	int used,capacity;
	char topchar, botchar;
	char filename[30];
	bool GameOver;
	bool IsItFirstMove,IsTopLineFinished, IsBotLineFinished;
	inline int CharToint(char character)const{	return static_cast<int>(character) - 65; };
	static int MarkedCells;
	
	
};

#endif
