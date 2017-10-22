#pragma once

#include <SD.h>

class SDAcuari
{

private:

	String nomFitxerConfig;
	String contingutConfiguracio;

public:

	
	String getConfiguracio();

	bool carregaConfiguracio();
	bool saveConfiguracio();

	int getIntValueConfig(String var);


	SDAcuari(String pnomFitxerConfig);
	~SDAcuari();
};

