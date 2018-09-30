//
// Created by matthias on 30-9-18.
//

#ifndef WM_DESTROYWINDOWEVENT_H
#define WM_DESTROYWINDOWEVENT_H

#include "Event.h"

class DestroyWindowEvent : public Event {
public:
    XDestroyWindowEvent &target;

    explicit DestroyWindowEvent(XDestroyWindowEvent &e_) : target{e_} {};

    void process(AbstractEventHandler &handler) override {
        handler.handle(*this);
    };
};

#endif //WM_DESTROYWINDOWEVENT_H
