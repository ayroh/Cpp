#ifndef MYEXCEPTIONS
#define MYEXCEPTIONS

#include <iostream>
class noLastMove{
	public:
		noLastMove(){ std::cout << "There is no last move.\n"; };
};

class WrongIndex{
	public:
		WrongIndex(){ std::cout << "You entered wrong index.\n"; };
};

#endif