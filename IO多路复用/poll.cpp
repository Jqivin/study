#include<iostream>
#include <netinet/in.h>
using namespace std;
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_FD 128

class MyServer
{
public:
    MyServer()
        : m_iListenSocket(-1)
    {
        // 初始化文件描述符数组
        for(int i = 0;i < MAX_FD;++i)
        {
            m_fds[i].fd = -1;
        }

        m_iListenSocket = CreateListenSocket();
        AddFdToArray(m_iListenSocket,POLLIN);
        if(m_iListenSocket != -1)
        {
            Run();
        }
    }

    ~MyServer(){}
private:
    // 创建监听套接字
    int CreateListenSocket()
    {
        int socketfd = socket(AF_INET,SOCK_STREAM,0);
        if(socketfd == -1)
        {
            return -1;
        }

        struct sockaddr_in serAddr;
        serAddr.sin_family = AF_INET;
        serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serAddr.sin_port = htons(8000);

        // 绑定
        int res = bind(socketfd,(struct sockaddr*)&serAddr,sizeof(serAddr));
        if(res == -1)
        {
            return -1;
        }

        // 监听
        int resLis = listen(socketfd,5);
        if(resLis == -1)
        {
            return -1;
        }

        std::cout << "listen success" << endl;
        return socketfd;
    }

    void AddFdToArray(int fd,short event )
    {
        for(int i = 0; i< MAX_FD;i++)
        {
            if(m_fds[i].fd == -1)
            {
                m_fds[i].fd = fd;
                m_fds[i].events = event;
                m_maxfd = m_maxfd > fd ? m_maxfd+1 : fd+1;
                std::cout << "insert " << fd << std::endl;
                break;
            }
        }
    }

    int AcceptCon()
    {
        // 监听套接字  --- 接受
        struct sockaddr_in clientAdd;
        int len = sizeof(clientAdd);
        int cfd = accept(m_iListenSocket,(struct sockaddr*)&clientAdd,(socklen_t*)&len);
        if(cfd == -1)
        {
            std::cout << "accept error" << std::endl;
            return -1;
        }

        std::cout << "one client connect:" << cfd << std::endl;
        return cfd;
    }

    void RecvData(int clifd)
    {
        std::cout << clifd << " Ready recv data:";
        char buf[1024] = {0};
        int n = recv(clifd,buf,sizeof(buf),0);
        if(n == -1)
        {
            std::cout << clifd << " client close" << std::endl;
            close(clifd);
            for(int i = 0;i < m_maxfd;++i)
            {
                if(m_fds[i].fd == clifd)
                {
                    m_fds[i].fd = -1;
                }
            }
        }
        else
        {
            std::cout << n << "bytes,buf is " << buf << std::endl;
            n = send(clifd,"ok",2,0); 
        }
    }

    // 接受连接
    int Run()
    {
        while(1)
        {
            int nready = poll(m_fds,m_maxfd,2000);
            if(nready < 0)
            {
                std::cout << "poll error";
            }
            else if(nready == 0)
            {
                std::cout << "time out." << std::endl;
            }
            else
            {
                for(int i = 0;i < m_maxfd;++i)
                {
                    if(m_fds[i].fd == -1)
                    {
                        continue;
                    }
                    if(m_fds[i].revents & POLLIN)
                    {
                        if(m_fds[i].fd == m_iListenSocket)
                        {// 监听套接字
                            int clifd = AcceptCon();
                            if(clifd != -1)
                            {
                                AddFdToArray(clifd,POLLIN);
                            }
                        }
                        else
                        {
                            RecvData(m_fds[i].fd);
                        }
                    }
                }
            }

        }
    }

private:
    int     m_iListenSocket;       // 监听套接字
    int     m_maxfd;               // 最大套接字，减少循环次数
    struct pollfd m_fds[128];      // 描述符数组
};

int main()
{
    std::cout << "hello server" << endl;

    MyServer myser;
    
    return 0;
}

