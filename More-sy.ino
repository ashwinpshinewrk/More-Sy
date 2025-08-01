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

std::map<String, char> WordToOp = {
  {"ADD", '+'},
  {"SUB", '-'},
  {"MUL", '*'}
};


const auto DOT_DASH_THRES = 300;
const auto LETTER_PAUSE = 1000;
const auto WORD_PAUSE = 2000; // 2 sec
const auto LAST_PAUSE = 5000;

const int TOUCH_PIN = 23; //D23

bool isTouching = false;


unsigned long touchStart = 0; //Timer 0
unsigned long lastTouch = 0; //Timer 1


String morseSymbl = "";
String decodedWord = "";
std::vector<String> token; //All the stuff in vector 

void morseProcess(){
  long result = 0;
    bool ok = true;
    String t0 = token[0];
    t0.toUpperCase();
    if(!wordToInt.count(t0)) ok= false;
    else result = wordToInt[t0];

    for(size_t i = 1; i + 1 < token.size() && ok; i+= 2){ // BRUH THIS TOOK TOO MCUH TIME< USE CHATGPT PLEASE
      String opw = token[i];
      opw.toUpperCase();
      String nw = token[i+1];
      nw.toUpperCase();

      if(!WordToOp.count(opw) || !wordToInt.count(nw)) {
        ok = false;
        break;
      }
      char op = WordToOp[opw];
      int val = wordToInt[nw]; // thanks cormac
      if (op == '+') result += val;
      else if (op == '-') result -= val;
      else if (op == '*') result *= val;
    }
    Serial.println();
    if(ok) {
      Serial.print("Result : ");
      Serial.println(result);
    }else {
      Serial.println("Invlaid or smt , BRO TOUCH GRASS");
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN,INPUT);

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

  // if(!isTouching && lastTouch > 0 ){
  //   unsigned long idleTime =  now - lastTouch ;
  //   if(idleTime >= WORD_PAUSE){
  //     if (morseSymbl.length()){
  //       if(morseMap.count(morseSymbl)) {
  //         decodedWord += morseMap[morseSymbl];
  //       }else {
  //         decodedWord = "?";
  //       }
  //       morseSymbl = "";
  //     }
  //     Serial.println(decodedWord);
  //     lastTouch = now;
  //   }else if (idleTime >= LETTER_PAUSE && morseSymbl.length()){
  //     if (morseMap.count(morseSymbl)){
  //       char c = morseMap[morseSymbl];
  //       Serial.print(c);
  //       decodedWord += c;
  //     }else {
  //       Serial.print('?');
  //       decodedWord += "?";
  //     }
  //     morseSymbl = "";
  //     lastTouch = now;
  //   }
  // }

  // if(!isTouching && morseSymbl.length() > 0 && (now - lastTouch >= LETTER_PAUSE) && (now - lastTouch < WORD_PAUSE)){
  //   char letter = morseMap.count(morseSymbl) ? morseMap[morseSymbl] : '?';
  //   Serial.print("-> ");
  //   Serial.println(letter);
  //   decodedWord += letter;
  //   morseSymbl = "";
  //   lastTouch = now;
  // }
  // else if(!isTouching && morseSymbl.length() > 0 && (now - lastTouch >= WORD_PAUSE) && (now - lastTouch < LAST_PAUSE)){
  //   Serial.print("Word -> ");
  //   Serial.println(decodedWord);
  //   token.push_back(decodedWord);
  //   decodedWord = "";
  //   lastTouch = now;
  // }

  // if(!isTouching && token.size() >= 3 && (now - lastTouch >= LAST_PAUSE)){

  //   token.clear();

  //   morseSymbl = "";
  //   decodedWord = "";
  //   lastTouch = now;
  //   Serial.println("\nNEXT WORD");

  // }

  if (!isTouching && lastTouch > 0) {
    unsigned long idleTime = now - lastTouch;

    if(morseSymbl.length() > 0 && idleTime >= LETTER_PAUSE){
      char letter = morseMap.count(morseSymbl) ? morseMap[morseSymbl] : '?';
      Serial.print("--> ");
      Serial.println(letter);
      decodedWord += letter;
      morseSymbl = "";
    }

    else if (decodedWord.length() > 0 && idleTime >= WORD_PAUSE){
      Serial.print("Word -> ");
      Serial.println(decodedWord);
      token.push_back(decodedWord);
      decodedWord = "";
    }

    else if (token.size() >= 3 && idleTime >= LAST_PAUSE){
      morseProcess();
      token.clear();
      morseSymbl = "";
      decodedWord ="";
      Serial.println("\n Ready for next calculation.... ");
    }
  }
  
  delay(1);

}
