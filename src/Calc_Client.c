#include <stdio.h>
#include <winsock2.h>
#include <unistd.h>
#pragma  comment(lib,"ws2_32.lib")

#define MAX_EXP 80

int Creat_Socket() {
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA data;
    SOCKADDR_IN serAddr;
    if(WSAStartup(sockVersion, &data) != 0) {
        return 0;
    }

    SOCKET sclient = socket(AF_INET, SOCK_STREAM, 0);
    if(sclient == INVALID_SOCKET) {
        printf("invalid socket !");
        return 0;
    }

    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(9990);
    serAddr.sin_addr.S_un.S_addr = inet_addr("192.168.3.54");

    if (connect(sclient, (struct sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
        printf("connect error !");
        closesocket(sclient);
        return 0;
    }
    return sclient;
}

int main(int argc, char* argv[]) {
    int ret = 0;
    char StrExp[MAX_EXP];
    int sclient = Creat_Socket();
    (void)memset(StrExp, 0, MAX_EXP*sizeof(char));

    while(1) {
        printf("Please input expression'=end':\n");
        scanf("%s", StrExp);
        send(sclient, StrExp, strlen(StrExp), 0);

        if (strncmp(StrExp, "end", 3) == 0) {
            printf("·þÎñÆ÷¹Ø±Õ£¡");
			break;
		}
		(void)memset(StrExp, 0, MAX_EXP*sizeof(char));
        ret = recv(sclient, StrExp, MAX_EXP, 0);
        if(ret > 0) {
            printf(StrExp);
        }
    }
    closesocket(sclient);
    WSACleanup();
    return 0;
}
