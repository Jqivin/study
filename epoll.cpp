#include<iostream>
#include <netinet/in.h>
using namespace std;
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/epoll.h>
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
        , m_epollfd(-1)
    {
        m_iListenSocket = CreateListenSocket();
        
        if(m_iListenSocket == -1)
        {
            return;
        }

        m_epollfd = epoll_create(1);
        if(m_epollfd != -1)
        {
            std::cout << "epollfd is " << m_epollfd << std::endl;
            // 添加到epollfd
            if(AddFdToEpollFd(m_iListenSocket) == -1)
            {
                return;
            }
            Run();
        }
    }

    ~MyServer(){}
private:
    //添加描述符到epollfd
    int AddFdToEpollFd(int fd)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = fd;
        if(epoll_ctl(m_epollfd,EPOLL_CTL_ADD,fd,&ev) == -1)
        {
            std::cout << "add fd error" << std::endl;
            close(fd);
            return -1;
        }
        return 0;
    }

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
        if(n == -1 || n == 0)
        {
            std::cout << clifd << " client close" << std::endl;
            close(clifd);
        }
        else
        {
            std::cout << n << "bytes,buf is " << buf << std::endl;
            n = send(clifd,"ok",2,0); 
        }
    }

    // 接受连接
    void Run()
    {
        while(1)
        {
            struct epoll_event events[MAX_FD];
            int nready = epoll_wait(m_epollfd,events,MAX_FD,2000);
            if(nready < 0)
            {
                std::cout << "poll error";
                break;
            }
            else if(nready == 0)
            {
                std::cout << "time out." << std::endl;
            }
            else
            {
                // 只有epoll才可以用nready
                for(int i = 0;i < nready;++i)
                {
                    if(events[i].data.fd == -1)
                    {
                        continue;
                    }
                    if(events[i].events & EPOLLIN)
                    {
                        if(events[i].data.fd == m_iListenSocket)
                        {// 监听套接字
                            int clifd = AcceptCon();
                            if(clifd != -1)
                            {
                                AddFdToEpollFd(clifd);
                            }
                        }
                        else
                        {
                            RecvData(events[i].data.fd);
                        }
                    }
                }
            }

        }
    }

private:
    int     m_iListenSocket;       // 监听套接字
    int     m_epollfd;             // epoll描述符
};

int main()
{
    std::cout << "hello server" << endl;

    MyServer myser;
    
    return 0;
}

