//const char webPageUpload[] = "<form action=\"/upload\" method=\"POST\" enctype=\"multipart/form-data\"><center class=\"valfiol\">First upload</center><br>*.html <input type=\"file\" name=\"file\"> <input type=\"submit\" value=\"Upload\"></form>\n";

String getWebPageTop( const char *pageTitle )
{
	return "<!DOCTYPE html> <html lang=\"en\"> <head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <meta charset=\"utf-8\"/> <META http-equiv=\"Pragma\" content=\"no-cache\"> <title>-= WiFi UART Terminal =-</title> </head> <body> <div class=\"content\"> <div class=\"logo\"> <div class=\"logoImg\"></div> WiFi UART Terminal </div> <div class=\"menu\"> <a href=\"/\" class=\"button\"><div class=\"homeImg\"></div>Terminal</a> <a href=\"/config\" class=\"button\"><div class=\"settingsImg\"></div>Settings</a> <a href=\"/admin\" class=\"button\"><div class=\"adminImg\"></div>Admin</a> </div> <hr>";
}

String getWebPageBottom(void)
{
	return "<hr> <div class=\"info\"> Created by <a href=\"http://drsmyrke.ru\" target=\"_blank\">Dr.Smyrke</a>&#160;&#160;&#160;<i> <br><i>ООО \"Шайтан технолоджи\"</i><br> <i>при поддержке ЗАО \"Мутные схемы\" и ОАО \"Любовь в займы\"</i> </div>  </div> </body> </html>";
}

String getWebPageJS(void)
{
	return "<script type=\"text/javascript\"> var request = makeHttpObject(); function makeHttpObject() { try {return new XMLHttpRequest();} catch (error) {} try {return new ActiveXObject(\"Msxml2.XMLHTTP\");} catch (error) {} try {return new ActiveXObject(\"Microsoft.XMLHTTP\");} catch (error) {} throw new Error(\"Could not create HTTP request object.\"); } </script>";
}

String getWebHomePage(void)
{
	return "<fieldset class=\"block\"><legend>Terminal</legend> <table> <tr><td id=\"terminalASCII\"></td><td id=\"terminalHEX\"></td></tr> <tr><td colspan=\"2\"> <div style=\"display: flex;justify-content: space-between;\"> <span>>:</span> <input type=\"text\" onInput=\"app.tData = this.value;\" placeholder=\"0x, ASCII\"> <span> | </span> <input type=\"button\" value=\"SEND\" onClick=\"sendData();\"> </div> </td></tr> </table> </fieldset> <script type=\"text/javascript\"> var app = { \"response\": false, \"debug\": false, \"tData\": \"\", }; window.onload = function(){ update(); }; window.addEventListener(\"unload\", function() { }); function sendData() { var request2 = makeHttpObject(); var str = \"data=\" + app.tData; request2.open( 'POST', \"/send\", true ); request2.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded'); request2.send( str ); } function update() { if( !app.response ){ request.open( 'GET', \"/update\", true ); request.send( null ); app.response = true; } setTimeout( \"update()\", 100 ); } request.onreadystatechange = function() { if( request.readyState == 4 && request.status == 200 ){ if( app.debug ){ console.log( request.responseText ); console.log( JSON.parse( request.responseText ) ); } var asciiBox = document.getElementById( \"terminalASCII\" ); var hexBox = document.getElementById( \"terminalHEX\" ); var obj = JSON.parse( request.responseText ); for( var i = 0; i < obj.a.length; i++ ){ var sym = obj.a[i]; if( sym == \"\\r\" ) coontinue; if( sym == \"\\n\" ){ asciiBox.innerHTML += \"<br>\"; continue; } asciiBox.innerHTML += sym; } for( var i = 0; i < obj.h.length; i++ ){ hexBox.innerHTML += obj.h[i]; if( i % 2 ) hexBox.innerHTML += \" \"; } app.response = false; } }; </script> <style> #terminalASCII, #terminalHEX{ vertical-align: top; } #terminalHEX{ width: 30%; } .block{ width: 95%; padding: 5px; margin: 10px 0px; } </style>";
}

