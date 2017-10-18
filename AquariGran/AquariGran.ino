#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <MenuBackend.h>   // ho he trobat aquí, hi ha exemples   https://forum.arduino.cc/index.php?topic=38053.0
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
#include <ArduinoJson.h>
//#include <aJSON.h>

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

		const char cntSkimmer[] = "Skimmer";
		const char cntBombaCircuilacio1[] = "Bomba Circ 1";
		const char cntBombaCircuilacio2[] = "Bomba Circ 2";
		const char cntBombesPujada[] = "Bombes Pujada";
		const char cntBombaSump[] = "Bomba Sump";
		const char cntCalentador[] = "Calentador";
		const char cntRefrigerador[] = "Refrigerador";
		const char cntVentiladorAcuari[] = "Ventil. Acuari";
		const char cntVentiladorInternSump[] = "Ventil. Sump";
		const char cntLlumRefugi[] = "Llum Refugi";
		const char cntUV[] = "UV";
		const char cntAccions[] = "Accions";
		const char cntDonardeMenjar[] = "Donar de Menjar";
		const char cntPararCirculacio[] = "Parar Circulació";
		const char cntCanviAigua[] = "Canvi dAigua";


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

int pinReleSkimmer = 3;
int pinReleBombaCirc1 = 6;
int pinReleBombaCirc2 = 2;
int pinReleBombesPujada = 2;
int pinReleBombaSump = 2;
int pinReleCalentador = 2;
int pinReleRefrigerador = 2;
int pinReleVentiladorAcuari = 2;
int pinRelemVentiladorInternSump = 2;
int pinReleLlumRefugi = 2;
int pinReleUV = 2;

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
	StaticJsonBuffer<1000> jsonBuffer;
	//DynamicJsonBuffer  jsonBuffer(200);
	String contingutFitxerConf;

#define MAX_FILE_SIZE 1024
#define JSON_BUFFER_SIZE 200

	void readFile(const char *fileName, char *buffer, size_t maxSize) {
		File file = SD.open(fileName, FILE_READ);
		file.readBytes(buffer, maxSize);
		file.close();
	}



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

