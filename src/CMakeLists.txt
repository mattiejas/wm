find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIRS})

find_package(glog 0.3.5 REQUIRED)

set(SRC
        main.cpp
        WindowManager.cpp
        WindowManager.h
        events/AbstractEventHandler.h
        events/Event.h
        events/CreateWindowEvent.h
        events/DestroyWindowEvent.h
        events/EventFactory.cpp events/EventFactory.h events/EventHandler.cpp events/EventHandler.h events/ConfigureRequestEvent.h events/MapRequestEvent.h events/UnmapEvent.h)

add_executable(wm ${SRC})
target_link_libraries(wm ${X11_LIBRARIES} glog::glog)