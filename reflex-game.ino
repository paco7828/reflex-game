const byte btnR = 2;
const byte btnL = 1;
const byte ledR = 3;
const byte ledL = 4;
bool gameStarted = false;
int score = 0;

struct ButtonState {
  bool lastState;
  unsigned long lastDebounceTime;
};

ButtonState btnRState = {HIGH, 0};
ButtonState btnLState = {HIGH, 0};
const unsigned long debounceDelay = 50;

void startGame();
void handleBtn(int btnPin, ButtonState* state, void (*callback)());

void setup() {
  pinMode(btnR, INPUT_PULLUP);
  pinMode(btnL, INPUT_PULLUP);
  pinMode(ledR, OUTPUT);
  pinMode(ledL, OUTPUT);
}

void loop() {
  if (!gameStarted) {
    handleBtn(btnR, &btnRState, startGame);
    handleBtn(btnL, &btnLState, startGame);
  } 
}

void handleBtn(int btnPin, ButtonState* state, void (*callback)()) {
  bool currentState = digitalRead(btnPin);
  
  if (currentState != state->lastState) {
    state->lastDebounceTime = millis();
  }
  
  if ((millis() - state->lastDebounceTime) > debounceDelay) {
    if (state->lastState == HIGH && currentState == LOW) {
      callback();
    }
  }
  
  state->lastState = currentState;
}

void startGame() {
  if (gameStarted) return;
  gameStarted = true;
  score = 0;

  for (int i = 0; i < 3; i++) {
    digitalWrite(ledR, HIGH);
    digitalWrite(ledL, HIGH);
    delay(500);
    digitalWrite(ledR, LOW);
    digitalWrite(ledL, LOW);
    delay(500);
  }
}