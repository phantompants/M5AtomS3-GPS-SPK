#include <M5AtomS3.h>
#include <TinyGPS++.h>
#include <SD.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Credentials
const char* ssid = "Mumbo-Jumbo";
const char* password = "BPJY3Z5RF8@23";

// GPS and SD logging
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);
File gpxFile;

// Web server
WebServer server(80);

// City data
struct City {
    const char* name;
    const char* state;
    double lat;
    double lon;
};

City cities[] = {
    {"Sydney", "NSW", -33.8688, 151.2093},
    {"Melbourne", "VIC", -37.8136, 144.9631},
    {"Brisbane", "QLD", -27.4698, 153.0251},
    {"Perth", "WA", -31.9505, 115.8605},
    {"Adelaide", "SA", -34.9285, 138.6007},
    {"Hobart", "TAS", -42.8821, 147.3272},
    {"Darwin", "NT", -12.4634, 130.8456},
    {"Canberra", "ACT", -35.2809, 149.1300},
    {"Auckland", "NZ", -36.8485, 174.7633}
};

// Define colors
#define BLACK 0x0000
#define GREEN 0x07E0
#define RED 0xF800
#define WHITE 0xFFFF

void setup() {
    M5.begin();
    Serial.begin(115200);
    SerialGPS.begin(9600, SERIAL_8N1, 32, 33);

    // Initialize SD card
    if (!SD.begin()) {
        Serial.println("SD Card initialization failed!");
        return;
    }
    Serial.println("SD Card initialized.");

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }
    Serial.println("SPIFFS mounted successfully.");

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Start the server
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
    }

    if (gps.location.isUpdated()) {
        double lat = gps.location.lat();
        double lon = gps.location.lng();
        double ele = gps.altitude.meters();
        double speed = gps.speed.kmph();
        uint32_t satellites = gps.satellites.value();

        // Print detailed information to Serial
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Lat: %.6f, Lon: %.6f, Ele: %.2f, Speed: %.2f, Satellites: %d", lat, lon, ele, speed, satellites);
        Serial.println(buffer);

        // Update LCD display
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.printf("Lat: %.6f\n", lat);
        M5.Lcd.printf("Lon: %.6f\n", lon);
        M5.Lcd.printf("Ele: %.2f\n", ele);
        M5.Lcd.printf("Speed: %.2f\n", speed);
        M5.Lcd.printf("Satellites: %d\n", satellites);

        if (satellites == 0) {
            M5.Lcd.setTextColor(RED);
            M5.Lcd.printf("No Fix\n");
        }

        // Log data to SD card
        if (satellites > 0) {
            if (SD.exists("/gpslog.gpx")) {
                gpxFile = SD.open("/gpslog.gpx", FILE_APPEND);
            } else {
                gpxFile = SD.open("/gpslog.gpx", FILE_WRITE);
                gpxFile.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
                gpxFile.println("<gpx version=\"1.1\" creator=\"M5AtomGPS\">");
            }
            gpxFile.printf("<trkpt lat=\"%.6f\" lon=\"%.6f\"><ele>%.2f</ele><time>%s</time></trkpt>\n", lat, lon, ele, gps.time.value());
            gpxFile.close();
        }
    }

    // Handle web server
    server.handleClient();
}

void handleRoot() {
    String message = "M5Atom GPS Logger\n";
    message += "Files on SD Card:\n";

    File root = SD.open("/");
    File file = root.openNextFile();
    while (file) {
        message += file.name();
        message += "\n";
        file = root.openNextFile();
    }

    server.send(200, "text/plain", message);
}
