long tiempoEncendido = 1000;
long tiempoApagado   = 1000;


void setup() {
 Serial.begin(115200);
 pinMode(2, OUTPUT);
  Serial.println("--- CONFIGURACIÓN INICIAL DEL SEMÁFORO ---");
  // 1. Configurar y guardar el tiempo de encendido
 Serial.println("Escribe el tiempo de ENCENDIDO (ms) y pulsa Intro:");
 while (Serial.available() == 0) {} // Espera fija hasta que el usuario teclee algo
 tiempoEncendido = Serial.parseInt();
 Serial.print(">> Guardado tiempo de ENCENDIDO: ");
 Serial.println(tiempoEncendido);
  // Limpieza rápida del buffer del puerto serie
 while(Serial.available() > 0) { Serial.read(); }


 // 2. Configurar y guardar el tiempo de apagado
 Serial.println("Escribe el tiempo de APAGADO (ms) y pulsa Intro:");
 while (Serial.available() == 0) {} // Espera fija hasta que el usuario teclee algo
 tiempoApagado = Serial.parseInt();
 Serial.print(">> Guardado tiempo de APAGADO: ");
 Serial.println(tiempoApagado);


 Serial.println("\n--- CONFIGURACIÓN COMPLETADA. INICIANDO BUCLE ---");
}


void loop() {
 // El bucle se ejecutará infinitamente usando ÚNICAMENTE los valores guardados en el setup
 digitalWrite(2, HIGH);  
 delay(tiempoEncendido);           


 digitalWrite(2, LOW);   
 delay(tiempoApagado);             
}
