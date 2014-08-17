#include <rxduino.h>

void setuph()
{
    pinMode(2,OUTPUT);
    digitalWrite(2, HIGH);
    //output Vcc
    pinMode(0,OUTPUT);
    pinMode(1,OUTPUT);
 
    pinMode(10,OUTPUT);
    digitalWrite(10, HIGH);
    //output Vcc
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
    
    pinMode(6,OUTPUT);
    digitalWrite(6, HIGH);
    //output Vcc
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
}

void right(){
    //rotate_clockwise
    digitalWrite(0, HIGH);
    digitalWrite(1, LOW);
    analogWrite(0,200);
}

void left(){
    //rotate_counterclockwise
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    analogWrite(1,200);
}

void up(){
    //upup
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
}

void down(){
    //down
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
}

void stopr(){
    //brake_rotate
    digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
}

void offr(){
    //off_rotate
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
}

void stopu(){
    //brake_updown
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
}

void offu(){
    //off_updown
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
}

int mode = 1;
void set()
{   
    int vol = 0;
    vol = analogRead(0);
    Serial.println(vol);
    if(mode == 1){
        if (vol <100){
            offu();
            offr();
            delay(50);

            mode = 0;
            down();
            left();
        }else{
            up();
            right();
        }
   }else{
        if (vol >400){
            offu();
            offr();
            delay(50);
            
            mode = 1;
            right();
            up();
        }else{
            left();
            down();
        }
    }
    delay(10); 
}
