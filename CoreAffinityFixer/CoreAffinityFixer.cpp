#include "pch.h"
#include "CoreAffinityFixer.h"

using namespace std;

// Feed the app all the cores
void CoreAffinityFixer::FeedMeCores() {

    TCHAR initialDelayTchar[32];
    TCHAR loopDelayTchar[32];
    TCHAR createLogTchar[32];
    GetPrivateProfileString(_T("CoreAffinityFixer"), _T("InitialDelay"), _T("5"), initialDelayTchar, 32, _T(".\\CoreAffinityFixer.ini"));
    GetPrivateProfileString(_T("CoreAffinityFixer"), _T("LoopInterval"), _T("1"), loopDelayTchar, 32, _T(".\\CoreAffinityFixer.ini"));
    GetPrivateProfileString(_T("CoreAffinityFixer"), _T("CreateLog"), _T("false"), createLogTchar, 32, _T(".\\CoreAffinityFixer.ini"));

    wstring loopDelayWchar(loopDelayTchar);
    wstring initialDelayWchar(initialDelayTchar);
    wstring createLogWchar(createLogTchar);
    string loopDelayStr(loopDelayWchar.begin(), loopDelayWchar.end());
    string initialDelayStr(initialDelayWchar.begin(), initialDelayWchar.end());
    string createLogStr(createLogWchar.begin(), createLogWchar.end());

    if (createLogStr == "true") {
        createLog = true;
    }
    if (isNumber(initialDelayStr)) {
        initialDelay = atoi(initialDelayStr.c_str());
    }
    if (isNumber(loopDelayStr)) {
        loopInterval = atoi(loopDelayStr.c_str());
    }

    LogMessage("Starting loop. Initial delay: " + std::to_string((DWORD)initialDelay) + ", Loop interval: " + std::to_string((DWORD)loopInterval), false);

    // Create a new thread that runs the startLoop method of this instance
    std::thread t(&CoreAffinityFixer::startLoop, this);
    t.detach();
}

// Run in a loop
void CoreAffinityFixer::startLoop() {
    while (true) {
        // Wait a bit on the first loop (default is 5 seconds)
        if (firstLoop) {
            firstLoop = false;
            std::this_thread::sleep_for(std::chrono::seconds(this->initialDelay));
        }

        // Sleep for given second on every loop (default is 1 seconds)
        std::this_thread::sleep_for(std::chrono::seconds(this->loopInterval));

        // Set process affinity
        setProcessAffinity();
    }
}

// Set process affinity
void CoreAffinityFixer::setProcessAffinity() {
    HANDLE process = GetCurrentProcess();
    this->affinityMask = getAffinityMask();

    if (this->affinityMask == 0) {
        LogMessage("[ERROR] Invalid affinity mask, skipping SetProcessAffinityMask.", true);
        return;
    }

    SetProcessAffinityMask(process, this->affinityMask);
}

// Get affinity mask
DWORD_PTR CoreAffinityFixer::getAffinityMask() {
    // Only run this once and then return processMask immediately
    if (this->affinityMask != NULL) {
        return this->affinityMask;
    }

    DWORD_PTR processAffinityMask;
    DWORD_PTR systemAffinityMask;

    if (!GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask)) {
        LogMessage("[ERROR] Failed to get process affinity mask, errcode: " + GetLastError(), true);
        return 0;
    }

    this->affinityMask = systemAffinityMask;

    std::string maskInString = std::to_string(this->affinityMask);
    LogMessage("Using new affinity mask: " + maskInString, false);

    return this->affinityMask;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
// https://stackoverflow.com/a/10467633/16642426
const std::string CoreAffinityFixer::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    errno_t err = localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

// Log a message on the logfile
void CoreAffinityFixer::LogMessage(std::string message, bool urgent) {
    if (!this->createLog && !urgent) {
        return;
    }
    std::string timeString = currentDateTime();
    ofstream file("CoreAffinityFixer.log", std::ios_base::app);
    file << "[" << timeString << "] " << message << endl;
    file.close();
}

// Check if string is number
// https://stackoverflow.com/a/4654718/16642426
bool CoreAffinityFixer::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
