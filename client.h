#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOGDI             
#define NOUSER            

#include <windows.h>
#include <winhttp.h>
#include "nlohmann/json.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

#pragma comment(lib, "winhttp.lib")

using json = nlohmann::json;

class Client {
    private:
        std::wstring hostname;
        std::wstring path;
        std::wstring wpath;
        std::wstring id;
        std::wstring adv_id;

        bool running = false;
        std::thread worker;
        std::mutex mtx;
        std::queue<std::string> msgQueue;
        
        std::wstring s2ws(const std::string& str);
        std::string makeRequest(const std::wstring& method, const std::string& data, const std::wstring& rpath);
        void pollingLoop();

    public:
        Client(std::string url, std::string client_id, std::string client2_id);
        ~Client();
        void start();
        void stop();
        void send(const json& msg);
        bool popMessage(std::string& out);
};