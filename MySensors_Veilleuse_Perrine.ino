/**
 * MySensors_Veilleuse
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0: GUILLOTON Laurent
 * Version 1.1 - 2017-10-24: Création du sketch initial
 * Version 1.2 - 2017-11-05: Modification des types de variables et du mode réveil
 *
 * DESCRIPTION
 *
 * MySensors_Veilleuse est un sketch d'une veilleuse connectée
 * utilisant un strip WS2812B et pilotable via Jeedom.
 *
 */


// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// ID du noeud
//#define MY_NODE_ID 5

// Enable repeater functionality for this node
#define MY_REPEATER_FEATURE

#include "Arduino.h"
#include <MySensors.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <TH02_dev.h>
//#include "SHT31.h"

/* Bloc de configuration du strip RGB */
#define STRIP_PIN  6  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define NUMBER_OF_LEDS 30 // Total number of attached relays
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

//SHT31 sht31 = SHT31();

// initialisation des constantes
unsigned long SLEEP_TIME = 60000; // Sleep time between reads (in milliseconds)
unsigned long lastRefreshTime = 0; // Use this to implement a non-blocking delay function
float lastTemp;
float lastHum;

int indent = 0;

// Fonction de réglage
int BRIGHTNESS = 255;
int RED = 75;
int GREEN = 0;
int BLUE = 127;
int MODE = 0;
int LUM_MIN = 0;
int LUM_MAX = 255;
int TEMPO = 20;

bool RGB_CHANGE = 0;
bool MODE_CHANGE = 0;

/* Bloc de création des Id et message Mysensors */
// ID des capteurs sur le noeud
#define CHILD_ID_MODE 1						// Identificateur du mode d'utilisation
#define CHILD_ID_RGB 2					    // Identificateur de la couleur RGB (0xFFFFFF)
#define CHILD_ID_BRIGHTNESS 3				// Identificateur de la luminosité
#define CHILD_ID_TEMP 4						// Identificateur de la température
#define CHILD_ID_HUM 5						// Identificateur de l'humidité
#define CHILD_ID_LUM_MIN 6
#define CHILD_ID_LUM_MAX 7
#define CHILD_ID_TEMPO 8

// Message MySensors
MyMessage modeMsg(CHILD_ID_MODE, V_VAR1);
MyMessage rgbMsg(CHILD_ID_RGB, V_RGB);
MyMessage dimmerMsg(CHILD_ID_BRIGHTNESS, V_PERCENTAGE);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgLumMin(CHILD_ID_LUM_MIN, V_VAR1);
MyMessage msgLumMax(CHILD_ID_LUM_MAX, V_VAR1);
MyMessage msgTempo(CHILD_ID_TEMPO, V_VAR1);


/* Prototype des fonctions */
void turnOff();
void set_rgb(int r, int g, int b, int l);
uint32_t Wheel(byte WheelPos);
void rainbowCycle(uint8_t wait);

void presentation()
{
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("MySensors_Veilleuse", "1.2");
	// Register all sensors to mySensors (they will be created as child devices)
	present(CHILD_ID_MODE, S_CUSTOM);
	present(CHILD_ID_RGB, S_RGB_LIGHT);
	present(CHILD_ID_BRIGHTNESS, S_DIMMER);
	present(CHILD_ID_TEMP, S_TEMP);
	present(CHILD_ID_HUM, S_HUM);
	present(CHILD_ID_LUM_MIN, S_CUSTOM);
	present(CHILD_ID_LUM_MAX, S_CUSTOM);
	present(CHILD_ID_TEMPO, S_CUSTOM);
}

void setup()
{
	Serial.begin(115200);
	TH02.begin();
	//	sht31.begin();
	strip.begin();
	strip.show();
	// Récupération des données du contrôleur
	MODE=request(CHILD_ID_MODE, V_VAR1);
	BRIGHTNESS=request(CHILD_ID_BRIGHTNESS, V_PERCENTAGE);
//	LUM_MIN=request(CHILD_ID_LUM_MIN, V_VAR1);
//	LUM_MAX=request(CHILD_ID_LUM_MAX, V_VAR1);
//	TEMPO=request(CHILD_ID_TEMPO, V_VAR1);
}

