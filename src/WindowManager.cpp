//
// Created by matthias on 30-9-18.
//

#include <glog/logging.h>
#include <X11/Xutil.h>

#include "WindowManager.h"
#include "events/Event.h"
#include "events/EventFactory.h"

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
        _display = display;
        _root = DefaultRootWindow(_display);
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

    CHECK_EQ(root, _root); // should be the same

    for (unsigned int i = 0; i < count; ++i) {
        Frame(topLevelWindows[i], true);
    }

    XFree(topLevelWindows);
    XUngrabServer(_display);

    while (isRunning) {
        XEvent e;
        XNextEvent(_display, &e);
        LOG(INFO) << "Received event: " << e.type;

        switch (e.type) {
            case CreateNotify:
                OnCreateNotify(e.xcreatewindow);
                break;
            case ConfigureRequest:
                OnConfigureRequest(e.xconfigurerequest);
                break;
            case MapRequest:
                OnMapRequest(e.xmaprequest);
                break;
            case UnmapNotify:
                OnUnmapNotify(e.xunmap);
                break;
            default:
                LOG(WARNING) << "Ignored event";
        }
    }
}

void WindowManager::OnXError(Display *display, XErrorEvent *e) {
    LOG(ERROR) << e->error_code << e->type;
}

void WindowManager::OnWMDetected(Display *display, XErrorEvent *e) {
    // Only BadAccess is expected here, happens when another X client has been registered.
    CHECK_EQ(e->error_code, BadAccess); // logs when error code is not BadAccess
//    _wmDetected = true;
}

void WindowManager::OnCreateNotify(const XCreateWindowEvent &e) {
    // window is invisible when created
}

void WindowManager::OnConfigureRequest(const XConfigureRequestEvent &e) {
    XWindowChanges changes;
    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;

    if (_clients.count(e.window)) {
        const Window frame = _clients[e.window];
        XConfigureWindow(_display, frame, (unsigned int) e.value_mask, &changes);
        LOG(INFO) << "Resize " << e.window << " to " << e.width << "x" << e.height;
    }

    XConfigureWindow(_display, e.window, (unsigned int) e.value_mask, &changes);
    LOG(INFO) << "Resize " << e.window << " to " << e.width << "x" << e.height;
}

void WindowManager::OnMapRequest(const XMapRequestEvent &e) {
    Frame(e.window);
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

void WindowManager::OnUnmapNotify(XUnmapEvent &e) {
    if (e.event == _root) {
        LOG(INFO) << "Ignore UnmapNotify for reparented pre-existing window " << e.window;
        return;
    } else if (!_clients.count(e.window)) {
        LOG(INFO) << "Ignore UnmapNotify for non-client window " << e.window;
        return;
    }

    Unframe(e.window);
}

void WindowManager::Unframe(Window w) {
    // We reverse the steps taken in Frame().
    const Window frame = _clients[w];
    // 1. Unmap frame.
    XUnmapWindow(_display, frame);
    // 2. Reparent client window back to root window.
    XReparentWindow(
            _display,
            w,
            _root,
            0, 0);  // Offset of client window within root.
    // 3. Remove client window from save set, as it is now unrelated to us.
    XRemoveFromSaveSet(_display, w);
    // 4. Destroy frame.
    XDestroyWindow(_display, frame);
    // 5. Drop reference to frame handle.
    _clients.erase(w);

    LOG(INFO) << "Unframed window " << w << " [" << frame << "]";
}
