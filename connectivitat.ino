// Rellotge NTP + peticio HTTP amb ESP32 (provat per a Wokwi)
// Guia WiFi de Wokwi: https://docs.wokwi.com/guides/esp32-wifi

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD(0x27, 16, 2);

// ---------- Configuracio ----------
#define NTP_SERVER   "pool.ntp.org"
// Zona horaria de Barcelona: CET (hivern) / CEST (estiu) amb canvi d'hora automatic
#define ZONA_HORARIA "CET-1CEST,M3.5.0,M10.5.0/3"
#define URL_IP       "http://api.ipify.org"   // retorna la IP publica en text pla

const unsigned long INTERVAL_IP_MS = 60000;   // refresca la IP cada minut
unsigned long ultimaConsultaIP = 0;
String ipPublica = "---";

// ---------- Animacio mentre es connecta ----------
void spinner() {
  static byte pas = 0;
  const char frames[] = { '|', '/', '-', '\\' };
  LCD.setCursor(15, 1);
  LCD.print(frames[pas]);
  pas = (pas + 1) % 4;
}

// ---------- Peticio HTTP GET ----------
void consultarIPPublica() {
  HTTPClient http;
  http.begin(URL_IP);
  int codi = http.GET();

  if (codi == HTTP_CODE_OK) {
    ipPublica = http.getString();
    Serial.println("IP publica: " + ipPublica);
  } else {
    ipPublica = "Error " + String(codi);
    Serial.println("Error HTTP: " + String(codi));
  }
  http.end();
}

// ---------- Mostrar hora i data a la pantalla ----------
void mostrarHora() {
  struct tm temps;
  if (!getLocalTime(&temps)) {
    LCD.setCursor(0, 1);
    LCD.print("Sense hora NTP  ");
    return;
  }

  LCD.setCursor(0, 0);
  LCD.print(&temps, "%H:%M:%S  %Z  ");   // p. ex. 14:32:05  CET

  LCD.setCursor(0, 1);
  LCD.print(&temps, "%d/%m/%Y      ");   // p. ex. 10/11/2026
}

void setup() {
  Serial.begin(115200);

  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Connectant WiFi");

  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    spinner();
  }

  Serial.println("\nWiFi connectat");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  // Sincronitza el rellotge intern amb el servidor NTP
  configTzTime(ZONA_HORARIA, NTP_SERVER);

  // Primera consulta HTTP i la mostrem 3 segons
  consultarIPPublica();
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("IP publica:");
  LCD.setCursor(0, 1);
  LCD.print(ipPublica);
  delay(3000);
  LCD.clear();

  ultimaConsultaIP = millis();
}

void loop() {
  mostrarHora();

  // Torna a consultar la IP cada minut sense bloquejar el rellotge
  if (millis() - ultimaConsultaIP >= INTERVAL_IP_MS) {
    consultarIPPublica();
    ultimaConsultaIP = millis();
  }

  delay(250);
}
