#include "cPin.h"
#include "LCDAcuari.h"

#define OUTPUT 0x1

cPin::cPin()
{

}

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	sd = &psd;
	utils = &putils;
	lcdAcuari = &plcdAcuari;

	numSubOpcions = pnumSubOpcions;
	opcionsSubMenus = psubMenu;
	
	//Serial.println(opcionsSubMenus[0].nom);
	//Serial.println(opcionsSubMenus[0].operacio);

	//Serial.println(opcionsSubMenus[2].nom);
	//Serial.println(opcionsSubMenus[2].operacio);

	ExecutaConfiguracioInicial();
}

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pProgDiaria, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	prgDiaria = pProgDiaria;
	sd = &psd;
	utils = &putils;
	lcdAcuari = &plcdAcuari;

	numSubOpcions = pnumSubOpcions;
	opcionsSubMenus = psubMenu;

	//Serial.println("Valors Prog : ");
	for (int i = 0; i < pProgDiaria.numIntervals; i++)
	{
		Serial.println(pProgDiaria.intervalsDiaris[i].ini);
		Serial.println(pProgDiaria.intervalsDiaris[i].fi);
	}

	ExecutaConfiguracioInicial();
}

cPin::cPin(char* pNom, int pNumPin, int pvalPinDefecte, char ptipusOberturaDefecte, char ptipusProgramacio, programacioSetmanal pProgSetamanal, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari)
{
	nom = pNom;
	numPin = pNumPin;
	valPinDefecte = pvalPinDefecte;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	prgSetmanal = pProgSetamanal;
	sd = &psd;
	utils = &putils;
	lcdAcuari = &plcdAcuari;

	numSubOpcions = pnumSubOpcions;
	opcionsSubMenus = psubMenu;

	ExecutaConfiguracioInicial();
}

void cPin::ExecutaConfiguracioInicial()
{
	pinMode(numPin, OUTPUT);

	valPinActual = valPinDefecte;

	int valor =  sd->getIntValueConfig(nom);

	if (valor != -1)
	{
		Serial.print(F("Val Recup Fitxer, ")); Serial.print(nom); Serial.print(' '); Serial.println(valor);
		valPinActual = valor;
	}

	utils->EscriuPinDigital(valPinActual, numPin);

}

bool cPin::setValorInvertit()
{
	bool bRet = false;

	//Serial.print(F("Valor abans d'invertir: ")); Serial.println(valPinActual);
	utils->InverteixValorRele(valPinActual);
	//Serial.print(F("Valor Pin Despres Canviat : ")); Serial.println(valPinActual);

	utils->EscriuPinDigital(valPinActual, numPin);

	bRet = sd->saveValueConfig(valPinActual, nom);
	sd->carregaConfiguracio(false);

	valPinDefecte = valPinActual;

	return bRet;
}

void cPin::indicaStatusActivacioLCD(byte numLinea)
{
	if (valPinActual == 1)
	{
		if (numLinea == 1)
			lcdAcuari->Print1Line("Activat");
		else
			lcdAcuari->Print2Line("Estat: Activat");
	}
	else
	{
		if (numLinea == 1)
			lcdAcuari->Print1Line("Desactiv.");
		else
			lcdAcuari->Print2Line("Estat: Desactiv.");
	}
}

void cPin::agafaElControl(ControlAplicacio &pControl, char pshortKey, RTC_DS3231 &prtc) //MenuBackend &pmenu, char pshortKey
{
	if (!tincElControl)
	{
		lcdAcuari->Clear();
		tincElControl = true;
		indexSubMenuIntern = 0;

		control = &pControl;
		control->algunMenuTeElControl = true;
		control->reAgafarUltimMenu = false;
		control->ultimaShortKey = pshortKey;

		rtc = &prtc;

		dibuixaSubMenus();
	}
}

void cPin::dibuixaSubMenus()
{
	Serial.println(indexSubMenuIntern);

	lcdAcuari->DibuixaPunter();

	if (opcionsSubMenus[indexSubMenuIntern].nom == operEstat)
	{
		if (valPinActual == 1)
		{
			lcdAcuari->Print1Line("Activat");
		}
		else
		{
			lcdAcuari->Print1Line("Desactivat");
		}
	}
	else
	{
		lcdAcuari->Print1Line(opcionsSubMenus[indexSubMenuIntern].nom);
	}

	//Serial.println(opcionsSubMenus[indexSubMenuIntern].nom);

	if (indexSubMenuIntern == (numSubOpcions-1))
	{
		lcdAcuari->Print2LineaEnBlanc();
	}
	else
	{
		lcdAcuari->Print2Line(opcionsSubMenus[indexSubMenuIntern+1].nom);
		//Serial.println(opcionsSubMenus[indexSubMenuIntern+1].nom);
	}

}

