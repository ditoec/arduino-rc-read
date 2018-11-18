#include <EnableInterrupt.h>

#define RC_POS1  1000
#define RC_POS2  1080
#define RC_POS3  1180
#define RC_POS4  1280
#define RC_POS5  1380
#define RC_POS6  1500
#define RC_POS7  1620
#define RC_POS8  1720
#define RC_POS9  1820
#define RC_POS10 1920
#define RC_POS11 2000

#define RC_ERROR 45

#define POS1 1
#define POS2 2
#define POS3 3
#define POS4 4
#define POS5 5
#define POS6 6
#define POS7 7
#define POS8 8
#define POS9 9
#define POS10 10
#define POS11 11

#define SERIAL_PORT_SPEED 115200
#define RC_NUM_CHANNELS  4

#define RC_CH1  0
#define RC_CH2  1
#define RC_CH3  2
#define RC_CH4  3

#define RC_CH1_INPUT  A0
#define RC_CH2_INPUT  A1
#define RC_CH3_INPUT  A2
#define RC_CH4_INPUT  A3

uint8_t pos[RC_NUM_CHANNELS];
uint8_t old_pos[RC_NUM_CHANNELS];
int16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile int16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void update_pos(uint8_t channel){
  if(abs(rc_values[channel]-RC_POS1)<RC_ERROR)pos[channel] = POS1;
  else if(abs(rc_values[channel]-RC_POS6)<RC_ERROR)pos[channel] = POS6;
  else if(abs(rc_values[channel]-RC_POS11)<RC_ERROR)pos[channel] = POS11;
  else if(abs(rc_values[channel]-RC_POS2)<RC_ERROR)pos[channel] = POS2;
  else if(abs(rc_values[channel]-RC_POS3)<RC_ERROR)pos[channel] = POS3;
  else if(abs(rc_values[channel]-RC_POS4)<RC_ERROR)pos[channel] = POS4;
  else if(abs(rc_values[channel]-RC_POS5)<RC_ERROR)pos[channel] = POS5;
  else if(abs(rc_values[channel]-RC_POS7)<RC_ERROR)pos[channel] = POS7;
  else if(abs(rc_values[channel]-RC_POS8)<RC_ERROR)pos[channel] = POS8;
  else if(abs(rc_values[channel]-RC_POS9)<RC_ERROR)pos[channel] = POS9;
  else if(abs(rc_values[channel]-RC_POS10)<RC_ERROR)pos[channel] = POS10;
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() { calc_input(RC_CH1, RC_CH1_INPUT); }
void calc_ch2() { calc_input(RC_CH2, RC_CH2_INPUT); }
void calc_ch3() { calc_input(RC_CH3, RC_CH3_INPUT); }
void calc_ch4() { calc_input(RC_CH4, RC_CH4_INPUT); }

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_INPUT, INPUT);
  pinMode(RC_CH2_INPUT, INPUT);
  pinMode(RC_CH3_INPUT, INPUT);
  pinMode(RC_CH4_INPUT, INPUT);

  enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_INPUT, calc_ch4, CHANGE);
}

void loop() {
  rc_read_values();

  for(uint8_t i=0;i< RC_NUM_CHANNELS;i++)update_pos(i);

  if(pos[RC_CH1]!=old_pos[RC_CH1]){
    Serial.print("F3(Foc): "); Serial.println(pos[RC_CH1]);
    old_pos[RC_CH1] = pos[RC_CH1];
  }
  if(pos[RC_CH2]!=old_pos[RC_CH2]){
    Serial.print("F4(Pal): "); Serial.println(pos[RC_CH2]);
    old_pos[RC_CH2] = pos[RC_CH2];
  }
  if(pos[RC_CH3]!=old_pos[RC_CH3]){
    Serial.print("F5(HSE): "); Serial.println(pos[RC_CH3]);
    old_pos[RC_CH3] = pos[RC_CH3];
  }
  if(pos[RC_CH4]!=old_pos[RC_CH4]){
    Serial.print("F6(Rec): "); Serial.println(pos[RC_CH4]);
    old_pos[RC_CH4] = pos[RC_CH4];
  }
  
  if(Serial.available()){
    if(Serial.read()=='c'){
      Serial.print("F3(Foc): "); Serial.println(pos[RC_CH1]);
      Serial.print("F4(Pal): "); Serial.println(pos[RC_CH2]);
      Serial.print("F5(HSE): "); Serial.println(pos[RC_CH3]);
      Serial.print("F6(Rec): "); Serial.println(pos[RC_CH4]);
    }
  }

  delay(250);
}
