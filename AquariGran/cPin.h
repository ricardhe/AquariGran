#pragma once

//#include "Eines.h"
#include "SDAcuari.h";

struct programacioDiaria
{
	int diaDeLaSetmana;
	//int intervalsDiaris[12][12];
	int *intervalsDiaris;
};

struct programacioSetmanal
{
	programacioDiaria *dies;
};

class cPin
{

private:

	//Eines utils = Eines();

	char* nom;
	int numPin;
	int valPinDefecte;
	int valPinActual;
	char tipusOberturaDefecte;
	char tipusProgramacio;
	programacioDiaria prgDiaria;
	programacioSetmanal prgSetmanal;
	SDAcuari *sd;

	void ExecutaConfiguracioInicial();

public:
	cPin();
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, SDAcuari &psd);
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pProgDiaria, SDAcuari &psd);
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioSetmanal pProgDiaria, SDAcuari &psd);
	~cPin();
};

