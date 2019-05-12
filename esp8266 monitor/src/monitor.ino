#include <ESP8266WiFi.h>
#include "functions.h"
#include "timer.h"

uint8_t clientMAC[6]	= { 0x38,0x2C,0x4A,0x6C,0x4F,0x68 };
uint8_t apMAC[6]		= { 0x18,0x65,0x99,0x4A,0xDC,0xE4 };

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
	//buildBeacon(clientMAC,"SecretNET", 9, 1);for(int h=0;h<4;h++) if(wifiSend()) pktCounter++;

	//buildAck();for(int h=0;h<4;h++) if(wifiSend()) pktCounter++;

	//buildRTS(apMAC,clientMAC);for(int h=0;h<4;h++){if(wifiSend()) pktCounter++;delay(5);}

	//for(int h=0;h<4;h++){
	//	buildDeauth(apMAC,clientMAC,0xC0);if(wifiSend()) pktCounter++;
	//	buildDeauth(apMAC,clientMAC,0xA0);if(wifiSend()) pktCounter++;
	//	delay(3);
	//}

	tickCounter++;
}

