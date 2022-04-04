#include <Arduino.h>
#include <FreeRTOS.h>
#include <TFT_eSPI.h>
#include <OneButton.h>
#include "Snake.h"

#define BUTTON_1            35
#define BUTTON_2            0

#define WIDTH               135
#define HEIGHT              240

TFT_eSPI tft = TFT_eSPI();

SnakeBoard *snakeBoard;

// int colors[23] = {TFT_NAVY, TFT_DARKGREEN, TFT_DARKCYAN, TFT_MAROON, TFT_PURPLE, TFT_OLIVE, TFT_LIGHTGREY, TFT_DARKGREY, TFT_BLUE, TFT_GREEN, TFT_CYAN, TFT_RED, TFT_MAGENTA, TFT_YELLOW, TFT_WHITE, TFT_ORANGE, TFT_GREENYELLOW, TFT_PINK, TFT_BROWN, TFT_GOLD, TFT_SILVER, TFT_SKYBLUE, TFT_VIOLET};

void drawBoard(void *params){
  SnakeBoard *snakeBoard = (SnakeBoard *) params;
  snakeBoard->snake->increase();
  snakeBoard->snake->increase();
  for(;;){
    SnakeBodyPart *tail = snakeBoard->moveSnake();
    tft.fillRect(snakeBoard->snake->head->point.x * 6 + 1, snakeBoard->snake->head->point.y * 6 + 1, 4, 4, TFT_PURPLE);
    tft.fillRect(tail->point.x * 6, tail->point.y * 6, 6, 6, TFT_BLACK);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

OneButton buttonRight(BUTTON_1, true);
OneButton buttonLeft(BUTTON_2, true);

static void btnLeftHandler() {
  snakeBoard->moveAnticlockwise();
}

static void btnRightHandler() {
  snakeBoard->moveClockwise();
}

void handleBtn(void * parameter){
  for(;;){
    buttonLeft.tick();
    buttonRight.tick();
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}


void setup() {
  Serial.begin(115200);
  
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  buttonLeft.attachClick(btnLeftHandler);
  buttonRight.attachClick(btnRightHandler);
  xTaskCreatePinnedToCore(
    handleBtn,     // Function that should be called
    "Handle Btn",   // Name of the task (for debugging)
    (5 * 1024),     // Stack size (bytes)
    NULL,           // Parameter to pass
    1,              // Task priority
    NULL,           // Task handle
    1               // Core
  );

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  snakeBoard = new SnakeBoard(WIDTH/6, HEIGHT/6);

  xTaskCreatePinnedToCore(
    drawBoard,
    "DrawBoard",
    (5 * 1024),
    (void *) snakeBoard,     // Parameter to pass
    1,                  // Task priority
    NULL,               // Task handle
    0                   // Core
  );
}

void loop() {}