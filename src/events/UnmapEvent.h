//
// Created by matthias on 30-9-18.
//

#ifndef WM_UNMAPEVENT_H
#define WM_UNMAPEVENT_H

#include "Event.h"

class UnmapEvent : public Event {
public:
    XUnmapEvent &target;

    explicit UnmapEvent(XUnmapEvent &e_) : target{e_} {};

    void process(AbstractEventHandler &handler) override {
        handler.handle(*this);
    };
};

#endif //WM_UNMAPREQUESTEVENT_H
