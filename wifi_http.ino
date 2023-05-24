#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP.h>


const int LED_BUILTIN = 2;
const char* ssid = "ESPWifi";
const char* password = "1234";
AsyncWebServer server(80);

//standart IP = 192.168.4.1

void testLocalCredentials(){

  server.on("/ssid", HTTP_POST, [](AsyncWebServerRequest *request){
    String ssid_value;
    String password_value;
    //If the there are both "ssid" and "password" values
    if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
      ssid_value = request->getParam("ssid", true)->value();
      password_value = request->getParam("password", true)->value();
      Serial.print("SSID: ");
      Serial.println(ssid_value);
      Serial.print("Password: ");
      Serial.println(password_value);
    }
    request->send(200, "text/html", "SSID: " + ssid_value + "<br>Password: " + password_value);
  });

}

void setup() {
  WiFi.mode(WIFI_STA);
  //Erase flash memory
  Serial.begin(115200);
  // set the built-in LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT); 
  //delete old credentials
  WiFi.disconnect(true);
  // Start Access Point mode with defined name and password
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  //print SSID name
  Serial.println(WiFi.softAPSSID());
  // Print the IP address of the Access Point
  Serial.print("Access Point started. IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  


  // Serve the HTML form on the root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){

    String html_page = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Local Network configuration</title>
      <style>
        body {
          background-color: #f2f2f2;
          font-family: Arial, sans-serif;
        }
        h1 {
          text-align: center;
          margin-top: 50px;
        }
        form {
          margin: 50px auto;
          max-width: 500px;
          padding: 20px;
          background-color: #fff;
          border-radius: 5px;
          box-shadow: 0 0 10px rgba(0,0,0,0.2);
        }
        label {
          display: block;
          font-size: 18px;
          margin-bottom: 10px;
        }
        input[type="text"],
        input[type="password"] {
          display: block;
          width: 100%;
          padding: 10px;
          margin-bottom: 20px;
          font-size: 18px;
          border-radius: 5px;
          border: 1px solid #ccc;
          box-sizing: border-box;
        }
        input[type="submit"] {
          background-color: #4CAF50;
          color: #fff;
          border: none;
          border-radius: 5px;
          padding: 10px 20px;
          font-size: 18px;
          cursor: pointer;
        }
      </style>
    </head>
    <body>
      <h1>ESP32 Wi-Fi Configuration</h1>
      <form method="post" action="/ssid">
        <label for="ssid">SSID:</label>
        <input type="text" id="ssid" name="ssid">
        <label for="password">Password:</label>
        <input type="password" id="password" name="password">
        <input type="submit" value="Submit">
      </form>
    </body>
    </html>
    )=====";

    request->send(200, "text/html", html_page);
  });

  // Handle the form submission and store the SSID and password in variables
  server.on("/ssid", HTTP_POST, [](AsyncWebServerRequest *request){
    String ssid_value;
    String password_value;
    //If the there are both "ssid" and "password" values
    if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
      ssid_value = request->getParam("ssid", true)->value();
      password_value = request->getParam("password", true)->value();
      if (ssid_value == ssid && password_value == password){
        String html_page = R"=====(
        <!DOCTYPE html>
        <html>
        <head>
          <meta charset="UTF-8">
          <title>ESP32 Lamp Control</title>
        </head>
        <body>
          <h1>ESP32 Lamp Control</h1>

          <h2>Lamp 1</h2>
          <button onclick="toggleLamp1()">Toggle</button>

          <h2>Lamp 2</h2>
          <button onclick="toggleLamp2()">Toggle</button>

        </body>
        </html>
        )=====";

        request->send(200, "text/html", html_page);

      }
      
    }

  });

}

void loop() {
  
  // turn the LED on
  digitalWrite(LED_BUILTIN, HIGH); 
  // wait for 0.5 second
  delay(500); 
  // turn the LED off
  digitalWrite(LED_BUILTIN, LOW); 
  // wait for 0.5 second
  delay(500);

}
