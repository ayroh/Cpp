#ifndef ABSTRACTHEX_H
#define ABSTRACTHEX_H

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
#endif