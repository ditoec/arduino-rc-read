#define RC_CENTRE 1500
#define RC_DOWN 1100
#define RC_UP 1900
#define RC_ERROR 100

#define PIN_2WAY 0
#define PIN_3WAY 1

#define ERR 0
#define DOWN 1
#define CENTRE 2
#define UP 3

volatile int pwm_2WAY = 0;
volatile int init_2WAY = 0;
volatile int pwm_3WAY = 0;
volatile int init_3WAY = 0;

uint8_t pos_2WAY = ERR;
uint8_t old_pos_2WAY = ERR;
uint8_t pos_3WAY = ERR;
uint8_t old_pos_3WAY = ERR;
 
void setup() {
  Serial.begin(115200);
  attachInterrupt(PIN_2WAY, rising_2WAY, RISING);
  attachInterrupt(PIN_3WAY, rising_3WAY, RISING);
}
 
void loop() {
  noInterrupts();
  int _pwm_2WAY = pwm_2WAY;
  int _pwm_3WAY = pwm_3WAY;
  interrupts();
  
  if(abs(_pwm_2WAY-RC_DOWN)<RC_ERROR){
    pos_2WAY = DOWN;
  }
  else if(abs(_pwm_2WAY-RC_UP)<RC_ERROR){
    pos_2WAY = UP;
  }
  else{
    pos_2WAY = ERR;
  }

  if(abs(_pwm_3WAY-RC_DOWN)<RC_ERROR){
    pos_3WAY = DOWN;
  }
  else if(abs(_pwm_3WAY-RC_CENTRE)<RC_ERROR){
    pos_3WAY = CENTRE;
  }
  else if(abs(_pwm_3WAY-RC_UP)<RC_ERROR){
    pos_3WAY = UP;
  }
  else{
    pos_3WAY = ERR;
  }

  if((pos_2WAY!=old_pos_2WAY)||(pos_3WAY!=old_pos_3WAY)){
    Serial.print(pos_2WAY);
    Serial.println(pos_3WAY);
    old_pos_2WAY=pos_2WAY;
    old_pos_3WAY=pos_3WAY;  
  }

  if(Serial.available()){
      Serial.print(pos_2WAY);
      Serial.println(pos_3WAY);
  }
  
  delay(500);
}
 
void rising_2WAY() {
  attachInterrupt(PIN_2WAY, falling_2WAY, FALLING);
  init_2WAY = micros();
}
 
void falling_2WAY() {
  attachInterrupt(PIN_2WAY, rising_2WAY, RISING);
  pwm_2WAY = micros()-init_2WAY;
}

void rising_3WAY() {
  attachInterrupt(PIN_3WAY, falling_3WAY, FALLING);
  init_3WAY = micros();
}
 
void falling_3WAY() {
  attachInterrupt(PIN_3WAY, rising_3WAY, RISING);
  pwm_3WAY = micros()-init_3WAY;
}
