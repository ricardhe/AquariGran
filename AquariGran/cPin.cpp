
//#include <Arduino.h>
#include "cPin.h"

#define OUTPUT 0x1

cPin::cPin()
{

}

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, SDAcuari &psd)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	sd = &psd;

	ExecutaConfiguracioInicial();
}

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pProgDiaria, SDAcuari &psd)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	prgDiaria = pProgDiaria;
	sd = &psd;

	ExecutaConfiguracioInicial();
}

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioSetmanal pProgSetamanal, SDAcuari &psd)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	prgSetmanal = pProgSetamanal;
	sd = &psd;

	ExecutaConfiguracioInicial();
}

void cPin::ExecutaConfiguracioInicial()
{
	pinMode(numPin, OUTPUT);

	valPinActual = valPinDefecte;

	int valor =  sd->getIntValueConfig(nom);

	//TODO: recuperar valor del fitxer de l'ùltim canvi d'estat manual i assignar-ho al valor actual

	//utils.EscriuPinDigital(numPin, valPinActual);

}


cPin::~cPin()
{
}