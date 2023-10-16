#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DMDESP.h>
#include <fonts/EMSansSP8x16.h>

//SETUP DMD
#define DISPLAYS_WIDE 1 // Column
#define DISPLAYS_HIGH 1 // Row


// Number of P10 Panels used (COLUMN, ROW)
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);

// Wi-Fi Credentials
const char* ssid = "PrayasHS"; // Name of the Host
const char* password = "prayas#samsung"; // Password of the corresponding Host

WiFiClient client;
HTTPClient http;


void setup() {

  Disp.start();
  Disp.setBrightness(10);
  Disp.setFont(EMSansSP8x16);

  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nWelcome to CODEINIT\n");
  setup_wifi();
}
void loop() {

      // Disp.loop(); // Run Disp loop to refresh LED
      // Disp.drawText(2, 0, "999"); // Show Text
      // return;
  

  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n\nPerforming HTTP GET Request\n");

    // HTTP Details
    String requestUrl = "http://192.168.74.24:3000/";

    http.begin(client, requestUrl);

    int httpResponseCode = http.GET();
    Serial.print("HTTP Response code for ");
    Serial.print(requestUrl);
    Serial.print(":");

    Serial.println(httpResponseCode);


    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Response payload: " + payload);

      /*****************
      Deserializarion HELP link :
      https://arduinojson.org/v6/assistant/
      *****************/

      StaticJsonDocument<384> doc;

      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      const char* teamOneName = doc["teamOneName"]; // "Quality Dark Horse XI"
      const char* teamOneScore = doc["teamOneScore"]; // "95/8(20.0 Ov)"
      const char* teamOneWickets = doc["teamOneWickets"]; // "8"
      const char* teamOneOvers = doc["teamOneOvers"]; // "20"
      const char* teanTwoName = doc["teanTwoName"]; // "FireHawks"
      const char* teamTwoScore = doc["teamTwoScore"]; // "10.4"
      const char* resultIs = doc["resultIs"]; // "FireHawks won by 4 wickets"


      Serial.println("team 1 score : ");
      Serial.print(teamOneScore);
      Serial.println("team 2 score : ");
      Serial.print(teamTwoScore);
      Disp.loop(); // Run Disp loop to refresh LED
      Disp.drawText(2, 0, teamOneScore); // Show Text

    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      Disp.loop(); // Run Disp loop to refresh LED
      Disp.drawText(2, 0, "Err"); // Show Text
    }
    // Free resources
    http.end();

  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(10000);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}