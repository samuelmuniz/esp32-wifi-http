#include <WiFi.h> //add library

#define relay1 21 // pin for relay control 1
#define relay2 22 // pin for relay control 2

const char *ssid = "********";     // assign WiFi network name
const char *password = "********"; // assign WiFi network password

unsigned long myTime = 0;

WiFiServer server(80); // standard http port

void menu() // future function for for a changeable network name and password
{
}

void relay_wifi()
{

  WiFiClient client = server.available(); // checks if there is a connected client with data to be transmitted

  if (client) // there is a client ?
  {           // YEP
    Serial.println("NEW CLIENT");
    String currentLine = ""; // store the mensagem from the client

    while (client.connected()) // keep it meanwhile the client is connected
    {

      if (client.available())   // there is a mensage?
      {                         // YEP
        char c = client.read(); // stores in the CHAR variable C
        Serial.write(c);

        if (c == '\n')                   // new line ?
        {                                // YEP
          if (currentLine.length() == 0) // mensage finished ?
          {                              // sim
            // gera a página HTML
            client.println("HTTP/1.1 200 OK"); // starts HTTP PROTOCOL
            client.println("Content-type:text/html");
            client.println();
            client.print("<h2>type<a href=\"/H1\">HERE</a> to turn the relay 1 on </h2><br>"); // send messsage by the serial port
            client.print("<h2>type<a href=\"/L1\">HERE</a> to turn the relay 1 off </h2><br>");
            client.print("<h2>type<a href=\"/H2\">HERE</a> to turn the relay 2 on </h2><br>");
            client.print("<h2>type<a href=\"/L2\">HERE</a> to turn the relay 2 off </h2><br>");
            client.println();
            break;

          } // end if currentLine.length

          else
            currentLine = ""; // NOP, prevents the string of stay void.

        } // end if c

        else if (c != '\r')
        {
          currentLine += c; // add each element of the message
        }

        // check the mensage to turn on or off each relay
        if (currentLine.endsWith("GET /H1"))
        {
          digitalWrite(relay1, HIGH);
        }
        if (currentLine.endsWith("GET /L1"))
        {
          digitalWrite(relay1, LOW);
        }
        if (currentLine.endsWith("GET /H2"))
        {
          digitalWrite(relay2, HIGH);
        }
        if (currentLine.endsWith("GET /L2"))
        {
          digitalWrite(relay2, LOW);
        }

      } // end if client.available()

    } // end if while client.connected

    client.stop(); // stops connection
    Serial.println("Client Disconnected.");
  }
}

void setup()
{
  Serial.begin(115200);    // inicializa Serial em 115200 baud rate
  pinMode(relay1, OUTPUT); // configura saída para relé 1
  pinMode(relay2, OUTPUT); // configura saída para relé 2

  Serial.println();                 //
  Serial.print("Conectando-se a "); //
  Serial.println(ssid);             //
  WiFi.begin(ssid, password);       // inicializa WiFi, passando o nome da rede e a senha

  while (WiFi.status() != WL_CONNECTED) // aguarda conexão (WL_CONNECTED é uma constante que indica sucesso na conexão)
  {
    if (millis() - myTime >= 1000)
    {
      Serial.print(".");
      myTime = millis();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");   
  Serial.println("IP ADRESS: "); 
  Serial.println(WiFi.localIP());     

  server.begin(); // inicializa o servidor web
}

void loop()
{

  while (WiFi.status() != WL_CONNECTED) // wait connection (WL_CONNECTED = TRUE)
  {
    WiFi.begin(ssid, password); //Starts WIFI object
    
    if (millis() - myTime >= 1000)
    {
      Serial.print(".");
      myTime = millis();
    }
  }

  relay_wifi(); // control each relay

} // end loop
