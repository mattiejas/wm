//
// Created by matthias on 30-9-18.
//

#include "EventFactory.h"
#include "DestroyWindowEvent.h"
#include "CreateWindowEvent.h"

std::unique_ptr<Event> EventFactory::getEvent(XEvent &e) {
    switch (e.type) {
        case CreateNotify:
            return std::make_unique<CreateWindowEvent>(e.xcreatewindow);
        case DestroyNotify:
            return std::make_unique<DestroyWindowEvent>(e.xdestroywindow);
//        case ReparentNotify:
//            OnReparentNotify(e.xreparent);
//            break;
//        case MapNotify:
//            OnMapNotify(e.xmap);
//            break;
//        case UnmapNotify:
//            OnUnmapNotify(e.xunmap);
//            break;
//        case ConfigureNotify:
//            OnConfigureNotify(e.xconfigure);
//            break;
//        case MapRequest:
//            OnMapRequest(e.xmaprequest);
//            break;
//        case ConfigureRequest:
//            OnConfigureRequest(e.xconfigurerequest);
//            break;
//        case ButtonPress:
//            OnButtonPress(e.xbutton);
//            break;
//        case ButtonRelease:
//            OnButtonRelease(e.xbutton);
//            break;
//        case MotionNotify:
//            // Skip any already pending motion events.
//            while (XCheckTypedWindowEvent(
//                    display_, e.xmotion.window, MotionNotify, &e)) {}
//            OnMotionNotify(e.xmotion);
//            break;
//        case KeyPress:
//            OnKeyPress(e.xkey);
//            break;
//        case KeyRelease:
//            OnKeyRelease(e.xkey);
//            break;
        default:
            return nullptr;
    }
}
