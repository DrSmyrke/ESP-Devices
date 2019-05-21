bool http_auth_check(void)
{
	//if( !webServer.authenticate(conf.devLogin, conf.devPass) ){
	//	webServer.requestAuthentication(DIGEST_AUTH, "DrSmyrke access", "authFail");
	//	return false;
	//}
	return true;
}

void http_send_file( char* fileName, char* mimeType )
{
	if(SPIFFS.exists(fileName)){
		File f = SPIFFS.open(fileName, "r");
		webServer.setContentLength(f.size());
		webServer.send(200, mimeType, "");
		webServer.client().write(f);
		f.close();
	}else{
		webServer.send(404, "text/html", "File not found");
	}
}

void http_send_data( char* data )
{
/*
	StaticJsonBuffer<256> jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject( data );
	if( !root.success() ) return;
	
	HTTPclient.begin(conf.sendUrl);
	HTTPclient.addHeader("Content-Type", "application/x-www-form-urlencoded");
	//HTTPclient.addHeader("Content-Type", "multipart/form-data");
	//HTTPclient.addHeader("Content-Type", "application/json");
	//HTTPclient.addHeader("Content-Type", "text/plain");
	String output;
	root.printTo(output);
	int httpCode = HTTPclient.POST("json="+output);
	HTTPclient.end();
	
	//Serial.print("JSON send code:");
	//Serial.println( httpCode );
*/
}

void httpGetConfig(void)
{
	if( !http_auth_check() ) return;
	http_send_file( CONFFILE, "plain/text" );
}

void httpHandleIndex(void)
{
	//Serial.println("/");
	if( !http_auth_check() ) return;

	webServer.setContentLength( CONTENT_LENGTH_UNKNOWN );
	webServer.send(200, "text/html", "");
	webServer.sendContent( getWebPageTop("Home") );
	webServer.sendContent( getWebPageStyle() );
	webServer.sendContent( getWebPageJS() );
	webServer.sendContent( getWebHomePage() );
	webServer.sendContent( getWebPageBottom() );
	//Serial.println("/ ok");
}

void httpHandleSettings(void)
{
	//Serial.println("/config");
	if( !http_auth_check() ) return;

	webServer.setContentLength( CONTENT_LENGTH_UNKNOWN );
	webServer.send(200, "text/html", "");
	webServer.sendContent( getWebPageTop("Settings") );
	webServer.sendContent( getWebPageStyle() );
	webServer.sendContent( getWebPageJS() );
	webServer.sendContent( getWebSettingsPage() );
	webServer.sendContent( getWebPageBottom() );
	//Serial.println("/config ok");
}

void httpHandleAdminKa(void)
{
	if( !http_auth_check() ) return;

	webServer.setContentLength( CONTENT_LENGTH_UNKNOWN );
	webServer.send(200, "text/html", "");
	webServer.sendContent( getWebPageTop("AdminKa") );
	webServer.sendContent( getWebPageStyle() );
	webServer.sendContent( getWebPageJS() );
	webServer.sendContent( getWebAdminPage() );
	webServer.sendContent( getWebPageBottom() );
}

void httpHandleNotFound(void)
{
	if( !http_auth_check() ) return;

	webServer.setContentLength( CONTENT_LENGTH_UNKNOWN );
	webServer.send(404, "text/html", "");
	webServer.sendContent( getWebPageTop("404 Not found") );
	webServer.sendContent( getWebPageStyle() );
	webServer.sendContent( getWebPageJS() );
	webServer.sendContent( "<h1>404 Error ( Path not found )</h1>" );
	webServer.sendContent( getWebPageBottom() );
}

