#pragma once

#include <LiquidCrystal_I2C.h>

class LCDAcuari
{
private:
	LiquidCrystal_I2C *lcd;
public:
	LCDAcuari(LiquidCrystal_I2C &plcd);
	~LCDAcuari();
	void Print2Line(char* val);
	void Print1Line(char* val);
	void Print1LineaEnBlanc();
	void Print2LineaEnBlanc();
	void DibuixaRodoneta();
	void DibuixaPunter();
	void Clear();
};

