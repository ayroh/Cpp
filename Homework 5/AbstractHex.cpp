#ifndef ABSTRACTHEX_H
#define ABSTRACTHEX_H
#include "MyExceptions.h"
#include "AbstractHex.h"
#include <string>
#include <cstring>


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
		AbstractHex(std::string tempfilename);
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
		void setFilename(std::string tempfilename){	strcpy(filename,tempfilename.c_str());	};
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

AbstractHex::AbstractHex(std::string tempfilename)			//	loads game
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
#endif