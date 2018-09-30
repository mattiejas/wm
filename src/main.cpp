#include <iostream>
#include <glog/logging.h>
#include "WindowManager.h"

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);

    auto wm = WindowManager::Create();
    if (!wm) {
        LOG(ERROR) << "Failed to init window manager";
        return EXIT_FAILURE;
    }

    wm->Run();

    return EXIT_SUCCESS;
}