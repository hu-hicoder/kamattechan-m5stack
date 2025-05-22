#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Avatar.h>

// 外部からavatarを参照できるようにする
extern m5avatar::Avatar avatar;

void start_http_server();

#endif // HTTP_SERVER_H
