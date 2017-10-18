#include "Eines.h"



Eines::Eines()
{
}


Eines::~Eines()
{
}


void EscriuPinDigital(int valPin, int numPin)
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