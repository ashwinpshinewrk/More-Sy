#include <map>

//MAP EVERYTHING , Use ChatGPT, I ain't not gonna type everything
std::map<String, char> morseMap = {
  {".-",'A'}, {"-...",'B'},{"-.-.",'C'},{"-..",'D'},{".",'E'},{"..-.",'F'},
  {"--.",'G'},{"....",'H'},{"..",'I'},{".---",'J'},{"-.-",'K'}, {".-..",'L'},{"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, {".--.", 'P'},
  {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
  {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'},
  {"-.--", 'Y'}, {"--..", 'Z'}
};

std::map<String, int> wordToInt = {
  {"ZERO",0}, {"ONE", 1}, {"TWO", 2}, {"THREE", 3},
  {"FOUR", 4}, {"FIVE", 5}, {"SIX", 6}, {"SEVEN", 7},
  {"EIGHT", 8}, {"NINE", 9}, {"TEN", 10}
};


const auto DOT_DASH_THRES = 300;
const auto LETTER_PAUSE = 10000;
const auto WORD_PAUSE = 2000; // 2 sec

const int TOUCH_PIN = 23; //D23

bool isTouching = false;
bool spaceSent = false;


unsigned long touchStart = 0; //Timer 0
unsigned long lastTouch = 0; //Timer 1


String morseSymbl = "";
String decodedWord = "";

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN,INPUT);
  Serial.println("Ready");

}

void loop() {

  int touchState = digitalRead(TOUCH_PIN);
  unsigned long now = millis();

  if(touchState == HIGH && !isTouching){
    isTouching = true;
    touchStart = now;
  }

  if(touchState == LOW && isTouching){
    isTouching = false;
    unsigned long timeTaken = now - touchStart;
    lastTouch = now;

   // Serial.print(timeTaken);
    //Serial.println(" ms. ");
    if (timeTaken < DOT_DASH_THRES) {
      Serial.print(".");
      morseSymbl += ".";
    }else {
      Serial.print("-");
      morseSymbl += "-";
    }
  }

  if(!isTouching && lastTouch > 0 ){
    unsigned long idleTime =  now - lastTouch ;
    if(idleTime >= WORD_PAUSE){
      if (morseSymbl.length()){
        if(morseMap.count(morseSymbl)) {
          decodedWord += morseMap[morseSymbl];
        }else {
          decodedWord = "?";
        }
        morseSymbl = "";
      }
      Serial.println(decodedWord);
      lastTouch = now;
    }else if (idleTime >= LETTER_PAUSE && morseSymbl.length()){
      if (morseMap.count(morseSymbl)){
        char c = morseMap[morseSymbl];
        Serial.print(c);
        decodedWord += c;
      }else {
        Serial.print('?');
        decodedWord += "?";
      }
      morseSymbl = "";
      lastTouch = now;
    }
  }

  delay(1);

}
