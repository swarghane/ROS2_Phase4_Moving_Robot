// =========================
// L298N + Arduino Uno Code
// =========================

// L298N Connections
// ENA  -> D5  (PWM)
// IN1  -> D6
// IN2  -> D7
// IN3  -> D8
// IN4  -> D9
// ENB  -> D10 (PWM)

#define ENA 5
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define ENB 10

unsigned long lastCommandTime = 0;
const int timeoutMs = 500;

void setup() {

  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();

  lastCommandTime = millis();
}

void loop() {

  // =========================
  // READ SERIAL COMMAND
  // =========================

  if (Serial.available()) {

    String data = Serial.readStringUntil('\n');

    data.trim();

    int commaIndex = data.indexOf(',');

    if (commaIndex > 0) {

      int leftSpeed =
        data.substring(0, commaIndex).toInt();

      int rightSpeed =
        data.substring(commaIndex + 1).toInt();

      leftSpeed = constrain(leftSpeed, -255, 255);
      rightSpeed = constrain(rightSpeed, -255, 255);

      lastCommandTime = millis();

      controlMotor(leftSpeed, rightSpeed);
    }
  }

  // =========================
  // SAFETY TIMEOUT
  // =========================

  if (millis() - lastCommandTime > timeoutMs) {

    stopMotors();
  }
}

void controlMotor(int leftSpeed, int rightSpeed) {

  // =========================
  // LEFT MOTOR DIRECTION
  // =========================

  if (leftSpeed > 0) {

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

  }
  else if (leftSpeed < 0) {

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

  }
  else {

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // =========================
  // RIGHT MOTOR DIRECTION
  // =========================

  if (rightSpeed > 0) {

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

  }
  else if (rightSpeed < 0) {

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

  }
  else {

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  // =========================
  // MOTOR SPEED
  // =========================

  analogWrite(ENA, abs(leftSpeed));
  analogWrite(ENB, abs(rightSpeed));
}

void stopMotors() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
