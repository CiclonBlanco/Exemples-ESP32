/*
 Exemple ESP32: Comptador de pulsacions d'un botó amb antirrebot (debounce)


 Connexió: un costat del botó al GPIO 13 i l'altre a GND.
 Es fa servir INPUT_PULLUP, per tant:
   - botó sense prémer -> HIGH
   - botó premut       -> LOW
*/


#define BUTTON_PIN 13     // GPIO13 connectat al botó
#define DEBOUNCE_TIME 50  // temps d'antirrebot en mil·lisegons


// Variables per a la lògica d'antirrebot
int lastSteadyState = HIGH;       // Amb INPUT_PULLUP l'estat de repòs és HIGH
int lastFlickerableState = HIGH;
int currentState;
unsigned long lastDebounceTime = 0;


// Comptador de pulsacions
unsigned int comptador = 0;


void setup() {
 Serial.begin(9600);
 pinMode(BUTTON_PIN, INPUT_PULLUP);
 Serial.println("Comptador de pulsacions iniciat");
}


// Retorna true una sola vegada cada cop que es prem el botó
bool button() {
 currentState = digitalRead(BUTTON_PIN);


 // Si l'estat ha canviat (pot ser soroll), reiniciem el temporitzador
 if (currentState != lastFlickerableState) {
   lastDebounceTime = millis();
   lastFlickerableState = currentState;
 }


 // Si l'estat s'ha mantingut estable més del temps d'antirrebot
 if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
   if (lastSteadyState != currentState) {
     lastSteadyState = currentState;
     // true només si el botó s'acaba de prémer (LOW amb INPUT_PULLUP)
     return (currentState == LOW);
   }
 }


 return false;
}


void loop() {
 if (button()) {
   comptador++;
   Serial.print("Botó premut. Total de pulsacions: ");
   Serial.println(comptador);
 }
}
