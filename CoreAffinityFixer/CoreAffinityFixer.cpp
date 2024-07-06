#include "pch.h"
#include "CoreAffinityFixer.h"

using namespace std;

// Private constructor to prevent instantiation
CoreAffinityFixer::CoreAffinityFixer() : affinityMask(NULL), firstLoop(true) {}

// Feed the app all the cores
void CoreAffinityFixer::FeedMeCores() {
    LogMessage("Starting loop.");

    // Create a new thread that runs the startLoop method of this instance
    std::thread t(&CoreAffinityFixer::startLoop, this);
    t.detach();
}

// Run in a loop
void CoreAffinityFixer::startLoop() {
    while (true) {
        // Wait 5 seconds on the first loop
        if (firstLoop) {
            firstLoop = false;

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        // Sleep for 1 second on every loop
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Set process affinity
        setProcessAffinity();
    }
}

// Set process affinity
void CoreAffinityFixer::setProcessAffinity() {
    HANDLE process = GetCurrentProcess();
    this->affinityMask = getAffinityMask();

    if (this->affinityMask == 0) {
        LogMessage("Invalid affinity mask, skipping SetProcessAffinityMask.");
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
        LogMessage("Failed to get process affinity mask. Error: " + GetLastError());
        return 0;
    }

    this->affinityMask = systemAffinityMask;

    std::string maskInString = std::to_string(this->affinityMask);
    LogMessage("Using new affinity mask: " + maskInString);

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
void CoreAffinityFixer::LogMessage(std::string message) {
    std::string timeString = currentDateTime();
    ofstream file("CoreAffinityFixer.log", std::ios_base::app);
    file << "[" << timeString << "] " << message << endl;
    file.close();
}
