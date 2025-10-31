#define LED1 10
#define LED2 9
#define LED3 8
#define PUSH 2

int ledState = LOW;          // Estado general (para modo 1)
int lastButtonState = LOW;   // Último estado leído del botón
int currentButtonState;      // Estado actual del botón

unsigned long lastDebounceTime = 0;
unsigned long lastPressTime = 0;
const unsigned long debounceDelay = 50;
const unsigned long doubleClickDelay = 500; // tiempo máximo entre clics para considerar doble clic

// Variables para el blink del LED2
unsigned long previousBlinkMillis = 0;
const unsigned long onTime = 1000;   // 1 segundo encendido
const unsigned long offTime = 2000;  // 2 segundos apagado
bool led2BlinkState = LOW;

// Estado general del sistema
// 0 = apagado
// 1 = 3 LEDs encendidos
// 2 = LED2 parpadeando
int mode = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(PUSH, INPUT);
}

void loop() {
  currentButtonState = digitalRead(PUSH);

  // Detectar flanco de subida (pulsación)
  if (currentButtonState == HIGH && lastButtonState == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    unsigned long currentTime = millis();

    // Si el tiempo entre esta pulsación y la anterior es corto, es doble clic
    if (currentTime - lastPressTime < doubleClickDelay) {
      // Doble clic detectado
      if (mode != 2) {
        mode = 2; // Activar modo blink
        led2BlinkState = LOW;
        digitalWrite(LED1, LOW);
        digitalWrite(LED3, LOW);
      } else {
        mode = 0; // Apagar todo si ya estaba en modo blink
      }
    } else {
      // Primer clic (posible simple)
      if (mode == 0) {
        mode = 1; // Encender todos
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
      } else if (mode == 1) {
        mode = 0; // Apagar todos
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
      } else if (mode == 2) {
        mode = 0; // Apagar todo desde modo blink
        digitalWrite(LED2, LOW);
      }
    }

    lastPressTime = currentTime;
    lastDebounceTime = currentTime;
  }

  lastButtonState = currentButtonState;

  // Si estamos en modo blink, ejecutar el parpadeo del LED2
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
}
