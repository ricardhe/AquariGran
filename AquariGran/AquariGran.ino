#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//#include <MenuBackend.h>   // ho he trobat aquí, hi ha exemples   https://forum.arduino.cc/index.php?topic=38053.0

#include <MenuBackend.h>

#include <Wire.h>
#include "RTClib.h"
#include "cPin.h"
#include "SDAcuari.h"
#include "ControlAplicacio.h"

//#include <Time.h>
//#include <DS1307RTC.h>

//#include <DateTimeStrings.h>

#include "MemoryFree.h"


	Eines utils = Eines();

	const char ObertPerDefecte = 'O';
	const char TancatPerDefecte = 'T';
	const char TipusProgramacioDiaria = 'D';
	const char TipusProgramacioSetmanal = 'S';
	const char TipusProgramacioSempreObert = 'A';

    char* operActiu = "ActiuoNo";
	char* operEstat = "Estat";
	char* oper10Min = "oper10Min";
	char* oper30Min = "oper30Min";
	char* oper1Hora = "oper1Hora";

//////////////////////////////////////////////////////////////// Variables LCD //////////////////////////////////////////////////////////////

	LiquidCrystal_I2C lcd(0x3F, 16, 2);  //Atenció la nostre fa servir la 0x3F per defecte sol ser 0x27 !!!

	LCDAcuari lcdAcuari = LCDAcuari(lcd);

	byte rodoneta[8] = {
		B00000,
		B01100,
		B11110,
		B01100,
		B00000,
		B00000,
		B00000,
	};

	byte Punter[8] = {
		B00000,
		B00000,
		B11111,
		B11111,
		B00000,
		B00000,
		B00000,
	};

//////////////////////////////////////////////////////////////// Variables Joystick //////////////////////////////////////////////////////////////
	
	int joyPinX = A1; // X Axis Joytick
	int joyPinY = A2; // Y Axis Joytick
	int joyPush = 5; // botó de pulsació
	int valueY = 0; // variable per Y Axis
	int valueX = 0; // variable per X Axis
	int valPush = 0; //valor pulsació Joystick


					 //////////////////////////////////////////////////////////////// Variables Rellotges //////////////////////////////////////////////////////////////
#pragma region Rellotges

	RTC_DS3231 rtc;
	String daysOfTheWeek[7] = { "Diumenge", "Dilluns", "Dimarts", "Dimecres", "Dijous", "Divendres", "Dissabte" };
	String monthsNames[12] = { "Gener", "Febrer", "Marc", "Abril", "Maig",  "Juny", "Juliol","Agost","Septembre","Octubre","Novembre","Desembre" };

#pragma endregion


	//////////////////////////////////////////////////////////////// Variables Micro SD Card //////////////////////////////////////////////////////////////

#pragma region SD Card

	File myFile;
	String nomfitxerConfig = "confPins.txt";

	SDAcuari sdAcuari = SDAcuari(nomfitxerConfig);

#pragma endregion

	/////////////////////////////////////////////////////////////// Variables Control //////////////////////////////////////////////////////////////
#pragma region Control

	int SetupCorrecte = 1;
	String ErrorSetup = "";

	//bool algunMenuTeElControl = false;

	ControlAplicacio  control = ControlAplicacio();

	//control.algunMenuTeElControl = false;


	cPin* PinAmbControl;

#pragma endregion

//////////////////////////////////////////////////////////////// Variables Menús //////////////////////////////////////////////////////////////

