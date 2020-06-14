#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/eventfd.h>
#include <stdlib.h>

#include "wrap.h"
#include "en_de_code.h"

#define MAX_BUF_LEN 256
#define LISTEN_PORT 10500

#define NONE                 "\e[0m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BROWN                "\e[0;33m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define L_BLUE               "\e[1;34m"
#define PURPLE               "\e[0;35m"
#define L_PURPLE             "\e[1;35m"
#define CYAN                 "\e[0;36m"
#define L_CYAN               "\e[1;36m"
#define GRAY                 "\e[0;37m"
#define WHITE                "\e[1;37m"

#define BOLD                 "\e[1m"
#define UNDERLINE            "\e[4m"
#define BLINK                "\e[5m"
#define REVERSE              "\e[7m"
#define HIDE                 "\e[8m"
#define CLEAR                "\e[2J"
#define CLRLINE              "\r\e[K" //or "\e[1K\r"


int printf_color(void)
{
    printf("This is a character control test!\n" );
    sleep(3);
    printf("[%2u]" CLEAR "CLEAR\n" NONE, __LINE__);

    printf("[%2u]" BLACK "BLACK " L_BLACK "L_BLACK\n" NONE, __LINE__);
    printf("[%2u]" RED "RED " L_RED "L_RED\n" NONE, __LINE__);
    printf("[%2u]" GREEN "GREEN " L_GREEN "L_GREEN\n" NONE, __LINE__);
    printf("[%2u]" BROWN "BROWN " YELLOW "YELLOW\n" NONE, __LINE__);
    printf("[%2u]" BLUE "BLUE " L_BLUE "L_BLUE\n" NONE, __LINE__);
    printf("[%2u]" PURPLE "PURPLE " L_PURPLE "L_PURPLE\n" NONE, __LINE__);
    printf("[%2u]" CYAN "CYAN " L_CYAN "L_CYAN\n" NONE, __LINE__);
    printf("[%2u]" GRAY "GRAY " WHITE "WHITE\n" NONE, __LINE__);

    printf("[%2u]\e[1;31;40m Red \e[0m\n",  __LINE__);

    printf("[%2u]" BOLD "BOLD\n" NONE, __LINE__);
    printf("[%2u]" UNDERLINE "UNDERLINE\n" NONE, __LINE__);
    printf("[%2u]" BLINK "BLINK\n" NONE, __LINE__);
    printf("[%2u]" REVERSE "REVERSE\n" NONE, __LINE__);
    printf("[%2u]" HIDE "HIDE\n" NONE, __LINE__);

    printf("Cursor test begins!\n" );
    printf("=======!\n" );
    sleep(10);
    printf("[%2u]" "\e[2ACursor up 2 lines\n" NONE, __LINE__);
    sleep(10);
    printf("[%2u]" "\e[2BCursor down 2 lines\n" NONE, __LINE__);
    sleep(5);
    printf("[%2u]" "\e[?25lCursor hide\n" NONE, __LINE__);
    sleep(5);
    printf("[%2u]" "\e[?25hCursor display\n" NONE, __LINE__);
    sleep(5);

    printf("Test ends!\n" );
    sleep(3);
    printf("[%2u]" "\e[2ACursor up 2 lines\n" NONE, __LINE__);
    sleep(5);
    printf("[%2u]" "\e[KClear from cursor downward\n" NONE, __LINE__);

    return 0;
}

static int is_running = 1;

static void printf_menu_ready()
{
    printf("Please select item:\n");
    printf("    1. sign in.\n");
    printf("    2. sign up.\n");
    printf("    3. administrator.\n");
    printf("    4. set.\n");
    printf("    5. exit.\n");
}

static void printf_menu_chat()
{
    printf("Please select item:\n");
    printf("    1. who online.\n");
    printf("    2. select item to chat.\n");
    printf("    3. log out.\n");
}

//是否存在'\n'，并将回车改为'\0'
int is_enter(char *p)
{
    while (*p != '\0')
    {
        if (*p == '\n')
        {
            *p = '\0';
            return 1;
        }

        p++;
    }
    return 0;
}

