#define LED1 10
#define LED2 9
#define LED3 8
#define PUSH 2

int ledState = LOW;         // Estado actual del LED
int lastButtonState = LOW;  // Último estado leído del botón
int currentButtonState;     // Estado actual del botón

void setup() {
  pinMode(LED1, OUTPUT);   // LED como salida
  pinMode(LED2, OUTPUT);   // LED como salida
  pinMode(LED3, OUTPUT);   // LED como salida
  pinMode(PUSH, INPUT);   // Pulsador como entrada (con resistencia pull-down externa)
}

void loop() {
  currentButtonState = digitalRead(PUSH);  // Leer estado actual del botón

  // Detecta el cambio de estado de LOW -> HIGH (pulsación)
  if (currentButtonState == HIGH && lastButtonState == LOW) {
    // Cambiar el estado del LED (toggle)
    ledState = !ledState;
    digitalWrite(LED1, ledState);
    digitalWrite(LED2, ledState);
    digitalWrite(LED3, ledState);
    delay(50); // pequeña pausa para evitar rebotes eléctricos
  }

  lastButtonState = currentButtonState; // Guardar estado actual
}
