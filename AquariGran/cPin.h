#pragma once

//#ifndef MenuBackend_h
//#include <MenuBackend.h>
//#endif 

#include "Eines.h"
#include "SDAcuari.h";
#include "LCDAcuari.h"
#include "ControlAplicacio.h"
#include "RTClib.h"
//#include <Time.h>
//#include <DS1307RTC.h>
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
	int diaDeLaSetmana=-1;
	int numIntervals=0;
	interval intervalsDiaris[10];
};

struct programacioSetmanal
{
	programacioDiaria dies[7];
};

class cPin
{

private:

	//Eines utils = Eines();

	char* nom;
	int numPin;
	int pinActiu;
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
	DateTime horaProperaParadaPerProgramacio;
	bool paratPerProgramacio = false;
	bool activatPerProgramacio = false;

	RTC_DS3231 *rtc;

	const char* operActiu = "ActiuoNo";
	const char* operEstat = "Estat";
	const char* oper10Min = "oper10Min";
	const char* oper30Min = "oper30Min";
	const char* oper1Hora = "oper1Hora";

	 char ObertPerDefecte = 'O';
	 char TancatPerDefecte = 'T';
	 char TipusProgramacioDiaria = 'D';
	 char TipusProgramacioSetmanal = 'S';
	 char TipusProgramacioSempreObert = 'A';

	void ExecutaConfiguracioInicial();
	//bool datatimeMesPetitQue(DateTime quin, DateTime amb);
	void SerialPrintDate(DateTime val);
	void PararPinsDurantUnsMinuts(int minuts);
	void DibuixaPrimeraOpcioSubMenu(subMenu sub);
	void DibuixaSegonaOpcioSubMenu(subMenu sub);

public:
	cPin();
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari);
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pProgDiaria, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari);
	cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pr1, programacioDiaria pr2, programacioDiaria pr3, programacioDiaria pr4, programacioDiaria pr5, programacioDiaria pr6, programacioDiaria pr7,  subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari);

	bool setValorActualInvertit();

	bool setValorpinActiuInvertit();

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

	bool AlgunIntervalAfectat(programacioDiaria prog, DateTime &HoraFi);

	bool AlgunIntervalSetmanalAfectat(programacioSetmanal prog, DateTime &HoraFi);

	~cPin();
};

