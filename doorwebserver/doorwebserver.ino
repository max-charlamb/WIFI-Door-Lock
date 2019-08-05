#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <FS.h>

#define WIFI_USERNAME "HERE"
#define WIFI_PASSWORD "HERE"


ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)

void handleRoot();              // function prototypes for HTTP handlers
void handleOpen();
void handleLock();
void handleNotFound();

bool doorLocked = false;
bool error = false;

void setup(void) {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP(WIFI_USERNAME, WIFI_PASSWORD);   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  SPIFFS.begin();

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/open", HTTP_POST, handleOpen);
  server.on("/lock", HTTP_POST, handleLock);
  server.onNotFound(handleNotFound);

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {
  if (doorLocked) {
    if (error) {
      error = false;
      if (!handleFileRead("/lockederror.html")) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
        server.send(404, "text/plain", "404: File Not Found");
      }
    }
    else {
      if (!handleFileRead("/locked.html")) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
        server.send(404, "text/plain", "404: File Not Found");
      }
    }
  }
  else {
    if (error) {
      error = false;
      if (!handleFileRead("/openederror.html")) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
        server.send(404, "text/plain", "404: File Not Found");
      }
    }
    else {
      if (!handleFileRead("/opened.html")) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
        server.send(404, "text/plain", "404: File Not Found");
      }
    }
  }

}

void handleOpen() {
  if (doorLocked == true) {
    if (server.hasArg("password")) {
      if (server.arg("password") == "123") {
        Serial.println("Open");
        doorLocked = false;
      } else {
        error = true;
        Serial.println("Wrong password");
      }
    }

  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLock() {
  if (doorLocked == false) {
    if (server.hasArg("password")) {
      if (server.arg("password") == "123") {
        Serial.println("Lock");
        doorLocked = true;
      } else {
        error = true;
        Serial.println("Wrong password.");
      }
    }

  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound() { // if the requested file or page doesn't exist, return a 404 not found error
  if (!handleFileRead(server.uri())) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
    server.send(404, "text/plain", "404: File Not Found");
  }
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed version
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false;                                          // If the file doesn't exist, return false
}

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
