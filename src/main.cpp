#include <M5Unified.h>
#include <M5GFX.h>
#include <Avatar.h>
#include "http_server.h" // 新しいHTTPサーバーヘッダーをインクルード

using namespace m5avatar;
Avatar avatar;

// ダミーの音量レベル取得関数（M5Stackのマイク入力に置き換える予定）
float getVolumeLevel()
{
    // 0.0から1.0の範囲でランダムな値を返すダミー実装
    return (float)random(0, 100) / 100.0f;
}

void setup()
{
    Serial.begin(115200);   /// シリアル通信を初期化（ボーレート115200）
    auto cfg = M5.config(); /// 設定用の構造体を取得。
    M5.begin(cfg);          /// M5Unifiedを使用する準備をする。

    // カラーセット変更用
    ColorPalette *cp;
    cp = new ColorPalette();
    cp->set(COLOR_PRIMARY, TFT_WHITE);
    cp->set(COLOR_BACKGROUND, TFT_BLACK);
    avatar.setColorPalette(*cp);

    avatar.init(8);

    // HTTPサーバーを起動
    start_http_server();
    M5.Lcd.println("HTTP server started on port 80");
}

void loop()
{
    M5.update();
    float volumeLevel = getVolumeLevel();
    if (volumeLevel >= 0)
    {
        avatar.setMouthOpenRatio(volumeLevel);
    }
    delay(100);
}