// The loop function is called in an endless loop
void loop()
{
	boolean needRefresh = (millis() - lastRefreshTime) > SLEEP_TIME;
	if (needRefresh)
	{
		lastRefreshTime = millis();
		//float Temperature = sht31.getTemperature();
		float Temperature = TH02.ReadTemperature();
		if (isnan(Temperature)) {
			Serial.println("Failed reading temperature from sensor");
		} else if (Temperature != lastTemp) {
			lastTemp = Temperature;
			send(msgTemp.set(Temperature, 1));
		}
		//float Humidity = sht31.getHumidity();
		float Humidity = TH02.ReadHumidity();
		if (isnan(Humidity)) {
			Serial.println("Failed reading humidity from sensor");
		} else if (Humidity != lastHum) {
			lastHum = Humidity;
			send(msgHum.set(Humidity, 1));
		}
	}
	if ((MODE==0) & (MODE_CHANGE==1))
	{
		// Mode Off
		turnOff();
		MODE_CHANGE=0;
	}
	else if ((MODE==1) && ((MODE_CHANGE==1)||(RGB_CHANGE==1)) && (BRIGHTNESS!=0))
	{
		// Mode On
		set_rgb(RED,GREEN,BLUE,BRIGHTNESS);
		MODE_CHANGE=0;
		RGB_CHANGE=0;
	}
	else if ((MODE==2))
	{
		// Mode Rainbow Cycle
		if (MODE_CHANGE==1)
		{
			MODE_CHANGE=0;
		}
		rainbowCycle(TEMPO);
	}
	else if ((MODE==4))
	{
		// Mode réveil lumineux
		if (MODE_CHANGE==1)
		{
			MODE_CHANGE=0;
			indent=LUM_MIN;
		}

		if (indent<=LUM_MAX)
		{
			set_rgb(RED,GREEN,BLUE,indent);
			delay(TEMPO*1000);
			indent++;
		}
	}
}

void receive(const MyMessage &message)
{
	int Type=message.type;

	if (Type == V_RGB)
	{
		String hexstring = message.getString();
		long number = (long) strtol( &hexstring[1], NULL, 16);
		// Split it up into r, g, b values
		BLUE = number &0xFF;
		GREEN = (number & 0xFF00 )>> 8;
		RED = (number & 0xFF0000) >> 16;
		RGB_CHANGE=1;
		send(rgbMsg.set(hexstring));
	}
	else if (Type == V_PERCENTAGE)
	{
		int val = atoi(message.data);
		Serial.print("Val = ");
		Serial.println(val);
		if (message.sensor==CHILD_ID_BRIGHTNESS)
		{
			if (val <= 0)
			{
				BRIGHTNESS = 0;
				turnOff();
			}
			else
			{
				BRIGHTNESS=map( val, 0, 100, 0, 255 );
				strip.setBrightness(BRIGHTNESS);
			}
			RGB_CHANGE=1;
			send(dimmerMsg.set(val));
		}
	}
	else if (Type == V_VAR1)
	{
		int val = atoi(message.data);
		if (message.sensor==CHILD_ID_MODE)
		{
			MODE = val;
			MODE_CHANGE=1;
			send(modeMsg.set(MODE));
		}
		if (message.sensor==CHILD_ID_LUM_MIN)
		{
			LUM_MIN=val;
			send(msgLumMin.set(LUM_MIN));
		}
		if (message.sensor==CHILD_ID_LUM_MAX)
		{
			LUM_MAX=val;
			send(msgLumMax.set(LUM_MAX));
		}
		if (message.sensor==CHILD_ID_TEMPO)
		{
			TEMPO=val;
			send(msgTempo.set(TEMPO));
		}
	}
}

	void turnOff()
	{
		for(int i=0;i<NUMBER_OF_LEDS;i++)
		{
			strip.clear();
		}
		strip.show();
	}

	void set_rgb(int r, int g, int b, int l) {
		strip.setBrightness(l);
		for(int i=0;i<NUMBER_OF_LEDS;i++)
		{
			strip.setPixelColor(i, strip.Color(r,g,b));
		}
		strip.show();
	}


	///////////////////////////////////
	/* Fonction d'effet du ruban Led */
	///////////////////////////////////
	// Slightly different, this makes the rainbow equally distributed throughout
	void rainbowCycle(uint8_t wait) {
		uint16_t i, j;

		for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
			for(i=0; i< strip.numPixels(); i++) {
				strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
			}
			strip.show();
			delay(wait);
		}
	}

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	uint32_t Wheel(byte WheelPos) {
		WheelPos = 255 - WheelPos;
		if(WheelPos < 85) {
			return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
		}
		if(WheelPos < 170) {
			WheelPos -= 85;
			return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
		}
		WheelPos -= 170;
		return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
