
const int TOUCH_PIN = 23; //D23

bool isTouching = false;

unsigned long touchStart = 0; //Timer 0

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN,INPUT);

}

void loop() {
  int touchState = digitalRead(TOUCH_PIN);

  if(touchState == HIGH && !isTouching){
    isTouching = true;
    touchStart = millis(); //Get milli secs
  }

  if(touchState == LOW && isTouching){
    isTouching = false;
    unsigned long timeTaken = millis() - touchStart;

   // Serial.print(timeTaken);
    //Serial.println(" ms. ");
    Serial.print("Registered as ");
    if (timeTaken < 300) {
      Serial.println(" short.");
    }else {
      Serial.println(" long.");
    }
  }

}
