//
// Created by matthias on 30-9-18.
//

#ifndef WM_ABSTRACTEVENTHANDLER_H
#define WM_ABSTRACTEVENTHANDLER_H

#include <X11/Xlib.h>

class CreateWindowEvent;
class DestroyWindowEvent;
class ConfigureRequestEvent;
class MapRequestEvent;
class UnmapEvent;

class AbstractEventHandler {
private:
public:
    AbstractEventHandler() = default;

    virtual void handle(CreateWindowEvent &event) = 0;
    virtual void handle(DestroyWindowEvent &event) = 0;
    virtual void handle(ConfigureRequestEvent &event) = 0;
    virtual void handle(MapRequestEvent &event) = 0;
    virtual void handle(UnmapEvent &event) = 0;
};

#endif //WM_ABSTRACTEVENTHANDLER_H
