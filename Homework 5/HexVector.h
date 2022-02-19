#ifndef HEXVECTOR_H
#define HEXVECTOR_H

#include "AbstractHex.cpp"
#include <vector>
#include <cstring>
class HexVector: public AbstractHex{
	public:
		HexVector();
		HexVector(std::string tempfilename);
		void print();
		void setSize();
		void writeToFile() const;
		void readFromFile();
		void reset();
		bool isEnd();
		AbstractHex::Cell& Play();
		bool Play(AbstractHex::Cell& control);
		virtual char operator()(char c, int i);
		
		void PlayGame();
		bool PointControl(AbstractHex::Cell control)const;
		void FindSymbol(int x,int y, const char symbol);
		void BotPointPlace(bool IsItTop);
		int FindBestWayDirection(AbstractHex::Cell& control);
	private:
		std::vector<std::vector<AbstractHex::Cell> > Cells;
};
#endif