/*
void handleGet(void)
{
	if( !http_auth_check() ) return;

	String retStr = "get";

	for (uint8_t i = 0; i < webServer.args(); i++) {
		retStr += "	" + webServer.argName(i) + "=";

		if( webServer.argName(i) == "SwitchState" )			retStr += conf.relayState;
		if( webServer.argName(i) == "SwitchMode" )			retStr += conf.relayMode;
		if( webServer.argName(i) == "SwitchCount" )			retStr += conf.relayNum;

		if( webServer.argName(i) == "DevName" )				retStr += ;
		if( webServer.argName(i) == "DevMAC" )				retStr += ;
		if( webServer.argName(i) == "DevAccessPointIP" )	retStr += ;
		if( webServer.argName(i) == "DevClientIP" )			retStr += ;
		if( webServer.argName(i) == "DevClientState" )		retStr += ;
		if( webServer.argName(i) == "DevVersion" )			retStr += ;
		if( webServer.argName(i) == "DevAccessPointSSID" )	retStr += conf.APWifiSSID;
		if( webServer.argName(i) == "DevAccessPointPass" )	retStr += conf.APWifiPass;
		if( webServer.argName(i) == "DevClientSSID" )		retStr += conf.clientWifiSSID;
		if( webServer.argName(i) == "DevClientPass" )		retStr += conf.clientWifiPass;
		if( webServer.argName(i) == "DevSendUrl" )			retStr += conf.sendUrl;
		if( webServer.argName(i) == "DevLogin" )			retStr += conf.devLogin;
		if( webServer.argName(i) == "DevPass" )				retStr += conf.devPass;

	}

	webServer.send(200, "text/html", retStr );
}
*/
void handleSet(void)
{
	if( !http_auth_check() ) return;

	bool error				= false;
	bool saveSettings		= false;
	String ret				= "";

	for (uint8_t i = 0; i < webServer.args(); i++) {
		if( webServer.argName(i) == "reload" ){
			ESP.restart();
			error = true;
			break;
		}
		if( webServer.argName(i) == "rescan" ){
			portReScan();
			break;
		}
		if( webServer.argName(i) == "reconf" ){
			SPIFFS.remove(CONFFILE);
			break;
		}
		if( webServer.argName(i) == "clear" ){
			SPIFFS.format();
			ESP.restart();
			error = true;
			break;
		}

		if( webServer.argName(i) == "portOn" ){
			uint8_t num = webServer.arg(i).toInt();
			portOn( num );
			ret = CheckBit( conf.relayState, num );
		}
		if( webServer.argName(i) == "portOff" ){
			uint8_t num = webServer.arg(i).toInt();
			portOff( num );
			ret = CheckBit( conf.relayState, num );
		}
		if( webServer.argName(i) == "port0mode" ){
			if( webServer.arg(i).toInt() ){
				setPlus(conf.relayMode,0);
			}else{
				setZero(conf.relayMode,0);
			}
			portSetMode( conf.relayMode );
		}
		if( webServer.argName(i) == "port1mode" ){
			if( webServer.arg(i).toInt() ){
				setPlus(conf.relayMode,1);
			}else{
				setZero(conf.relayMode,1);
			}
			portSetMode( conf.relayMode );
		}
		if( webServer.argName(i) == "port2mode" ){
			if( webServer.arg(i).toInt() ){
				setPlus(conf.relayMode,2);
			}else{
				setZero(conf.relayMode,2);
			}
			portSetMode( conf.relayMode );
		}
		if( webServer.argName(i) == "port3mode" ){
			if( webServer.arg(i).toInt() ){
				setPlus(conf.relayMode,3);
			}else{
				setZero(conf.relayMode,3);
			}
			portSetMode( conf.relayMode );
		}
		if( webServer.argName(i) == "port4mode" ){
			if( webServer.arg(i).toInt() ){
				setPlus(conf.relayMode,4);
			}else{
				setZero(conf.relayMode,4);
			}
			portSetMode( conf.relayMode );
		}
		if( webServer.argName(i) == "port5mode" ){
			if( webServer.arg(i).toInt() ){
				setPlus(conf.relayMode,5);
			}else{
				setZero(conf.relayMode,5);
			}
			portSetMode( conf.relayMode );
		}
		if( webServer.argName(i) == "port6mode" ){
			if( webServer.arg(i).toInt() ){
				setPlus(conf.relayMode,6);
			}else{
				setZero(conf.relayMode,6);
			}
			portSetMode( conf.relayMode );
		}
		if( webServer.argName(i) == "port7mode" ){
			if( webServer.arg(i).toInt() ){
				setPlus(conf.relayMode,7);
			}else{
				setZero(conf.relayMode,7);
			}
			portSetMode( conf.relayMode );
		}

		String value = webServer.arg(i);

		if( webServer.argName(i) == "APSSID" ){
			value.toCharArray(conf.APWifiSSID, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "APKey" ){
			value.toCharArray(conf.APWifiPass, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "CAPSSID" ){
			value.toCharArray(conf.clientWifiSSID, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "CAPKey" ){
			value.toCharArray(conf.clientWifiPass, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "WLogin" ){
			value.toCharArray(conf.devLogin, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "WPass" ){
			value.toCharArray(conf.devPass, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "DevName" ){
			value.toCharArray(conf.devName, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "SendUrl" ){
			value.toCharArray(conf.sendUrl, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "port0name" ){
			value.toCharArray(conf.port1name, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "port1name" ){
			value.toCharArray(conf.port2name, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "port2name" ){
			value.toCharArray(conf.port3name, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "port3name" ){
			value.toCharArray(conf.port4name, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "port4name" ){
			value.toCharArray(conf.port5name, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "port5name" ){
			value.toCharArray(conf.port6name, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "port6name" ){
			value.toCharArray(conf.port7name, value.length()+1);
			saveSettings = true;
		}
		if( webServer.argName(i) == "port7name" ){
			value.toCharArray(conf.port8name, value.length()+1);
			saveSettings = true;
		}
	}

	if( saveSettings ) saveConfToMem();
	if( error ) return;

	webServer.send(200, "text/html", ret );
}

void handleUpload(void)
{
	HTTPUpload& upload = webServer.upload();
	String filename;
	
	switch( upload.status ){
		case UPLOAD_FILE_START:
			filename = upload.filename;
			if(!filename.startsWith("/")) filename = "/"+filename;

			if( filename == CONFFILE ){
				conf.uploadType = upload_type_file;
				fsUploadFile = SPIFFS.open(CONFFILE, "w");
			}
			if( upload.filename == UPDFILE ){
				WiFiUDP::stopAll();
				uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
				conf.uploadType = upload_type_firmware;
				Update.begin(maxSketchSpace);
			}
		break;
		case UPLOAD_FILE_WRITE:
			if( conf.uploadType == upload_type_file && fsUploadFile ) fsUploadFile.write(upload.buf,upload.currentSize);
			//if( fsUploadFile) fsUploadFile.write(upload.buf,upload.currentSize);
			//if(Update.write(upload.buf, upload.currentSize) != upload.currentSize)
			if( conf.uploadType == upload_type_firmware) Update.write(upload.buf, upload.currentSize);
		break;
		case UPLOAD_FILE_END:
			if( conf.uploadType == upload_type_file && fsUploadFile ){
				fsUploadFile.close();
				delay(500);
				webServer.sendHeader("Location","/config");
				webServer.send(303);
			}
			if( conf.uploadType == upload_type_firmware){
				if( Update.end(true) ){
					//webServer.sendHeader("Location","/config");
					//webServer.send(303);
					//delay(500);
					ESP.restart();
				}else{
					webServer.send(500, "text/html", "Upload error" );
				}
			}
		break;
	}
}
