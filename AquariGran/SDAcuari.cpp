#include "SDAcuari.h"


bool SDAcuari::carregaConfiguracio()
{
	File dataFile;

	bool ret = true;
	
	Serial.print(F("Iniciant SD ..."));
	if (!SD.begin(4))
	{
		Serial.println(F("Error a l'iniciar la targeta SD"));
		ret = false;
		return;
	}

	Serial.println(F("SD iniciada correctamente"));

	dataFile = SD.open(nomFitxerConfig);
	if (dataFile)
	{

		if (dataFile.available())
		{
			contingutConfiguracio = dataFile.readString();
			Serial.println(contingutConfiguracio);
		}

		dataFile.close();
	}
	else
	{
		Serial.println(F("Error a l'obrir l'arxiu."));
		ret = false;
	}

	return ret;
}

bool SDAcuari::saveConfiguracio()
{
	File dataFile;

	bool ret = true;

	Serial.print(F("Iniciant SD ..."));
	if (!SD.begin(4))
	{
		Serial.println(F("Error a l'iniciar la targeta SD"));
		ret = false;
		return;
	}

	Serial.println(F("SD iniciada correctamente"));

	dataFile = SD.open(nomFitxerConfig);
	if (dataFile)
	{

		if (dataFile.available())
		{
			contingutConfiguracio = dataFile.readString();
		}

		dataFile.close();
	}
	else
	{
		Serial.println(F("Error a l'obrir l'arxiu."));
		ret = false;
	}

	return ret;
}

int SDAcuari::getIntValueConfig(String var)
{
	int indexVar =  contingutConfiguracio.indexOf(var);
	int indexValor = indexVar + var.length() + 1;
	String sValor = contingutConfiguracio.substring(indexValor, indexValor + 1);
	Serial.println("getIntValueConfig");
	Serial.println(var);
	Serial.println(sValor);

	return -1;
}

String SDAcuari::getConfiguracio()
{
	return contingutConfiguracio;
}


SDAcuari::SDAcuari(String pnomFitxerConfig)
{
	nomFitxerConfig = pnomFitxerConfig;
}


SDAcuari::~SDAcuari()
{
}
