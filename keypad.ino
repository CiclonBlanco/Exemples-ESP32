#include <Keypad.h>

// ---------- Teclat ----------
const byte FILES = 4;
const byte COLUMNES = 4;

char mapaTecles[FILES][COLUMNES] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte pinsFiles[FILES]       = { 19, 18, 5, 17 };
byte pinsColumnes[COLUMNES] = { 16, 4, 2, 15 };

Keypad teclat(makeKeymap(mapaTecles), pinsFiles, pinsColumnes, FILES, COLUMNES);

// ---------- Configuració ----------
const String CODI_SECRET   = "7A3C9";   // 5 caràcters (numeros i lletres)
const byte    MAX_INTENTS  = 3;
const unsigned long BLOQUEIG_MS = 10000; // 10 segons de bloqueig

// ---------- Estat ----------
String entrada = "";
byte intentsFallits = 0;

// ---------- Funcions ----------
void mostrarMissatgeInicial() {
  Serial.println("\n=== SISTEMA D'ACCES ===");
  Serial.println("Escriu el codi i prem # per confirmar.");
  Serial.println("Prem * per esborrar l'ultim caracter.");
}

void comprovarCodi() {
  Serial.println();

  if (entrada == CODI_SECRET) {
    Serial.println(">> Acces CONCEDIT. Benvingut!");
    intentsFallits = 0;
  } else {
    intentsFallits++;
    Serial.print(">> Codi INCORRECTE. Intents restants: ");
    Serial.println(MAX_INTENTS - intentsFallits);

    if (intentsFallits >= MAX_INTENTS) {
      bloquejarSistema();
    }
  }

  entrada = "";
}

void bloquejarSistema() {
  Serial.println("!! Massa intents. Sistema bloquejat.");
  unsigned long inici = millis();

  while (millis() - inici < BLOQUEIG_MS) {
    teclat.getKey();  // ignora les tecles durant el bloqueig
  }

  intentsFallits = 0;
  Serial.println("Sistema desbloquejat.");
  mostrarMissatgeInicial();
}

void processarTecla(char tecla) {
  if (tecla == '#') {                 // confirmar
    comprovarCodi();
  } else if (tecla == '*') {          // esborrar ultim caracter
    if (entrada.length() > 0) {
      entrada.remove(entrada.length() - 1);
      Serial.print("\b \b");
    }
  } else if (entrada.length() < CODI_SECRET.length()) {
    entrada += tecla;
    Serial.print("-");                // mostra un guio per privacitat
  }
}

// ---------- Programa principal ----------
void setup() {
  Serial.begin(9600);
  mostrarMissatgeInicial();
}

void loop() {
  char tecla = teclat.getKey();
  if (tecla) {
    processarTecla(tecla);
  }
}