#pragma region Variables Menús
	/*
		Root

		Estat Relés
				Skimmer
				Bomba Circulació 1
				Bomba Circulació 2
				Bombes Pujada
				Bomba Sump
				Calentador
				Refrigerador
				Ventilador Acuari
				Ventilador Intern Sump
				Llum Refugi
				UV

		Accions
				Donar de Menjar
				Parar Circulació
				Canvi d'Aigua
	*/

	const char cntEstatReles[] = "Estat Reles";

	char* cntSkimmer = "Skimmer";
	char* cntBombaCircuilacio1 = "Bomba Circ 1";
	char* cntBombaCircuilacio2 = "Bomba Circ 2";
	char* cntBombesPujada = "Bombes Pujada";
	char* cntBombaSump =    "Bomba Sump";
	char* cntCalentador =   "Calentador";
	char* cntRefrigerador = "Refrigerador";
	char* cntVentiladorAcuari = "Ventil. Acuari";
	char* cntVentiladorInternSump = "Ventil. Sump";
	char* cntLlumRefugi = "Llum Refugi";
	char* cntUV = "UV";
	char* cntAccions = "Accions";
	char* cntDonardeMenjar = "Donar de Menjar";
	char* cntPararCirculacio = "Parar Circulació";
	char* cntCanviAigua = "Canvi dAigua";

	//this controls the menu backend and the event generation
	MenuBackend menu = MenuBackend(menuUseEvent, menuChangeEvent);
	MenuItem mRoot = MenuItem("Root", 'A');
	MenuItem mEstatReles = MenuItem(cntEstatReles,'B');
	MenuItem mSkimmer = MenuItem(cntSkimmer, 'C');
	MenuItem mBombaCircuilacio1 = MenuItem(cntBombaCircuilacio1, 'D');
	MenuItem mBombaCircuilacio2 = MenuItem(cntBombaCircuilacio2, 'E');
	MenuItem mBombesPujada = MenuItem(cntBombesPujada, 'F');
	MenuItem mBombaSump = MenuItem(cntBombaSump, 'G');
	MenuItem mCalentador = MenuItem(cntCalentador, 'H');
	MenuItem mRefrigerador = MenuItem(cntRefrigerador, 'I');
	MenuItem mVentiladorAcuari = MenuItem(cntVentiladorAcuari, 'J');
	MenuItem mVentiladorInternSump = MenuItem(cntVentiladorInternSump, 'K');
	MenuItem mLlumRefugi = MenuItem(cntLlumRefugi, 'L');
	MenuItem mUV = MenuItem(cntUV, 'M');
	MenuItem mAccions = MenuItem(cntAccions, 'N');
	MenuItem mDonardeMenjar = MenuItem(cntDonardeMenjar, 'O');
	MenuItem mPararCirculacio = MenuItem(cntPararCirculacio, 'P');
	MenuItem mCanviAigua = MenuItem(cntCanviAigua, 'Q');

	subMenu sbmActiuoNo = { "Actiu",   operActiu };
	subMenu sbmEstat = { "Estat",   operEstat };
	subMenu sbm10Min = { "P. 10 min",  oper10Min };
	subMenu sbm30Min = { "P. 30 min",  oper30Min };
	subMenu sbm1Hora = { "P. 1 hora",  oper1Hora };

	subMenu menusGenerics[5] = { sbmActiuoNo, sbmEstat ,sbm10Min ,sbm30Min ,sbm1Hora };


#pragma endregion

