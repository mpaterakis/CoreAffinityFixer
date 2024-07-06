#pragma once

#include <thread>
#include <fstream>
#include <windows.h>

class CoreAffinityFixer {
public:
    void FeedMeCores();
    CoreAffinityFixer();
private:
    DWORD_PTR getAffinityMask();
    DWORD_PTR affinityMask;
    void startLoop();
    void setProcessAffinity();
    bool firstLoop;
    void LogMessage(std::string);
    const std::string currentDateTime();
};
