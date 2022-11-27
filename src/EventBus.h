#ifndef SnakeGame_h
#define SnakeGame_h

#include <Arduino.h>

class EventBase{
    public:
        virtual int getEventType();
};

class EventListener{
    public:
        virtual void receiveEvent(EventBase *event);
};

class EventBus{
    public:
        void addEventListener();
        void removeEventListener();
        void dispatchEvent();
};

#endif