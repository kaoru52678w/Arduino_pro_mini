// Arduino Pro Mini の簡単な Blink テストコード

// LED_BUILTIN は通常 Arduino Pro Mini では13番ピンに接続されています
const int ledPin = 13;

void setup() {
  // デジタルピンを出力として初期化
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);   // LEDをオンにする（HIGHは電圧レベル）
  delay(1000);                  // 1秒待つ
  digitalWrite(ledPin, LOW);    // LEDをオフにする（LOWは電圧レベル）
  delay(1000);                  // 1秒待つ
}