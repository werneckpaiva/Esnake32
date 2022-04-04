#ifndef SnakeGame_h
#define SnakeGame_h

#include <Arduino.h>

class Point{
    public:
        int x;
        int y;
        Point(){}
        Point(int x, int y){
            this->x = x;
            this->y = y;
        }
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

        Snake(int x, int y);
        void increase();
        SnakeBodyPart* removeTail();
};

Snake::Snake(int x, int y){
    SnakeBodyPart *body = new SnakeBodyPart(x, y);
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

// SnakeGame -----------------------------

class SnakeGame {
    private:
        int snakeDirection = 0; // (0 East, 1 North, 2 West, 3 South)
        unsigned int width;
        unsigned int height;
        int gameState;
        void changeSnakeFromDirection(int direction);
    public:
        enum SnakeGameState {
            PLAYING,             // 0
            STOPPED,             // 1
        };
        Snake *snake;
        Point *pill = NULL;
        int speed = 200;
        SnakeGame(unsigned int width, unsigned int height);
        SnakeBodyPart* moveSnake();
        void moveClockwise();
        void moveAnticlockwise();
        void generateNewPill();
};

SnakeGame::SnakeGame(unsigned int width, unsigned int height){
    this->width = width;
    this->height = height;
    this->gameState = SnakeGameState::PLAYING;
    this->snake = new Snake(width/2, height/2);
    this->snake->increase();
    this->snake->increase();
    this->generateNewPill();
}

SnakeBodyPart* SnakeGame::moveSnake(){
    if (this->gameState != SnakeGameState::PLAYING) return NULL;
    this->snake->increase();
    if (this->pill == NULL){
        this->generateNewPill();
    }
    SnakeBodyPart *head = this->snake->head;

    // Is hitting the body?
    SnakeBodyPart *body = this->snake->tail;
    while (body != NULL && body != head){
        if (head->point.x == body->point.x
            && head->point.y == body->point.y){
            gameState = SnakeGameState::STOPPED;
        }
        body = body->next;
    }

    // Check boundaries
    if (head->point.x < 0) {
        head->point.x = this->width;
    } else if (head->point.x > this->width) {
        head->point.x = 0;
    } else if (head->point.y < 0) {
        head->point.y = this->height;
    } else if (head->point.y > this->height) {
        head->point.y = 0;
    }

    // Did hit a pill? 
    if (head->point.x==this->pill->x 
        && head->point.y == this->pill->y){
        this->generateNewPill();
        return NULL;
    }

    return this->snake->removeTail();
};

void SnakeGame::moveClockwise(){
    this->snakeDirection = (this->snakeDirection + 1) % 4;
    this->changeSnakeFromDirection(this->snakeDirection);
}
void SnakeGame::moveAnticlockwise(){
    this->snakeDirection = (this->snakeDirection + 4 - 1) % 4;
    this->changeSnakeFromDirection(this->snakeDirection);
}

void SnakeGame::changeSnakeFromDirection(int direction){
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

void SnakeGame::generateNewPill(){
    if (this->pill != NULL){
        delete(this->pill);
        this->pill = NULL;
    }
    // Avoid generate a pill on the body
    while(this->pill == NULL){
        Point *newPill = new Point(random(this->width), random(this->height));
        SnakeBodyPart *body = this->snake->tail;
        while (body != NULL){
            if (newPill->x == body->point.x
                && newPill->y == body->point.y){
                break;
            }
            body = body->next;
        }
        if (body == NULL){
            this->pill = newPill;
        } else {
            delete(newPill);
        }
    }

}

#endif