String getWebSettingsPage(void)
{
	String str;

	String aip = String( conf.APip[0] ) + "." + String( conf.APip[1] ) + "." + String( conf.APip[2] ) + "." + String( conf.APip[3] );
	String agw = String( conf.APgw[0] ) + "." + String( conf.APgw[1] ) + "." + String( conf.APgw[2] ) + "." + String( conf.APgw[3] );
	String amsk = String( conf.APmask[0] ) + "." + String( conf.APmask[1] ) + "." + String( conf.APmask[2] ) + "." + String( conf.APmask[3] );
	
	return "<div id=\"settingsBlock\"> <fieldset class=\"block\"><legend>Access Point</legend> <table> <tr><td>SSID</td><td><input type=\"text\" id=\"APSSID\" maxlength=\"16\" placeholder=\"Wifi_Power_Shitch\" value=\"" + String( conf.APWifiSSID ) + "\"></td></tr> <tr><td>Key</td><td><input type=\"text\" id=\"APKey\" maxlength=\"16\" value=\"' + String( conf.APWifiPass ) + '\"></td></tr> <tr><td>IP</td><td><input type=\"text\" id=\"APip\" maxlength=\"16\" placeholder=\"192.168.4.1\" pattern=\"^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$\" value=\"' + aip + '\"></td></tr> <tr><td>Gateway</td><td><input type=\"text\" id=\"APgw\" maxlength=\"16\" placeholder=\"0.0.0.0\" pattern=\"^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$\" value=\"' + agw + '\"></td></tr> <tr><td>Mask</td><td><input type=\"text\" id=\"APmask\" maxlength=\"16\" placeholder=\"255.255.255.0\" pattern=\"^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$\" value=\"' + amsk + '\"></td></tr> </table> </fieldset> <fieldset class=\"block\"><legend>Remote Access Point</legend> <table> <tr><td>SSID</td><td><input type=\"text\" id=\"CAPSSID\" maxlength=\"16\" placeholder=\"SECRETNET\" value=\"' + String( conf.clientWifiSSID ) + '\"></td></tr> <tr><td>Key</td><td><input type=\"text\" id=\"CAPKey\" maxlength=\"16\" value=\"' + String( conf.clientWifiPass ) + '\"></td></tr> </table> </fieldset> <fieldset class=\"block\"><legend>Security Access</legend> <table> <tr><td>Login</td><td><input type=\"text\" id=\"WLogin\" maxlength=\"16\" placeholder=\"admin\" value=\"' + String( conf.devLogin ) + '\"></td></tr> <tr><td>Password</td><td><input type=\"text\" id=\"WPass\" maxlength=\"16\" placeholder=\"admin\" value=\"' + String( conf.devPass ) + '\"></td></tr> </table> </fieldset> <fieldset class=\"block\"><legend>Other</legend> <table> <tr><td>Device name</td><td><input type=\"text\" id=\"DevName\" maxlength=\"16\" placeholder=\"WiFiPowerSwitch\" value=\"' + String( conf.devName ) + '\"></td></tr> <tr><td colspan=\"3\" align=\"center\"> <input type=\"button\" value=\"Save settings\" onClick=\"saveSettings();\"> | <a href=\"/settings.cfg\" class=\"a\" target=\"_blank\">Download settings</a> </td></tr> </table> <hr> <form action=\"/upload\" method=\"POST\" enctype=\"multipart/form-data\"><center class=\"valfiol\">Upload assets</center><br><input type=\"file\" name=\"file\"> <input type=\"submit\" value=\"Upload\"></form> </fieldset> <fieldset class=\"block\"><legend>Ports</legend> ' + str + ' </fieldset> </div> <div class=\"action\" id=\"prgressBox\"> <span>Save settings...</span> </div> <style> .switcher { width: 100px; height:24px; background-color:silver; position:relative; overflow:hidden; white-space:nowrap; display: inline-block; border-radius:100px; padding:2px; box-shadow: inset 0px 3px 10px 0px rgba(0, 0, 0, 0.4), 0px 3px 10px 0px rgba(255, 255, 255, 0.4); font-size:14px; -webkit-transition: all 0.3s ease-in-out; text-transform: uppercase; } .rail { position: absolute; top: 2px; left: 7px; -webkit-transition: all 0.3s ease-in-out; cursor: pointer; } .state1 { display: inline-block; position: relative; top: -7px; left: 1px; padding-left: 5px; } .state2 { display: inline-block; position: relative; top: -7px; left: -2px; } .slider { display: inline-block; height:24px; width: 24px; border-radius: 100px; background-color:#ccc; box-shadow: 0px 3px 4px 0px rgba(0, 0, 0, 0.2), inset 0px 1px 1px 1px rgba(255, 255, 255, 0.7); } :checked + .switcher .rail{ left:-67px; } :not(:checked) + .switcher { background-color: yellowgreen; } :checked + .switcher { background-color: orange; } </style> <script type=\"text/javascript\"> function saveSettings() { document.getElementById(\"prgressBox\").style.display = \"block\"; var array = document.getElementById(\"settingsBlock\").getElementsByTagName(\"INPUT\"); var updateParams = new Array(); var str = \"\"; for( var i = 0; i < array.length; i++ ){ if( array[i].id == \"\" ) continue; if( i > 0 ) str += \"&\"; if( array[i].type == \"checkbox\" ){ str += array[i].id + \"=\"; str += ( array[i].checked ) ? \"1\" : \"0\" ; continue; } str += array[i].id + \"=\" + array[i].value; } request.open( 'POST', \"/set\", true ); request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded'); request.send(str); } request.onreadystatechange = function() { if( request.readyState == 4 && request.status == 200 ) { document.getElementById(\"prgressBox\").style.display = \"none\"; } if( request.readyState == 1 && request.status == 0 ) { setTimeout(\"if(request.readyState == 1) document.location.reload();\",3000); } if( request.readyState == 4 && request.status == 0 ) document.location.reload(); }; </script>";
}

