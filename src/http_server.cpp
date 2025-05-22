#include "http_server.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include "Expression.h"

const char *ssid = "elecom-a19d07";    // Wi-Fi SSID
const char *password = "c2prrkjhc8nk"; // Wi-Fi Password

AsyncWebServer server(80);

// 文字列からExpressionへの変換
m5avatar::Expression stringToExpression(const String &expressionStr)
{
    if (expressionStr.equalsIgnoreCase("default"))
        return m5avatar::Expression::Default;
    if (expressionStr.equalsIgnoreCase("cry"))
        return m5avatar::Expression::Cry;
    if (expressionStr.equalsIgnoreCase("sad"))
        return m5avatar::Expression::Sad;
    if (expressionStr.equalsIgnoreCase("happy"))
        return m5avatar::Expression::Happy;
    if (expressionStr.equalsIgnoreCase("angry"))
        return m5avatar::Expression::Angry;
    if (expressionStr.equalsIgnoreCase("gangimari"))
        return m5avatar::Expression::Gangimari;

    // デフォルトの表情
    return m5avatar::Expression::Default;
}

// ExpressionからStringへの変換
String expressionToString(m5avatar::Expression expression)
{
    switch (expression)
    {
    case m5avatar::Expression::Default:
        return "default";
    case m5avatar::Expression::Cry:
        return "cry";
    case m5avatar::Expression::Sad:
        return "sad";
    case m5avatar::Expression::Happy:
        return "happy";
    case m5avatar::Expression::Angry:
        return "angry";
    case m5avatar::Expression::Gangimari:
        return "gangimari";
    default:
        return "default";
    }
}

void start_http_server()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello from M5Stack HTTP Server!"); });

    // GET /expression - 現在の表情を取得
    server.on("/expression", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        DynamicJsonDocument doc(200);
        doc["currentExpression"] = expressionToString(avatar.getExpression());
        
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response); });

    // POST /expression - 表情を変更
    auto *handler = new AsyncCallbackJsonWebHandler("/expression", [](AsyncWebServerRequest *request, JsonVariant &json)
                                                    {
        // JSONデータを取得
        JsonObject jsonObj = json.as<JsonObject>();
        
        // expressionフィールドを取得
        if (jsonObj.containsKey("expression")) {
            String expressionStr = jsonObj["expression"].as<String>();
            m5avatar::Expression expression = stringToExpression(expressionStr);
            
            // 表情を設定
            avatar.setExpression(expression);
            
            // 成功レスポンスを送信
            DynamicJsonDocument responseDoc(200);
            responseDoc["code"] = 0;
            responseDoc["message"] = "Expression changed to: " + expressionStr;
            String response;
            serializeJson(responseDoc, response);
            request->send(200, "application/json", response);
            
            Serial.print("Expression changed to: ");
            Serial.println(expressionStr);
        } else {
            // expressionフィールドがない場合はエラー
            DynamicJsonDocument errorDoc(200);
            errorDoc["code"] = 400;
            errorDoc["message"] = "Expression field not found";
            String response;
            serializeJson(errorDoc, response);
            request->send(400, "application/json", response);
        } });
    server.addHandler(handler);

    server.begin();
}
