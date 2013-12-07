
#include <TinkerKit.h>
#include <Servo.h> 
#include <Ultrasonic.h>

int LEFT_SERVO_PIN = 9;
int RIGHT_SERVO_PIN = 10;
int TRIGGER_PIN = 12;
int ECHO_PIN = 13;

int BAUD = 9600;

int STILL = 94;
int LFORWARD = 180;
int LBACKWARD = 0;
int RFORWARD = 0;
int RBACKWARD = 180;
 
int AVOIDANCE_DISTANCE_IN_INCHES = 18;

int HAPPY = 1337;
int ANGRY = 9999;

Servo lefty; 
Servo righty;
TKButton right_button(I0);
TKButton left_button(I1);
TKLed angry_led(O0);
TKLed happy_led(O1);
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup(){
    Serial.begin(BAUD);
    lefty.attach(LEFT_SERVO_PIN);  
    righty.attach(RIGHT_SERVO_PIN);
}

void loop(){

    bool safe = is_it_safe_to_move_forward();
    if(safe){
        set_led(HAPPY);
        emotion_move(HAPPY);
    }
    else{
        set_led(ANGRY);
        emotion_move(ANGRY);
    }
}

float confused(){
    Serial.println("I am confused!");
    still();
    happy_led.on();
    angry_led.on();
    delay(5000);
}

void set_led(int emotion)
{
    if (emotion == HAPPY){
        Serial.println("Set LED to happy!");
        happy_led.on();
        angry_led.off();
        return;
    }
    if (emotion == ANGRY){
        Serial.println("Set LED to angry!");
        angry_led.on();
        happy_led.off();
        return;
    }
    Serial.println("I can't handle all this emotion!");
    confused();
    return;
}

void emotion_move(int emotion)
{
    if (emotion == HAPPY){
        Serial.println("Move forward!");
        forward();
        delay(50);
        return;
    }
    if (emotion == ANGRY){
        Serial.println("Hold it!");
        backward();
        delay(1000);
        left();
        delay(1000);
        return;
    }
    Serial.print("My emotions! MY EMOTIONS!");
}

bool is_it_safe_to_move_forward()
{
    return (( bumper() || ultrasound_inches() < AVOIDANCE_DISTANCE_IN_INCHES) == false);
}

float ultrasound_inches(){
  float inMsec;
  long microsec = ultrasonic.timing();

  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  Serial.print("MS: ");
  Serial.print(microsec);
  Serial.print(", IN: ");
  Serial.println(inMsec);
  return inMsec;
}

bool bumper(){
    return (left_button.read() == HIGH || right_button.read() == HIGH);
}
 
void still()
{
  lefty.write(STILL);
  righty.write(STILL);
}


void forward()
{
  lefty.write(LFORWARD);
  righty.write(RFORWARD);
}

void left()
{
  lefty.write(LFORWARD);
}

void right()
{
  righty.write(RFORWARD);
}

void backward()
{
  lefty.write(LBACKWARD);
  righty.write(RBACKWARD);  
}

