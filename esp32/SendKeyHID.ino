/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;
char keyVK_SHIFT;
char keyVK_CTRL;
char keyVK_ALT;
char keyVK_WIN;
char keybuffer;
void keyBuf(char);
void keyFire(void);
void keyEnd(void);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  keyEnd();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    if (Serial.available() >= 3) {
      // シリアルから3バイト読み込む
      char receivedByte1 = Serial.read();
      char receivedByte2 = Serial.read();
      char receivedByte3 = Serial.read();
    
      // HEXコードを文字コードに変換
      char convertedChar = hexToChar(receivedByte1, receivedByte2);
      if(receivedByte3=='p'){
        keyBuf(convertedChar);
      }else{
        keyEnd();
      }
    }
  }
}

void keyBuf(char key){
  switch(key){
    case 0x10:
      keyVK_SHIFT=key;
      return;
    case 0x11:
      keyVK_CTRL=key;
      return;
    case 0x12:
      keyVK_ALT=key;
      return;
    case 0x5b:
      keyVK_WIN=key;
      return;
    default:
      keybuffer=key;
      keyFire();
  }
}

void keyFire(void){
    // bleKeyboard.press(KEY_LEFT_CTRL);
    // bleKeyboard.press(KEY_LEFT_ALT);
    // bleKeyboard.press(KEY_DELETE);
    // delay(100);
  if (keyVK_SHIFT!=0){
    bleKeyboard.press(KEY_LEFT_SHIFT);
  }
  if(keyVK_CTRL!=0){
    bleKeyboard.press(KEY_LEFT_CTRL);
  }
  if(keyVK_ALT!=0){
    bleKeyboard.press(KEY_LEFT_ALT);
  }
  if(keyVK_WIN!=0){
    bleKeyboard.press(KEY_LEFT_GUI);
  }
  bleKeyboard.press(keybuffer);
}

void keyEnd(void){
  bleKeyboard.releaseAll();
  keyVK_SHIFT=0;
  keyVK_CTRL=0;
  keyVK_ALT=0;
  keyVK_WIN=0;
  keybuffer=0;
}

// HEXコードを文字コードに変換する関数
char hexToChar(char hexByte1, char hexByte2) {
  // 16進数の文字を数値に変換
  int value = 0;
  if (isHexDigit(hexByte1) && isHexDigit(hexByte2)) {
    value = (hexDigitToValue(hexByte1) << 4) | hexDigitToValue(hexByte2);
  }
  return char(value);
}

// 16進数の文字を数値に変換するヘルパー関数
int hexDigitToValue(char hexDigit) {
  if (hexDigit >= '0' && hexDigit <= '9') {
    return hexDigit - '0';
  } else if (hexDigit >= 'A' && hexDigit <= 'F') {
    return hexDigit - 'A' + 10;
  } else if (hexDigit >= 'a' && hexDigit <= 'f') {
    return hexDigit - 'a' + 10;
  }
  return 0; // エラー時は0を返す
}

// 16進数の文字かどうかを判定するヘルパー関数
bool isHexDigit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}
