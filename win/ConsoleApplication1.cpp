#include <iostream>
#include <Windows.h>
#include <vector>
#include "keynum.h"

unsigned char changeKeyMap(unsigned char);

int main() {
    // シリアルポートの設定
    //HANDLE hSerial = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hSerial = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port." << std::endl;
        return 1;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }
    std::vector<int> pressedKeys;

    while (true)
    {
        for (int key = 0; key < 256; ++key)
        {
            if (key >= 0x00 && key <= 0x07) continue;
            if (key >= 0x0a && key <= 0x0b) continue;
            if (key >= 0x0e && key <= 0x0f) continue;
            if (key >= 0x3a && key <= 0x40) continue;
            if (key == 0x5e) continue;
            if (key >= 0x88 && key <= 0x8f) continue;
            if (key >= 0x92 && key <= 0x9f) continue;
            if (key >= 0xa0 && key <= 0xa5) continue;
            if (key >= 0xb8 && key <= 0xb9) continue;
            if (key >= 0xc1 && key <= 0xda) continue;
            if (key >= 0xe0 && key <= 0xe1) continue;
            if (key >= 0xe3 && key <= 0xe4) continue;
            if (key == 0xe6) continue;
            if (key >= 0xe8 && key <= 0xf5) continue;
            if (key == 0xff) continue;
            if (GetAsyncKeyState(key) & 0x8000)
            {
                // Key is pressed
                if (std::find(pressedKeys.begin(), pressedKeys.end(), key) == pressedKeys.end())
                {
                    pressedKeys.push_back(key);
                    key = changeKeyMap(key);
                    printf("%02Xp\n", key);
                    // シリアルポートに送信
                    DWORD bytesWritten;
                    char buffer[256];
                    sprintf_s(buffer, sizeof(buffer), "%02Xp", key);
                    WriteFile(hSerial, buffer, strlen(buffer), &bytesWritten, NULL);
                }
            }
            else
            {
                // Key is released
                auto it = std::find(pressedKeys.begin(), pressedKeys.end(), key);
                if (it != pressedKeys.end())
                {
                    pressedKeys.erase(it);
                    key = changeKeyMap(key);
                    printf("%02Xr\n", key);
                    // シリアルポートに送信
                    DWORD bytesWritten;
                    char buffer[256];
                    sprintf_s(buffer, sizeof(buffer), "%02Xr", key);
                    WriteFile(hSerial, buffer, strlen(buffer), &bytesWritten, NULL);
                }
            }
        }
    }
}

unsigned char changeKeyMap(unsigned char key) {
    if (key >= 0x41 && key <= 0x5a) {
        key = key + 0x20;
        return key;
    }
    switch (key) {
    case VK_NONCONVERT:
        return 0x5b;
    case VK_PROCESSKEY:
        return 0x5b;
    case VK_NUMPAD0:
        return KEY_NUM_0;
    case VK_NUMPAD1:
        return KEY_NUM_1;
    case VK_NUMPAD2:
        return KEY_NUM_2;
    case VK_NUMPAD3:
        return KEY_NUM_3;
    case VK_NUMPAD4:
        return KEY_NUM_4;
    case VK_NUMPAD5:
        return KEY_NUM_5;
    case VK_NUMPAD6:
        return KEY_NUM_6;
    case VK_NUMPAD7:
        return KEY_NUM_7;
    case VK_NUMPAD8:
        return KEY_NUM_8;
    case VK_NUMPAD9:
        return KEY_NUM_9;
    case VK_SUBTRACT:
        return KEY_NUM_MINUS;
    case VK_DIVIDE:
        return KEY_NUM_SLASH;
    case VK_ADD:
        return KEY_NUM_PLUS;
    case VK_MULTIPLY:
        return KEY_NUM_ASTERISK;
    case VK_LEFT:
        return KEY_LEFT_ARROW;
    case VK_UP:
        return KEY_UP_ARROW;
    case VK_RIGHT:
        return KEY_RIGHT_ARROW;
    case VK_DOWN:
        return KEY_DOWN_ARROW;
    case VK_BACK:
        return KEY_BACKSPACE;
    default:
        return key;
    }
}
