// defintation section 
#define ARRAY_SIZE 50
#define T1 3
#define T2 9
#define T3 5
#define T4 6
// end defeition section 
// function decleatraion section
void InitTimersSafe();
bool SetPinFrequencySafe(int8_t pin, uint32_t frequency);
void pwmWriteHR(uint8_t pin, uint16_t val);
// end function decleration section
// variable decleration section
char count = 1;
int32_t frequency = 10000;
int32_t volt;
unsigned int time = 10000;
int count1 = 1;
int count2 = 1;
int delaytime;
float sineWave[ARRAY_SIZE];
float angle;
// variable decaltion section

void setup() {
  for (int i = 0; i < ARRAY_SIZE; i++) {
    angle = (PI * i) / (ARRAY_SIZE - 1);
    sineWave[i] = sin(angle);
  }
  InitTimersSafe();
  SetPinFrequencySafe(10, frequency);
  SetPinFrequencySafe(9, frequency);
  DDRD |= 0b11100000;
    //Serial.begin(250000);
}
void loop() {
 // volt = analogRead(A0);
  //time = map(analogRead(A0),0,1023,5000 , 25000 );

    PORTD |=  (0b01000000);
  pwmWriteHR(10, 32700);  //Maximum 65535
  pwmWriteHR(9, 32700);
  TCCR1A |= bit (COM1A0);  
//Serial.println(volt);



}
