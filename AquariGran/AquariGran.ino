#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <MenuBackend.h>   // ho he trobat aquí, hi ha exemples   https://forum.arduino.cc/index.php?topic=38053.0
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
#include "cPin.h"
#include "Eines.h"
//#include <ArduinoJson.h>
//#include <aJSON.h>


Eines utils = Eines();

//////////////////////////////////////////////////////////////// Variables LCD //////////////////////////////////////////////////////////////

LiquidCrystal_I2C lcd(0x3F, 16, 2);  //Atenció la nostre fa servir la 0x3F per defecte sol ser 0x27 !!!
byte rodoneta[8] = {
	B00000,
	B01100,
	B11110,
	B01100,
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
		char* cntBombaSump = "Bomba Sump";
		char* cntCalentador = "Calentador";
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
		MenuItem mRoot = MenuItem("Root");
		MenuItem mEstatReles = MenuItem(cntEstatReles);
		MenuItem mSkimmer = MenuItem(cntSkimmer);
		MenuItem mBombaCircuilacio1 = MenuItem(cntBombaCircuilacio1);
		MenuItem mBombaCircuilacio2 = MenuItem(cntBombaCircuilacio2);
		MenuItem mBombesPujada = MenuItem(cntBombesPujada);
		MenuItem mBombaSump = MenuItem(cntBombaSump);
		MenuItem mCalentador = MenuItem(cntCalentador);
		MenuItem mRefrigerador = MenuItem(cntRefrigerador);;
		MenuItem mVentiladorAcuari = MenuItem(cntVentiladorAcuari);
		MenuItem mVentiladorInternSump = MenuItem(cntVentiladorInternSump);
		MenuItem mLlumRefugi = MenuItem(cntLlumRefugi);
		MenuItem mUV = MenuItem(cntUV);
		MenuItem mAccions = MenuItem(cntAccions);
		MenuItem mDonardeMenjar = MenuItem(cntDonardeMenjar);
		MenuItem mPararCirculacio = MenuItem(cntPararCirculacio);
		MenuItem mCanviAigua = MenuItem(cntCanviAigua);


#pragma endregion

//////////////////////////////////////////////////////////////// Variables Relés //////////////////////////////////////////////////////////////
#pragma region Variables Reles

int numPinReleSkimmer = 3;
int numPinReleBombaCirc1 = 6;
int numPinReleBombaCirc2 = 2;
int numPinReleBombesPujada = 2;
int numPinReleBombaSump = 2;
int numPinReleCalentador = 2;
int numPinReleRefrigerador = 2;
int numPinReleVentiladorAcuari = 2;
int numPinRelemVentiladorInternSump = 2;
int numPinReleLlumRefugi = 2;
int numPinReleUV = 2;

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

#pragma endregion

//////////////////////////////////////////////////////////////// Variables Rellotges //////////////////////////////////////////////////////////////
#pragma region Rellotges

	RTC_DS3231 rtc;
	String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
	String monthsNames[12] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre" };


#pragma endregion


//////////////////////////////////////////////////////////////// Variables Micro SD Card //////////////////////////////////////////////////////////////

#pragma region SD Card
	
	File myFile;
	String nomfitxerConfig = "confPins.txt";

#pragma endregion

/////////////////////////////////////////////////////////////// Variables Control //////////////////////////////////////////////////////////////
#pragma region Control

	int SetupCorrecte = 1;
	String ErrorSetup = "";

#pragma endregion


/////////////////////////////////////////////////////////////// Variables Json Parser //////////////////////////////////////////////////////////////
#pragma region Json Parser

#pragma endregion



void menuSetup()
{
	Serial.println("Configurant Menú...");

	mEstatReles.addRight(mSkimmer).addRight(mBombaCircuilacio1).addRight(mBombaCircuilacio2).addRight(mBombesPujada).addRight(mBombaSump).addRight(mCalentador).addRight(mRefrigerador).addRight(mVentiladorAcuari).addRight(mVentiladorInternSump).addRight(mLlumRefugi).addRight(mUV);
	mAccions.addRight(mDonardeMenjar).addRight(mPararCirculacio).addRight(mCanviAigua);

	menu.getRoot().add(mEstatReles).add(mAccions);
}

void menuUseEvent(MenuUseEvent used)
{
	Serial.print("Menu use ");
	Serial.println(used.item.getName());

	if (used.item.getName() == cntSkimmer) //comparison using a string literal
	{
		Serial.println("menuUseEvent found Skimmer");

		//if (valpinReleSkimmer == 1)
		//	valpinReleSkimmer = 0;
		//else
		//	valpinReleSkimmer = 1;

		InverteixValorRele(valpinReleSkimmer);

		Serial.print("Valor Pin Skimmer : "); Serial.println(valpinReleSkimmer);

		EscriuPinDigital(valpinReleSkimmer, pinReleSkimmer);

		if (valpinReleSkimmer == 1)
		{
			Print2Line("Activat");
		}
		else
		{
			Print2Line("Desactivat");
		}
	}

}

void InverteixValorRele(int &valpinRele)
{
	if (valpinRele == 1)
		valpinRele = 0;
	else
		valpinRele = 1;
}



void menuChangeEvent(MenuChangeEvent changed)
{
	Serial.print("Menu change ");
	Serial.print(changed.from.getName());
	
	Serial.print(" ");
	Serial.println(changed.to.getName());
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

void setup() {

	lcd.init();
	lcd.backlight();
	lcd.createChar(0, rodoneta);

	pinMode(joyPinX, INPUT);
	pinMode(joyPinY, INPUT);
	pinMode(joyPush, INPUT_PULLUP);

	//pinMode(pinReleSkimmer, OUTPUT);
	//pinMode(pinReleBombaCirc1, OUTPUT);
	//pinMode(pinReleBombaCirc2, OUTPUT);
	//pinMode(pinReleBombesPujada, OUTPUT);
	//pinMode(pinReleBombaSump, OUTPUT);
	//pinMode(pinReleCalentador, OUTPUT);
	//pinMode(pinReleRefrigerador, OUTPUT);
	//pinMode(pinReleVentiladorAcuari, OUTPUT);
	//pinMode(pinRelemVentiladorInternSump, OUTPUT);
	//pinMode(pinReleLlumRefugi, OUTPUT);
	//pinMode(pinReleUV, OUTPUT);

	Serial.begin(9600);

	menuSetup();
	EscriuPantalla();
	Serial.println("Comencem navegació pel menú");

	utils.EscriuPinDigital(valpinReleSkimmer, numPinReleSkimmer);

	if (!rtc.begin()) {
		Serial.println(F("Couldn't find RTC"));
		SetupCorrecte = 0;
		ErrorSetup = "Couldn't find RTC";
	}

	if (!SD.begin(4)) {
		Serial.println("SD Incorrecte");
		SetupCorrecte = 0;
		ErrorSetup = "SD Incorrecte";
		return;
	}

	// Si se ha perdido la corriente, fijar fecha y hora
	if (rtc.lostPower()) {
		// Fijar a fecha y hora de compilacion
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

		// Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
		// rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
	}

	pinSkimmer = cPin(cntSkimmer, numPinReleSkimmer, valpinReleSkimmer, 'O', 'A');
	
	programacioDiaria progBombaSump = { -1, { {1000,1015},{1100,1130} } };
	pinBombaSump = cPin(cntBombaSump,  numPinReleBombaSump, valpinReleBombaSump, 'T', 'D', progBombaSump);

	programacioDiaria progUVDilluns = { 1,{ { 1000,1015 },{ 1100,1130 } } };
	programacioDiaria progUVDimarts = { 2,{ { 1000,1015 },{ 1100,1130 } } };
	programacioSetmanal progUV = { progUVDilluns, progUVDimarts };
	pinUV = cPin(cntUV, numPinReleUV, valpinReleUV, 'T', 'S', progUV);


	if (SetupCorrecte == 0)
	{
		Serial.println(ErrorSetup);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.println(ErrorSetup);

	}

}

void loop() {
	
	
	if (SetupCorrecte == 0) // si el setup ha anat malament no fem res.
	{
		Serial.println("dins Setup Incorrecte");
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
		//printDate(now);

		if (!digitalRead(joyPush)) // si apreta el botó del mig del joystick
		{
			Serial.println("Hem apretat");
			menu.use();
			delay(100);
		}
		else
		{
			// mirem si ha apretat a la X o a la Y
			valueX = TransformaValorJoystick(analogRead(joyPinX));
			if (valueX == 0)  // valueX pot ser 1 o 0, si és -1 no fer res.
			{
				menu.moveRight();
				EscriuPantalla();
				delay(100);
			}
			else if (valueX == 1)
			{
				menu.moveLeft();
				EscriuPantalla();
				delay(100);
			}

			valueY = TransformaValorJoystick(analogRead(joyPinY));
			if (valueY == 0)  // valueY pot ser 1 o 0, si és -1 no fer res.
			{
				menu.moveDown();
				EscriuPantalla();
				delay(100);
			}
			else if (valueY == 1)
			{
				menu.moveUp();
				EscriuPantalla();
				delay(100);
			}

		}

	}

	delay(100);

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
		if (menu.getCurrent().getName() == "Skimmer")
		{
			if (valpinReleSkimmer == 1)
			{
				Print2Line("Activat      ");
			}
			else
			{
				Print2Line("Desactivat   ");
			}
		}

	}
}

void Print2Line(char* val)
{
	lcd.setCursor(2, 1);
	lcd.print(val);
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

