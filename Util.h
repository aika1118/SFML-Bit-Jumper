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
	// �Ҽ��� digit �ڸ�����ŭ string���� ��ȯ �� return 
	static string floatToString(float num, int digit);
};