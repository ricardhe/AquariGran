#pragma once

//#ifndef MenuBackend_h
//#include <MenuBackend.h>
//#endif 

#include "Eines.h"
#include "SDAcuari.h";
#include "LCDAcuari.h"
#include "ControlAplicacio.h"
#include "RTClib.h"
//#include "DateTime\DateTime.h"

struct subMenu
{
	char* nom;
	char* operacio;
};

struct interval {
	char* ini;
	char* fi;
};

struct programacioDiaria
{
	int diaDeLaSetmana;
	int numIntervals;
	//int intervalsDiaris[12][12];
	interval* intervalsDiaris;
	
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
	Eines *utils;
	LCDAcuari *lcdAcuari;
	int indexSubMenuIntern=0;
	int numSubOpcions=0;
	subMenu* opcionsSubMenus;
	ControlAplicacio  *control;
	bool tincElControl = false;

	bool *algunMenuTeElControl;
	DateTime horaProperaArrencadaPerProgramacio;
	bool paratPerProgramacio = false;

	RTC_DS3231 *rtc;


	const char* operEstat = "Estat";
	const char* oper10Min = "oper10Min";
	const char* oper30Min = "oper30Min";
	const char* oper1Hora = "oper1Hora";

	void ExecutaConfiguracioInicial();
	bool datatimeMesPetitQue(DateTime quin, DateTime amb);
	void SerialPrintDate(DateTime val);
	void PararPinsDurantUnsMinuts(int minuts);

public:
	cPin();
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari);
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pProgDiaria, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari);
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioSetmanal pProgDiaria, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari);

	bool setValorInvertit();

	bool Desactiva();

	void indicaStatusActivacioLCD(byte numLinea);

	void agafaElControl(ControlAplicacio &pControl, char pshortKey, RTC_DS3231 &prtc); //MenuBackend &pmenu, char pshortKey

	void perdElControl();

	void dibuixaSubMenus();

	void executemVolta();

	void avancaSubmenu();

	void retrocedeixSubMenu();

	void executaSubMenu();

	void para10Minuts();
	void para30Minuts();
	void para1Hora();

	~cPin();
};

