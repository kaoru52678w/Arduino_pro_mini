// LED点灯(受信側) - PWM出力対応版

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);                // CE,CSNピンの指定
const byte address[6] = "00001";  // データを受信するアドレス

// PWM出力ピンの設定
const int PWM_PIN = 9;  // タイマー1のOC1A（ピン9）を使用
                        // ※元コードのLED_PIN=3はタイマー2を使用

// PWM設定の固定値
const unsigned long PWM_FREQUENCY = 1000;  // 周波数: 1000Hz
const float PWM_DUTY_CYCLE = 0.5;          // デューティ比: 50%

boolean SW_state = 0;  // 受信したスイッチの状態
boolean pwmEnabled = false;  // PWM出力の有効/無効状態

void setup() {
  // シリアル通信の初期化（デバッグ用）
  Serial.begin(9600);
  
  // PWM出力ピンの設定
  pinMode(PWM_PIN, OUTPUT);
  
  // 初期状態ではPWM出力を無効に
  digitalWrite(PWM_PIN, LOW);
  
  // 無線通信の初期化
  radio.begin();                      // 無線オブジェクトの初期化 
  radio.openReadingPipe(0, address);  // データ受信アドレスを指定
  radio.setPALevel(RF24_PA_MIN);      // 出力を最小に
  radio.startListening();             // 受信側として設定
  
  Serial.println("PWM対応 nRF24L01 受信機の準備完了");
}

void loop() {
  if (radio.available()) {
    radio.read(&SW_state, sizeof(SW_state));  // スイッチの状態を受信する
    
    // 受信データをシリアルに出力（デバッグ用）
    Serial.print("受信: スイッチ状態=");
    Serial.println(SW_state ? "HIGH" : "LOW");
    
    if (SW_state == HIGH) {
      // 受信データがHIGHの場合、PWM出力を無効化
      if (pwmEnabled) {
        disablePWM();
        Serial.println("PWM出力: 無効");
      }
    } else {
      // 受信データがLOWの場合、PWM出力を有効化
      if (!pwmEnabled) {
        enablePWM(PWM_FREQUENCY, PWM_DUTY_CYCLE);
        Serial.println("PWM出力: 有効");
      }
    }
  }
  
  delay(10);  // メインループのディレイ
}

// PWM出力を有効化する関数
void enablePWM(unsigned long freq, float duty) {
  // PWM設定の適用
  setupPWM(freq, duty);
  pwmEnabled = true;
}

// PWM出力を無効化する関数
void disablePWM() {
  // タイマーを停止してPWM出力を無効化
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
  digitalWrite(PWM_PIN, LOW);
  pwmEnabled = false;
}

// PWM設定の適用関数
void setupPWM(unsigned long freq, float duty) {
  // レジスタをリセット
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  // PWMモード設定：高速PWM、TOP値=ICR1
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  
  // Arduino Pro Mini 3.3V版は8MHzクロック
  // 8分周を使用して適切な周波数範囲を実現
  TCCR1B |= (1 << CS11);
  
  // TOP値計算: 8MHz ÷ 8分周 ÷ 周波数 - 1
  // 例: 8000000 / 8 / 1000 - 1 = 999
  uint16_t top = (8000000UL / 8 / freq) - 1;
  ICR1 = top;
  
  // デューティ比設定: TOP値 × デューティ比
  OCR1A = (uint16_t)(top * duty);
  
  // 出力モード設定：非反転PWM
  TCCR1A |= (1 << COM1A1);
}