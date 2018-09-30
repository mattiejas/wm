//
// Created by matthias on 30-9-18.
//

#ifndef WM_CONFIGUREREQUESTEVENT_H
#define WM_CONFIGUREREQUESTEVENT_H

#include "Event.h"

class ConfigureRequestEvent : public Event {
public:
    XConfigureRequestEvent &target;

    explicit ConfigureRequestEvent(XConfigureRequestEvent &e_) : target{e_} {};

    void process(AbstractEventHandler &handler) override {
        handler.handle(*this);
    };
};

#endif //WM_CONFIGUREREQUESTEVENT_H
