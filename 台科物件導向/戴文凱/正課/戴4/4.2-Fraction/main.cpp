#include <iostream>
#include "Fraction.h"
using namespace std;

int main()
{
	Fraction f1, f2;
	f1.setNumerator(4);
	f1.setDenominator(2);
	f1.getDouble();
	f1.outputReducedFraction();//最簡分數形式

	f2.setNumerator(20);
	f2.setDenominator(60);
	f2.getDouble();
	f2.outputReducedFraction();//最簡分數形式
	//system("pause");
	return 0;
}