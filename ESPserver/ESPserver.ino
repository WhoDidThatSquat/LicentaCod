#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"

const char * ssid = "****";
const char * password = "****";

AsyncWebServer server(80);

char temp[6];
char hum[10];
char light[10];
char str[15];

const int TEMP = 20;
const int HUM = 50;


void GetData() {
   while (Serial.available()) {
      Serial.readBytes(str, 15);
   }
}

String Temperature() {
   Serial.print("TEMP ");
      Serial.print(TEMP);
   GetData();

   for (int i = 0; i < sizeof(str); i++) {
      if (isAscii(str[i])) {
         if (str[i]!=' ')
            temp[i] = str[i];
      }
   }
   return temp;
}

String Humidity() {
   GetData();
Serial.print("HUM ");
      Serial.print(HUM);
   for (int i = 0; i < sizeof(str); i++)
      if (isAscii(str[i]))
         for (int i = 0; i < sizeof(str); i++)
            if (str[i] == ' ')
               for (int j = 0; j <= 3; j++) {                
                  hum[j] = str[i];
                  i++;                           
               }
   return hum;
}

String Luminosity() {
   GetData();

   for (int i = 0; i < sizeof(str); i++)
      if (isAscii(str[i]))
         for (int i = 0; i < sizeof(str); i++)
            if (str[i] == ' ')
               for (int j = 0; j <= 3; j++) {
                  light[j] = str[i+3];
                  i++;
               }
   return light;
}

const char index_html[] PROGMEM = R"rawliteral( 
<!DOCTYPE HTML><html><link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300&display=swap" rel="stylesheet"> <head><script src="https://kit.fontawesome.com/3144f6e09c.js" crossorigin="anonymous"></script><meta name="viewport" content="width=device-width, initial-scale=1"><style>
    html {
     font-family: 'Roboto', sans-serif;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    div { font-size: 3.0rem; padding: 1.0rem 1.0rem 1.0rem 1.0rem; }
    span { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .labels{
      font-size: 2.0rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    .button {
  background-color: #00add6;
  border: none;
  color: white;
  padding: 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
  border-radius: 12px;
  }
.button:hover {background-color: #006780}
  </style></head><body><h2>Home Server</h2><div><i class="fas fa-thermometer-half" style="color:#059e8a;font-size: 2.3rem;"></i><span class="labels"> Temperature</span></br><span id="temperature">%TEMPERATURE%</span></div><div><i class="fas fa-tint" style="color:#00add6;font-size: 2.3rem;"></i><span class="labels"> Humidity</span></br><span id="humidity">%HUMIDITY%</span></div><div><i class="far fa-sun" style="color:#f0f509;font-size: 2.3rem;"></i><span class="labels"> Luminosity</span></br><span id="luminosity">%LUMINOSITY%</span></div>
  <form action="/temperature/input/" class="labels">
    Set max temperature</br>
    <input type="text" name="setTemp" min="15" max="30" placeholder="Default:20" class"labels">
    <input type="submit" value="Submit">
  </form><br>

    <button id="tempBtn" class="button" onclick="javascript:alert('Alerts for temperature are off')">Alerts for temperature</button>
</br></br>
    <form action="/humidity/input/" class="labels">
    Set humidity</br>
    <input type="text" name="setHum" min="20" max="60" placeholder="Default:50" class"labels">
    <input type="submit" value="Submit">
  </form><br>

    <button class="button" onclick="javascript:alert('Alerts for humidity are off')">Alerts for humidity</button>
  
  </body><script>
  

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      if (this.readyState == 4 && this.status == 200) {
        var s = this.responseText;
        var res;
        var pageText = document.getElementById("temperature").innerHTML;
        
        if(pageText.length>7){
          document.getElementById("temperature").innerHTML = " ";
          console.log(pageText,pageText.length);
          }
       
          res = s.substr(0,4);
          console.log(res);
          document.getElementById("temperature").innerHTML = res+"&deg;C";
        
        //else document.getElementById("temperature").innerHTML = this.responseText+"&deg;C";
      }
    }  
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 3000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var s = this.responseText;
      var sliced = s.slice(0,3);
        document.getElementById("humidity").innerHTML = sliced.trim()+"&percnt;";
      }
    };
  
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 3000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) { 
      let number = parseInt(this.responseText);
            if (number > 100)
              number = 100;
      document.getElementById("luminosity").innerHTML = number+"&percnt;";
    }
  };
  xhttp.open("GET", "/luminosity", true);
  xhttp.send();
}, 3000 ) ;
</script></html>
)rawliteral";

// Replaces placeholder with DHT values
String processor(const String &
   var) {
   if (var == "TEMPERATURE") {
      return Temperature();
   } else if (var == "HUMIDITY") {
      return Humidity();
   } 
   else if (var == "LUMINOSITY")
      return Luminosity();
   return String();
}

void setup() {
   // Serial port for debugging purposes
   Serial.begin(115200);

   // Connect to Wi-Fi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
   }

   // Print ESP32 Local IP Address
   Serial.println(WiFi.localIP());

   // Route for root / web page
   server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/html", index_html, processor);
   });
   server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/plain", Temperature().c_str());
   });
   server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
      request-> send_P(200, "text/plain", Humidity().c_str());
   });
   server.on("/luminosity", HTTP_GET, [](AsyncWebServerRequest * request) {
      request-> send_P(200, "text/plain", Luminosity().c_str());
   });

  server.on("/temperature/input/", HTTP_PUT, [] (AsyncWebServerRequest *request) {
      String inputMessage;
      String inputParam;
      inputMessage = request->getParam(TEMP)->value();
      inputParam = TEMP;
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field ("+ inputParam + ") with value: " + inputMessage +"<br><a href=\"/\">Return to Home Page</a>");
  });
  

  server.on("/humidity/input/", HTTP_PUT, [] (AsyncWebServerRequest *request) {
      String inputMessage;
      String inputParam;
      inputMessage = request->getParam(HUM)->value();
      inputParam = HUM;
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field ("+ inputParam + ") with value: " + inputMessage +"<br><a href=\"/\">Return to Home Page</a>");
  });

   // Start server
   server.begin();
}

void loop() {

}
