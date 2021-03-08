/*************************************************************************************************************************************
 * @file amNetwork.cpp
 * @author theAgingApprentice
 * @brief Handle all of the networking logic required by the robot. 
 * @details Manages TCP/IP, HTTP server, OTA firmware loading and MQTT messaging. See https://lastminuteengineers.com/esp32-ota-web-updater-arduino-ide/
 * @copyright Copyright (c) 2021 the Aging Apprentice
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files 
 * (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished 
 * to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * YYYY-MM-DD Dev        Description
 * ---------- ---------- -------------------------------------------------------------------------------------------------------------
 * 2021-03-08 Old Squire Added option page after login to select between MQTT broker IP configuraiton and OTA firmware updates.
 * 2021-03-07 Old Squire Program created
 *************************************************************************************************************************************/
 #include <amNetwork.h> // Required for Webserver data type

WebServer server(httpPort);
String webPageStyle = "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
                     "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
                     "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
                     "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
                     "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
                     ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";
String loginPage = "<head><meta charset='utf-8'/></head>"
                     "<form name=loginForm>"
                     "<h2>Zippy Login</h2>"
                     "<input name=userid placeholder='User ID'> "
                     "<input name=pwd placeholder=Password type=Password> "
                     "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
                     "<script>"
                     "function check(form) {"
                     "if(form.userid.value=='admin' && form.pwd.value=='admin')"
                     "{window.open('/chooseAction')}"
                     "else"
                     "{alert('Error Password or Username')}"
                     "}"
                     "</script>" + webPageStyle;
String optionPage = "<head><meta charset='utf-8'/></head>"
                     "<form name=optionForm>"
                     "<h2>Zippy Web Control</h2>"
                     "<input type=submit onclick=ota() class=btn value=OTA>"
                     "<input type=submit onclick=cfg() class=btn value=Config>"
                     "</form>"
                     "<script>"
                     "function ota() {"
                     "window.open('/otaWebUpdate')"
                     "}"
                     "function cfg() {"
                     "window.open('/cfgWebUpdate')"
                     "}"
                     "</script>" + webPageStyle;
String cfgPage = "<head><meta charset='utf-8'/></head>"
                     "<form method='post' action='/setMqtt' name='configForm'>"
                     "<h2>Zippy Config Updater</h2>"
                     "<input name=mqttIp placeholder='MQTT Broker IP'> "
//                     "<input type=submit onclick=sendMqttIp(this.form) class=btn value=Update></form>"
                     "<input type=submit class=btn value=Update></form>"
//                     "<script>"
//                     "function sendMqttIp(form) {"
//                     "alert(form.mqttIp.value)"
//                     "}"
//                     "</script>" + webPageStyle;
                     + webPageStyle;
String otaPage = "<head><meta charset='utf-8'/></head>"
                     "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
                     "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
                     "<h2>Zippy OTA Web Updater</h2>"
                     "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
                     "<label id='file-input' for='file'>   Choose file...</label>"
                     "<input type='submit' class=btn value='Update'>"
                     "<br><br>"
                     "<div id='prg'></div>"
                     "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
                     "<script>"
                     "function sub(obj){"
                     "var fileName = obj.value.split('\\\\');"
                     "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
                     "};"
                     "$('form').submit(function(e){"
                     "e.preventDefault();"
                     "var form = $('#upload_form')[0];"
                     "var data = new FormData(form);"
                     "$.ajax({"
                     "url: '/update',"
                     "type: 'POST',"
                     "data: data,"
                     "contentType: false,"
                     "processData:false,"
                     "xhr: function() {"
                     "var xhr = new window.XMLHttpRequest();"
                     "xhr.upload.addEventListener('progress', function(evt) {"
                     "if (evt.lengthComputable) {"
                     "var per = evt.loaded / evt.total;"
                     "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                     "$('#bar').css('width',Math.round(per*100) + '%');"
                     "}"
                     "}, false);"
                     "return xhr;"
                     "},"
                     "success:function(d, s) {"
                     "console.log('success!') "
                     "},"
                     "error: function (a, b, c) {"
                     "}"
                     "});"
                     "});"
                     "</script>" + webPageStyle;

/**
 * @brief This is the constructor for this class.
 * @details This class extends the LiquidCrystal_I2C class
===================================================================================================*/
amNetwork::amNetwork() // Constructor for this class
{

} //amNetwork::amNetwork()

