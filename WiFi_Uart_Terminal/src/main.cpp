#include "pin_define.h"
#include "config.h"
#include "iniConf.h"
#include "data.h"
#include "functions.h"
#include "http.h"
#include "timer.h"

void setup()
{
	Serial.begin( 9600 );
	delay(3000);
	
	portInit();
	if (SPIFFS.begin()) conf.spiffsActive = true;
	delay(300);
	setDefaultConfig();
	delay(300);
	iniConfPars( CONFFILE );
	delay(100);
	wifi_init();
	
	webServer.on("/", httpHandleIndex);
	webServer.on("/config", httpHandleSettings);
	webServer.on("/admin", httpHandleAdminKa);
	webServer.on("/set", handleSet);
	webServer.on("/update", handleUpdate);
	webServer.on("/send", HTTP_POST, handleSend);
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

	conf.responseStr.reserve( 128 );
	conf.response.reserve( 128 );
	conf.execFile.reserve( 64 );
	
	
	delay(100);
	timer_init();
	
}

void loop()
{
	webServer.handleClient();
	
	while( Serial.available() ) {
		char sym = (char)Serial.read();
		conf.rxBuff[ conf.wIndx++ & UART_BUFFER_MASK] = sym;
    }
}
