//
// Created by matthias on 30-9-18.
//

#include <glog/logging.h>

#include "EventHandler.h"
#include "ConfigureRequestEvent.h"
#include "MapRequestEvent.h"
#include "UnmapEvent.h"

void EventHandler::handle(CreateWindowEvent &event) {
    // do nothing
}

void EventHandler::handle(DestroyWindowEvent &event) {

}

void EventHandler::handle(ConfigureRequestEvent &event) {
    XWindowChanges changes;
    changes.x = event.target.x;
    changes.y = event.target.y;
    changes.width = event.target.width;
    changes.height = event.target.height;
    changes.border_width = event.target.border_width;
    changes.sibling = event.target.above;
    changes.stack_mode = event.target.detail;

    auto w = event.target.window;

    if (wm._clients.count(event.target.window)) {
        w = wm._clients[event.target.window];
    }

    XConfigureWindow(wm._display, w, (unsigned int) event.target.value_mask, &changes);
    LOG(INFO) << "Resize " << w << " to " << event.target.width << "x" << event.target.height;
}

void EventHandler::handle(MapRequestEvent &event) {
    wm.Frame(event.target.window);
}

void EventHandler::handle(UnmapEvent &event) {
    if (event.target.event == wm._root) {
        LOG(INFO) << "Ignore UnmapNotify for reparented pre-existing window " << event.target.window;
        return;
    } else if (!wm._clients.count(event.target.window)) {
        LOG(INFO) << "Ignore UnmapNotify for non-client window " << event.target.window;
        return;
    }

    wm.Unframe(event.target.window);
}
