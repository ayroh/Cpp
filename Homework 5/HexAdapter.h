#include <string>
#include "AbstractHex.cpp"
#include <vector>
#include <deque>

namespace ayroh{
template<class T>
class HexAdapter: public AbstractHex{
public:
	HexAdapter();
	HexAdapter(std::string tempfilename);
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
	T Cells;
};

}
