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