String getWebAdminPage(void)
{
	String conState = ( WiFiConnectionState() ) ? "<span class=\"valgreen\">Connected</span>" : "<span class=\"valorange\">Disconnected</span>" ;
	//				" +  + "
	return "<script type=\"text/javascript\"> request.onreadystatechange = function() { if( request.readyState == 4 && request.status == 200 ) { var box = document.getElementById(\"prgressBox\"); var title = document.getElementById(\"progressTitle\"); box.style.display = \"none\"; title.innerHTML = \"\"; lastLink = \"\"; } if( request.readyState == 1 && request.status == 0 ) { setTimeout(\"if(request.readyState == 1) reload();\",3000); } if( request.readyState == 4 && request.status == 0 ) reload(); }; function reload() { request.open( 'GET', \"/admin\", true ); request.send(null); } function actionB( type ) { if( !type ) return; var box = document.getElementById(\"prgressBox\"); var title = document.getElementById(\"progressTitle\"); box.style.display = \"block\"; switch( type ){ case \"reload\": title.innerHTML = \"Reloading...\"; request.open( 'GET', \"/set?reload=true\", true ); request.send(null); break; case \"rescan\": title.innerHTML = \"Reloading...\"; request.open( 'GET', \"/set?rescan=true\", true ); request.send(null); break; case \"reconf\": title.innerHTML = \"Reset configuration...\"; request.open( 'GET', \"/set?reconf=true\", true ); request.send(null); break; case \"clear\": title.innerHTML = \"Formating memory...\"; request.open( 'GET', \"/set?clear=true\", true ); request.send(null); break; default: title.innerHTML = \"Loading...\"; setTimeout(\"document.reload()\",2500); break; } } </script> <div class=\"action\" id=\"prgressBox\"> <span id=\"progressTitle\"></span> </div> <br> <fieldset class=\"block\"><legend>Info</legend> <table width=\"100%\"> <tr><td width=\"150px\">Device</td><td>" + String(conf.devName) + "</td></tr> <tr><td>MAC</td><td>" + WiFi.macAddress() + "</td></tr> <tr><td>Access Point IP</td><td>" + WiFi.softAPIP().toString() + "</td></tr> <tr><td>Client IP</td><td>" + WiFi.localIP().toString() + "</td></tr> <tr><td>Client Status</td><td>" + conState + "</td></tr> <tr><td>Firmware version</td><td>" + String(conf.version) + "</td></tr> </table> </fieldset> <fieldset class=\"block\"><legend>Actions</legend> <a href=\"javascript:void(0);\" class=\"a\" onClick=\"if( confirm('Are you sure?') ) actionB('reconf');\">Reset to factory</a> | <a href=\"javascript:void(0);\" class=\"a\" onClick=\"actionB('reload');\">Reload</a> | <a href=\"javascript:void(0);\" class=\"a\" onClick=\"if( confirm('Are you sure?') ) actionB('clear');\">Clear</a> </fieldset>";
}

