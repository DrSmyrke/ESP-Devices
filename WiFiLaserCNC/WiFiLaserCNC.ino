#include "pin_define.h"
#include "config.h"
#include "iniConf.h"
#include "data.h"
#include "http.h"
#include "functions.h"
#include "timer.h"

void setup()
{
	Serial.begin(115200);
	delay(3000);
	
	portInit();
	if (SPIFFS.begin()) conf.spiffsActive = true;
	delay(300);
	Serial.print("spiffsActive :");Serial.println(conf.spiffsActive);
	Serial.println("Configure system...");
	setDefaultConfig();
	delay(300);
	Serial.println("Reconfigure system...");
	iniConfPars( CONFFILE );
	delay(100);
	wifi_init();
	
	webServer.on("/", httpHandleIndex);
	webServer.on("/config", httpHandleSettings);
	webServer.on("/admin", httpHandleAdminKa);
	webServer.on("/set", handleSet);
	//webServer.on("/get", HTTP_POST, handleGet);
	webServer.on("/upload", HTTP_POST, [](){}, handleUpload);
	webServer.on(CONFFILE, httpGetConfig);
	webServer.onNotFound(httpHandleNotFound);
	webServer.begin();

	/*
	
	EEPROM.begin(4096);
	conf.relayNum		= EEPROM.read( EEPROM_ADDRESS_NUM );
	conf.relayState		= EEPROM.read( EEPROM_ADDRESS_STATE );
	conf.relayMode		= EEPROM.read( EEPROM_ADDRESS_MODE );
	
	
	//Serial.print("SPIFFS active:");Serial.println(conf.spiffsActive);

	
  
	*/

	//SPIFFS.format();

	
	
	delay(100);
	timer_init();
	Serial.println("Init complete");
}

void loop()
{
	webServer.handleClient();
}
