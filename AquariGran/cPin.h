#pragma once

struct programacioDiaria
{
	int diaDeLaSetmana;
	//int intervalsDiaris[12][12];
	int intervalsDiaris[20][2];
};

struct programacioSetmanal
{
	programacioDiaria dies[7];
};

class cPin
{

private:
	char* nom;
	int numPin;
	int valPinDefecte;
	int valPinActual;
	char tipusOberturaDefecte;
	char tipusProgramacio;
	programacioDiaria prgDiaria;
	programacioSetmanal prgSetmanal;

public:
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio);
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pProgDiaria);
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioSetmanal pProgDiaria);
	~cPin();
};