String getWebPageStyle(void)
{
	return "<style> *, *:before, *:after { transition: .25s ease-in-out; } body{ transition: none; background: #E6E4E7; font:small Verdana,Sans-serif; -webkit-touch-callout: none; /* iOS Safari */ -webkit-user-select: none; /* Safari */ -khtml-user-select: none; /* Konqueror HTML */ -moz-user-select: none; /* Firefox */ -ms-user-select: none; /* Internet Explorer/Edge */ user-select: none; /* Non-prefixed version, currently supported by Chrome and Opera */ } hr{ border: 0px dashed; height: 1px; background-image: -webkit-linear-gradient(left, #fff, #000, #fff); background-image: -moz-linear-gradient(left, #fff, #000, #fff); background-image: -ms-linear-gradient(left, #fff, #000, #fff); background-image: -o-linear-gradient(left, #fff, #000, #fff); } a{ color: #459EBC; } .logo{ font-size:24pt; color:#383152; display: inline-block; vertical-align: center; } .info i{ color: gray; fint-style: italic; font-size:8pt; } .content{ width: 1170px; margin: auto; text-align: center; } .menu{ text-align: center; } .button{ margin: 3px 5px; border: 2px dashed #459EBC; border-radius: 15px; color: black; text-decoration: none; text-align: center; padding: 3px 15px; display: inline-block; } table{ border-collapse: collapse; } tr{ border-bottom: 1px solid silver; } tr:last-child{ border: none; } legend{ text-transform: uppercase; margin: 0px; color: #7D7C81; position: relative; top: -12px; left: -12px; } .block table{ width: 100%; } .block{ width: 300px; border: none; display: inline-block; background: white; padding: 10px; border-radius: 5px; vertical-align: top; margin: 10px; text-align: left; } input[type=\"text\"]{ border: none; padding: 3px; background: white; } input[type=\"button\"], input[type=\"submit\"]{ transition: none; } input[type=checkbox]{ display: none; } .valgreen{ text-shadow:0px 0px 5px #61FF62; color:#00FF03; } .valorange{ text-shadow:0px 0px 5px #FFB67B; color:#FF7200; } .action{ position: fixed; width: 100%; height: 100%; left: 0px; top: 0px; background: rgba(255,255,255, 0.8); display: none; font-size: 18pt; font-weight: bold; } @media only screen and (max-width: 685px){ .block { width: 95%; margin: 50px auto; display: block; } } @media only screen and (max-width: 1170px){ .content {width: 95%;} } /* @media only screen and (orientation:portrait){ .menu .a {display: block; width: 95%; margin: auto; margin-bottom: 5px;} } */ </style>";
}
