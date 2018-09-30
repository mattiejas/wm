//
// Created by matthias on 30-9-18.
//

#ifndef WM_EVENTFACTORY_H
#define WM_EVENTFACTORY_H


#include <memory>
#include <X11/Xlib.h>
#include "Event.h"

class EventFactory {
public:
    static std::unique_ptr<Event> getEvent(XEvent &e);
};


#endif //WM_EVENTFACTORY_H
