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

    friend class EventHandler;
public:
    static std::unique_ptr<WindowManager> Create();

    ~WindowManager();

    void Run();

    void Frame(Window w, bool createdBeforeWindowManager = false);

    void Unframe(Window window);

    static void OnXError(Display *display, XErrorEvent *e);

    static void OnWMDetected(Display *display, XErrorEvent *e);

    static bool wmHasBeenDetected;
};

#endif //WM_WINDOWMANAGER_H
