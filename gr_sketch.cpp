#include <rxduino.h>
#include <ethernet.h>
#include <string>
//#include "mizu.hpp"
#include <vector>

byte mac[] = {0x00,0x16,0x3e,0x4e,0x8a,0x6c};
byte ip[] = {192,168,220,133};
TEthernet Ethernet;
EthernetServer server(80);

void setup(){
    Ethernet.begin(mac,ip);
    //これつけると落ちなくなった。）
    while (Ethernet.localIP() == "0.0.0.0"){
        Ethernet.begin(mac,ip);
    }
    server.begin();
    Serial.begin(9600);
}

void returnHTML(EthernetClient& client,char value[]){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type:text/html; charset=UTF-8");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>GR-SAKURA 水まき</title>");
  client.println( value );
  client.println("<form method=GET>");
  client.println("<input type=submit name=m value=GO /><br />");
  client.println("</form>");
  client.println("</body>");
  client.println("</html>");
}

void ethernet(EthernetClient& client){
    if(client){
        Serial.println("Client Connected");
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
            //ここで読み取りが終わり。
            for (int i = 0; i < line.size();i++){
                Serial.print(line[i]);
            }
            //検索とかはstringにすると使いやすそう
            std::string lines(line.begin(),line.end());
            //paramの検索
            std::string param;
            int a = lines.find("?");
            int b = lines.find("HTTP");
            if (a != std::string::npos && b != std::string::npos){
                param = lines.substr(a+1,b-a-2);
            }
            Serial.println();
            if (param != ""){
                Serial.print("params: ");
                for (int i = 0; i < param.size();i++){
                    Serial.print(param[i]);
                }
                Serial.println();
                //paramを分析する
                int m_s = param.find("m=");
                if (m_s != std::string::npos){
                    int m_e = param.find("&",m_s);
                    Serial.print("m: ");
                    if (m_e != std::string::npos){
                        for (int i = m_s; i < m_e; i++){
                            Serial.print(param[i]);
                        }
                    }else{
                        for (int i = m_s; i < param.size(); i++){
                            Serial.print(param[i]);
                        }
                    }
                    Serial.println();
                }
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
    //if (client){
        ethernet(client);
        client.stop();
    //}
    //t = mizu();
    //うごかす,trueorfalseでかえす?
}
