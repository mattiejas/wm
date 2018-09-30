//
// Created by matthias on 30-9-18.
//

#include <glog/logging.h>
#include <X11/Xutil.h>

#include "WindowManager.h"
#include "events/Event.h"
#include "events/EventFactory.h"
#include "events/EventHandler.h"

std::unique_ptr<WindowManager> WindowManager::Create() {
    Display *display = XOpenDisplay(nullptr);

    if (display == nullptr) {
        LOG(ERROR) << "Failed to open X display \'" << XDisplayName(nullptr) << "\'";
        return nullptr;
    }

    return std::unique_ptr<WindowManager>(new WindowManager(display));
}

WindowManager::WindowManager(Display *display) {
    if (display) {
        display = display;
        _root = DefaultRootWindow(display);
    }
}

WindowManager::~WindowManager() {
    XCloseDisplay(_display);
}

void WindowManager::Run() {
    bool isRunning = true;

    XGrabServer(_display);
    Window root, parent;

    Window *topLevelWindows;
    unsigned int count;

    CHECK(XQueryTree(_display, _root, &root, &parent, &topLevelWindows, &count));

    CHECK_EQ(root, root); // should be the same

    for (unsigned int i = 0; i < count; ++i) {
        Frame(topLevelWindows[i], true);
    }

    XFree(topLevelWindows);
    XUngrabServer(_display);

    auto eventHandler = EventHandler(*this);

    while (isRunning) {
        XEvent e;
        XNextEvent(_display, &e);
        LOG(INFO) << "Received event: " << e.type;

        EventFactory::getEvent(e)->process(eventHandler);
    }
}

void WindowManager::OnXError(Display *display, XErrorEvent *e) {
    LOG(ERROR) << e->error_code << e->type;
}

void WindowManager::OnWMDetected(Display *display, XErrorEvent *e) {
    // Only BadAccess is expected here, happens when another X client has been registered.
    CHECK_EQ(e->error_code, BadAccess); // logs when error code is not BadAccess
//    wmHasBeenDetected = true;
}

void WindowManager::Frame(Window w, bool createdBeforeWindowManager) {
    const unsigned int BORDER_WIDTH = 3;
    const unsigned int BORDER_COLOR = 0xffff00;
    const unsigned int BG_COLOR = 0x0000ff;

    CHECK(!_clients.count(w)); // check if present

    XWindowAttributes attr;
    CHECK(XGetWindowAttributes(_display, w, &attr));

    if (createdBeforeWindowManager) {
        if (attr.override_redirect || attr.map_state != IsViewable) {
            return;
        }
    }

    auto frame = XCreateSimpleWindow(_display, _root, attr.x, attr.y, (unsigned int) attr.width,
                                     (unsigned int) attr.height, BORDER_WIDTH,
                                     BORDER_COLOR, BG_COLOR);

    XSelectInput(_display, frame, SubstructureRedirectMask | SubstructureNotifyMask);
    XAddToSaveSet(_display, w);
    XReparentWindow(_display, w, frame, 0, 0);
    XMapWindow(_display, frame);
    _clients[w] = frame;

    //   a. Move windows with alt + left button.
    XGrabButton(
            _display,
            Button1,
            Mod1Mask,
            w,
            false,
            ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
            GrabModeAsync,
            GrabModeAsync,
            None,
            None);
    //   b. Resize windows with alt + right button.
    XGrabButton(
            _display,
            Button3,
            Mod1Mask,
            w,
            false,
            ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
            GrabModeAsync,
            GrabModeAsync,
            None,
            None);
    //   c. Kill windows with alt + f4.
    XGrabKey(
            _display,
            XKeysymToKeycode(_display, XK_F4),
            Mod1Mask,
            w,
            false,
            GrabModeAsync,
            GrabModeAsync);
    //   d. Switch windows with alt + tab.
    XGrabKey(
            _display,
            XKeysymToKeycode(_display, XK_Tab),
            Mod1Mask,
            w,
            false,
            GrabModeAsync,
            GrabModeAsync);

    LOG(INFO) << "Framed window " << w << " [" << frame << "]";
}

void WindowManager::Unframe(Window w) {
    const Window frame = _clients[w];
    XUnmapWindow(_display, frame);
    XReparentWindow(_display, w, _root, 0, 0);  // Offset of client window within root.
    XRemoveFromSaveSet(_display, w);
    XDestroyWindow(_display, frame);
    _clients.erase(w);

    LOG(INFO) << "Unframed window " << w << " [" << frame << "]";
}
