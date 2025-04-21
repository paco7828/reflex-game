#define BTN_R 2
#define BTN_L 1
#define LED_R 3
#define LED_L 4
bool gameRunning = false;
uint16_t maxTime = 700;  // Maximum time for first round
uint8_t score = 0;        // Score counter

// Include necessary library for the Watchdog Timer
#include <avr/wdt.h>

// Debounce button press
bool isPressed(uint8_t pin) {
  if (bitRead(PINB, pin) == 0) {
    delay(10);  // Debounce time
    return bitRead(PINB, pin) == 0;
  }
  return false;
}

void setup() {
  // Disable watchdog timer first thing in setup
  wdt_disable();
  
  // Set buttons as input
  DDRB &= ~((1 << BTN_R) | (1 << BTN_L));  // Buttons as inputs
  
  // Set LEDs as output
  DDRB |= (1 << LED_R) | (1 << LED_L);  // LEDs as outputs
  
  // Enable pull-ups for buttons
  PORTB |= (1 << BTN_R) | (1 << BTN_L);  // Pull-up resistors enabled
  
  // Reset game state
  gameRunning = false;
  score = 0;
  
  // Indicate system is ready by briefly flashing LEDs
  PORTB |= (1 << LED_R) | (1 << LED_L);
  delay(200);
  PORTB &= ~((1 << LED_R) | (1 << LED_L));
}

void flashStart() {
  // Flash both LEDs 4 times at the start
  for (uint8_t i = 0; i < 4; i++) {
    PORTB |= (1 << LED_R) | (1 << LED_L);  // Turn on LEDs
    delay(100);
    PORTB &= ~((1 << LED_R) | (1 << LED_L));  // Turn off LEDs
    delay(100);
  }
}

void waitRelease() {
  // Wait until both buttons are released
  while (((PINB & (1 << BTN_L)) == 0) || ((PINB & (1 << BTN_R)) == 0)) {
    delay(10);
  }
}

bool waitButton(uint8_t pin, uint16_t t) {
  for (uint16_t i = 0; i < t / 10; i++) {
    if ((PINB & (1 << pin)) == 0) {
      delay(10);  // Debounce
      if ((PINB & (1 << pin)) == 0) return true;
    }
    delay(10);
  }
  return false;
}

void flashRestart() {
  // Flash both LEDs to indicate 10th press reached (game restart)
  for (uint8_t i = 0; i < 5; i++) {
    PORTB |= (1 << LED_R) | (1 << LED_L);  // Turn on LEDs
    delay(150);
    PORTB &= ~((1 << LED_R) | (1 << LED_L));  // Turn off LEDs
    delay(150);
  }
}

void loop() {
  // Check if both buttons are pressed to start the game
  if (!gameRunning && ((PINB & (1 << BTN_L)) == 0) && ((PINB & (1 << BTN_R)) == 0)) {
    delay(50); // Debounce
    if (((PINB & (1 << BTN_L)) == 0) && ((PINB & (1 << BTN_R)) == 0)) {
      gameRunning = true;
      waitRelease();  // Wait for buttons to be released
      flashStart();   // Flash LEDs to start the game
      
      uint16_t t = maxTime;
      while (gameRunning) {
        uint8_t side = rand() & 1;  // Randomly pick a side
        uint8_t led = side ? LED_R : LED_L;
        uint8_t btn = side ? BTN_R : BTN_L;
        
        PORTB |= (1 << led);  // Turn on the LED (correct side)
        if (!waitButton(btn, t)) {
          gameRunning = false;  // Game over if wrong button is pressed or time runs out
          break;
        }
        PORTB &= ~(1 << led);  // Turn off LED
        delay(200);
        
        // Increase the score
        score++;
        
        // Check if score reached 10
        if (score >= 10) {
          flashRestart();  // Flash LEDs to indicate game restart
          score = 0;  // Reset score
          gameRunning = false;
          return;  // Exit to the main loop instead of using watchdog
        }
        
        if (t > 200) t -= 50;  // Reduce time after each correct press
      }
      
      // Simply turn off LEDs when game ends - no special flashing
      PORTB &= ~((1 << LED_R) | (1 << LED_L));
      gameRunning = false;  // Reset game state
      score = 0;  // Reset score
    }
  }
}