void input(char *buf, int len)
{
    int flag = 0;
    while (1)
    {
        char temp[20] = {0};
        if (flag == 0)
        {
            flag = 1;
            fgets(buf, len, stdin);
            if (is_enter(buf) == 1) return;
        }
        else
        {
            fgets(temp, sizeof(temp), stdin);
            if (is_enter(temp) == 1) return;
        }
    }
}

void printf_buf(char *buf)
{
    printf("[%c][%d], t1:[%d], t2:[%d]\n",
           buf[0], buf[1], buf[2], buf[3]);
    int to = -1;
    int n = sizeof(to);
    memcpy(&to, (const char *)(buf+4), n);
    printf("to: [%d]\n", to);
    printf("nmsg: [%d]\n", buf[4+n]);
    printf("msg: [%s]\n", buf+4+n+1);
}

void module_sign_in(int sockfd, char *buf)
{
    // DataDesc sendData;
    // memset(&sendData, 0, sizeof(sendData));
    // printf("sign in......\n");
    // printf("please input name: clzhang\n");
    // printf("please input pswd: ******\n");
    // sendData.dataType = DATA_VERIFY;
    // sendData.dataVerify.type = VERIFY_SIGN_IN;
    // strcpy((char *)sendData.dataVerify.name, "clzhang");
    // strcpy((char *)sendData.dataVerify.pswd, "******");
    // encode(sendData, buf);
    // // printf_SendData(&sendData);

    // int len = 0;
    // memcpy(&len, &buf[LEN_OFFSET], LEN_SIZE);
    // Write(sockfd, buf, len);
    // memset(buf, 0, len);
}
void module_sign_up(int sockfd, char *buf)
{
    // DataDesc sendData;
    // memset(&sendData, 0, sizeof(sendData));
    // printf("sign up......\n");
    // printf("please input name: newname\n");
    // printf("please input pswd: ******\n");
    // sendData.dataType = DATA_VERIFY;
    // sendData.dataVerify.type = VERIFY_SIGN_UP;
    // strcpy((char *)sendData.dataVerify.name, "newname");
    // strcpy((char *)sendData.dataVerify.pswd, "******");
    // encode(sendData, buf);
    // printf_SendData(&sendData);

    // int len = 0;
    // memcpy(&len, &buf[LEN_OFFSET], LEN_SIZE);
    // Write(sockfd, buf, len);
    // memset(buf, 0, len);
}

void module_admin(int sockfd, char *buf)
{
    char buf_buf[200] = {0};
    DataDesc sendData;
    memset(&sendData, 0, sizeof(sendData));
    sendData.dataType = DATA_COMMAND;
    sendData.dataCommand.command = COMMANG_GET_FRIEND_LIST;
    strcpy(sendData.dataCommand.info, "");

    if (encode(sendData, buf_buf) < 0)
    {
        printf("encode failed!\n");
        return;
    }
    int len = get_buf_len(buf_buf);
    Write(sockfd, buf_buf, len);
}

// int efd = 0;
void signalstop(int sign_no)
{
    if(sign_no == SIGINT)
    {
        is_running = 0;
        // uint64_t u = 10;
        // ssize_t s = write(efd, &u, sizeof(uint64_t));
        // if (s != sizeof(uint64_t))
        //     perror("write");
        printf("in signalstop!\n");
    }
}

int handle_command(DataCommand *recvData)
{
    switch (recvData->command)
    {
        case COMMANG_SHOW_INFO:
            printf("ser-> %s\n", recvData->info);
            break;

        case COMMAND_APPLY_CONNECT_SUCCESS:
            printf("COMMAND_APPLY_CONNECT_SUCCESS\n");
            break;

        case COMMAND_APPLY_CONNECT_FAIL:
            printf("COMMAND_APPLY_CONNECT_FAIL\n");
            break;

        case COMMAND_SIGN_IN_SUCCESS:
            printf("COMMAND_SIGN_IN_SUCCESS\n");
            break;

        case COMMAND_SIGN_IN_FAIL:
            printf("COMMAND_SIGN_IN_FAIL\n");
            break;

        case COMMAND_SIGN_UP_SUCCESS:
            printf("COMMAND_SIGN_UP_SUCCESS\n");
            break;

        case COMMAND_SIGN_UP_FAIL:
            printf("COMMAND_SIGN_UP_FAIL\n");
            break;

        default:
            break;
    }

    return 0;
}