//////////////////////////////////////////////////////////////// Variables Relés //////////////////////////////////////////////////////////////
#pragma region Variables Reles

	int numPinReleSkimmer = 31;
	int numPinReleBombaCirc1 = 33;
	int numPinReleBombaCirc2 = 35;
	int numPinReleBombesPujada = 37;
	int numPinReleBombaSump = 39;
	int numPinReleCalentador = 41;
	int numPinReleRefrigerador = 43;
	int numPinReleVentiladorAcuari = 45;
	int numPinRelemVentiladorInternSump = 2;
	int numPinReleLlumRefugi = 2;
	int numPinReleUV = 30;

	int valpinReleSkimmer = 1;
	int valpinReleBombaCirc1 = 1;
	int valpinReleBombaCirc2 = 1;
	int valpinReleBombesPujada = 1;
	int valpinReleBombaSump = 1;
	int valpinReleCalentador = 0;
	int valpinReleRefrigerador = 0;
	int valpinReleVentiladorAcuari = 0;
	int valpinRelemVentiladorInternSump = 0;
	int valpinReleLlumRefugi = 0;
	int valpinReleUV = 0;

	cPin pinSkimmer ;
	cPin pinBombaCirc1 ;
	cPin pinBombaCirc2 ;
	cPin pinBombesPujada ;
	cPin pinBombaSump ;
	cPin pinCalentador ;
	cPin pinRefrigerador ;
	cPin pinVentiladorAcuari ;
	cPin pinVentiladorInternSump ;
	cPin pinLlumRefugi ;
	cPin pinUV;

	void ExecutaVoltaPins()
	{
		pinSkimmer.executemVolta();
		pinBombaCirc1.executemVolta();
		pinBombaCirc2.executemVolta();
		pinBombesPujada.executemVolta();
		pinBombaSump.executemVolta();
		pinCalentador.executemVolta();
		pinRefrigerador.executemVolta();
		pinVentiladorAcuari.executemVolta();
		pinVentiladorInternSump.executemVolta();
		pinLlumRefugi.executemVolta();
		//pinUV.executemVolta();
	}

	//void copy(int* src, int* dst, int len) {
	//		memcpy(dst, src, sizeof(src[0])*len);
	//}


	void ConfigurarPins()
	{

		//#pragma region UV

		//programacioSetmanal progSemUV = programacioSetmanal();

		programacioDiaria prUVDilluns = programacioDiaria();
		prUVDilluns.intervalsDiaris[0] = { "00:05","0900" };
		prUVDilluns.numIntervals = 1;
		prUVDilluns.diaDeLaSetmana = 1; //dilluns

		programacioDiaria prUVDimarts = programacioDiaria();
		prUVDimarts.intervalsDiaris[0] = { "00:05","0900" };
		prUVDimarts.numIntervals = 1;
		prUVDimarts.diaDeLaSetmana = 2; 

		programacioDiaria prUVDimecres = programacioDiaria();
		prUVDimecres.intervalsDiaris[0] = { "00:05","0900" };
		prUVDimecres.numIntervals = 1;
		prUVDimecres.diaDeLaSetmana = 3;

		programacioDiaria prUVDijous = programacioDiaria();
		prUVDijous.intervalsDiaris[0] = { "00:05","0900" };
		prUVDijous.numIntervals = 1;
		prUVDijous.diaDeLaSetmana = 3;

		programacioDiaria prUVDivendres = programacioDiaria();
		prUVDivendres.intervalsDiaris[0] = { "00:05","0900" };
		prUVDivendres.numIntervals = 1;
		prUVDivendres.diaDeLaSetmana = 5; //divendres

		programacioDiaria prUVDissabte = programacioDiaria();
		prUVDissabte.intervalsDiaris[0] = { "00:05","0900" };
		prUVDissabte.numIntervals = 1;
		prUVDissabte.diaDeLaSetmana = 3;

		programacioDiaria prUVDiumenge= programacioDiaria();
		prUVDiumenge.intervalsDiaris[0] = { "00:05","0900" };
		prUVDiumenge.numIntervals = 1;
		prUVDiumenge.diaDeLaSetmana = 3;

		programacioDiaria prBuida = programacioDiaria();
		prBuida.numIntervals = 0;

		pinUV = cPin(cntUV, numPinReleUV, valpinReleUV, TancatPerDefecte, TipusProgramacioSetmanal,1, prUVDilluns, prUVDimarts, prUVDimecres, prUVDijous, prUVDivendres, prUVDissabte, prUVDiumenge, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		//#pragma endregion

#pragma region Skimmer
		Serial.println("Skimmer");
		pinSkimmer = cPin(cntSkimmer, numPinReleSkimmer, valpinReleSkimmer, ObertPerDefecte, TipusProgramacioSempreObert, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

#pragma endregion

		#pragma region BombaCirculacio1
		Serial.println("pinBombaCirc1");
			pinBombaCirc1 = cPin(cntBombaCircuilacio1,  numPinReleBombaCirc1, valpinReleBombaCirc1, ObertPerDefecte, TipusProgramacioSempreObert, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion

		#pragma region BombaCirculacio2

			pinBombaCirc2 = cPin(cntBombaCircuilacio2, numPinReleBombaCirc2, valpinReleBombaCirc2, ObertPerDefecte, TipusProgramacioSempreObert, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion

		#pragma region BombesPujada

			pinBombesPujada = cPin(cntBombesPujada, numPinReleBombesPujada, valpinReleBombesPujada, ObertPerDefecte, TipusProgramacioSempreObert, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion

		#pragma region BombaSump
		
			programacioDiaria progBombaSump = programacioDiaria();
			progBombaSump.intervalsDiaris[0] = { "0730", "0900" };
			progBombaSump.intervalsDiaris[1] = { "1300", "1500" };
			progBombaSump.intervalsDiaris[2] = { "1900", "2100" };
			progBombaSump.intervalsDiaris[3] = { "0010", "0400" };

			progBombaSump.numIntervals = 4;
			progBombaSump.diaDeLaSetmana = -1;

			pinBombaSump = cPin(cntBombaSump, numPinReleBombaSump, valpinReleBombaSump, TancatPerDefecte, TipusProgramacioDiaria, progBombaSump, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion

		#pragma region Calentador

			pinCalentador = cPin(cntCalentador, numPinReleCalentador, valpinReleCalentador, TancatPerDefecte, TipusProgramacioSempreObert, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion

		#pragma region Refrigerador

			pinRefrigerador = cPin(cntRefrigerador, numPinReleRefrigerador, valpinReleRefrigerador, TancatPerDefecte, TipusProgramacioSempreObert, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion

		#pragma region Ventilador Acuari

			 pinVentiladorAcuari = cPin(cntVentiladorAcuari, numPinReleVentiladorAcuari, valpinReleVentiladorAcuari, TancatPerDefecte, TipusProgramacioSempreObert, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion

		#pragma region VentiladorInternSump

			 pinVentiladorInternSump = cPin(cntVentiladorInternSump, numPinRelemVentiladorInternSump, valpinRelemVentiladorInternSump, TancatPerDefecte, TipusProgramacioSempreObert, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion

		#pragma region Llum Refugi



			 programacioDiaria progLlumRefugi = programacioDiaria();
			 progLlumRefugi.intervalsDiaris[0] = { "0730", "0900" };
			 progLlumRefugi.numIntervals = 1;
			 progLlumRefugi.diaDeLaSetmana = -1;

			 pinLlumRefugi = cPin(cntLlumRefugi, numPinReleLlumRefugi, valpinReleLlumRefugi, TancatPerDefecte, TipusProgramacioDiaria, progLlumRefugi, menusGenerics, 5, sdAcuari, utils, lcdAcuari);

		#pragma endregion



	}


#pragma endregion




void menuSetup()
{
	//Serial.println(F("Config Menú..."));

	mEstatReles.addRight(mSkimmer).addRight(mBombaCircuilacio1).addRight(mBombaCircuilacio2).addRight(mBombesPujada).addRight(mBombaSump).addRight(mCalentador).addRight(mRefrigerador).addRight(mVentiladorAcuari).addRight(mVentiladorInternSump).addRight(mLlumRefugi).addRight(mUV);
	mAccions.addRight(mDonardeMenjar).addRight(mPararCirculacio).addRight(mCanviAigua);

	menu.getRoot().add(mEstatReles).add(mAccions);
}

void menuUseEvent(MenuUseEvent used)
{

	//Serial.print(F("Menu use "));
	//Serial.println(used.item.getName());

	cPin pinSeleccionat;

	PinAmbControl = 0;

	if (used.item.getName() == cntSkimmer) //comparison using a string literal
	{
		
		//pinSkimmer.agafaElControl(control, used.item.getShortkey(), rtc); //menu, used.item.getShortkey()		
		PinAmbControl = &pinSkimmer;
	}
	else if (used.item.getName() == cntBombaCircuilacio1)
	{
		PinAmbControl = &pinBombaCirc1;
	}
	else if (used.item.getName() == cntBombaCircuilacio2)
	{
		PinAmbControl = &pinBombaCirc2;
	}
	else if (used.item.getName() == cntBombesPujada)
	{
		PinAmbControl = &pinBombesPujada;
	}
	else if (used.item.getName() == cntBombaSump)
	{
		PinAmbControl = &pinBombaSump;
	}
	else if (used.item.getName() == cntCalentador)
	{
		PinAmbControl = &pinCalentador;
	}
	else if (used.item.getName() == cntRefrigerador)
	{
		PinAmbControl = &pinRefrigerador;
	}
	else if (used.item.getName() == cntVentiladorAcuari)
	{
		PinAmbControl = &pinVentiladorAcuari;
	}
	else if (used.item.getName() == cntVentiladorInternSump)
	{
		PinAmbControl = &pinVentiladorInternSump;
	}
	else if (used.item.getName() == cntLlumRefugi)
	{
		PinAmbControl = &pinLlumRefugi;
	}
	else if (used.item.getName() == cntUV)
	{
		PinAmbControl = &pinUV;
	}
	
	if (PinAmbControl != 0)
		PinAmbControl->agafaElControl(control, used.item.getShortkey(), rtc); //menu, used.item.getShortkey()		

}

void menuChangeEvent(MenuChangeEvent changed)
{
	//Serial.print(F("Menu change "));
	//Serial.print(changed.from.getName());

	//Serial.print(" ");
	//Serial.println(changed.to.getName());
}


int TransformaValorJoystick(int val) //transformem en un valor discret el valor del joystick
{
	//posem un limit per considerar que s'està fent una acció a l'eix de les X o les Y, ja que al ser analògic, dona un rang, no un valor absolut
	if (val <= 150)
		return 0;
	else if (val >= 900)
		return 1;
	else
		return -1;
}

void setup() {
	
	Serial.begin(9600);

	Serial.println(F("Ini"));
	

	lcd.init();
	lcd.backlight();
	lcd.createChar(0, rodoneta);

	lcd.createChar(1, Punter);

	pinMode(joyPinX, INPUT);
	pinMode(joyPinY, INPUT);
	pinMode(joyPush, INPUT_PULLUP);


	menuSetup();
	EscriuPantalla();
	//Serial.println(F("Ini Nav menú"));

	//utils.EscriuPinDigital(valpinReleSkimmer, numPinReleSkimmer);

	if (!rtc.begin()) {
		Serial.println(F("RTC Not F"));
		SetupCorrecte = 0;
		ErrorSetup = F("RTC Not F");
	}
	
	if (!sdAcuari.carregaConfiguracio(true))
	{
		Serial.println(F("Fitx Config Error"));
		SetupCorrecte = 0;
		ErrorSetup = F("Fitx Config Error");
		return;
	}

	// Si se ha perdido la corriente, fijar fecha y hora
	if (rtc.lostPower()) {
		// Fijar a fecha y hora de compilacion
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

		// Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
		//rtc.adjust(DateTime(2017, 10, 31, 22, 37, 0));
	}

	//rtc.adjust(DateTime(2017, 10, 31, 22, 42, 0));

	printDate(rtc.now());

	ConfigurarPins();

	if (SetupCorrecte == 0)
	{
		Serial.println(ErrorSetup);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.println(ErrorSetup);
	}

	Serial.print("freeMemory()=");        Serial.println(freeMemory());

}


void loop() {
	
	byte tempsDelay = 75;
	
	if (SetupCorrecte == 0) // si el setup ha anat malament no fem res.
	{
		Serial.println(F(" Setup Incorrecte"));
		Serial.println(ErrorSetup);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.println(ErrorSetup);
		delay(1000);
		return;
	}
	else
	{
		DateTime now = rtc.now();
		if (!digitalRead(joyPush)) // si apreta el botó del mig del joystick
		{
			if (control.algunMenuTeElControl)
			{
				PinAmbControl->executaSubMenu();
			}
			else
			{
				Serial.println(F("Click"));
				menu.use();
				delay(tempsDelay);
			}
		}
		else
		{
			// mirem si ha apretat a la X o a la Y
			valueX = TransformaValorJoystick(analogRead(joyPinX));
			if (valueX == 0)  // valueX pot ser 1 o 0, si és -1 no fer res.
			{
				if (control.algunMenuTeElControl)
				{
				}
				else
				{
					menu.moveRight();
					EscriuPantalla();
					delay(tempsDelay);
				}
			}
			else if (valueX == 1)
			{
				if (control.algunMenuTeElControl)
				{
				}
				else
				{
					menu.moveLeft();
					EscriuPantalla();
					delay(tempsDelay);
				}
			}

			valueY = TransformaValorJoystick(analogRead(joyPinY));
			if (valueY == 0)  // valueY pot ser 1 o 0, si és -1 no fer res.
			{
				if (control.algunMenuTeElControl)
				{
					PinAmbControl->avancaSubmenu();
				}
				else
				{
					menu.moveDown();
					EscriuPantalla();
					delay(tempsDelay);
				}
			}
			else if (valueY == 1)
			{
				if (control.algunMenuTeElControl)
				{
					PinAmbControl->retrocedeixSubMenu();
				}
				else
				{
					menu.moveUp();
					EscriuPantalla();
					delay(tempsDelay);
				}
			}
		}
		ExecutaVoltaPins();
	}
	//Serial.print("freeMemory()=");        Serial.println(freeMemory());
	delay(tempsDelay);
}

void EscriuPantalla()
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.write(0);
	lcd.setCursor(2, 0);
	lcd.print(menu.getCurrent().getName());

	if (menu.getCurrent().getAfter())
	{
		lcd.setCursor(2, 1);
		lcd.print(menu.getCurrent().getAfter()->getName());
	}
	else
	{
		if (control.algunMenuTeElControl)
		{
			PinAmbControl->indicaStatusActivacioLCD(2);
		}

		//if (menu.getCurrent().getName() == "Skimmer")
		//{
		//	pinSkimmer.indicaStatusActivacioLCD(2);
		//}
	}
}


void printDate(DateTime date)
{
	Serial.print(date.year(), DEC);
	Serial.print('/');
	Serial.print(date.month(), DEC);
	Serial.print('/');
	Serial.print(date.day(), DEC);
	Serial.print(" (");
	Serial.print(daysOfTheWeek[date.dayOfTheWeek()]);
	Serial.print(") ");
	Serial.print(date.hour(), DEC);
	Serial.print(':');
	Serial.print(date.minute(), DEC);
	Serial.print(':');
	Serial.print(date.second(), DEC);
	Serial.println();
}

//void EscriuSubApartatPantalla()
//{
//	char currentM[] = menu.getCurrent().getName();
//	switch (currentM)
//	{
//		 case cntSkimmer:
//
//			if (valpinReleSkimmer == 1)
//			{
//				Print2Line("Activat");
//			}
//			else
//			{
//				Print2Line("Desactivat");
//			}
//		
//		break;
//	default:
//		break;
//	}
//
//}

