# M5AtomS3-GPS-SPK
M5 AtomS3 with GPS and Speaker for GPS Logging

Hi I am trying to get a simple GPS logger function working so I can geolocate photos and utilize elevation/speed info in GoPro videos when I'm travelling.

I bought the M5Stack ATOM GPS Kit, and have tried numerous coding attempts, including using ChatGPT to assist, but can't get it to do what I need.

Current code is here https://github.com/phantompants/M5AtomGPSKit/blob/main/M5-AtomicGPS-ExampleCode-GPX-Elevation-ChatGPT-Coding.ino

ChatGPT (Code Pilot) Prompt: 

Write code for the that records NMEA data in GPX format (lat, long, elevation and time) in files saved by daily and named by the nearest Australian State/Territories city in format YYYYMMDD-StateCity.GPX.
- M5Stack AtomS3
- M5 GPS/BDS Unit AT6558 (BDS/GPS) + MAX2659
- M5 Atom SPK unit

Only record position data if unit moves more than 2m.

Write the code for upload in Arduino IDE 2.3.2

Enable the following functions:
- Enable logging with power on
- Enable toggle on/off with main button click
- Enable G27 LED to turn Red when Power On
- Enable G27 LED to Blue program started but Satellite fix not yet enabled
- Enable G27 LED to Flash Green when logging data to SD Card
- Enable G27 LED to Flash Red when in error
- Allow a Wi-Fi connection to get NTP data (when Wi-Fi available) and read the SD remotely via a we browser
- Allow a Bluetooth connection for iOS Bluetooth Terminal functions
- Pick Nearest Australian city by size of city as at least 5% of State/Territories population (generate this list (for all Australian and New Zealand States/Territories) in the code with latitude and longitude sorted by State then City)

Put wifi credentials in a file called secrets.py and include:
- Use Wi-Fi SSID "Mumbo-Jumbo"
- Use Wi-Fi Password "BPJY3Z5RF8@23"