/**
 * @brief This function translates the type of encryption that an Access Point (AP) advertises (an an ENUM) 
 * and returns a more human readable description of what that encryption method is.
 * @param wifi_auth_mode_t encryptionType is the ENUM that the Access Point advertises for encryption.
 * @return String containing human readable label for the type of encryption the Access Point uses.
 */
String amNetwork::_translateEncryptionType(wifi_auth_mode_t encryptionType)
{
   switch (encryptionType)
   {
      case (WIFI_AUTH_OPEN):
         return "Open";
      case (WIFI_AUTH_WEP):
         return "WEP";
      case (WIFI_AUTH_WPA_PSK):
         return "WPA_PSK";
      case (WIFI_AUTH_WPA2_PSK):
         return "WPA2_PSK";
      case (WIFI_AUTH_WPA_WPA2_PSK):
         return "WPA_WPA2_PSK";
      case (WIFI_AUTH_WPA2_ENTERPRISE):
         return "WPA2_ENTERPRISE";
      default:
         return "UNKNOWN";
   } //switch
} //amNetwork::translateEncryptionType()

/**
 * @brief Scan the 2.4Ghz band for known Access Points and select the one with the strongest signal.
 * @return const char _apSsid which is the SSID of the best AP candidate to connect to
=================================================================================================== */
const char* amNetwork::_lookForAP()
{
   _apSsid = "unknown"; //  At the start no known Access Point has been foundto connect to
   int numberOfNetworks = WiFi.scanNetworks(); // Used to track how many APs are detected by the scan
   int StrongestSignal = -127; // Used to find the strongest signal. Set as low as possible to start
   int SSIDIndex = 0; // Contains the SSID index number from the known list of APs
   bool APknown; // Flag to indicate if the current AP appears in the known AP list
   Serial.println(numberOfNetworks);

   // Loop through all detected APs
   for (int i = 0; i < numberOfNetworks; i++)
   {
      APknown = false;
   
      // Scan table of known APs to see if the current AP is known to us
      for (int j = 0; j < numKnownAPs; j++)
      {
         // If the current scanned AP appears in the known AP list note the index value and flag found
         if (WiFi.SSID(i) == SSID[j])
         {
            APknown = true;
            SSIDIndex = j;
         } //if
      }   //for

      // If the current AP is known and has a stronger signal than the others that have been checked
      // then store it in the variables that will be used to connect to the AP later
      if ((APknown == true) && (WiFi.SSID(i).toInt() > StrongestSignal))
      {
         _apSsid = SSID[SSIDIndex].c_str();
         _apPassword = Password[SSIDIndex].c_str();
         StrongestSignal = WiFi.SSID(i).toInt();
      } //if
   } //for
   return _apSsid;
}  //amNetwork::lookForAP()

/**
 * @brief Return the SSID of the Access Point the we are using.
 * @return String WiFi.SSID() Name of the AP  
===================================================================================================*/
String amNetwork::getAccessPointName()
{
   return WiFi.SSID();
} //amNetwork::getAccessPointName()

/**
 * @brief Return the MAC of the Access Point of our WiFi radio transmitter.
 * @return String WiFi.macAddress() MAC address of WiFi radio  
===================================================================================================*/
String amNetwork::getMacAddress()
{
   return WiFi.macAddress();
} //amNetwork::getMACaddress()

/**
 * @brief Return the IP address that was assigned to us by the DHCP service on the local router.
 * @return IPAddress WiFi.localIP() IPv4 address assigned by APs DHCP service.    
===================================================================================================*/
IPAddress amNetwork::getIpAddress()
{
   return WiFi.localIP();
} //amNetwork::getIpAddress()

/**
 * @brief Return the robot name which is both unique and predictable.
 * @return String HOST_NAME_PREFIX  + _format.noColonMAC(getMacAddress()).   
===================================================================================================*/
String amNetwork::getUniqueName()
{
   return HOST_NAME_PREFIX  + _format.noColonMAC(getMacAddress());
} //amNetwork::getUniqueName()