void EscriuPinDigital(int valPin, int numPin)
{ 
	if (valPin == 1)
	{
		Serial.println("HIGH");
		digitalWrite(numPin, HIGH);
	}
	else
	{
		Serial.println("LOW");
		digitalWrite(numPin, LOW);
	}
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

	pinMode(pinReleSkimmer, OUTPUT);
	pinMode(pinReleBombaCirc1, OUTPUT);
	pinMode(pinReleBombaCirc2, OUTPUT);
	pinMode(pinReleBombesPujada, OUTPUT);
	pinMode(pinReleBombaSump, OUTPUT);
	pinMode(pinReleCalentador, OUTPUT);
	pinMode(pinReleRefrigerador, OUTPUT);
	pinMode(pinReleVentiladorAcuari, OUTPUT);
	pinMode(pinRelemVentiladorInternSump, OUTPUT);
	pinMode(pinReleLlumRefugi, OUTPUT);
	pinMode(pinReleUV, OUTPUT);

	Serial.begin(9600);

	menuSetup();
	EscriuPantalla();
	Serial.println("Comencem navegació pel menú");

	EscriuPinDigital(valpinReleSkimmer, pinReleSkimmer);

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

	contingutFitxerConf = "";
	//myFile = SD.open(nomfitxerConfig, FILE_READ);//abrimos  el archivo 
	//if (myFile) {
	//	if (myFile.available())
	//	{
	//		Serial.println(nomfitxerConfig);
	//		contingutFitxerConf = myFile.readString();
	//	}
	//	
	//	//while (myFile.available()) {

	//	//	char caracter = myFile.read();
	//	//	contingutFitxerConf = contingutFitxerConf + caracter;

	//	//	//contingutFitxerConf = contingutFitxerConf + myFile.read();

	//	//	//Serial.write(myFile.read());
	//	//}
	//	myFile.close(); //cerramos el archivo
	//}
	//else {
	//	Serial.println("Error al abrir el archivo");
	//}

	//const byte bufferSize = 150; //define number of bytes expected on the JSON thread
	//char json[bufferSize]; //create a char array buffer that is 150 bytes in size in this case
	//myFile = SD.open(nomfitxerConfig, FILE_READ);//abrimos  el archivo 
	//if (myFile) {

	//		if (myFile.available())
	//		{
	//			myFile.readBytes(json, bufferSize);
	//			myFile.close();
	//			Serial.println(json); //prints whatever JSON string values were read
	//			StaticJsonBuffer<JSON_OBJECT_SIZE(12)> jsonBuffer; //We know our JSON string just has 12 objects only and no arrays, so our buffer is set to handle that many
	//			//DynamicJsonBuffer  jsonBuffer(200);
	//			JsonObject& root = jsonBuffer.parseObject(json);    // breaks the JSON string into individual items, saves into JsonObject 'root'
	//			if (!root.success()) {
	//				Serial.println("parseObject() failed");
	//				return;   //if parsing failed, this will exit the function and not change the values to 00

	//			}
	//			else { // update variables
	//				Serial.println("parseObject() success");
	//				
	//			}


	//		}
	//		else 
	//		{
	//			myFile.close(); //cerramos el archivo
	//		}
	//		
	//	}
	//	else {
	//		Serial.println("Error al abrir el archivo");
	//	}

	//char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

	//myFile = SD.open(nomfitxerConfig, FILE_READ);//abrimos  el archivo 
	//if (myFile) {

	//		if (myFile.available())
	//		{
	//			contingutFitxerConf = myFile.readString();
	//		}
	//		myFile.close();
	//}
	//Serial.println(contingutFitxerConf);

	////
	//// Step 1: Reserve memory space
	////
	//StaticJsonBuffer<200> jsonBuffer;

	////
	//// Step 2: Deserialize the JSON string
	////
	//JsonObject& root = jsonBuffer.parseObject(contingutFitxerConf);

	//if (!root.success())
	//{
	//	Serial.println("parseObject() failed");
	//	return;
	//}
	//else
	//{
	//	Serial.println("parseObject success");
	//}

	//Serial.println(contingutFitxerConf);
	//
	////char cJson[]= new char[contingutFitxerConf.length()];
	//JsonObject& root = jsonBuffer.parseObject(contingutFitxerConf.toCharArray());

	//if (!root.success()) {
	//	Serial.println("parseObject() failed");
	//	return;
	//}

	//char* skimmer = root["﻿Skimmer"];

	//Serial.println(skimmer);

//char json[MAX_FILE_SIZE];
//readFile("confPins.txt", json, MAX_FILE_SIZE);
//
//Serial.println(json); // <- SHOW ME THE OUTPUT OF THIS!!!!!
//
//StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
//
//JsonObject& root = jsonBuffer.parseObject(json);
//
//if (!root.success()) {
//	Serial.println("parseObject() failed");
//	return;
//}
//else
//{
//	Serial.println("parseObject() success");
//}
	
	//myFile = SD.open(nomfitxerConfig, FILE_READ);//abrimos  el archivo 
	//if (myFile) {

	//		if (myFile.available())
	//		{
	//			contingutFitxerConf = myFile.readString();
	//		}
	//		myFile.close();
	//}
	//Serial.println(contingutFitxerConf);


				//DynamicJsonBuffer  jsonBuffer(200);
	//JsonObject& root = jsonBuffer.parseObject(SD.open(nomfitxerConfig, FILE_READ));    // breaks the JSON string into individual items, saves into JsonObject 'root'
	//			if (!root.success()) {
	//				Serial.println("parseObject() failed");
	//				return;   //if parsing failed, this will exit the function and not change the values to 00

	//			}
	//			else { // update variables
	//				Serial.println("parseObject() success");
	//				
	//			}
	//aJsonObject* root = aJson.parse(contingutFitxerConf.c_str());

	//if (root != NULL) {
	//	Serial.println("Parsed successfully 1 " );
	//	aJsonObject* name = aJson.getObjectItem(root, "name");
	//	
	//	Serial.println(name);
	//}
	//else {
	//	Serial.println("Parsed error 1 ");
	//}
	//aJsonObject* jsonObject = aJson.parse(SD.open(nomfitxerConfig, FILE_READ));


	// Si se ha perdido la corriente, fijar fecha y hora
	if (rtc.lostPower()) {
		// Fijar a fecha y hora de compilacion
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

		// Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
		// rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
	}

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

