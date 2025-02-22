#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "json.hpp"
#include "Config.h"

using namespace std;
using namespace nlohmann;

class Util
{
public:
	// 소숫점 digit 자리수만큼 string으로 변환 후 return 
	static string floatToString(float num, int digit);

	static int getUID();
	static void setUID(int uid);
	static string getUserName(int uid);
};