/**
 * @brief Manage the process of connecting to the WiFi network.
 * @details Scan for a suitable Access Point and if one is found then connect to it using the 
 * init_wifi() method. This method connects to the Access Point, Starts up a webserver that serves
 * up a web page that allows you to configure network settings withut recompiling as well as 
 * handling Over The Air updates using websockets.
 * @return bool where true means a conneciton was made and false means no connectioon was made.
===================================================================================================*/
bool amNetwork::connect()
{
   _lookForAP(); // Scan the 2.4Ghz band for known Access Points and select the one with the strongest signal 
   if((String)_apSsid == "unknown") // If a suitable Access point is found 
   {
      Serial.println("<amNetwork::connect> No known Access Point SSID was detected. Cannot connect to WiFi at this time.");
      return false; // No network
   } //if
   WiFi.mode(WIFI_STA);
   _hostName = getUniqueName().c_str(); // Host name becomes Zippy + MAC address without colons
   WiFi.softAP(_hostName); //Assign unique name for robot that the network sees
   WiFi.begin(_apSsid, _apPassword); // COnnect too strongest AP found
   if (WiFi.waitForConnectResult() != WL_CONNECTED) // Wait for connection
   {
      Serial.print("<amNetwork::connect> Failed to connect to SSID ");
      Serial.println(_apSsid);
      return false; // No network
   } //if  
   /*use mdns for host name resolution*/
   if (!MDNS.begin(_hostName)) //http://esp32.local
   { 
      Serial.println("<amNetwork::connect> Error setting up MDNS responder. No web based OTA possible");
      return true; // Yes we have a network but we are not hosting a website
   } //if
   Serial.println("<amNetwork::connect> mDNS responder started");
   /*return index page which is stored in serverIndex */
   server.on("/", HTTP_GET, []() 
   {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", loginPage);
   });
   // Send OTA page to select binary image to upload
   server.on("/otaWebUpdate", HTTP_GET, []() 
   {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", otaPage);
   });
   // Send option page to select what action the user wishes to take
   server.on("/chooseAction", HTTP_GET, []() 
   {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", optionPage);
   });
   // Get MQTT value and send same config page back to browser
   server.on("/setMqtt", HTTP_POST, []() 
   {
      Serial.println("<amNetwork::setMqtt> Broker IP address = ");
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", optionPage);
   });   // Send CFG page to set new variiable's without needing to reboot
   server.on("/cfgWebUpdate", HTTP_GET, []() 
   {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", cfgPage);
   });
   /*handling uploading firmware file */
   server.on("/update", HTTP_POST, []() 
   {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
   }, 
   []() 
   {
      HTTPUpload& upload = server.upload();
      if(upload.status == UPLOAD_FILE_START) 
      {
         Serial.printf("Update: %s\n", upload.filename.c_str());
         if(!Update.begin(UPDATE_SIZE_UNKNOWN)) 
         { //start with max available size
            Update.printError(Serial);
         } //if
      } //if 
      else if(upload.status == UPLOAD_FILE_WRITE) 
      {
         /* flashing firmware to ESP*/
         if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) 
         {
         Update.printError(Serial);
         } //if
      } //else if 
      else if (upload.status == UPLOAD_FILE_END) 
      {
         if (Update.end(true)) //true to set the size to the current progress
         { 
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
         } // if
         else 
         {
            Update.printError(Serial);
         } //else
      } //else if
   });
   server.begin();
   return true;
} //amNetwork::connect()

/**
 * @brief Check to see if there is a request coming in from a web browser. If there is, service it.
===================================================================================================*/
void amNetwork::checkForClientRequest()
{
   server.handleClient();
} //amNetwork::checkForClientRequest()

/**
 * @brief Send detailed network configuration information to the console.
 * @details The ESP32-D0WDQ6 chip has dual Tensilica LX6 micro controllers. All WiFi logic runs on 
 * the PRO_CPU (core0) aka the processor core. This firmware runs on the APP_CPU (core 1) aka the 
 * application core.     
===================================================================================================*/
void amNetwork::cfgToConsole()
{
   Serial.println("<amNetwork::cfgToConsole> Network settings:");  
   Serial.print("<amNetwork::cfgToConsole> ... Access Point Name = "); Serial.println(getAccessPointName()); 
   Serial.print("<amNetwork::cfgToConsole> ... Robot MAC address: "); Serial.println(getMacAddress());
   Serial.print("<amNetwork::cfgToConsole> ... Robot IP address: "); Serial.println(getIpAddress());
   Serial.print("<amNetwork::cfgToConsole> ... Robot Host Name: "); Serial.println(getUniqueName());   
} //amNetwork::cfgToConsole()
