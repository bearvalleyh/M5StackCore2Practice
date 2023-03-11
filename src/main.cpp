#include <WiFi.h>
#include <Arduino.h>
#include <M5Core2.h>

const char* ssid = "SHARELOUNGE-Wi-Fi"; // SSID
const char* password = "tsutaya_sharelounge"; // PASSWORD

WiFiServer server(80);

void setup() {
  M5.begin();  //Initialize M5Core2
  int X = M5.Lcd.getCursorX();
  int Y = M5.Lcd.getCursorY();
  M5.Lcd.loadFont("filename", SD);
  M5.Lcd.println("");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10,20);
  M5.Lcd.println("Loading...");
  M5.Lcd.progressBar(10, 100, 240, 20, 0);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 20);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 40);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 60);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 80);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 100);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  //M5.Lcd.fillScreen(M5.Lcd.alphaBlend(128, 0X00FF00, 0XFF0000));
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.unloadFont();
      M5.Lcd.println("Connecting");

    // wifi接続開始
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Lcd.print(".");
    }

    // 接続完了したらIP表示
    M5.Lcd.println("Successed");
    M5.Lcd.println("IP: ");
    M5.Lcd.println(WiFi.localIP());
  
    server.begin();
}

void loop(){
  
  


     //M5.Lcd.println(". ");
    WiFiClient client = server.available();  // クライアントのアクセスを待つ
        if (client) {                        // クライアントがサクセスしてきたら
        M5.Lcd.println("Connected.");
        // クライアントからの受信データを保持する文字列
        String currentLine = "";
        // クライアントが接続している間ループ
        while (client.connected()) {
            if (client.available()) {  
                char c = client.read(); 
                // もし改行コードが送られてきたら
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        // HTTPヘッダーの応答コードとコンテンツタイプ
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        // 色を変更するためにリンクを表示
                        client.print("<a href=\"/R\">Red</a><br>");
                        client.print("<a href=\"/B\">here</a><br>");

                        // HTTP応答は、別の空白行で終了しループを抜ける
                        client.println();
                        break;
                    } else {            // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                // CR文字でなければcurrentLineに文字を追加
                } else if (c != '\r') {
                    currentLine += c;
                }

                // currentLineが「GET /R」で後方一致すれば赤色背景
                if (currentLine.endsWith("GET /R")) {
                    M5.Lcd.fillScreen(RED);
                }

                // currentLineが「GET /B」で後方一致すれば青色背景
                if (currentLine.endsWith("GET /B")) {
                    M5.Lcd.fillScreen(BLUE);
                }
            }
        }
        // 接続解除
        client.stop();
        M5.Lcd.println("Disconnected.");
    }
    M5.update();
}





