
// highest score ever
// |Daniel B. ---> 113|

// —— CONFIGURATION ——————————————————————————————————————————————————————
static const int   NUM_BUTTONS            = 9;
static const unsigned long GAME_DURATION  = 30000UL;
static const unsigned long DEBOUNCE_DELAY = 150UL;
const int ledPins   [NUM_BUTTONS] = {  2,  3,  4, 27, 29, 31, 45, 47, 49 };
const int switchPins[NUM_BUTTONS] = { A0, A1, A2, A3, A4, A5, A6, A7, A8 };
const int restartPin = A4;

// —— GAME STATE VARIABLES ———————————————————————————————————————————————
bool isGameActive = false;    // true if a round is in progress
int currentButton = -1;       // index of the lit button (0–8)
unsigned long gameStartTime = 0;        // millis() when the round began
int score = 0;        // keeps track of the correct hits

// —— SETUP & MAIN LOOP ————————————————————————————————————————————————————
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));  
  pinMode(restartPin, INPUT_PULLUP);

  // kick off the first game
  startNewGame();
}

void loop() {
  if (isGameActive) {
    updateGame();
  } else {
    waitForRestart();
  }
}

// —— GAME LOGIC FUNCTIONS —————————————————————————————————————————————————
void updateGame() {
  // If time’s still left, watch for the current target being pressed
  if (millis() - gameStartTime < GAME_DURATION) {
    if (digitalRead(switchPins[currentButton]) == LOW) {
      // correct hit!
      score++;
      digitalWrite(ledPins[currentButton], LOW);
      delay(DEBOUNCE_DELAY);    // simple debounce
      pickNextButton();
    }
  } else {
    // time’s up!
    endCurrentGame();
  }
}

void waitForRestart() {
  if (digitalRead(restartPin) == LOW) {
    delay(DEBOUNCE_DELAY);             // debounce the restart press
    while (digitalRead(restartPin) == LOW) {} // wait until it’s released
    startNewGame();
  }
}

void startNewGame() {
  score           = 0;
  gameStartTime   = millis();
  isGameActive    = true;
  flashAllButtons(2);   // quick “get ready” flash
  pickNextButton();     // light the first target
  Serial.println("Game started!");
}

void endCurrentGame() {
  // turn off all LEDs
  for (int i = 0; i < NUM_BUTTONS; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  isGameActive = false;
  Serial.print("Time’s up! Score: ");
  Serial.println(score);
  Serial.println("Press the center button to play again.");
}

void pickNextButton() {
  currentButton = random(NUM_BUTTONS);
  // clear every LED, then light only the chosen one
  for (int i = 0; i < NUM_BUTTONS; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[currentButton], HIGH);
}

void flashAllButtons(int times) {
  for (int t = 0; t < times; t++) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    delay(300);
    for (int i = 0; i < NUM_BUTTONS; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(300);
  }
}
