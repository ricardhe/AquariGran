#pragma once

#include <SD.h>

class SDAcuari
{

private:

	String nomFitxerConfig;
	String contingutConfiguracio;
	int getPosicioValorVariable(String var);

public:

	
	String getConfiguracio();

	bool carregaConfiguracio(bool initSD);

	int getIntValueConfig(String var);

	bool saveValueConfig(int value, String var);

	SDAcuari(String pnomFitxerConfig);
	~SDAcuari();
};

