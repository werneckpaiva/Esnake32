#ifndef SnakeBoard_h
#define SnakeBoard_h

#include <Arduino.h>

class Point{
    public:
        int x;
        int y;
};

// SnakeBodyPart -----------------------------

class SnakeBodyPart {
    public:
        Point point;
        SnakeBodyPart *next;

        SnakeBodyPart(int x, int y){
            this->point.x = x;
            this->point.y = y;
            this->next = NULL;
        };
};


// Snake -----------------------------

class Snake{
    public:
        int vertDir = 0;  // -1, 0, 1
        int horizDir = 1; // -1, 0, 1

        SnakeBodyPart *head;
        SnakeBodyPart *tail;

        Snake();
        void increase();
        SnakeBodyPart* removeTail();
};

Snake::Snake(){
    SnakeBodyPart *body = new SnakeBodyPart(0, 0);
    head = tail = body;
};

void Snake::increase(){
    int newX = this->head->point.x + this->horizDir;
    int newY = this->head->point.y + this->vertDir;
    SnakeBodyPart *newPart = new SnakeBodyPart(newX, newY);
    this->head->next = newPart;
    this->head = newPart;
};

SnakeBodyPart* Snake::removeTail(){
    if (this->head == this->tail){
        return NULL;
    }
    SnakeBodyPart *toRemove = this->tail;
    this->tail = this->tail->next;
    return toRemove;
};

// SnakeBoard -----------------------------

class SnakeBoard {
    private:
        int snakeDirection = 0; // (0 East, 1 North, 2 West, 3 South)
        unsigned int width;
        unsigned int height;
        void changeSnakeFromDirection(int direction);
    public:
        SnakeBoard(unsigned int width, unsigned int height);
        Snake *snake;
        SnakeBodyPart* moveSnake();
        void moveClockwise();
        void moveAnticlockwise();
};

SnakeBoard::SnakeBoard(unsigned int width, unsigned int height){
    this->width = width;
    this->height = height;
    this->snake = new Snake();
}

SnakeBodyPart* SnakeBoard::moveSnake(){
    this->snake->increase();
    if (this->snake->head->point.x < 0) {
        this->snake->head->point.x = this->width;
    } else if (this->snake->head->point.x > this->width) {
        this->snake->head->point.x = 0;
    } else if (this->snake->head->point.y < 0) {
        this->snake->head->point.y = this->height;
    } else if (this->snake->head->point.y > this->height) {
        this->snake->head->point.y = 0;
    } 
    return this->snake->removeTail();
};

void SnakeBoard::moveClockwise(){
    this->snakeDirection = (this->snakeDirection + 1) % 4;
    this->changeSnakeFromDirection(this->snakeDirection);
}
void SnakeBoard::moveAnticlockwise(){
    this->snakeDirection = (this->snakeDirection + 4 - 1) % 4;
    this->changeSnakeFromDirection(this->snakeDirection);
}

void SnakeBoard::changeSnakeFromDirection(int direction){
    switch(direction){
        case 0:
            this->snake->horizDir = 1;
            this->snake->vertDir  = 0;
            break;
        case 1:
            this->snake->horizDir = 0;
            this->snake->vertDir  = 1;
            break;
        case 2:
            this->snake->horizDir = -1;
            this->snake->vertDir  = 0;
            break;
        case 3:
            this->snake->horizDir = 0;
            this->snake->vertDir  = -1;
            break;
    }
}

#endif