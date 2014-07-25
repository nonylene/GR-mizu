#include <rxduino.h>
#include <ethernet.h>
#include <string>
#include "mizu.hpp"
#include <vector>

byte mac[] = {0x00,0x16,0x3e,0x4e,0x8a,0x6c};
byte ip[] = {192,168,220,133};
TEthernet Ethernet;
EthernetServer server(80);

void setup(){
    pinMode(8,OUTPUT);
    digitalWrite(8, HIGH);
    //output Vcc
    pinMode(0,OUTPUT);
    pinMode(1,OUTPUT);
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
        if (!client.connected()){
            Serial.println("client not connected");
            return;
        }
        bool isend = true;
        while (isend){
            //読み取り
            std::vector<char> line;
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
            //必ずこの後\nがくるので読み飛ばし
            Ethernet.processPackets();
            while (!client.available()){
                delay(1);
                Ethernet.processPackets();
            }
            client.read();
            //ここで読み取りが終わり。→表示
            for (int i = 0; i < line.size();i++){
                Serial.print(line[i]);
            }
            Serial.println();

            //検索とかはstringで行う
            std::string lines(line.begin(),line.end());

            //paramの検索
            std::string param;
            int a = lines.find("?");
            int b = lines.find("HTTP");
            if (a != std::string::npos && b != std::string::npos){
                param = lines.substr(a+1,b-a-2);
            }

            if (param != ""){
                //URLにparamがあった時の処理
                Serial.print("params: ");
                for (int i = 0; i < param.size();i++){
                    Serial.print(param[i]);
                }
                Serial.println();

                //paramを分析する
                // mの分析
                int m_s = param.find("m=");
                if (m_s != std::string::npos){
                    int m_e = param.find("&",m_s);
                    Serial.print("m: ");
                    //最後に&があるかどうかで処理が別れる
                    if (m_e != std::string::npos){
                        m = param.substr(m_s+2,m_e - m_s-2);
                        //printしているだけ
                        for (int i = 0; i < m.size(); i++){
                            Serial.print(m[i]);
                        }
                    }else{
                        //&がなかった場合
                        m = param.substr(m_s+2);
                        for (int i = 0; i < m.size(); i++){
                            Serial.print(m[i]);
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
    }else{
    }
}



void loop(){
    std::string m;
    EthernetClient client = server.available();
    ethernet(client);
    if (m == "GO"){
        mizu::set()
    }
    client.stop();
}
