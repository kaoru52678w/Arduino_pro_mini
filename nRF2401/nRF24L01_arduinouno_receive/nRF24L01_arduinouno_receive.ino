// LED点灯(受信側)

#include <SPI.h>                  // ライブラリのインクルード
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);                // CE,CSNピンの指定
const byte address[6] = "00001";  // データを受信するアドレス

int LED_PIN = 3;
boolean SW_state = 0;


void setup() {
  
  pinMode(3, OUTPUT);
  
  radio.begin();                      // 無線オブジェクトの初期化 
  radio.openReadingPipe(0, address);  // データ受信アドレスを指定
  radio.setPALevel(RF24_PA_MIN);      // 出力を最小に
  radio.startListening();             // 受信側として設定
  
}


void loop(){
  
  if (radio.available()){
    radio.read(&SW_state, sizeof(SW_state));  // スイッチの状態を受信する
    
    if(SW_state == HIGH){
      digitalWrite(3, LOW);
    } else {
      digitalWrite(3, HIGH);
    }
  }

delay(10);

}