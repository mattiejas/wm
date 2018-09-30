//
// Created by matthias on 30-9-18.
//

#ifndef WM_EVENT_H
#define WM_EVENT_H

#include "AbstractEventHandler.h"

class Event {
private:
public:
    Event() = default;
    ~Event() = default;

    virtual void process(AbstractEventHandler &handler) = 0;
};

#endif //WM_EVENT_H
