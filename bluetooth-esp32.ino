#include "BluetoothSerial.h"

// ===============================
// CONFIGURACIÓN DE PINES ESP32-CAM
// ===============================
#define LED1 2
#define LED2 4
#define LED3 15
#define PUSH 14   // Botón con INPUT_PULLUP

BluetoothSerial SerialBT;

// ===============================
// VARIABLES DEL BOTÓN
// ===============================
int lastButtonState = HIGH;
int currentButtonState;

unsigned long lastDebounceTime = 0;
unsigned long lastPressTime = 0;

const unsigned long debounceDelay = 50;
const unsigned long doubleClickDelay = 500;

// ===============================
// VARIABLES DEL MODO
// ===============================
// 0 = apagado
// 1 = light (3 LEDs encendidos)
// 2 = blink (LED2 parpadeando)
int mode = 0;

// ===============================
// VARIABLES DEL BLINK
// ===============================
unsigned long previousBlinkMillis = 0;
const unsigned long onTime = 1000;   // LED encendido 1s
const unsigned long offTime = 2000;  // LED apagado 2s
bool led2BlinkState = LOW;


// ===============================
// FUNCIONES
// ===============================

// Apagar todo
void turnOff() {
  mode = 0;
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

// Activar modo LIGHT
void modeLight() {
  mode = 1;
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
}

// Activar modo BLINK
void modeBlink() {
  mode = 2;
  led2BlinkState = LOW;
  digitalWrite(LED1, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED2, LOW);
}


// ===============================
// SETUP
// ===============================
void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(PUSH, INPUT_PULLUP);

  // Iniciar Bluetooth
  if (!SerialBT.begin("MU-1")) {
    Serial.println("❌ Error al iniciar Bluetooth");
    while (true);
  }

  Serial.println("✅ Bluetooth iniciado");
}



// ===============================
// LOOP PRINCIPAL
// ===============================
void loop() {

  // ===============================
  // LECTURA DEL BOTÓN (flanco HIGH → LOW)
  // ===============================

  currentButtonState = digitalRead(PUSH);

  if (currentButtonState == LOW && lastButtonState == HIGH && 
      (millis() - lastDebounceTime) > debounceDelay) {

    unsigned long currentTime = millis();

    // Detectar doble clic
    if (currentTime - lastPressTime < doubleClickDelay) {
      // DOBLE CLIC → BLINK
      if (mode != 2) {
        modeBlink();
      } else {
        turnOff();
      }
    } else {
      // CLIC SIMPLE → LIGHT o OFF
      if (mode == 0) {
        modeLight();
      } else if (mode == 1) {
        turnOff();
      } else if (mode == 2) {
        turnOff();
      }
    }

    lastPressTime = currentTime;
    lastDebounceTime = currentTime;
  }

  lastButtonState = currentButtonState;


  // ===============================
  // MANEJO DEL BLINK
  // ===============================
  if (mode == 2) {
    unsigned long currentMillis = millis();

    if (led2BlinkState == LOW && (currentMillis - previousBlinkMillis >= offTime)) {
      led2BlinkState = HIGH;
      previousBlinkMillis = currentMillis;
      digitalWrite(LED2, HIGH);

    } else if (led2BlinkState == HIGH && (currentMillis - previousBlinkMillis >= onTime)) {
      led2BlinkState = LOW;
      previousBlinkMillis = currentMillis;
      digitalWrite(LED2, LOW);
    }
  }


  // ===============================
  // RECEPCIÓN BLUETOOTH
  // ===============================

  if (SerialBT.hasClient()) {
    if (SerialBT.available()) {
      String cmd = SerialBT.readStringUntil('\n');
      cmd.trim();
      cmd.toLowerCase();

      Serial.print("📩 Recibido: ");
      Serial.println(cmd);

      if (cmd == "ligth") {
        modeLight();
        SerialBT.println("✔ Modo LIGHT activado");

      } else if (cmd == "blink") {
        modeBlink();
        SerialBT.println("✔ Modo BLINK activado");

      } else if (cmd == "off") {
        turnOff();
        SerialBT.println("✔ LEDs apagados");
      }
    }
  }
}
