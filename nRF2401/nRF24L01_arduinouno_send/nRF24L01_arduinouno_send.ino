#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);                // CE,CSNピンの指定       
const byte address[6] = "00001";  // データを送信するアドレス

int SW_PIN = 3;
boolean SW_state = 0;

void setup() {
  pinMode(SW_PIN, INPUT_PULLUP);  // スイッチをプルアップで使用
  
  radio.begin();                  // 無線オブジェクトの初期化
  radio.openWritingPipe(address); // データ送信先のアドレスを指定
  radio.setPALevel(RF24_PA_MIN);  // 出力を最小に
  radio.stopListening();          // 送信側として設定
}

void loop() {
  SW_state = digitalRead(SW_PIN);  // スイッチの状態を読み取る
  radio.write(&SW_state, sizeof(SW_state));  // スイッチの状態を送信する
  
  delay(100);  // 100msごとに送信（応答性を高めるため短くしました）
}