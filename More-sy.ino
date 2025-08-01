#include <map>

//MAP EVERYTHING , Use ChatGPT, I ain't not gonna type everything
std::map<String, char> morseMap = {
  {",-",'A'}, {"-...",'B'},{"-.-.",'C'},{"-..",'D'},{".",'E'},{"..-,",'F'},
  {"--.",'G'},{"....",'H'},{"..",'I'},{".---",'J'},{"-.-",'K'}, {".-..",'L'},{"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, {".--.", 'P'},
  {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
  {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'},
  {"-.--", 'Y'}, {"--..", 'Z'}
}

std::map<String, int> wordToInt = {
  {"ZERO",0}, {"ONE", 1}, {"TWO", 2}, {"THREE", 3},
  {"FOUR", 4}, {"FIVE", 5}, {"SIX", 6}, {"SEVEN", 7},
  {"EIGHT", 8}, {"NINE", 9}, {"TEN", 10}
}


const int TOUCH_PIN = 23; //D23

bool isTouching = false;
bool spaceSent = false;


unsigned long touchStart = 0; //Timer 0
unsigned long lastTouch = 0; //Timer 1


void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN,INPUT);

}

void loop() {
  int touchState = digitalRead(TOUCH_PIN);

  if(touchState == HIGH && !isTouching){
    isTouching = true;
    touchStart = millis(); //Get milli secs
    spaceSent = false;
  }

  if(touchState == LOW && isTouching){
    isTouching = false;
    unsigned long timeTaken = millis() - touchStart;
    lastTouch = millis();

   // Serial.print(timeTaken);
    //Serial.println(" ms. ");
    if (timeTaken < 300) {
      Serial.print(".");
    }else {
      Serial.print("-");
    }
  }

  if(!isTouching && lastTouch > 0 ){
    unsigned long times =  millis() - lastTouch ;
    if(times >= 1500 && times < 3000 && !spaceSent){
      Serial.println(" "); //Space never prints, if it works dont touch
      spaceSent = true;
    }else if (times >= 3000 && spaceSent){
      Serial.print("\n");
      spaceSent = false;
      lastTouch = 0; //dont repeat end word 
    }
  }

  delay(10);

}
