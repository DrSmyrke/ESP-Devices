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
	//fd.write( '"' );
	while( value[i] != '\0' ) fd.write( value[i++] );
	//fd.write( '"' );
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

	//Serial.print(param);Serial.print(":");Serial.println(value);
	
	if( strcmp(param,"APWifiSSID") == 0 ){
		i = 0;
		while( value[i] != '\0' ){
			conf.APWifiSSID[i] = value[i];
			i++;
		}
		conf.APWifiSSID[i] = '\0';
		return;
	}
	if( strcmp(param,"APWifiPass") == 0 ){
		i = 0;
		while( value[i] != '\0' ){
			conf.APWifiPass[i] = value[i];
			i++;
		}
		conf.APWifiPass[i] = '\0';
		return;
	}
	if( strcmp(param,"clientWifiSSID") == 0 ){
		i = 0;
		while( value[i] != '\0' ){
			conf.clientWifiSSID[i] = value[i];
			i++;
		}
		conf.clientWifiSSID[i] = '\0';
		return;
	}
	if( strcmp(param,"clientWifiPass") == 0 ){
		i = 0;
		while( value[i] != '\0' ){
			conf.clientWifiPass[i] = value[i];
			i++;
		}
		conf.clientWifiPass[i] = '\0';
		return;
	}
	if( strcmp(param,"devName") == 0 ){
		i = 0;
		while( value[i] != '\0' ){
			conf.devName[i] = value[i];
			i++;
		}
		conf.devName[i] = '\0';
		return;
	}
	if( strcmp(param,"devLogin") == 0 ){
		i = 0;
		while( value[i] != '\0' ){
			conf.devLogin[i] = value[i];
			i++;
		}
		conf.devLogin[i] = '\0';
		return;
	}
	if( strcmp(param,"devPass") == 0 ){
		i = 0;
		while( value[i] != '\0' ){
			conf.devPass[i] = value[i];
			i++;
		}
		conf.devPass[i] = '\0';
		return;
	}
}

void iniConfPars( const char* filename )
{
	if( !conf.spiffsActive ) return;
	if( !SPIFFS.exists( filename ) ) return;
	
	//Serial.println( "SPIFFS.exists" );
	
	fd = SPIFFS.open( filename, "r" );

	if( !fd ) return;

	//Serial.println( "fd.open" );

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
			if( sym == '=' ){
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
