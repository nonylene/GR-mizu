/*GR-SAKURA Sketch Template Version: V1.08*/
#include <rxduino.h>

void set()
{
    //角度の読みとり
    int a = analogRead(0);
    int b = analogRead(1);
    server.print('%d\n' %a);
    server.print('%d\n' %b);
    //モーターでうごかす→角度計測→みずをだす
    digitalWrite(0, HIGH);
    analogWrite(0,150);
    digitalWrite(1, LOW);
    delay(1000);
    digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
    int a = analogRead(0);
    int b = analogRead(1);
    server.print('%d\n' %a);
    server.print('%d\n' %b);
    if (なんたらかんたら){
        //電流を流す
    }
    delay(100);
}
