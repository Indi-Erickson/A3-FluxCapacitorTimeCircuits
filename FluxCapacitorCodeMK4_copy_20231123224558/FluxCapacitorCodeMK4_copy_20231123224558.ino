// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LEFTLEDPIN 12 // On Trinket or Gemma, suggest changing this to 1

#define RIGHTLEDPIN 13 // On Trinket or Gemma, suggest changing this to 1

#define BOTTOMLEDPIN 15 // On Trinket or Gemma, suggest changing this to 1


// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 8// Popular NeoPixel ring size

#define WHITEHEX 0xFFFFFF//Define the hex for the colour white.

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel topLeftLEDStrip(NUMPIXELS, LEFTLEDPIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel topRightLEDStrip(NUMPIXELS, RIGHTLEDPIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel bottomLEDStrip(NUMPIXELS, BOTTOMLEDPIN, NEO_GRB + NEO_KHZ800);


// The network credentials
const char* ssid     = "Flux-Capacitor";
const char* password = "OUTATIME";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

String whiteState = "off"; 
String flashingState = "off"; 
bool refreshPage = false;

unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 1000;  // interval at which to blink (milliseconds)
bool ledState = false; 
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();

  topLeftLEDStrip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  topRightLEDStrip.begin();

  bottomLEDStrip.begin();

  topLeftLEDStrip.clear();

  topRightLEDStrip.clear();

  bottomLEDStrip.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') { 
            if (currentLine.length() == 0 ||refreshPage == true) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();// if the byte is a newline character
              // The HTTP response ends with another blank line

              //START of main body of code.
              
              //Code below occur when the button is pressed calling the coresponding functions.
              //Duplicate this below code for adding in more buttons.
              if (header.indexOf("GET /white/on") >= 0){
                Serial.println("White button on");
                activateColour(WHITEHEX);
                whiteState = "on";
              }else if (header.indexOf("GET /white/off") >= 0){
                Serial.println("White button off");
                deactivate();
                whiteState = "off";
              }

              if (header.indexOf("GET /flashing/on") >= 0){
                Serial.println("grey button on");
                //activateColour(WHITEHEX);
                flashingState = "on";
              }else if (header.indexOf("GET /flashing/off") >= 0){
                Serial.println("grey button off");
                flashingState = "off";
              }
              // The Display of the HTML web page
              client.println("<!DOCTYPE html> <html>"); 
              client.println("<body>"); 
              client.println("<h2>Welcome to my remodelling of Back To The Future's Time Circuits!</h2>"); 
              client.println("<p>Each drop down represents a different element of the time circuit. The<strong> Destination time</strong> tells you where you are going, The<strong> Present time</strong> tells you where you are, The<strong> Last time departed</strong> tells you where you were</p>"); 
              client.println("<p>You input<strong> your destination time</strong> on this keypad:</p>");
              client.println("<form action=""/action_page.php""> <label for=""DESTINATION TIME"">DESTINATION TIME:</label> <input type=""datetime-local"" />");
              client.println("<form action=""/action_page.php""> <label for=""PRESENT TIME"">PRESENT TIME:</label> <input type=""datetime-local"" />"); 
              client.println("</form> </form> <form action=""/action_page.php""> <label for=""LAST TIME DEPARTED"">LAST TIME DEPARTED:</label> <input type=""datetime-local"" />"); 
              client.println("</form> <p>For example:<strong> Say you want to see the signing of the Declaration of Independence?</p>"); 
              client.println("<h2>GENERATE</h2> <input type=""button"" onclick=""alert('Congratulations! You have become the worlds first time traveller')"" value=""Let's go!"">"); 
              client.println("<h2>RESET</h2> <input type=""button"" onclick="" alert('You are back!')"" value=""Let's go again!""> </body> </html>"); 
              

              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the on/off buttons 
              // Feel free to change the background-color and font-size attributes to fit your preferences
              
              client.println("</head>");
              //The below code defines the style of the body
              client.println("<body style = ""background-color:white;""><h2 style= ""color:white"";>Colour Options:</h2>");

              //The below code defines all of the style for the page
              client.println("<style>html { font-family: text-align: center; Helvetica; display: inline-block; margin: 1px 1px auto;}");
              //The below code defines the style for the specific white button.
              //Note: There is two definitions of this button the off state(whiteButton) and the on state(whiteButton2)
              //Duplicate this below code for adding in more buttons.
              client.println(".whiteButton { background-color: white; border: 2px solid white; color: white; padding: 20px 20px; display: inline-block; border-radius: 50%;");
              client.println("text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}");
              client.println(".whiteButton2 {border: 2px solid grey;}");

              client.println(".flashingButton { background-color: grey; border: 2px solid white; color: white; padding: 20px 20px; display: inline-block; border-radius: 50%;");
              client.println("text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}");
              client.println(".flashingButton2 {border: 2px solid grey;}");

              client.println(".ButtonAlign {inline-block;}");
            
              client.println("<div class=""ButtonAlign""> <div p1></div><div p2></div></div>");

              client.println("</style>");


              // If the whitestate is off, it displays the ON button, and calls the above if statement.
              //Duplicate this below code for adding in more buttons.       
              if (whiteState=="off") {
                client.println("<p1><a href=\"/white/on\"><whiteButton class=\"whiteButton\"> </button></a></p1>");
              } else {
                client.println("<p1><a href=\"/white/off\"><whiteButton class=\"whiteButton whiteButton2\"> </button></a></p1>");
              } 

              if (flashingState=="off") {
                client.println("<p1><a href=\"/flashing/on\"><flashingButton class=\"flashingButton\"> </button></a></p1>");
              } else {
                client.println("<p1><a href=\"/flashing/off\"><flashingButton class=\"flashingButton flashingButton2\"> </button></a></p1>");
              } 
              client.println("</div>");

              client.println("</body></html>");

               //END of main body of code.         
              client.println();
              // Break out of the while loop
              break;
            }else { // if you got a newline, then clear currentLine
              currentLine = "";
            }  
        }else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
        // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

//if flashingState is on }
  if(flashingState == "on"){
    flashing(); 
  }else{
    deactivate();
  }
}

//This funtion reads in the hex of the desired colour and set the whole strip to that specific colour.
void activateColour(unsigned long specificColour){
  topLeftLEDStrip.clear();
  topRightLEDStrip.clear();
  bottomLEDStrip.clear();
  topLeftLEDStrip.fill(specificColour);  
  topRightLEDStrip.fill(specificColour);
  bottomLEDStrip.fill(specificColour);
  topLeftLEDStrip.show();
  topRightLEDStrip.show();
  bottomLEDStrip.show();
}
//This function turns off the whole LED strip.
void deactivate(){
  topLeftLEDStrip.clear();
  topRightLEDStrip.clear();
  bottomLEDStrip.clear();
  topLeftLEDStrip.show();
  topRightLEDStrip.show();
  bottomLEDStrip.show();
}
void flashing(){
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == true) {
      activateColour(0xFAEF94); 
      ledState = false; 
    } else {
      deactivate(); 
      ledState = true;
    }
  }
}