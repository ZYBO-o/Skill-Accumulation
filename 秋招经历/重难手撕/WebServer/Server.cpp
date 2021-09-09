#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <arpa/inet.h>

# define PORT 6666

int main() {
    //创建socket
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        perror("socket error");
        exit(1);
    }

    //命名socket
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sfd, (struct sockaddr *)& serv_addr, sizeof(serv_addr)) < 0 ) {
        perror("bind error");
        exit(1);
    }

    //监听socket
    if(listen(sfd, 5) < 0) {
        perror("listen error");
        exit(1);
    }

    printf("wait for client connect ...\n");

    //链接socket
    struct sockaddr_in clie_addr;
     /*获取客户端地址结构大小*/ 
    socklen_t clie_addr_len = sizeof(clie_addr_len);
    int cfd = accept(sfd, (struct sockaddr*)&clie_addr, &clie_addr_len);
    if(cfd < 0) {
        perror("accpet error");
        exit(1);
    }

    char clie_IP[BUFSIZ];
    printf("client IP:%s\tport:%d\n", 
            inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(clie_addr.sin_port));
    
    //处理客户端请求
    char buf[BUFSIZ];
    while(1) {
        /*读取客户端发送数据*/
        int len = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);

        /*处理客户端数据*/
        for (int i = 0; i < len; i++)
            buf[i] = toupper(buf[i]);

        /*处理完数据回写给客户端*/
        write(cfd, buf, len); 
    }

    close(cfd);
    close(sfd);

    return 0;
}
