#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SERV_IP "127.0.0.1"
#define PORT 6666

int main() {

    //创建socket
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd < 0) {
        perror("socket error");
        exit(1);
    }

    //连接服务器
    struct sockaddr_in clie_addr;
    bzero(&clie_addr, sizeof(clie_addr));
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERV_IP, &clie_addr.sin_addr.s_addr);

    if(connect(cfd, (struct sockaddr*)&clie_addr, sizeof(clie_addr))) {
        perror("connect error");
        exit(1);
    }

    //向服务器端写内容
    char buf[BUFSIZ];
    while(1) {
        /*从标准输入获取数据*/
        fgets(buf, sizeof(buf), stdin);
         /*将数据写给服务器*/
        write(cfd, buf, strlen(buf));       //写个服务器
        /*从服务器读回转换后数据*/
        int len = read(cfd, buf, sizeof(buf));
        /*写至标准输出*/
        write(STDOUT_FILENO, buf, len);
    }

    close(cfd);

    return 0;
}
