
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define BUFF_SIZE 128

using namespace std;

class mySocket
{
public:
    mySocket()
    {
        m_listenSocketfd = socket(AF_INET,SOCK_STREAM,0);
        assert(m_listenSocketfd >= 0);
    }
    ~mySocket()
    {
        close(m_listenSocketfd);   // 关闭文件描述符 （linux中一切都是文件）
    }

    int GetListenSocket(){ return m_listenSocketfd;}

protected:
    int     m_listenSocketfd;    // 监听套接字
};

class ClientSocket : public mySocket
{
public:
    ClientSocket(const std::string& strIp,const int iPort = 8000)
    {
        // 构建服务器信息
        struct sockaddr_in addressServer;
        addressServer.sin_family = AF_INET;
        addressServer.sin_port = htons(iPort);
        addressServer.sin_addr.s_addr = inet_addr(strIp.c_str());

        // 连接
        int ret = connect(m_listenSocketfd,(struct sockaddr*)&addressServer,sizeof(addressServer));
        assert(ret != -1);
        std::cout << "connect success" << std::endl;
    }

    int Send(char* buffer, int size)
    {
        return send(m_listenSocketfd, buffer, size, 0);
    }
};


int main(int argc,char* argv[])
{
    if (argc <= 1)
    {
        cout << "error! please input ip again" << endl;
        return 1;
    }
    std::string strIp = argv[1];
    cout << "server Ip is " << strIp << endl;
    
    ClientSocket cli(strIp);

    while (1)
    {
        cout << "please input data:";
        char buffer[BUFF_SIZE] = {0};
        memset(buffer,0,BUFF_SIZE);
        std::cin >> buffer;
        
        cli.Send(buffer, strlen(buffer));
        std::cout << "send " << buffer << " size:" << strlen(buffer) << std::endl;

        if (strcmp(buffer,"end") ==  0) 
            break;
    }
}
