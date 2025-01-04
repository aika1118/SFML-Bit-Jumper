#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

class Util
{
public:
	// 소숫점 digit 자리수만큼 string으로 변환 후 return 
	static string floatToString(float num, int digit);
};