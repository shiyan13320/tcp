#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <signal.h>
#include "WriteCompound.h"
#include "SeqStack.h"

#define PORT 9990
#define MAX_EXP 80

typedef long long DATA;

int Creat_Socket() {
    int listen_socket;
	int ret;

	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1) {
        printf("socket");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    
    addr.sin_family = AF_INET; 
	addr.sin_port = htons(PORT); /* 端口号 */
	addr.sin_addr.s_addr = htonl(INADDR_ANY); /* IP地址 */
	
	ret = bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1) {
        printf("bind");
		return -1;
	}
	ret = listen(listen_socket, 5);
	if (ret == -1) {
        printf("listen");
		return -1;
	}
	return listen_socket;
}

int wait_client(int listen_socket) {
    int addrlen;
	int client_socket;
	struct sockaddr_in cliaddr;

	memset(&cliaddr, 0, sizeof(cliaddr));
	addrlen = sizeof(cliaddr);
	printf("等待客户端连接。。。。\n");
	client_socket = accept(listen_socket, (struct sockaddr *)&cliaddr, &addrlen);//创建一个和客户端交流的套接字
	if(client_socket == -1) {
        printf("accept");
		return -1;
    }
	printf("成功接收到一个客户端：%s\n", inet_ntoa(cliaddr.sin_addr));
	return client_socket;
}

/* 判断是否为运算符，是返回1，不是返回0 */
int IsOperator (char c) {
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

/*判断两个运算符的优先级，oper1>oper2返回1,oper1<oper2返回-1*/
int PRI (char oper1, char oper2) {
    int pri;
    switch (oper2) {
    case '+':
    case '-':
        if (oper1 == '=') {
            pri = -1;
        } else {
            pri = 1;
        }
        break;
    case '*':
    case '/':
    case '%':
        if (oper1 == '*' || oper1 == '/' || oper1 == '%') {
            pri = 1;//oper1>oper2
        } else {
            pri = -1;
        }
        break;
    case '=':
        pri = 1;
        break;
    }
    return pri;
}

/* 计算两个操作数的结果 */
long long Calc (long long a, int oper, long long b) {
    switch (oper) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b != 0) {
            return a / b;
        } else {
            printf("分子不能为0！\n");
            exit(0);
        }
    case '%':
        if (b != 0) {
            return a % b;
        } else {
            printf("分子不能为0！\n");
            exit(0);
        }
    }
}

long long CalcExp(char StrExp[]) {
    SeqStack *StackOper, *StackData;
    int i = 0;

    /* flag用以处理多位数，while中每次从表达式中取一个字符，
    flag=0时：操作数已入栈；flag=1时：有操作数要入栈 */
    int flag = 0;
    DATA a, b, c, q, x, t, oper;

    StackOper = Init_SeqStack();
    StackData = Init_SeqStack();

    q = 0;
    x = '=';
    Push_SeqStack(StackOper, x);
    c = StrExp[i++];

    while (c != '=' || x != '=') {
        if (IsOperator(c)) {
            if (flag) {
                Push_SeqStack(StackData, q);
                q = 0;
                flag = 0;
            }
            switch(PRI(x, c)) {
            case -1:
                Push_SeqStack(StackOper, c);
                c = StrExp[i++];
                break;
            case 1:
                oper = Pop_SeqStack(StackOper);
                b = Pop_SeqStack(StackData);
                a = Pop_SeqStack(StackData);
                t = Calc(a, oper, b);
                Push_SeqStack(StackData, t);
                break;
            }
        } else if (c >= '0' && c <= '9') {
            c -= '0';
            q = q * 10 + c;
            flag = 1;
            c = StrExp[i++];
        } else {
            printf("input wrong!\n");
            exit(0);
        }
        x = Top_SeqStack(StackOper);
    }
    q = Pop_SeqStack(StackData);
    free(StackData);
    free(StackOper);
    return q;
}

/* 把从客户端接收到的收据进行处理 */
void hanld_client(int listen_socket, int client_socket) {
    char buf[MAX_EXP];
	long long result;
	int ret;
    
	while (1) {
		(void)memset(buf, 0, MAX_EXP*sizeof(char));
		ret = recv(client_socket, buf, MAX_EXP, 0);

		if (ret == -1) {
			int client_socket = wait_client(listen_socket);
			hanld_client(listen_socket, client_socket);
	    }

		printf("%s", buf);

		if (strncmp(buf, "end", 3) == 0) {
            printf("服务器关闭！\n");
			close(listen_socket);
			exit(0);
		}
		    
		result = CalcExp(buf);
        printf("%lld\n", result);

		sprintf(buf, "%s%lld\n", buf, result);		
		Write_Result(sizeof(char)*MAX_EXP, buf);

        (void)memset(buf, 0, MAX_EXP*sizeof(char));
		
		sprintf(buf, "%lld\n", result);
		send(client_socket, buf, strlen(buf), 0);
	}
	
}  

int main() {
    int listen_socket = Creat_Socket();
	int client_socket = wait_client(listen_socket);
	hanld_client(listen_socket, client_socket);
	return 0;
}



