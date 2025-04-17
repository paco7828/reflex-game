const byte btnR = 2, btnL = 1, ledR = 3, ledL = 4;
byte gameState = 0; // 0 = game not started, 1 = game in progress, 2 = game over
int flashDelay = 200;

void startGame();
void continueGame();
void gameOver();
byte chooseRandomLed();
void flashLeds();
bool debounce(byte btnPin);

void setup()
{
  pinMode(btnR, INPUT_PULLUP);
  pinMode(btnL, INPUT_PULLUP);
  pinMode(ledR, OUTPUT);
  pinMode(ledL, OUTPUT);
}

void loop()
{
  if (gameState == 0 && (debounce(btnR) || debounce(btnL)))
    startGame();
  else if (gameState == 1)
  {
    flashLeds();
    byte chosenLed = chooseRandomLed();
    digitalWrite(chosenLed, HIGH);
    delay(flashDelay * 2);

    if ((debounce(btnR) && chosenLed == ledR) || (debounce(btnL) && chosenLed == ledL))
    {
      continueGame();
    }
    else
    {
      gameOver();
    }
  }
}

void continueGame()
{
  digitalWrite(ledL, HIGH);
  digitalWrite(ledR, HIGH);
  delay(1000);
  digitalWrite(ledL, LOW);
  digitalWrite(ledR, LOW);
  flashDelay -= 10;
}

void gameOver()
{
  digitalWrite(ledL, LOW);
  digitalWrite(ledR, LOW);
  flashDelay = 200;
  gameState = 0;
}

byte chooseRandomLed()
{
  return random(ledR, ledL + 1); // Randomly choose between ledR and ledL
}

void flashLeds()
{
  for (byte i = 0; i < random(3, 11); i++)
  {
    digitalWrite(ledL, HIGH);
    delay(flashDelay);
    digitalWrite(ledL, LOW);
    delay(flashDelay);
    digitalWrite(ledR, HIGH);
    delay(flashDelay);
    digitalWrite(ledR, LOW);
    delay(flashDelay);
  }
}

void startGame()
{
  gameState = 1;
}

bool debounce(byte btnPin)
{
  static byte btnState = HIGH; // Single flag to track button state
  byte currentState = digitalRead(btnPin);
  if (currentState != btnState)
  {
    delay(10); // Simple debounce
    currentState = digitalRead(btnPin);
  }

  if (currentState == LOW && btnState == HIGH)
  {
    btnState = LOW;
    return true;
  }
  else if (currentState == HIGH && btnState == LOW)
  {
    btnState = HIGH;
  }

  return false;
}
