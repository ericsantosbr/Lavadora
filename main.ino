/*
 * Coded by Eric Santos and hosted on https://github.com/ericsantosbr/Lavadora
 * 
 * 
 * This program intends to replace mechanical rotary timers on wash machines, but can replace anything else that you can imagine
 * 
 * 
 * 
 * 
 * 
 * 
 */


int btn1 = 9;
int btn2 = 10;
int led1 = 7;
int led2 = 4;
int led3 = 3;
int rele = 12;

int cycles = 0;

//const int shortWash = 2;
//const int mediumWash = 4;
//const int longWash = 6;

const int shortWash = 1;
const int mediumWash = 2;
const int longWash = 3;

bool counting = false;

unsigned long baseMillis = 0;
unsigned long cycleRefresh = (unsigned long) 100;

unsigned long endOfWash = 0;

/*
 * Current states:
 *  's' = short wash
 *  'm' = medium wash
 *  'l' = long wash
 *  'i' = idle
 * 
 */

char state = 'i';

unsigned long nextSerialWrite = (unsigned long) 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(btn1, INPUT);
  pinMode(rele, OUTPUT);

  Serial.begin(115200);

  nextSerialWrite = millis() + (unsigned long) 1000;

}

void(* resetFunc) (void) = 0;

void loop() {
  // put your main code here, to run repeatedly:

  // stops the machine
  if(digitalRead(btn2)){
    state = 'i';

    counting = false;
    cycles = 0;

    baseMillis = 0;

    endOfWash = 0;
  }

  // interprets the button press
  if(digitalRead(btn1)){
    counting = true;

    if(baseMillis == (unsigned long) 0){
      baseMillis = millis();
    }

    cycles = (millis() - baseMillis) / 1000;

    if(cycles >= longWash){
      endOfWash = millis() + (60000 * 10);
      state = 'l';

      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
    }

    else if(cycles >= mediumWash && cycles <= longWash){
      endOfWash = millis() + (60000 * 7);
      state = 'm';

      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
    }
  
    else if(cycles >= shortWash && cycles <= mediumWash){
      endOfWash = millis() + (60000 * 5);
      state = 's';

      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }
  
  }

  else{
    counting = false;
    cycles = 0;

    baseMillis = 0;
  }

  

  if(counting == false){
    if(state == 'i'){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);

      digitalWrite(rele, LOW);
    }

    if(state == 's'){
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);

      digitalWrite(rele, HIGH);
    }

    if(state == 'm'){
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);

      digitalWrite(rele, HIGH);
    }

    if(state == 'l'){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);

      digitalWrite(rele, HIGH);
    }
    // updates the timer in hope to stop the machine
    if(state != 'i'){
      if(millis() >= endOfWash){
        state = 'i';
    
        counting = false;
        cycles = 0;
    
        baseMillis = 0;
      }
    
    }
  }

  if(millis() >= nextSerialWrite){
    nextSerialWrite = millis() + 1000;

    Serial.print("\ncycle: ");
    Serial.print(cycles);
    Serial.print("\nstate: ");
    Serial.print(state);
    Serial.print("\ntime: ");
    Serial.print(millis() / 1000);
    Serial.print("\nleft: ");
    Serial.print((endOfWash / 1000) - (millis() / 1000));
  }
}
