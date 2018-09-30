//
// Created by matthias on 30-9-18.
//

#ifndef WM_EVENTHANDLER_H
#define WM_EVENTHANDLER_H

#include "AbstractEventHandler.h"
#include "../WindowManager.h"

class EventHandler : public AbstractEventHandler {
private:
    WindowManager &wm;
public:
    explicit EventHandler(WindowManager &wm_) : wm{wm_} {};

    void handle(CreateWindowEvent &event) override;

    void handle(DestroyWindowEvent &event) override;

    void handle(ConfigureRequestEvent &event) override;

    void handle(MapRequestEvent &event) override;

    void handle(UnmapEvent &event) override;
};

#endif //WM_EVENTHANDLER_H
