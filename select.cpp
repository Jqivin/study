#include<iostream>
#include <netinet/in.h>
using namespace std;
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

class MyServer
{
public:
    MyServer()
        : m_iListenSocket(-1)
    {
        m_iListenSocket = CreateListenSocket();
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

        FD_ZERO(&m_rfds);
        FD_SET(socketfd,&m_rfds);
        m_maxfd = socketfd;

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
        if(n <= 0)
        {
            std::cout << clifd << " client close" << std::endl;
            FD_CLR(clifd,&m_rfds);
            close(clifd);
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
        // m_rfds用于记录原始的集合，rset每次循环的时候，用于放到内核态，这个每次select之后，都会改变值
        fd_set rset;
        while(1)
        {
            // 用于将用户空间的fd集合传到内核
            rset = m_rfds;
            struct timeval tv;
            tv.tv_sec = 5;
            tv.tv_usec = 0;
            int nready = select(m_maxfd+1,&rset,NULL,NULL,&tv);
            if(FD_ISSET(m_iListenSocket,&rset))
            {// 监听套接字有数据了
                int cfd = AcceptCon();
                if(cfd == -1)
                {
                    continue;
                }
                // 设置到集合中
                FD_SET(cfd,&m_rfds);
                // 设置最大fd
                if(cfd > m_maxfd)
                {
                    m_maxfd = cfd;
                }
            }
            else
            {// 连接套接字上有消息  --- 遍历集合
                //std::cout << "jqwtest" << std::endl;
                for(int i = m_iListenSocket + 1;i <= m_maxfd;++i)
                {
                    if(FD_ISSET(i,&rset))
                    {
                        RecvData(i);
                    }
                }
            }

        }
    }

private:
    int     m_iListenSocket;       // 监听套接字
    int     m_maxfd;               // 最大套接字
    fd_set  m_rfds;                // 套接字集合
};

int main()
{
    std::cout << "hello server" << endl;

    MyServer myser;
    
    return 0;
}

