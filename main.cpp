#include <iostream>
#include <string>
#include <array>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <chrono>

int main() {
    system("xterm -xrm 'XTerm.vt100.allowTitleOps: false' -T \"bgterm\" -bg black -fg green &");

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::array<char, 128> buffer;
    std::string windowIdList;

    FILE* pipe = popen("wmctrl -l | grep \"bgterm\" | awk '{print $1}'", "r");

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        windowIdList += buffer.data();
    }
    
    pclose(pipe);
    
    std::istringstream stream(windowIdList);
    std::string lastId;
    while (stream >> lastId);

    std::stringstream hexStream;
    hexStream << "0x" << std::uppercase << std::hex << std::stoul(lastId, nullptr, 16);
    std::string windowId = hexStream.str();

    system(("wmctrl -i -r " + windowId + " -b add,fullscreen; wmctrl -i -r " + windowId + " -b add,skip_taskbar; wmctrl -i -r " + windowId + " -b add,below").c_str());
    return 0;
}
