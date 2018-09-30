//
// Created by matthias on 30-9-18.
//

#ifndef WM_CREATEWINDOWEVENT_H
#define WM_CREATEWINDOWEVENT_H

#include "Event.h"

class CreateWindowEvent : public Event {
public:
    XCreateWindowEvent &target;

    explicit CreateWindowEvent(XCreateWindowEvent &e_) : target{e_} {};

    void process(AbstractEventHandler &handler) override {
        handler.handle(*this);
    };
};

#endif //WM_CREATEWINDOWEVENT_H
