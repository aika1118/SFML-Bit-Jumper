#include "Util.h"

string Util::floatToString(float num, int digit)
{
	stringstream ss;
	ss << fixed << setprecision(digit) << num;
	return ss.str();
}
