#include <Arduino.h>
#include "Eines.h"


Eines::Eines()
{
}

Eines::~Eines()
{
}

void Eines::EscriuPinDigital(int valPin, int numPin)
{

	Serial.print("Num Pin: "); Serial.print(numPin);

	if (valPin == 1)
	{
		Serial.println("HIGH");
		digitalWrite(numPin, HIGH);
	}
	else
	{
		Serial.println("LOW");
		digitalWrite(numPin, LOW);
	}

}

void Eines::InverteixValorRele(int &valpinRele)
{
	if (valpinRele == 1)
	    valpinRele = 0;
	else
		valpinRele = 1;
}

bool Eines::datatimeMesPetitQue(DateTime quin, DateTime amb)
{
	return (quin.unixtime() < amb.unixtime());
}

DateTime Eines::TransformaHoraEnData(char* chora, DateTime ara)
{
	String val = String(chora);

	int hora = val.substring(0, 2).toInt();
	int minuts = val.substring(3, 2).toInt();

	return DateTime(ara.year(), ara.month(), ara.day(), (uint8_t)hora, (uint8_t)minuts, 0);
}