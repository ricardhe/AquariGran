#pragma once
#include "RTClib.h"

class Eines
{
public:
	Eines();
	~Eines();
	void EscriuPinDigital(int valPin, int numPin);
    void InverteixValorRele(int &valpinRele);
	bool datatimeMesPetitQue(DateTime quin, DateTime amb);
	DateTime TransformaHoraEnData(char* chora, DateTime ara);
};

