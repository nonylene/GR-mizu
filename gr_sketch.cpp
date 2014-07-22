#include <rxduino.h>
#include <ethernet.h>
#include <string>
//#include "mizu.hpp"
#include <vector>

byte mac[] = {0x00,0x16,0x3e,0x4e,0x8a,0x6c};
byte ip[] = {192,168,220,133};
TEthernet Ethernet;
EthernetServer server(80);

void setup()
{
    Ethernet.begin(mac,ip);
    while (Ethernet.localIP() == "0.0.0.0"){
        Ethernet.begin(mac,ip);
    }
    server.begin();
    Serial.begin(9600);
}

void returnHTML(EthernetClient& client,char value[]) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type:text/html; charset=UTF-8");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>GR-SAKURA 水まき</title>");
  client.println( value );
  client.println("<form method=POST>");
  client.println("<input type=submit name=1 value=みずをまく  /><br />");
  client.println("</form>");
  client.println("</body>");
  client.println("</html>");
}

void ethernet(EthernetClient& client)
{
    if(client){
        Serial.println("client");
        //Serial.println("ethernet");
        if (!client.connected()){
            Serial.println("it not works");
            return;
        }
        bool isend = true;
        while (isend){
            std::vector<char> line;
            //client.println("tes");
            char c;
            while (!client.available()){
                delay(1);
                Ethernet.processPackets();
            }
            while (c = client.read(),c !='\r'){
                Ethernet.processPackets();
                line.push_back(c);
                while (!client.available()){
                    delay(1);
                    Ethernet.processPackets();
                }
            }
            //\nがくる
            Ethernet.processPackets();
            while (!client.available()){
                delay(1);
                Ethernet.processPackets();
            }
            client.read();
            for (int i = 0; i <line.size();i++){
                Serial.print(line[i]);
            }
            if (line.size() == 0){
                isend = false;
            }
        }
        returnHTML(client,"とりあえずは動くだけplease click button...");
        /* if (client.available()){
           int a = client.read();

        //yomitoria
        if (a > 0)
        Serial.print(char(a));
        Serial.print(",");
        Serial.println(a);
        }

        }
        }*/
        }else{
        }
}



void loop(){
    EthernetClient client = server.available();
    Serial.println(Ethernet.localIP());
    //if (client){
        ethernet(client);
        client.stop();
    //}
    //t = mizu();
    //うごかす,trueorfalseでかえす?
}
