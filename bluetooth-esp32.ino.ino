#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth no está habilitado. Activa Bluetooth en tu configuración del ESP32.
#endif

BluetoothSerial SerialBT;

const int ledPin = 4; // LED rojo trasero
char receivedChar;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Nombre del Bluetooth visible en el celular
  if (!SerialBT.begin("MU-1")) {
    Serial.println("❌ Error al iniciar Bluetooth");
    while (true);
  }

  Serial.println("✅ Bluetooth iniciado correctamente");
  Serial.println("Esperando conexión desde App Inventor...");
}

void loop() {
  if (SerialBT.hasClient()) {
    if (SerialBT.available()) {
      receivedChar = SerialBT.read();
      Serial.print("📩 Dato recibido: ");
      Serial.println(receivedChar);

      if (receivedChar == '0') {
        digitalWrite(ledPin, LOW);
        Serial.println("🌑 LED apagado");
      } else if (receivedChar == '1') {
        digitalWrite(ledPin, HIGH);
        Serial.println("💡 LED encendido");
      }
    }
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(10);
}
