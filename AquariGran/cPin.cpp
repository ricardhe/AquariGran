#include "cPin.h"

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;

	ExecutaConfiguracioInicial();
}

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pProgDiaria)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	prgDiaria = pProgDiaria;

	ExecutaConfiguracioInicial();
}

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioSetmanal pProgSetamanal)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	prgSetmanal = pProgSetamanal;

	ExecutaConfiguracioInicial();
}

void cPin::ExecutaConfiguracioInicial()
{
	pinMode(numPin, OUTPUT);

	valPinActual = valPinDefecte;
	//TODO: recuperar valor del fitxer de l'ùltim canvi d'estat manual i assignar-ho al valor actual

	utils.EscriuPinDigital(numPin, valPinActual);

}





cPin::~cPin()
{
}