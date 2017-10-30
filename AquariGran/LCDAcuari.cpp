#include "LCDAcuari.h"

LCDAcuari::LCDAcuari(LiquidCrystal_I2C &plcd)
{
	lcd = &plcd;
}

LCDAcuari::~LCDAcuari()
{
}

void LCDAcuari::Print1Line(char* val)
{
	lcd->setCursor(2, 0);
	//lcd->print(val);

	int length = lcd->print(val);

	for (int i = length; i < 16; i++) {
		lcd->write(' ');
	}

}

void LCDAcuari::Print2Line(char* val)
{
	lcd->setCursor(2, 1);
	//lcd->print(val);

	int length = lcd->print(val);

	for (int i = length; i < 16; i++) {
		lcd->write(' ');
	}
}

void LCDAcuari::Print2LineaEnBlanc()
{
	lcd->setCursor(2, 1);

	for (int i = 0; i < 16; i++) {
		lcd->write(' ');
	}
}

void LCDAcuari::Print1LineaEnBlanc()
{
	lcd->setCursor(2, 0);

	for (int i = 0; i < 16; i++) {
		lcd->write(' ');
	}
}


void LCDAcuari::DibuixaRodoneta()
{
	lcd->setCursor(0, 0);
	lcd->write(0);
}

void LCDAcuari::DibuixaPunter()
{
	lcd->setCursor(0, 0);
	lcd->write(1);
}

void LCDAcuari::Clear() {
	lcd->clear();
}
