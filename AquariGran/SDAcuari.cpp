#include "SDAcuari.h"


bool SDAcuari::carregaConfiguracio(bool initSD)
{
	File dataFile;

	bool ret = true;
	
	Serial.print(F("Ini SD"));

	if (initSD)
	{
		if (!SD.begin(4))
		{
			Serial.println(F("Err init SD"));
			ret = false;
			return;
		}
		Serial.println(F("SD ini ok"));
	}


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
		Serial.println(F("Err Open File."));
		ret = false;
	}

	return ret;
}


bool SDAcuari::saveValueConfig(int value, String var)
{
	File dataFile;
	bool ret = true;

	int valorActual = getIntValueConfig(var);

	if (value != valorActual)
	{

		SD.remove(nomFitxerConfig);

		dataFile = SD.open(nomFitxerConfig, FILE_WRITE);
		if (dataFile)
		{
			if (contingutConfiguracio.indexOf(var) <= 0)
			{
				contingutConfiguracio += var + "=" + value + "\r\n";
			}
			else
			{
				contingutConfiguracio.replace(var + "=" + valorActual, var + "=" + value);
			}			
			
			dataFile.write(contingutConfiguracio.c_str());

			dataFile.close();
		}
		else
		{
			Serial.println(F("Err Open File."));
			ret = false;
		}

	}

	return ret;
}




int SDAcuari::getPosicioValorVariable(String var)
{
	int indexVar = contingutConfiguracio.indexOf(var);
	int indexValor = indexVar + var.length() + 1;

	return indexValor;
}

int SDAcuari::getIntValueConfig(String var)
{
	int valor = -1;
	int indexValor = getPosicioValorVariable(var);

	if (indexValor >= 0)
	{
		String sValor = contingutConfiguracio.substring(indexValor, indexValor + 1);
		Serial.println(F("getIntValueConfig"));
		Serial.println(var);
		Serial.println(sValor);

		valor = sValor.toInt();
	}

	return valor;
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
