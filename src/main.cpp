#include <Arduino.h>
#include <FreeRTOS.h>
#include <TFT_eSPI.h>
#include <OneButton.h>
#include "Snake.h"

#define BUTTON_1            35
#define BUTTON_2            0

#define WIDTH               135
#define HEIGHT              240

#define SCREEN_PROPORTION 10

TFT_eSPI tft = TFT_eSPI();

SnakeGame *snakeGame;

// int colors[23] = {TFT_NAVY, TFT_DARKGREEN, TFT_DARKCYAN, TFT_MAROON, TFT_PURPLE, TFT_OLIVE, TFT_LIGHTGREY, TFT_DARKGREY, TFT_BLUE, TFT_GREEN, TFT_CYAN, TFT_RED, TFT_MAGENTA, TFT_YELLOW, TFT_WHITE, TFT_ORANGE, TFT_GREENYELLOW, TFT_PINK, TFT_BROWN, TFT_GOLD, TFT_SILVER, TFT_SKYBLUE, TFT_VIOLET};

void drawGame(void *params){
  SnakeGame *snakeGame = (SnakeGame *) params;
  for(;;){
    SnakeBodyPart *tail = snakeGame->moveSnake();
    // Tail
    if (tail != NULL) {
      tft.fillRect(tail->point.x * SCREEN_PROPORTION,
        tail->point.y * SCREEN_PROPORTION,
        SCREEN_PROPORTION,
        SCREEN_PROPORTION,
        TFT_BLACK);
    }
    // Head
    tft.fillRect(snakeGame->snake->head->point.x * SCREEN_PROPORTION,
      snakeGame->snake->head->point.y * SCREEN_PROPORTION,
      SCREEN_PROPORTION - 1,
      SCREEN_PROPORTION - 1,
      TFT_PURPLE);

    // PILL
    tft.fillRect(snakeGame->pill->x * SCREEN_PROPORTION + 1,
      snakeGame->pill->y * SCREEN_PROPORTION + 1,
      SCREEN_PROPORTION - 2,
      SCREEN_PROPORTION - 2,
      TFT_ORANGE);
    
    vTaskDelay(snakeGame->speed / portTICK_PERIOD_MS);
  }
}

OneButton buttonRight(BUTTON_1, true);
OneButton buttonLeft(BUTTON_2, true);

static void btnLeftHandler() {
  snakeGame->moveAnticlockwise();
}

static void btnRightHandler() {
  snakeGame->moveClockwise();
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

  snakeGame = new SnakeGame(WIDTH/SCREEN_PROPORTION - 1, HEIGHT/SCREEN_PROPORTION - 1);

  xTaskCreatePinnedToCore(
    drawGame,
    "DrawGame",
    (5 * 1024),
    (void *) snakeGame,     // Parameter to pass
    1,                  // Task priority
    NULL,               // Task handle
    0                   // Core
  );
}

void loop() {}