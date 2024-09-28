#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

int main() {
    // 配置参数
    std::string username = "***";
    std::string remote_ip = "*********";
    int remote_port = 30000;
    int start_port = 28000;
    int end_port = 28100;

    // 构建 SSH 命令
    std::stringstream sshCommand;
    sshCommand << "ssh -p " << remote_port;

    // 添加端口转发参数
    for (int port = start_port; port <= end_port; ++port) {
        sshCommand << " -L " << port << ":127.0.0.1:" << port;
    }

    // 添加用户名和远程服务器 IP
    sshCommand << " " << username << "@" << remote_ip << " -N";

    // 将命令转换为 Windows 宽字符格式
    std::string commandStr = sshCommand.str();
    std::wstring commandWStr(commandStr.begin(), commandStr.end());

    // 创建进程启动信息结构体
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // 使用 CreateProcessW 来启动 SSH 进程
    if (!CreateProcessW(NULL, &commandWStr[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Failed to start SSH process. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // 等待 SSH 进程结束（或者在后台运行）
    std::cout << "SSH tunnel established successfully." << std::endl;

    // 等待 SSH 进程结束（或者在后台运行）
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 关闭进程和线程句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
