#include <ESP8266WiFi.h>
#include "functions.h"
#include "timer.h"

uint8_t clientMAC[6]	= { 0x28,0xED,0x6A,0x54,0x10,0x69 };
uint8_t apMAC[6]		= { 0xCC,0x2D,0xE0,0x39,0x26,0x73 };

void setup()
{
	Serial.begin( 115200 );
	delay(100);
	Serial.println();

	timer_init();

	wifi_set_opmode(STATION_MODE);
	WiFi.disconnect();
	wifi_set_channel( 9 );
	wifi_promiscuous_enable(0);
	wifi_set_promiscuous_rx_cb(promisc_cb);
	wifi_promiscuous_enable(true);
	Serial.println("Starting... OK");
	//delay(25000);
	//ESP.deepSleep(5000000, WAKE_RF_DEFAULT);
}

void loop()
{
	buildBeacon(clientMAC,"SecretNET", 9, 1);
	for(int h=0;h<4;h++) if(wifiSend()) pktCounter++;
	tickCounter++;
}

