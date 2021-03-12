bool http_auth_check(void)
{
	if( !webServer.authenticate(conf.devLogin, conf.devPass) ){
		webServer.requestAuthentication(DIGEST_AUTH, "DrSmyrke access", "authFail");
		return false;
	}
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

void httpGetConfig(void)
{
	if( !http_auth_check() ) return;
	http_send_file( CONFFILE, "plain/text" );
}

void httpHandleIndex(void)
{
	//Serial.println("/");
	//if( !http_auth_check() ) return;

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
	//if( !http_auth_check() ) return;

	webServer.setContentLength( CONTENT_LENGTH_UNKNOWN );
	webServer.send(404, "text/html", "");
	webServer.sendContent( getWebPageTop("404 Not found") );
	webServer.sendContent( getWebPageStyle() );
	webServer.sendContent( getWebPageJS() );
	webServer.sendContent( "<h1>404 Error ( Path not found )</h1>" );
	webServer.sendContent( getWebPageBottom() );
}


void handleUpdate(void)
{
	if( ( (conf.wIndx - conf.rIndx) & UART_BUFFER_MASK + 1) >= UART_BUFFER_SIZE){
		conf.wIndx = 0;
		conf.rIndx = 0;
	}

	String ascii;
	String hex;

	while( conf.rIndx != conf.wIndx ){
		char sym = conf.rxBuff[ conf.rIndx++ & UART_BUFFER_MASK];
		ascii += sym;
		hex += String( sym, HEX );
	}

	String retStr = "{ \"a\": \"" + ascii + "\", \"h\": \"" + hex + "\"}";

	webServer.send(200, "text/html", retStr );
}

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
		if( webServer.argName(i) == "remove" ){
			SPIFFS.remove(webServer.arg(i));
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
		
		if( webServer.argName(i) == "execute" ){
			conf.lineNum = 0;
			conf.lastPosition = 0;
			conf.execute = true;
			conf.execFile = webServer.arg(i);
			webServer.send(200, "text/html", "" );
			start_execution();
			return;
		}
		if( webServer.argName(i) == "stopexecute" ){
			conf.lineNum = 0;
			conf.lastPosition = 0;
			conf.execute = false;
			conf.execFile = "";
			webServer.send(200, "text/html", "" );
			return;
		}

		if( webServer.argName(i) == "portOn" ){
			uint8_t num = webServer.arg(i).toInt();
			//portOn( num );
			//ret = CheckBit( conf.relayState, num );
		}
		if( webServer.argName(i) == "portOff" ){
			uint8_t num = webServer.arg(i).toInt();
			//portOff( num );
			//ret = CheckBit( conf.relayState, num );
		}

		String value = webServer.arg(i);
		
		//Serial.print("Param: ");Serial.print(webServer.argName(i));
		//Serial.print("Value: ");Serial.println(webServer.arg(i));

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
	}

	if( saveSettings ) saveConfToMem();
	if( error ) return;

	webServer.send(200, "text/html", ret );
}

void handleSend(void)
{
	for (uint8_t i = 0; i < webServer.args(); i++) {
		if( webServer.argName(i) == "data" ){
			uint8_t ch = 0;
			uint8_t iStart = 0;
			uint8_t hexF = 0;
			uint8_t buff;

			if( webServer.arg(i)[0] == '0' && webServer.arg(i)[1] == 'x' ){
				iStart = 2;
				hexF = 1;
			}

			for( ch = iStart; ch < webServer.arg(i).length(); ch++ ){
				char sym = webServer.arg(i)[ch];

				if( hexF ){
					buff = symToHex( sym );
					buff *= 16;
					buff += symToHex( webServer.arg(i)[++ch] );
					sym = (char)buff;
				}

				Serial.write( sym );
			}
			break;
		}
	}
	webServer.send(200, "text/html", "" );
}

void handleUpload(void)
{
	HTTPUpload& upload = webServer.upload();
	String filename;
	
	switch( upload.status ){
		case UPLOAD_FILE_START:
			filename = upload.filename;
			if(!filename.startsWith("/")) filename = "/"+filename;

			if( filename.endsWith(".gcode") || filename.endsWith(".nc") ){
				conf.uploadType = upload_type_file;
				fsUploadFile = SPIFFS.open(filename, "w");
			}
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
					ESP.restart();
				}else{
					webServer.send(500, "text/html", "Upload error" );
				}
			}
		break;
	}
}
