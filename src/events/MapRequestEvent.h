//
// Created by matthias on 30-9-18.
//

#ifndef WM_MAPREQUESTEVENT_H
#define WM_MAPREQUESTEVENT_H

#include "Event.h"

class MapRequestEvent : public Event {
public:
    XMapRequestEvent &target;

    explicit MapRequestEvent(XMapRequestEvent &e_) : target{e_} {};

    void process(AbstractEventHandler &handler) override {
        handler.handle(*this);
    };
};

#endif //WM_MAPREQUESTEVENT_H
