#include <ESP8266WiFi.h>

void LED_CHANGE();

// LED FET
#define ledPIN    0
#define whiteledPIN    0

int led_delay = 0;
#define time_at_colour 1200 
unsigned long TIME_LED = 0;
int LED; 
int LED_A[] = {0, 0, 0, 0, 0, 0};
int pinled = 0;

// WiFi Settings
const char *ssid =  "San_Francisco_2,4-GHz";
const char *pass =  "unskilled bobs flattened awkward zucchini";

// Start WiFi Server
WiFiServer server(80);

void setup()
{
    /*set main light to 255 by default
  int ledVal = 1023;
        LED = constrain(ledVal,0,1023);
        
        int diff_led = abs(LED-0);
        if(diff_led > 0){
          led_delay = time_at_colour / abs(LED-0); 
        }else{
          led_delay = time_at_colour / 100; 
        }
  pinled = 0;
  */
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  analogWrite(0, 1023);
  int ledVal = 1023;
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

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
    
      // Read the first line of the request
      String req = client.readStringUntil('\r');
      Serial.println(req);
      client.flush();
    
      // Prepare the response. Start with the common header:
      String s = "HTTP/1.1 200 OK\r\n";
      s += "Content-Type: text/html\r\n\r\n";
      s += "<!DOCTYPE HTML>\r\n<html>\r\n";
      
      if((req.indexOf("/led/") != -1)) {
         
        // get RGB value
        int pos = req.length();
        int ind1 = req.indexOf("/led/") + 5;
        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        String ledValue = req.substring(ind1, ind2+ind1);
        
        int ledVal = map(ledValue.toInt(),0,100,0,1023);
        
        if(ledVal==0){
          digitalWrite(2, LOW);
        }
        else {
          digitalWrite(2, HIGH);
        }
        
        // confirm to client
        s += "LED value changed to ";
        s += ledValue;
      
      }
      if((req.indexOf("/white/") != -1)) {
        
        // get RGB value
        int pos = req.length();
        int ind1 = req.indexOf("/white/") + 7;
        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        String ledValue = req.substring(ind1, ind2+ind1);
        
        int ledVal = map(ledValue.toInt(),0,100,0,1023);
        LED = constrain(ledVal,0,1023);
        pinled = 0;
        int diff_led = abs(LED-LED_A[pinled]);
        if(diff_led > 0){
          led_delay = time_at_colour / abs(LED-LED_A[pinled]); 
        }else{
          led_delay = time_at_colour / 100; 
        }
        
        // confirm to client
        s += "LED value changed to white ";
        s += ledValue;
       
      }
      if((req.indexOf("/red/") != -1)) {
         
        // get RGB value
        int pos = req.length();
        int ind1 = req.indexOf("/red/") + 5;
        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        String ledValue = req.substring(ind1, ind2+ind1);
        
        int ledVal = map(ledValue.toInt(),0,100,0,1023);
        LED = constrain(ledVal,0,1023);
        pinled = 5;
        int diff_led = abs(LED-LED_A[pinled]);
        if(diff_led > 0){
          led_delay = time_at_colour / abs(LED-LED_A[pinled]); 
        }else{
          led_delay = time_at_colour / 100; 
        }
        
        // confirm to client
        s += "LED value changed to red ";
        s += ledValue;
       
      }
      if((req.indexOf("/blue/") != -1)) {
         
        // get RGB value
        int pos = req.length();
        int ind1 = req.indexOf("/blue/") + 6;
        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        String ledValue = req.substring(ind1, ind2+ind1);
        
        int ledVal = map(ledValue.toInt(),0,100,0,1023);
        LED = constrain(ledVal,0,1023);
        pinled = 4;
        int diff_led = abs(LED-LED_A[pinled]);
        if(diff_led > 0){
          led_delay = time_at_colour / abs(LED-LED_A[pinled]); 
        }else{
          led_delay = time_at_colour / 100; 
        }
        
        // confirm to client
        s += "LED value changed to blue ";
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
  
  if(millis() - TIME_LED >= led_delay){
    TIME_LED = millis();
    LED_CHANGE();
  }
  
}

void LED_CHANGE()
{
  if(LED != LED_A[pinled]){
    if(LED_A[pinled] > LED) LED_A[pinled] = LED_A[pinled] - 1;
    if(LED_A[pinled] < LED) LED_A[pinled]++;
    analogWrite(pinled, LED_A[pinled]);
  }
}
