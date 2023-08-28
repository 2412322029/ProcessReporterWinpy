#include <winsock2.h>
#include <Windows.h>
#include <Psapi.h>
#include <thread>
#include "a.h"
#include "sendmsg.cpp"
#include "reader.cpp"
#include <iostream>
using namespace std;

INFO getprocessinfo()
{
    setbuf(stdout, NULL);  // 禁用输出缓冲
    INFO Info;
    DWORD processId;
    HWND foregroundWindow = GetForegroundWindow();
    if (foregroundWindow == NULL)
    {
        Info.programName = "桌面";
    }
    // 获取进程ID
    GetWindowThreadProcessId(foregroundWindow, &processId);
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle == NULL)
    {
        cout << "Failed to open process." << endl;
    }
    // 获取标题
    char Title[256];
    GetWindowTextA(foregroundWindow, Title, sizeof(Title));
    Info.windowTitle = Title;
    // 获取名字
    char moduleName[MAX_PATH];
    if (GetModuleBaseNameA(processHandle, NULL, moduleName, sizeof(moduleName)) == 0)
    {
        cout << "Failed to get module name." << endl;
    }
    Info.programName = moduleName;
    size_t pos = Info.programName.find(".exe");
    if (pos != std::string::npos)
    {
        Info.programName.replace(pos, 4, "");
    }
    // 获取当前时间点
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // 将当前时间点转换为 time_t 类型
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    // 使用 ctime 函数将 time_t 转换为可打印的字符串格式
    char time_buffer[100];
    std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_time_t));
    cout << "[" << time_buffer << "] - " << "name: " << Info.programName << ";title: " << Info.windowTitle<< " > "<< flush;
    CloseHandle(processHandle);
    return Info;
}


