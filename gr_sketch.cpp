#include <rxduino.h>
#include <ethernet.h>
#include <string>
#include "mizu.hpp"
#include "html.hpp"
#include <vector>

byte mac[] = {0x00,0x16,0x3e,0x4e,0x8a,0x6c};
byte ip[] = {192,168,220,133};
TEthernet Ethernet;
EthernetServer server(80);
std::string m;
std::string u;


void setup(){
    Ethernet.begin(mac,ip);
    //これつけると落ちなくなった。）
    while (Ethernet.localIP() == "0.0.0.0"){
        Ethernet.begin(mac,ip);
    }
    server.begin();
    Serial.begin(9600);
}

void para(std::string& param){
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
    // uの分析
    int u_s = param.find("u=");
    if (u_s != std::string::npos){
        int u_e = param.find("&",u_s);
        Serial.print("u: ");
        //最後に&があるかどうかで処理が別れる
        if (u_e != std::string::npos){
            u = param.substr(u_s+2,u_e - u_s-2);
            //printしているだけ
            for (int i = 0; i < u.size(); i++){
                Serial.print(u[i]);
            }
        }else{
            //&がなかった場合
            u = param.substr(u_s+2);
            for (int i = 0; i < u.size(); i++){
                Serial.print(u[i]);
            }
        }
        Serial.println();
    }
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
                //paramがあれば…
                para(param);
            }
            if (line.size() == 0){
                isend = false;
            }
        }
        if (m == ""){
        returnHTML(client,"とりあえずは動くだけplease click button...");
        }
    }else{
    }
}


void loop(){
    m = "";
    u = "";
    EthernetClient client = server.available();
    ethernet(client);
    int p = -1;
    //uの数字化
    if (u != ""){
        int a;
        a = atoi(u.c_str());
        p = a*1000;
    }
    if (m == "GO"){
        setuph();
        //初期数値の設定
        if(p == -1){
            p = 3000;
        }
        for (int l=0; l<p/10; l++ ){
            set();
        }
        offu();
        offr();
        returnHTML(client,"たぶん水をまきました");
    }else if(m == "DOWN"){
        setuph();
        offu();
        delay(50);
        
        down();
        //初期数値の設定
        if(p == -1){
            p = 1000;
        }
        delay(p);
        offu();
        returnHTML(client,"俯角を下げました");
    }else if(m == "UP"){
        setuph();
        offu();
        delay(50);
        
        up();
        //初期数値の設定
        if(p == -1){
            p = 1000;
        }
        delay(p);
        offu();
        returnHTML(client,"俯角をageました");
    }else if(m == "STOP"){
        offu();
        offr();
        returnHTML(client,"とめました");
    }
    client.stop();
}