void cPin::avancaSubmenu()
{
	if (indexSubMenuIntern == (numSubOpcions - 1))
	{
		indexSubMenuIntern = indexSubMenuIntern; // ho deixem igual que es quedi l'ultim
	}
	else
	{
		indexSubMenuIntern++;
	}
	dibuixaSubMenus();
}

void cPin::retrocedeixSubMenu()
{
	if (indexSubMenuIntern == 0)
	{
		indexSubMenuIntern = indexSubMenuIntern; // ho deixem igual que es quedi l'ultim
		control->algunMenuTeElControl = false;
		control->reAgafarUltimMenu = true;
		tincElControl = false;
	}
	else
	{
		indexSubMenuIntern--;
		dibuixaSubMenus();
	}
}

void cPin::executaSubMenu()
{
	char* oper = opcionsSubMenus[indexSubMenuIntern].operacio;
	
	if (oper == operEstat)
	{

		if (setValorInvertit())
		{
			paratPerProgramacio = false;
			indicaStatusActivacioLCD(1);
		}

	}
	else if (oper == oper10Min)
	{
		para10Minuts();
	}
	else if (oper == oper30Min)
	{
		para30Minuts();
	}
	else if (oper == oper1Hora)
	{
		para1Hora();
	}

}

void cPin::executemVolta()
{

	if (paratPerProgramacio && valPinActual == 0)
	{
		
		DateTime ara = rtc->now();
		if (datatimeMesPetitQue(horaProperaArrencadaPerProgramacio, ara))
		{
			Serial.println(F("Reactivem pin"));

			valPinActual = 1;

			utils->EscriuPinDigital(valPinActual, numPin);
			paratPerProgramacio = false;
		}
	}

}

void cPin::perdElControl()
{

}

void cPin::SerialPrintDate(DateTime val)
{
	Serial.print(val.day());
	Serial.print('/');
	Serial.print(val.month());
	Serial.print('/');
	Serial.print(val.year());

	Serial.print(' ');

	Serial.print(val.hour());
	Serial.print(':');
	Serial.print(val.minute());
	Serial.print(':');
	Serial.print(val.second());
	Serial.println(' ');
}

bool cPin::datatimeMesPetitQue(DateTime quin, DateTime amb)
{
	return (quin.unixtime() < amb.unixtime());
}

void cPin::PararPinsDurantUnsMinuts(int minuts)
{

	if (valPinActual == 1)
	{
		int segons = minuts * 60;

		DateTime ara = rtc->now();
		DateTime despres = ara + segons;

		Serial.print(F("Ara són les : ")); SerialPrintDate(ara);
		Serial.print(F("Parada programada per les : "));  SerialPrintDate(despres);

		horaProperaArrencadaPerProgramacio = despres;
		paratPerProgramacio = true;

		valPinActual = 0;

		utils->EscriuPinDigital(valPinActual, numPin);

		lcdAcuari->Print1Line("Ok, executat.");

	}
	else
	{
		Serial.print(F("Ja estava parat"));
	}

}

void cPin::para10Minuts()
{
	PararPinsDurantUnsMinuts(1);
}

void cPin::para30Minuts()
{
	PararPinsDurantUnsMinuts(2);
}

void cPin::para1Hora()
{
	PararPinsDurantUnsMinuts(5);
}

bool cPin::Desactiva()
{

}

cPin::~cPin()
{
}

//uint16_t quinyear = quin.year();
//uint8_t quinmonth = quin.month();
//uint8_t quinday = quin.day();
//uint8_t quinhour = quin.hour() ;
//uint8_t quinminute = quin.minute();
//uint8_t quinsecond = quin.second();

//  uint16_t ambyear = amb.year();
//uint8_t ambmonth = amb.month();
//uint8_t ambday = amb.day();
//uint8_t ambhour = amb.hour();
//uint8_t ambminute = amb.minute();
//uint8_t ambsecond = amb.second();

//if (quinyear < ambyear) return true;
//else if ((quinyear >= ambyear) & (quinmonth < ambmonth)) return true;
//else if (((quinyear >= ambyear) & (quinmonth >= ambmonth)) & (quinday < ambday))  return true;
//else if ((((quinyear >= ambyear) & (quinmonth >= ambmonth)) & (quinday >= ambday))  & (quinhour < ambhour))  return true;
//else if (((((quinyear >= ambyear) & (quinmonth >= ambmonth)) & (quinday >= ambday)) & (quinhour >= ambhour)) & (quinminute < ambminute)) return true;
//else if ((((((quinyear >= ambyear) & (quinmonth >= ambmonth)) & (quinday >= ambday)) & (quinhour >= ambhour)) & (quinminute >= ambminute)) & (quinsecond < ambsecond))   return true;
//else return false;