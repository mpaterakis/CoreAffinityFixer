#pragma once

#include <thread>
#include <fstream>
#include <windows.h>
#include <tchar.h>

class CoreAffinityFixer {
public:
    void FeedMeCores();
private:
    DWORD_PTR getAffinityMask();
    DWORD_PTR affinityMask = NULL;
    int loopInterval = 1;
    int initialDelay = 5;
    void startLoop();
    void setProcessAffinity();
    bool firstLoop = true;
    void LogMessage(std::string, bool);
    const std::string currentDateTime();
    bool isNumber(const std::string&);
    bool createLog = false;
};
