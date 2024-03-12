/* Example of ESP32 Captive Portal
*  by Alex Chen 2024-3-12
*  The following codes showcase the use of DNSServer and ESPAsyncWebServer libraries
*  to implement a captive portal, which is an index.html file stored in flash memory via
*  LittleFS library. The example portal includes a form which allows users to enter data 
*  which then be received by ESP32.
*/

#include <WiFi.h>
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>

// user defined variables 
#define AP_SSID "ESP32-CaptivePortal"
#define AP_PASSWORD "password"

// claim server related variables
DNSServer dnsServer;
AsyncWebServer server(80);

// create class object for later use in server.addHandler()
// this class is modified based on CaptivePortal example in ESPAsyncWebServer library 
class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    //return true;
    return request->url() == "/";
  }
};


void setup() {
  
  Serial.begin(115200);
  Serial.println(AP_SSID);

  // initiate LittleFS
  if(!LittleFS.begin()){
    Serial.println("Failed to start LittleFS");
    return;
  }

  // start WiFi AP mode
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  delay(100);

  // start dnsserver
  dnsServer.start(53, "*", WiFi.softAPIP());

  // use addHandler to intercept http request and initate captive portal
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);

  // load captive portal index page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
  });

  // load bootstrap css file
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/bootstrap.min.css", "text/css");
  });

  // the following is called when users hit the "submit" button in captive portal
  // the route "/submit" is from html form's action <form action="submit">
  // if all work, the serial monitor should print out the input values
  server.on("/submit", HTTP_GET, [](AsyncWebServerRequest *request){
    // get value from input tage named "userInputSSID", text input
    if(request->hasParam("userInputSSID")){ 
      String ssid = request->getParam("userInputSSID")->value();
      Serial.print("User SSID: ");Serial.println(ssid);
    }
    // get value from input tag named "userInputPW", password input
    if(request->hasParam("userInputPW")){
      String user_pw = request->getParam("userInputPW")->value();
      Serial.print("User PW: ");Serial.println(user_pw);
    }
    // get value from input tag named "userInputPW", select input
    if(request->hasParam("utcInput")){
      String user_tz = request->getParam("utcInput")->value();
      Serial.print("Timezone UTC: ");Serial.println(user_tz);
    }
    // get value from input tage named "dstCheck", checkbox. If unchecked, code not run.
    if(request->hasParam("dstCheck")){
      String user_dst = request->getParam("dstCheck")->value();
      Serial.print("DST check: ");Serial.println(user_dst);
    }
    // once user submitted the form, the portal returns a plain page showing "OK"
    request->send(200, "text/plain", "OK");
  });
  
  server.begin();

}

void loop() {

  dnsServer.processNextRequest();

}
