#include "FS.h"

File fd;

void iniConfSaveInit( const char* filename )
{
	if( !conf.spiffsActive ) return;
	
	fd = SPIFFS.open( filename, "w" );
}

void iniConfSaveDeInit(void)
{
	fd.close();
}

void iniConfSet( const char* param, const char* value )
{
	uint8_t i = 0;
	
	while( param[i] != '\0' ) fd.write( param[i++] );
	i = 0;
	fd.write( '=' );
	fd.write( '"' );
	while( value[i] != '\0' ) fd.write( value[i++] );
	fd.write( '"' );
	fd.write( '\n' );
}

void iniConfSet( const char* param, const uint32_t value )
{
	uint8_t i = 0;
	String string = String( value );
	
	while( param[i] != '\0' ) fd.write( param[i++] );
	i = 0;
	fd.write( '=' );
	while( string[i] != '\0' ) fd.write( string[i++] );
	fd.write( '\n' );
}

void iniConfSetComment( const char* comment )
{
	uint8_t i = 0;
	
	fd.write( '#' );
	while( comment[i] != '\0' ) fd.write( comment[i++] );
	fd.write( '\n' );
}

void iniConfParsVals( const char* param, const char* value )
{
	uint8_t i = 0;

	Serial.print(param);Serial.print(":");Serial.println(value);
	
	if( param == "APWifiSSID" ){
		i = 0; while( value[i] != '\0' ) conf.APWifiSSID[i] = value[i];
		return;
	}
	if( param == "APWifiPass" ){
		i = 0; while( value[i] != '\0' ) conf.APWifiPass[i] = value[i];
		return;
	}
	if( param == "clientWifiSSID" ){
		i = 0; while( value[i] != '\0' ) conf.clientWifiSSID[i] = value[i];
		return;
	}
	if( param == "clientWifiPass" ){
		i = 0; while( value[i] != '\0' ) conf.clientWifiPass[i] = value[i];
		return;
	}
	if( param == "sendUrl" ){
		i = 0; while( value[i] != '\0' ) conf.sendUrl[i] = value[i];
		return;
	}
	if( param == "devName" ){
		i = 0; while( value[i] != '\0' ) conf.devName[i] = value[i];
		return;
	}
	if( param == "devLogin" ){
		i = 0; while( value[i] != '\0' ) conf.devLogin[i] = value[i];
		return;
	}
	if( param == "devPass" ){
		i = 0; while( value[i] != '\0' ) conf.devPass[i] = value[i];
		return;
	}
/*

	iniConfSetComment("Ports Names");
	iniConfSet("port1name", conf.port1name);
	iniConfSet("port2name", conf.port2name);
	iniConfSet("port3name", conf.port3name);
	iniConfSet("port4name", conf.port4name);
	iniConfSet("port5name", conf.port5name);
	iniConfSet("port6name", conf.port6name);
	iniConfSet("port7name", conf.port7name);
	iniConfSet("port8name", conf.port8name);
	iniConfSetComment("IP Settings");
	iniConfSet("APip1", conf.APip[0]);
	iniConfSet("APip2", conf.APip[1]);
	iniConfSet("APip3", conf.APip[2]);
	iniConfSet("APip4", conf.APip[3]);
	iniConfSet("APmask1", conf.APmask[0]);
	iniConfSet("APmask2", conf.APmask[1]);
	iniConfSet("APmask3", conf.APmask[2]);
	iniConfSet("APmask4", conf.APmask[3]);
	iniConfSet("APgw1", conf.APgw[0]);
	iniConfSet("APgw2", conf.APgw[1]);
	iniConfSet("APgw3", conf.APgw[2]);
	iniConfSet("APgw4", conf.APgw[3]);
*/
}

void iniConfPars( const char* filename )
{
	if( !conf.spiffsActive ) return;
	if( !SPIFFS.exists( filename ) ) return;

	Serial.println( "SPIFFS.exists" );
	
	fd = SPIFFS.open( filename, "r" );

	if( !fd ) return;

	Serial.println( "fd.open" );

	char param[32];
	char value[32];
	uint8_t i = 0;
	bool paramF = true;
	bool commentF = false;
	
	while( fd.available() ){
		char sym;
		fd.readBytes( &sym, 1 );
		
		if( sym == '#' && i == 0 ) commentF = true;

		if( sym == '\n' ){
			paramF = true;
			value[i] = '\0';
			i = 0;
			if( !commentF ) iniConfParsVals( param, value );
			if( commentF ) commentF = false;
			continue;
		}

		if( commentF ) continue;

		if( paramF ){
			if( sym == ' ' ){
				param[i] = '\0';
				paramF = false;
				i = 0;
				continue;
			}
			param[i++] = sym;
		}else{
			if( sym == ' ' || sym == '=' ) continue;
			value[i++] = sym;
		}
	}

	fd.close();
}
