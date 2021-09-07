#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <ctype.h>
#include <arpa/inet.h>

#define OPEN_MAX 5000

int main(int argc, char* argv[] ) {

    if( argc <= 2 ){
        printf( "ip_address port_number\n" );
        return 1;
    }
    const char* ip = argv[1];        //ip地址是字符串类型
    int port = atoi( argv[2] );      //端口是int类型
    //创建socket
    /**
     * @brief socket()打开一个网络通讯端口，如果成功的话返回一个文件描述符，应用程序可以像读写文件一样用read/write在网络上收发数据
     * int socket(int domain, int type, int protocol);
     * domain:套接字中使用的协议族信息
     *        AF_INET   使用TCP或UDP来传输，用IPv4的地址
     *        AF_INET6  用IPv6的地址
     *        AF_UNIX   本地协议，使用在Unix和Linux系统上
     * type:  套接字数据传输类型信息
     *        SOCK_STREAM 面向连接的套接字。这个socket是使用TCP来进行传输。
     *        SOCK_DGRAM 面向消息的套接字。使用UDP来进行它的连接。
     * protocol:在前两个参数构成的协议集合下，再选择一个具体的协议。
     *        传0 表示使用默认协议。
     * 返回值：成功：返回指向新创建的socket的文件描述符，失败：返回-1，设置errno
     */
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        perror("socket error");
        exit(1);
    }


    //端口复用
    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));     


    /*函数的参数都用struct sockaddr *类型表示，在传递参数之前要强制类型转换*/
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    /**
     * @brief 将strptr字符串利用 协议族 传入 addrptr
     * int inet_pton(int family, const char *strptr, void*addrptr);
     * 若成功则为1，若输入不是有效的表达格式则为 0，若出错则为-1
     */
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );
    /**
     * @brief 将参数sfd和address绑定在一起，使sockfd这个用于网络通讯的文件描述符监听address所描述的地址和端口号。
     * int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
     * sockfd：  socket文件描述符
     * addr:    构造出IP地址 + 端口号
     * addrlen: sizeof(addr)长度
     * 返回值：   成功返回0，失败返回-1。
     */
    if(bind(sfd, (struct sockaddr *)& address, sizeof(address)) < 0 ) {
        perror("bind error");
        exit(1);
    }

    /**
     * @brief 命名之后，不能马上接受客户连接，需要如下系统用来创建一个监听队列以存放待处理的客户连接。
     * int listen(int sockfd, int backlog);
     * sockfd：  socket文件描述符
     * backlog： 排队建立3次握手队列和刚刚建立3次握手队列的连接数和
     * 返回值：   成功返回0，失败返回-1。
     */
    if(listen(sfd, 5) < 0) {
        perror("listen error");
        exit(1);
    }

    //链接socket
    struct sockaddr_in clie_addr;
     /*获取客户端地址结构大小*/ 
    socklen_t clie_addr_len = sizeof(clie_addr_len);
    char clie_IP[BUFSIZ];

    struct epoll_event tep, ep[OPEN_MAX];  
    //创建epoll模型, efd指向红黑树根节点
    efd = epoll_create(OPEN_MAX);              
    if (efd == -1)
        perr_exit("epoll_create error");
    tep.events = EPOLLIN; 
    tep.data.fd = sfd; 
    //将lfd及对应的结构体设置到树上,efd可找到该树          
    res = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &tep);    
    if (res == -1)
        perr_exit("epoll_ctl error");

    while(true) {
        nready = epoll_wait(efd, ep, OPEN_MAX, -1); 
        if (nready == -1)
            perr_exit("epoll_wait error");
        
        for(int i = 0; i < nready; ++i) {
             //如果不是"读"事件, 继续循环
            if (!(ep[i].events & EPOLLIN))      
                continue;
            //判断满足事件的fd是不是lfd      
            if (ep[i].data.fd == sfd) {          
                clilen = sizeof(cliaddr);
                //接受连接
                /**
                 * @brief三方握手完成后，服务器调用accept()接受连接
                 * int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
                 * sockfd：  socket文件描述符
                 * addr：    传出参数，返回链接客户端地址信息，含IP地址和端口号
                 * addrlen： 传入传出参数（值-结果）,传入sizeof(addr)大小；函数返回时返回真正接收到地址结构体的大小
                 * 返回值：成功返回 一个新的socket文件描述符 ，用于和客户端通信，失败返回-1，设置errno
                 */
                connfd = accept(sfd, (struct sockaddr*)&clie_addr, &clie_addr_len);
                if(cfd < 0) {
                    perror("accpet error");
                    exit(1);
                }
                printf("client IP:%s\tport:%d\n", 
                        inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
                        ntohs(clie_addr.sin_port));
                printf("cfd %d---client %d\n", connfd, ++num);

                //添加至红黑树进行监听
                tep.events = EPOLLIN; tep.data.fd = connfd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
                if (res == -1)
                    perr_exit("epoll_ctl error");
            }else {                                
                sockfd = ep[i].data.fd;
                n = read(sockfd, buf, MAXLINE);

                if (n == 0) {                       //读到0,说明客户端关闭链接
                    //将该文件描述符从红黑树摘除
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);  
                    if (res == -1)
                        perror("epoll_ctl error");
                    //关闭与该客户端的链接
                    close(sockfd);                  
                    printf("client[%d] closed connection\n", sockfd);

                } else if (n < 0) {                 //出错
                    perror("read n < 0 error: ");
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                    close(sockfd);

                } else {                            //实际读到了字节数
                    for (i = 0; i < n; i++)
                        buf[i] = toupper(buf[i]);   //转大写,写回给客户端

                    write(STDOUT_FILENO, buf, n);
                    writen(sockfd, buf, n);
                }
            }
        }
    }

 
    

    close(sfd);
    close(sfd);

    return 0;
}