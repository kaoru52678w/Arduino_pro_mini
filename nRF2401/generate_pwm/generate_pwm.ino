// Arduino Pro Mini 3.3V/8MHz版 向け 固定値PWM生成コード
// シリアル通信不要の最小構成

// PWM出力ピンの設定（タイマー1を使用）
const int PWM_PIN = 9;  // OC1Aピン(9番)を使用

// PWM設定の固定値
const unsigned long PWM_FREQUENCY = 1000;  // 周波数: 1000Hz
const float PWM_DUTY_CYCLE = 0.5;          // デューティ比: 50%

void setup() {
  // 出力ピンの設定
  pinMode(PWM_PIN, OUTPUT);
  
  // PWM設定の適用
  setupPWM(PWM_FREQUENCY, PWM_DUTY_CYCLE);
  
  // プログラム実行中はPWM波形が継続的に出力されます
}

void loop() {
  // PWMはタイマー/カウンタハードウェアによって自動的に生成されるため、
  // loop内では何も行う必要はありません
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