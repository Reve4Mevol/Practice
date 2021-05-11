/**
 * 练习winsock库的使用，平时都是用封装好的跨平台库，对于这些系统提供的底层库使用较少
 * 需要加以练习熟悉 CMakeList需要添加-lwsock32
 * 1、存储端口，ip地址信息的结构体各个字段是做啥的
 * 2、初始化时候的sockVersion有什么要求
 * 3、SOCK_STREAM一定对于TCP吗
 * 4、bind时候强转，能不能直接之前创建对象就是要求的对象
 * 5、超时接收需要自己实现？还有如何设置非阻塞一次一包？
 *
 *
 */

#include <iostream>
#include <winsock2.h>
#include <inaddr.h>

#pragma comment(lib,"ws2_32.lib")
#define PKG_LEN (1024*10)

int main() {
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaTemp;
    // 初始化相关库
    if(0 != WSAStartup(sockVersion,&wsaTemp))
    {
        std::cout<<"initialize failed!"<<std::endl;
        return -1;
    }
    // 创建套接字
    SOCKET listenHandle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(INVALID_SOCKET == listenHandle)
    {
        std::cout<<"create sockFD for listening failed!"<<std::endl;
        return -1;
    }

    sockaddr_in ipInfo;
    ipInfo.sin_addr.S_un.S_addr = inet_addr("192.168.0.105");
    ipInfo.sin_port = htons(10890);
    ipInfo.sin_family = AF_INET;
    if(SOCKET_ERROR == bind(listenHandle,(sockaddr *)&ipInfo, sizeof(ipInfo)))
    {
        std::cout<<"bind failed!"<<std::endl;
        return -1;
    }
    if(SOCKET_ERROR == listen(listenHandle,2))
    {
        std::cout<<"listen failed!"<<std::endl;
        return -1;
    }
    SOCKET tmpSocket;
    sockaddr_in clientInfo;
    int clientAddrLen = 0;
    char *pData = NULL;
    pData = new char[PKG_LEN];
    memset(pData,0,PKG_LEN);
    if(!pData)
    {
        std::cout<<"create buffer failed!"<<std::endl;
        return -1;
    }
    std::cout<<"wait a client!"<<std::endl;
    while(1)
    {
        tmpSocket = accept(listenHandle,(sockaddr *)&clientInfo,&clientAddrLen);
        if(INVALID_SOCKET == tmpSocket)
        {
            Sleep(10);
            continue;
        }
        std::cout<<"comes a client!"<<std::endl;
        int ret = recv(tmpSocket,pData,100,0);
        std::cout<<"recv len:"<<ret<<std::endl<<"msg:"<<pData<<std::endl;
        closesocket(tmpSocket);
        break;
    }
    closesocket(listenHandle);
    WSACleanup();
    return 0;
}