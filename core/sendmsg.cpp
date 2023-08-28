#include <chrono>
#include <iostream>
#include <Windows.h>
#include <winhttp.h>
#include <json/json.h> // pacman -S mingw-w64-x86_64-jsoncpp
#include <string>
#include <locale>
#include <codecvt>
using namespace std;

bool sendInfo(const INFO &info, const char *api_url, string api_key)
{
    bool flag = false;
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    DWORD bytesRead = 0;
    wstring wApiUrl = wstring(api_url, api_url + strlen(api_url));
    hSession = WinHttpOpen(L"WinHTTP /1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession)
    {
        hConnect = WinHttpConnect(hSession, wApiUrl.c_str(), INTERNET_DEFAULT_PORT, 0);
        if (hConnect)
        {
            wstring data;
            Json::Value json_data;
            json_data["programName"] = info.programName;
            json_data["windowTitle"] = info.windowTitle;
            json_data["api_key"] = api_key;
            auto now = chrono::system_clock::now();
            auto timestamp = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
            json_data["timestamp"] = timestamp;

            wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
            wstring json_str = converter.from_bytes(json_data.toStyledString());
            wstring headers = L"Content-Type: application/json\r\n";
            cout << endl;
            DWORD dataSize = json_str.size() * sizeof(wchar_t);
            BOOL result = WinHttpSendRequest(hConnect, headers.c_str(), -1, (LPVOID)json_str.c_str(), dataSize, dataSize, 0);
            if (result)
            {
                flag = true;
            }
        }
    }
    if (flag)
    {
        cout << "Ok" << endl;
    }
    else
    {
        cout << "request failed" << endl;
        return false;
    }
    return true;
}
