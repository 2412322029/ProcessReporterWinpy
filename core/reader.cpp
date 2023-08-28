#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>  // 包含用于字符串处理的头文件
#include <functional> // 包含用于 std::bind 的头文件

using namespace std;

string trim(const string &s)
{
    auto isWhitespace = [](char c)
    {
        return std::isspace(static_cast<unsigned char>(c));
    };
    auto start = std::find_if_not(s.begin(), s.end(), isWhitespace);
    auto end = std::find_if_not(s.rbegin(), s.rend(), isWhitespace).base();

    return (start >= end) ? string() : string(start, end);
}

vector<string> readconfig()
{
    vector<string> lines;
    ifstream infile;
    infile.open("config.txt");
    if (!infile)
    {
        ofstream outfile("config.txt");
        if (!outfile)
        {
            cerr << "Unable to create the config file" << endl;
        }
        outfile << "http://localhost:8080/api/msg\n123456\n5" << endl;
        outfile.close();
        cerr << "Config file not found. Creating and writing to it." << endl;
    }
    string line;
    while (getline(infile, line))
    {
        lines.push_back(trim(line)); // 去除首尾空格和换行
    }
    infile.close();
    cout <<"api url:"<< lines[0]  << ", pre :" << lines[2] << "(s)" << endl;
    return lines;
}
// int main(){
//     for (const string &line : readconfig())
//     {
//         cout << line << endl;
//     }
// }
