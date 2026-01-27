#include "client.h"
#include <chrono>

Client::Client(std::string url, std::string client_id, std::string client2_id){
    if(url.find("https://") == 0){
        url.erase(0,8);
    }
    size_t dotJson = url.find(".json");
    if(dotJson != std::string::npos){
        url.erase(dotJson);
    }
    hostname = s2ws(url.substr(0, url.find('/')));
    path = L"/partida/" + s2ws(client2_id) + L".json";
    wpath = L"/partida/" + s2ws(client_id) + L".json";
    this->id = s2ws(client_id);
    this->adv_id = s2ws(client2_id);
}

Client::~Client()
{
    stop();
}

void Client::start(){
    running = true;

    worker = std::thread(&Client::pollingLoop,this);
    std::cout<<"Client demarré via WinHTTP\n";
}

void Client::stop(){
    running = false;
    if(worker.joinable()){
        worker.join();
    }
}

std::wstring Client::s2ws(const std::string& str){
    if(str.empty()){
        return L"";
    }
    int size = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size);
    return wstrTo;
}

std::string Client::makeRequest(const std::wstring& method, const std::string& data, const std::wstring& rpath) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    std::string responseString = "";

    hSession = WinHttpOpen(L"RaylibFirebase/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession){
         return "";
    }

    hConnect = WinHttpConnect(hSession, hostname.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect){
        WinHttpCloseHandle(hSession);
        return "";
    }

    hRequest = WinHttpOpenRequest(hConnect, method.c_str(), rpath.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest){ 
        WinHttpCloseHandle(hConnect); 
        WinHttpCloseHandle(hSession); 
        return ""; 
    }

    BOOL bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)data.c_str(), (DWORD)data.length(), (DWORD)data.length(), 0);
    if (bResults){
        bResults = WinHttpReceiveResponse(hRequest, NULL);
        DWORD dwSize = 0;
        DWORD dwDownloaded = 0;
        do{
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)){
                break;
            }
            if (dwSize == 0){
                break;
            }
            std::vector<char> buffer(dwSize + 1);
            if (WinHttpReadData(hRequest, &buffer[0], dwSize, &dwDownloaded)){
                responseString.append(buffer.data(), dwDownloaded);
            }
        }while (dwSize > 0);
    }

    if (hRequest){
        WinHttpCloseHandle(hRequest);
    }
    if (hConnect){
        WinHttpCloseHandle(hConnect);
    }
    if (hSession){
        WinHttpCloseHandle(hSession);
    }

    return responseString;
}

void Client::send(const json& msg){
    if(!running){
        return;
    }
    json message = msg;
    makeRequest(L"PUT", message.dump(), wpath);
}

void Client::pollingLoop() {
    while (running) {
        std::string resp = makeRequest(L"GET", "", path);
        
        if (!resp.empty() && resp[0] != '<' && resp != "null") {

            json partida = json::parse(resp);

            std::lock_guard<std::mutex> lock(mtx);
            while(!msgQueue.empty()) msgQueue.pop(); 
            msgQueue.push(partida.dump());
                       
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool Client::popMessage(std::string& out) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!msgQueue.empty()) {
        out = msgQueue.front();
        msgQueue.pop();
        return true;
    }
    return false;
}
