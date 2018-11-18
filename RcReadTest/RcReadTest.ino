#define RC_CENTRE 1500
#define RC_DOWN 1100
#define RC_UP 1900
#define RC_ERROR 100

#define PIN_2WAY 0
#define PIN_3WAY 1

volatile int pwm_2WAY = 0;
volatile int init_2WAY = 0;
volatile int pwm_3WAY = 0;
volatile int init_3WAY = 0;

String pos_2WAY = "";
String pos_3WAY = "";
 
void setup() {
  Serial.begin(115200);
  attachInterrupt(PIN_2WAY, rising_2WAY, RISING);
  attachInterrupt(PIN_3WAY, rising_3WAY, RISING);
}
 
void loop() {
  delay(1000);
  
  if(abs(pwm_2WAY-RC_DOWN)<RC_ERROR){
    pos_2WAY = "DOWN";
  }
  else if(abs(pwm_2WAY-RC_UP)<RC_ERROR){
    pos_2WAY = "UP";
  }
  else{
    pos_2WAY = "UNKNOWN";
  }

  if(abs(pwm_3WAY-RC_DOWN)<RC_ERROR){
    pos_3WAY = "DOWN";
  }
  else if(abs(pwm_3WAY-RC_CENTRE)<RC_ERROR){
    pos_3WAY = "CENTRE";
  }
  else if(abs(pwm_3WAY-RC_UP)<RC_ERROR){
    pos_3WAY = "UP";
  }
  else{
    pos_3WAY = "UNKNOWN";
  }
  
  Serial.print("2 WAY SWITCH position = ");
  Serial.println(pos_2WAY); 
  Serial.print("3 WAY SWITCH position = ");
  Serial.println(pos_3WAY); 
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