int handle_msg(char *buf_recv, char *buf_send)
{
    DataDesc recvData;
    memset(&recvData, 0, sizeof(recvData));
    decode(&recvData, buf_recv);
    // printf_SendData(&recvData);

    // only rece command
    if (recvData.dataType == DATA_COMMAND)
    {
        handle_command(&recvData.dataCommand);
    }
    else
    {
        printf("invalid msg from server!\n");
    }

    return 0;
}

// 测试变长参数
void test_printf(const char *fmt, ...)
{
    printf("%s", fmt);
}

#define PRINTF_ERROR(fmt, ...) (printf(L_RED "[ERROR] " NONE "%s" , fmt))
#define PRINTF_WARN(fmt, ...)  (printf(YELLOW "[WARN] %s" NONE, fmt))
#define PRINTF_INFO(fmt, ...)  (printf(GREEN "[INFO] %s" NONE, fmt))
#define PRINTF_DEBUG(fmt, ...) (printf(WHITE "[DEBUG] %s" NONE, fmt))

int main()
{
    // printf("[%2u]" RED "RED " L_RED "L_RED\n" NONE, __LINE__);
    // printf("[%2u]" GREEN "GREEN " L_GREEN "L_GREEN\n" NONE, __LINE__);
    // printf("[this]" " is" " test" "\n");
    // test_printf("[this]" " is" " test" "\n");
    // PRINTF_ERROR("444\n");
    // PRINTF_WARN("333\n");
    // PRINTF_INFO("222\n");
    // PRINTF_DEBUG("111\n");

    // return 0;

    signal(SIGINT, signalstop);
    //printf_color();

    char buf_send[MAX_BUF_LEN] = {0};
    char buf_recv[MAX_BUF_LEN] = {0};
    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(LISTEN_PORT);

    Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // efd = eventfd(0, 0);
    // if (efd < 0) {
    //     perror("efd init failed: \n");
    // }

    fd_set allset;
    fd_set fdset;
    FD_ZERO(&allset);
    FD_ZERO(&fdset);
    FD_SET(STDIN_FILENO, &allset);
    FD_SET(sockfd, &allset);
    // FD_SET(efd, &allset);

    // to optimization
    int max_fd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;
    // max_fd = efd > max_fd ? efd : max_fd;

    printf("-----Welcome to chatroom-----\n");
    printf(BLINK "^_^\n" NONE);
    printf_menu_ready();
    int status = 0;
    while (is_running)
    {
        fdset = allset;
        int nready = select(max_fd+1, &fdset, NULL, NULL, NULL);
        if (nready <= 0)
        {
            perror("select error");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &fdset))
        {
            if (status == 0){
            char buf[5] = {0};
            input(buf, 2);
            switch (buf[0])
            {
                case '1':
                    printf("enter sign in\n");
                    module_sign_in(sockfd, buf_send);
                    break;

                case '2':
                    printf("enter sign up\n");
                    module_sign_up(sockfd, buf_send);
                    break;

                case '3':
                    printf("enter administrator\n");
                    module_admin(sockfd, buf_send);
                    break;

                case '4':
                    printf("enter set\n");
                    break;

                case '5':
                    printf("enter exit\n");
                    is_running = 0;
                    //Write(sockfd, "456", sizeof("456"));
                    break;

                default:
                    printf("invalid input\n"); break;
            }
            }
        }

        if (FD_ISSET(sockfd, &fdset))
        {
            int n = Read(sockfd, buf_recv, MAX_BUF_LEN);
            if (n == 0)
            {
                printf("the other side has been closed.\n");
                break;
            }
            else
            {
                handle_msg(buf_recv, buf_send);

                //clear buff
                memset(buf_recv, 0, n);
                int len = 0;
                memcpy(&len, &buf_send[LEN_OFFSET], LEN_SIZE);
                memset(buf_send, 0, len);

            }
        }
    }

    printf("main exit!\n");
    Close(sockfd);

    return 0;
}
