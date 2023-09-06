//4 way Traffic light controller EE301N


//Defining Macros
//signal 1
#define r1 2 // Red  Led Pin Out
#define y1 3 // Yellow Led Pin Out
#define g1 4 // Green   Led Pin Out
//signal 2
#define r2 5 // Red  Led Pin Out
#define y2 6 // Yellow Led Pin Out
#define g2 7 // Green  Led Pin Out
//signal 3
#define r3 8 // Red  Led Pin Out
#define y3 9 // Yellow Led Pin Out
#define g3 10// Green Led Pin Out
//signal 4
#define r4 11// Red Led Pin Out
#define y4 12// Yellow Led Pin Out
#define g4 13// Green Led Pin Out
//For HC-SRO4 sensor
#define echoPin A0 // attach pin A0 Arduino to pin Echo of HC-SR04
#define trigPin A1 //attach pin A1 Arduino to pin Trig of HC-SR04

int G1_time = 4;  //Set singal 1 open time
int G2_time = 6;  //Set singal 2 open time
int G3_time = 8;  //Set singal 3 open time
int G4_time = 10; //Set singal 4 open time
int emer_time= 4; //Set emergency open time
int emer_state=0;

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


int Y_time = 1; //Set signal Wait time

int Mode=0;
int Second=0;
word MilliSecond=0;

void setup(){ // 

pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

pinMode(r1, OUTPUT);
pinMode(y1, OUTPUT);
pinMode(g1, OUTPUT);

pinMode(r2, OUTPUT);
pinMode(y2, OUTPUT);
pinMode(g2, OUTPUT);

pinMode(r3, OUTPUT);
pinMode(y3, OUTPUT);
pinMode(g3, OUTPUT);

pinMode(r4, OUTPUT);
pinMode(y4, OUTPUT);
pinMode(g4, OUTPUT);

noInterrupts();         // disable all interrupts
TCCR1A = 0;             // set entire TCCR1A register to 0  //set timer1 interrupt at 1kHz  // 1 ms
TCCR1B = 0;             // same for TCCR1B
TCNT1  = 0;             // set timer count for 1khz increments
OCR1A = 1999;           // = (16*10^6) / (1000*8) - 1
//had to use 16 bit timer1 for this because 199989>255, but could switch to timers 0 or 2 with larger prescaler
// turn on CTC mode
TCCR1B |= (1 << WGM12); 
// Set CS11 bit for 8 prescaler
TCCR1B |= (1 << CS11);  // enable timer compare interrupt
TIMSK1 |= (1 << OCIE1A);
interrupts();           // enable
}

void loop(){

digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
digitalWrite(trigPin, HIGH);
  
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;     //cm

if(distance<= 35)
{
  emer_state=1;
  Second = emer_time;
  Emergency(emer_state);
  emer_state=0;
  Mode=0;

} 

if(Mode==0 && Second==0){Second=G1_time;  Open(Mode);Mode=1;}
if(Mode==1 && Second==0){Second=Y_time;  yellow(Mode);Mode=2; }

if(Mode==2 && Second==0){Second=G2_time;  Open(Mode);Mode=3;}
if(Mode==3 && Second==0){Second=Y_time;  yellow(Mode);Mode=4; }

if(Mode==4 && Second==0){Second=G3_time;  Open(Mode);Mode=5;}
if(Mode==5 && Second==0){Second=Y_time;  yellow(Mode);Mode=6; }

if(Mode==6 && Second==0){Second=G4_time; Open(Mode);Mode=7; }
if(Mode==7 && Second==0){Second=Y_time; yellow(Mode); Mode=0; }

}


void yellow(int y){
digitalWrite(r1, 1);  
if(y==1){digitalWrite(r1, 0);digitalWrite(y1, 1);}
       else{digitalWrite(y1, 0);}
digitalWrite(g1, 0);

digitalWrite(r2, 1);  
if(y==3){digitalWrite(r2, 0);digitalWrite(y2, 1);}
       else{digitalWrite(y2, 0);}
digitalWrite(g2, 0);

digitalWrite(r3, 1);  
if(y==5){digitalWrite(r3, 0); digitalWrite(y3, 1);}
       else{digitalWrite(y3, 0);}
digitalWrite(g3, 0);

digitalWrite(r4, 1);  
if(y==7){digitalWrite(r4, 0);digitalWrite(y4, 1);}
       else{digitalWrite(y4, 0);}
digitalWrite(g4, 0);
}

void Open(int Set){
 
digitalWrite(y1, 0);
if(Set==0){digitalWrite(g1, 1); digitalWrite(r1, 0);}
       else{digitalWrite(g1, 0); digitalWrite(r1, 1);}

digitalWrite(y2, 0);
if(Set==2){digitalWrite(g2, 1); digitalWrite(r2, 0);}
       else{digitalWrite(g2, 0); digitalWrite(r2, 1);}
 
digitalWrite(y3, 0);
if(Set==4){digitalWrite(g3, 1); digitalWrite(r3, 0);}
       else{digitalWrite(g3, 0); digitalWrite(r3, 1);}

digitalWrite(y4, 0);
if(Set==6){digitalWrite(g4, 1); digitalWrite(r4, 0);}
       else{digitalWrite(g4, 0); digitalWrite(r4, 1);}

}

//Function for Emergency State
void Emergency(int stop)
{
  if(stop == 1)
  {
    digitalWrite(r1, 1);
    digitalWrite(y1, 0);
    digitalWrite(g1, 0);
    digitalWrite(r2, 1);
    digitalWrite(y2, 0);
    digitalWrite(g2, 0);
    digitalWrite(r3, 1);
    digitalWrite(y3, 0);
    digitalWrite(g3, 0);
    digitalWrite(r4, 1);
    digitalWrite(y4, 0);
    digitalWrite(g4, 0);
  }
}

// Timer function
ISR(TIMER1_COMPA_vect){   
MilliSecond++;
if(MilliSecond >= 1000){MilliSecond = 0;
       Second = Second-1;
 }
}