//
// Created by matthias on 30-9-18.
//

#ifndef WM_WINDOWMANAGER_H
#define WM_WINDOWMANAGER_H

#include <X11/Xlib.h>

#include <memory>
#include <unordered_map>

class WindowManager {
private:
    Display *_display;
    Window _root;

    std::unordered_map<Window, Window> _clients;

    WindowManager(Display *display);

    void Frame(Window w, bool createdBeforeWindowManager = false);

    // Event handlers
    void OnCreateNotify(const XCreateWindowEvent &e);
    void OnConfigureRequest(const XConfigureRequestEvent &e);
    void OnUnmapNotify(XUnmapEvent &e);
    void Unframe(Window window);
    void OnMapRequest(const XMapRequestEvent &e);

public:
    static std::unique_ptr<WindowManager> Create();

    ~WindowManager();

    void Run();

    static void OnXError(Display *display, XErrorEvent *e);

    static void OnWMDetected(Display *display, XErrorEvent *e);

    static bool _wmDetected;

};

#endif //WM_WINDOWMANAGER_H
