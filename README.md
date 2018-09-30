# WindowManager
_The worst window manager you have ever seen_

## Prerequisites
- CMake
- Xephyr
- X11
- Glog

## Getting started
To build this pile of shit, run the following commands:

```bash
mkdir build

cmake --build build -- -j3
```  

Open in Xephyr:

```bash
Xephyr -ac -screen 1920x1080 -reset :1 &

DISPLAY=:1

// open up some windows to the new display
firefox &
code &

// finally open wm
./build/wm &
```
