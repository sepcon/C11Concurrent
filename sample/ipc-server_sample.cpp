//#include <maf/LocalIPCStub.h>
//#include <maf/utils/TimeMeasurement.h>

//#include <future>
//#include <iostream>

//#include "Server.h"

// using namespace maf;

// int main() {
//  std::cout.sync_with_stdio(false);
//  auto logFunc = [](const std::string &msg) {
//    std::cout << "[ " << std::this_thread::get_id() << " ]" << msg <<
//    std::endl;
//  };
//  auto errFunc = [](const std::string &msg) {
//    std::cerr << "[ " << std::this_thread::get_id() << " ]" << msg <<
//    std::endl;
//  };

//  maf::logging::init(
//      maf::logging::LOG_LEVEL_FROM_INFO | maf::logging::LOG_LEVEL_DEBUG,
//      std::move(logFunc), std::move(errFunc));

//  std::vector<std::future<void>> serverStopSignals;

//  MAF_LOGGER_DEBUG("Server is starting up!");
//  ServerComponent server(localipc::createStub(
//      {SERVER_NAME, WEATHER_SERVER_PORT}, SID_WeatherService));

//  for (int i = 0; i < 1; ++i) {
//    serverStopSignals.emplace_back(
//        std::async(std::launch::async, [&server] { server.run(); }));
//  }

//  for (auto &waiter : serverStopSignals) {
//    if (waiter.valid()) {
//      waiter.wait();
//    }
//  }
//  for (auto &waiter : serverStopSignals) {
//    if (waiter.valid()) {
//      waiter.wait();
//    }
//  }

//  MAF_LOGGER_DEBUG("Component shutdown!");
//}

#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Local\\nocpes.github.com.hello.world");
TCHAR szMsg[] = TEXT("Message from first process.");

int _tmain() {
  HANDLE hMapFile;
  LPCTSTR pBuf;

  hMapFile =
      CreateFileMapping(INVALID_HANDLE_VALUE,  // use paging file
                        NULL,                  // default security
                        PAGE_READWRITE,        // read/write access
                        0,         // maximum object size (high-order DWORD)
                        BUF_SIZE,  // maximum object size (low-order DWORD)
                        szName);   // name of mapping object

  if (hMapFile == NULL) {
    _tprintf(TEXT("Could not create file mapping object (%lu).\n"),
             GetLastError());
    return 1;
  }
  pBuf = (LPTSTR)MapViewOfFile(hMapFile,             // handle to map object
                               FILE_MAP_ALL_ACCESS,  // read/write permission
                               0, 0, BUF_SIZE);

  if (pBuf == NULL) {
    _tprintf(TEXT("Could not map view of file (%lu).\n"), GetLastError());

    CloseHandle(hMapFile);

    return 1;
  }

  auto pid = GetCurrentProcessId();

  _getch();
  CopyMemory((PVOID)pBuf, &pid, sizeof(pid));
  _getch();

  UnmapViewOfFile(pBuf);

  CloseHandle(hMapFile);

  return 0;
}
