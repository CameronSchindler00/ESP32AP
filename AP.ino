#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

#define LED_PIN 5 // Onboard LED pin number

WebServer server(80);

bool ledState = false; // Initial state of the LED

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up AP...");

  // Set up AP
  WiFi.softAP(ssid, password, 6, 0, 1); // Allow only 1 connection

  Serial.println("AP set up!");
  Serial.println("SSID: " + String(ssid));
  Serial.println("Password: " + String(password));
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  // Set up web server
  server.on("/", handleRoot);
  server.on("/button", handleButton);
  server.begin();

  // Initialize the onboard LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>ESP32 Web Page</title>";
  html += "</head>";
  html += "<body>";
  html += "<h1>ESP32 Web Page</h1>";
  html += "<button onclick='sendRequest()'>Toggle LED!</button>";
  html += "<script>";
  html += "function sendRequest() {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/button', true);";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
}

void handleButton() {
  Serial.println("Button pressed!");

  ledState = !ledState; // Toggle the LED state

  digitalWrite(LED_PIN, ledState ? HIGH : LOW);

  Serial.println("Button pressed! LED is now " + String(ledState ? "on" : "off"));

  server.send(200, "text/plain", "Button pressed! LED is now " + String(ledState ? "on" : "off"));
}
