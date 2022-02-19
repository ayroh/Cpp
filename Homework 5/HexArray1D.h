#ifndef HEXARRAY1D
#define HEXARRAY1D

#include "AbstractHex.h"


namespace ayroh{
	class HexArray1D: public AbstractHex{
	public:
		HexArray1D();
		~HexArray1D();
		HexArray1D(std::string tempfilename);
		HexArray1D(const HexArray1D &other);
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
		Cell *Cells;
};
}

#endif