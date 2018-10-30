#include <ESP8266WiFi.h>
#include <credentials.h>

#define panel1GPIO 4
#define whiteGPIO 16
#define redGPIO 5
#define greenGPIO 2
#define blueGPIO 12
#define panel2GPIO 15

int brightness = 0;
bool standard = true;

// Start WiFi Server
WiFiServer server(80);

void setup()
{
  pinMode(panel1GPIO, OUTPUT);
  pinMode(panel2GPIO, OUTPUT);
  pinMode(whiteGPIO, OUTPUT);
  pinMode(redGPIO, OUTPUT);
  pinMode(blueGPIO, OUTPUT);
  pinMode(greenGPIO, OUTPUT);
  analogWrite(whiteGPIO, 1023);
  analogWrite(redGPIO, 900);
  analogWrite(blueGPIO, 900);
  analogWrite(greenGPIO, 900);
  // Setup console
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("");
  
  server.begin();
  Serial.println("Webserver started");
 
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
      if(standard) {
        standard=false;
          analogWrite(whiteGPIO, 0);
          analogWrite(redGPIO, 0);
          analogWrite(blueGPIO, 0);
          analogWrite(greenGPIO, 0);
      }
      // Read the first line of the request
      String req = client.readStringUntil('\r');
      Serial.println(req);
      client.flush();
    
      // Prepare the response. Start with the common header:
      String s = "HTTP/1.1 200 OK\r\n";
      s += "Content-Type: text/html\r\n\r\n";
      s += "<!DOCTYPE HTML>\r\n<html>\r\n";
      
      if((req.indexOf("/panel1/") != -1)) {
        int pos = req.length();
       int ind1 = req.indexOf("/panel1/") + 8;
        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        int ledValue = req.substring(ind1, ind2+ind1).toInt();

        if(ledValue>0){
         digitalWrite(panel1GPIO, HIGH);        
         }       
        else {
          digitalWrite(panel1GPIO, LOW);
        }
      }
          if((req.indexOf("/panel2/") != -1)) {
        int pos = req.length();
       int ind1 = req.indexOf("/panel2/") + 8;
        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        int ledValue = req.substring(ind1, ind2+ind1).toInt();

        if(ledValue>0){
         digitalWrite(panel2GPIO, HIGH);        
         }       
        else {
          digitalWrite(panel2GPIO, LOW);
        }
          }
      if((req.indexOf("/white/") != -1))
      {
        int ledValue = getLedValue("/white/", req);
        analogWrite(whiteGPIO, ledValue);
        // confirm to client
        s += "LED value changed to ";
        s += ledValue;
       
      }

      if((req.indexOf("/blue/") != -1))
      {
        int ledValue = getLedValue("/blue/", req);
        analogWrite(blueGPIO, ledValue);
        // confirm to client
        s += "LED value changed to ";
        s += ledValue;
       
      }

      if((req.indexOf("/green/") != -1))
      {
        int ledValue = getLedValue("/green/", req);
        analogWrite(greenGPIO, ledValue);
        // confirm to client
        s += "LED value changed to ";
        s += ledValue;
       
      }
      
      if((req.indexOf("/red/") != -1))
      {
        int ledValue = getLedValue("/red/", req);
        analogWrite(redGPIO, ledValue);
        // confirm to client
        s += "LED value changed to ";
        s += ledValue;
       
      }

      s += "<br>";
      s += "Request OK!";
      
      s += "</html>\n";
     
      client.flush();
    
      // Send the response to the client
      client.print(s);
      //delay(1);
      Serial.println("Client disonnected");
    
      // The client will actually be disconnected 
      // when the function returns and 'client' object is detroyed
      
  }

  
}

int getLedValue(String color, String req) {
  int pos = req.length();
  int clenght = color.length();
  int ind1 = req.indexOf(color) + clenght;
  String teststring = req.substring(ind1, pos);
  int ind2 = teststring.indexOf('/');
  String ledValue = req.substring(ind1, ind2+ind1);
  int ledVal = map(ledValue.toInt(),0,100,0,1023);
  return constrain(ledVal,0,1023);
}