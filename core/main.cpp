#include "process.cpp"
int main()
{
    vector<string> c = readconfig();
    while (1)
    {
        INFO i = getprocessinfo();
        sendInfo(i, c[0].c_str(), c[1]);
        int t = stoi(c[2]);
        this_thread::sleep_for(chrono::seconds(t));
    }
}