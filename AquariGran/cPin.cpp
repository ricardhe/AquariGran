#include "cPin.h"
#include "LCDAcuari.h"

#define OUTPUT 0x1

cPin::cPin()
{
}

cPin::cPin(char* pNom, int pNumPin, int ppinActiu, char ptipusOberturaDefecte, char ptipusProgramacio, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari)
{
	nom = pNom;
	numPin = pNumPin;
	pinActiu = ppinActiu;
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

cPin::cPin(char* pNom, int pNumPin, int ppinActiu, char ptipusOberturaDefecte, char ptipusProgramacio, programacioDiaria pProgDiaria, subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari)
{
	nom = pNom;
	numPin = pNumPin;
	pinActiu = ppinActiu;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	prgDiaria = pProgDiaria;
	sd = &psd;
	utils = &putils;
	lcdAcuari = &plcdAcuari;

	numSubOpcions = pnumSubOpcions;
	opcionsSubMenus = psubMenu;

	//Serial.println("Valors Prog : ");
	//for (int i = 0; i < pProgDiaria.numIntervals; i++)
	//{
	//	Serial.println(pProgDiaria.intervalsDiaris[i].ini);
	//	Serial.println(pProgDiaria.intervalsDiaris[i].fi);
	//}

	ExecutaConfiguracioInicial();
}

cPin::cPin(char* pNom, int pNumPin, int ppinActiu, char ptipusOberturaDefecte, char ptipusProgramacio, 
			programacioDiaria pr1, programacioDiaria pr2, programacioDiaria pr3, programacioDiaria pr4, programacioDiaria pr5, programacioDiaria pr6, programacioDiaria pr7, 
			subMenu psubMenu[], int pnumSubOpcions,  SDAcuari &psd, Eines &putils, LCDAcuari &plcdAcuari)
{
	Serial.println(F("ini d. Setm"));
	nom = pNom;
	numPin = pNumPin;
	pinActiu = ppinActiu;
	tipusOberturaDefecte = ptipusOberturaDefecte;
	tipusProgramacio = ptipusProgramacio;
	
	prgSetmanal.dies[0] = pr1;
	prgSetmanal.dies[1] = pr2;
	prgSetmanal.dies[2] = pr3;
	prgSetmanal.dies[3] = pr4;
	prgSetmanal.dies[4] = pr5;
	prgSetmanal.dies[5] = pr6;
	prgSetmanal.dies[6] = pr7;

	sd = &psd;
	utils = &putils;
	lcdAcuari = &plcdAcuari;

	numSubOpcions = pnumSubOpcions;
	opcionsSubMenus = psubMenu;
	Serial.println(F("exec d. Setm"));
	ExecutaConfiguracioInicial();
	Serial.println(F("ini d. Setm"));
}

void cPin::ExecutaConfiguracioInicial()
{
	pinMode(numPin, OUTPUT);
	Serial.println(F("pinModeFet"));

	int valor =  sd->getIntValueConfig(nom);

	if (valor != -1)
	{
		Serial.print(F("Val Recup Fitxer, ")); Serial.print(nom); Serial.print(' '); Serial.println(valor);
		pinActiu = valor;
	}

	if (tipusProgramacio == TipusProgramacioSempreObert && pinActiu == 1)
		valPinActual = 1;
	else
		valPinActual = 0;

	utils->EscriuPinDigital(valPinActual, numPin);
}

bool cPin::setValorpinActiuInvertit()
{
	bool bRet = false;

	utils->InverteixValorRele(pinActiu);

	if (pinActiu == 0)
	{
		valPinActual = 0;
		utils->EscriuPinDigital(valPinActual, numPin);
	}

	bRet = sd->saveValueConfig(pinActiu, nom);
	sd->carregaConfiguracio(false);

	return bRet;
}

bool cPin::setValorActualInvertit()
{
	bool bRet = true;

	utils->InverteixValorRele(valPinActual);
	utils->EscriuPinDigital(valPinActual, numPin);

	return bRet;
}

void cPin::indicaStatusActivacioLCD(byte numLinea)
{
	//if (valPinActual == 1)
	//{
	//	if (numLinea == 1)
	//		lcdAcuari->Print1Line("Activat");
	//	else
	//		lcdAcuari->Print2Line("Estat: Activat");
	//}
	//else
	//{
	//	if (numLinea == 1)
	//		lcdAcuari->Print1Line("Desactiv.");
	//	else
	//		lcdAcuari->Print2Line("Estat: Desactiv.");
	//}

		
	if (valPinActual == 1)
		lcdAcuari->Print2Line("Pin Actiu: Si");
	else
		lcdAcuari->Print2Line("Pin Actiu: No");
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

void cPin::DibuixaPrimeraOpcioSubMenu(subMenu sub) {

	if (sub.operacio == operActiu)
	{
		if (pinActiu == 1)
			lcdAcuari->Print1Line("Pin Oper.: Si");
		else
			lcdAcuari->Print1Line("Pin Oper.: No");
	}
	else if (sub.operacio == operEstat && pinActiu == 1)
	{
		if (valPinActual == 1)
			lcdAcuari->Print1Line("Pin Actiu: Si");
		else 
			lcdAcuari->Print1Line("Pin Actiu: No");
	}
	else if (pinActiu == 1)
	{
		lcdAcuari->Print1Line(sub.nom);
	}

}

void cPin::DibuixaSegonaOpcioSubMenu(subMenu sub) {
	if (sub.operacio == operActiu)
	{
		if (pinActiu == 1)
			lcdAcuari->Print2Line("Pin Oper.: Si");
		else
			lcdAcuari->Print2Line("Pin Oper.: No");
	}
	else if (sub.operacio == operEstat  && pinActiu == 1)
	{
		if (valPinActual == 1)
			lcdAcuari->Print2Line("Pin Actiu: Si");
		else
			lcdAcuari->Print2Line("Pin Actiu: No");
	}
	else if (pinActiu == 1)
	{
		lcdAcuari->Print2Line(sub.nom);
	}
}

void cPin::dibuixaSubMenus()
{
	Serial.println(indexSubMenuIntern);

	lcdAcuari->DibuixaPunter();

	DibuixaPrimeraOpcioSubMenu(opcionsSubMenus[indexSubMenuIntern]);

	if (indexSubMenuIntern == (numSubOpcions - 1)  || pinActiu == 0 )
	{
		lcdAcuari->Print2LineaEnBlanc();
	}
	else
	{
		DibuixaSegonaOpcioSubMenu(opcionsSubMenus[indexSubMenuIntern + 1]);
	}
}

void cPin::avancaSubmenu()
{
	if (pinActiu == 1)
	{
		if (indexSubMenuIntern == (numSubOpcions - 1))
		{
			indexSubMenuIntern = indexSubMenuIntern; // ho deixem igual que es quedi l'ultim
		}
		else
		{
			indexSubMenuIntern++;
		}
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
	
	if (oper == operActiu)
	{
		if (setValorpinActiuInvertit())
		{
			paratPerProgramacio = false;
			activatPerProgramacio = false;
			dibuixaSubMenus();
		}
	}
	else if (oper == operEstat)
	{

		if (setValorActualInvertit())
		{
			paratPerProgramacio = false;
			activatPerProgramacio = false;
			dibuixaSubMenus();
			delay(100);
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

bool cPin::AlgunIntervalAfectat(programacioDiaria prog, DateTime &HoraFi)
{
	DateTime ara = rtc->now();

	bool ret = false;

	//si és -1, vol dir que aplica a tots els dies, sino mirem si correspon
	if (prog.diaDeLaSetmana == -1 || prog.diaDeLaSetmana == ara.dayOfTheWeek() )
	{
		uint32_t araunix = ara.unixtime();

		for (short i = 0; i < prog.numIntervals; i++)
		{
			uint32_t ini = utils->TransformaHoraEnData(prog.intervalsDiaris[i].ini,ara).unixtime();
			uint32_t fi = utils->TransformaHoraEnData(prog.intervalsDiaris[i].fi, ara).unixtime();

			if (araunix >= ini && araunix <= fi)
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool cPin::AlgunIntervalSetmanalAfectat(programacioSetmanal prog, DateTime &HoraFi)
{
	bool bret = false;

	for (short i = 0; i < 7; i++)
	{
		if (prog.dies[i].numIntervals > 0)
		{
			if (AlgunIntervalAfectat(prog.dies[i], HoraFi))
			{
				bret = true;
				break;
			}
		}
	}

	return bret;
}

void cPin::executemVolta()
{
	//si hem parat per programació, vol dir que hem seleccionat para per 10, 30 min o una hora
	//per tant mentres estigui parat per pgroamació, no farem res més que esperar el temps de reiniciar el pin a 1
	if (paratPerProgramacio && valPinActual == 0)
	{
		activatPerProgramacio = false;//per si de cas hi ha algun error en el codi.

		DateTime ara = rtc->now();
		if ( utils->datatimeMesPetitQue(horaProperaArrencadaPerProgramacio, ara))
		{
			Serial.println(F("Reactivem pin"));

			valPinActual = 1;

			utils->EscriuPinDigital(valPinActual, numPin);
			paratPerProgramacio = false;
			dibuixaSubMenus();
		}
	}
	else if (activatPerProgramacio && valPinActual == 1)
	{
		DateTime  HoraFi;

		if (tipusProgramacio == TipusProgramacioDiaria)
		{
			if (!AlgunIntervalAfectat(prgDiaria, HoraFi))
			{
				valPinActual = 0;
				utils->EscriuPinDigital(valPinActual, numPin);
				activatPerProgramacio = false;
				horaProperaParadaPerProgramacio = NULL;
				dibuixaSubMenus();
			}
		}
		//else if (tipusProgramacio == TipusProgramacioSetmanal)
		//{
		//	if (!AlgunIntervalSetmanalAfectat(prgSetmanal, HoraFi))
		//	{
		//		valPinActual = 0;
		//		utils->EscriuPinDigital(valPinActual, numPin);
		//		activatPerProgramacio = false;
		//		horaProperaParadaPerProgramacio = NULL;
		//		dibuixaSubMenus();
		//	}
		//}
			

	}
	//si no estem en programació, mirarem si el pin es de tipus diari o setmanal per veure si té algun interval actiu.
	//pot ser que posteriorment al reseteig de una prorgramació quedés solapadas sobre una activació dinterval.
	//en aquest cas es reactivará en el temps restant
	else if (tipusProgramacio == TipusProgramacioDiaria && pinActiu == 1)
	{
		DateTime  HoraFi;
		if (AlgunIntervalAfectat(prgDiaria, HoraFi) && valPinActual == 0 && !paratPerProgramacio)
		{
			valPinActual = 1;
			utils->EscriuPinDigital(valPinActual, numPin);
			activatPerProgramacio = true;
			horaProperaParadaPerProgramacio = HoraFi;
			dibuixaSubMenus();
		}
	}
	//else if (tipusProgramacio == TipusProgramacioSetmanal && pinActiu == 1)
	//{
	//	DateTime  HoraFi;
	//	//prgSetmanal
	//	if (AlgunIntervalSetmanalAfectat(prgSetmanal, HoraFi) && valPinActual == 0 && !paratPerProgramacio)
	//	{
	//		valPinActual = 1;
	//		utils->EscriuPinDigital(valPinActual, numPin);
	//		activatPerProgramacio = true;
	//		horaProperaParadaPerProgramacio = HoraFi;
	//		dibuixaSubMenus();
	//	}

	//}

}

void cPin::perdElControl()
{
	indexSubMenuIntern = 0; // ho deixem igual que es quedi l'ultim
	control->algunMenuTeElControl = false;
	control->reAgafarUltimMenu = true;
	tincElControl = false;
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

		activatPerProgramacio = false;//per si de cas hi ha algun error en el codi.
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


//if (opcionsSubMenus[indexSubMenuIntern].operacio == operEstat)
//{
//	if (valPinActual == 1)
//	{
//		lcdAcuari->Print1Line("Activat");
//	}
//	else
//	{
//		lcdAcuari->Print1Line("Desactivat");
//	}
//}
//else
//{
//	if (pinActiu == 1)
//	{
//		lcdAcuari->Print1Line(opcionsSubMenus[indexSubMenuIntern].nom);
//	}
//}

//if (pinActiu == 1)
//{
//	if (indexSubMenuIntern == (numSubOpcions - 1))
//	{
//		lcdAcuari->Print2LineaEnBlanc();
//	}
//	else
//	{
//		lcdAcuari->Print2Line(opcionsSubMenus[indexSubMenuIntern + 1].nom);
//	}
//}