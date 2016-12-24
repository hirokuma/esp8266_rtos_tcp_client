#include <string.h>
#include "lwip/sockets.h"
#include "user_config.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "tcp_client.h"

static int m_fd = -1;


void tcp_client_start(void)
{
    struct sockaddr_in server_addr;

    DBG_PRINT("TCP connect start\n");
    while (1) {
        m_fd = socket(PF_INET, SOCK_STREAM, 0);
        if (m_fd == -1) {
            DBG_PRINT("fail: sock\n");
            vTaskDelay(1000 / portTICK_RATE_MS);
            continue;
        }
        DBG_PRINT("socket ok\n");

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(TCP_SERVER_IPADDR);
        server_addr.sin_port = htons(TCP_SERVER_REMOTE_PORT);
        int ret = connect(m_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
        if (ret != 0) {
            DBG_PRINT("fail: connect(%d)\n", ret);
            close(m_fd);
            m_fd = -1;
            vTaskDelay(1000 / portTICK_RATE_MS);
            continue;
        }
        break;
    }
    DBG_PRINT("connect ok\n");
    write(m_fd, TCP_CLIENT_GREETING, sizeof(TCP_CLIENT_GREETING) - 1);

    char buf[128];
    ssize_t sz;
    while ((sz = read(m_fd, buf, sizeof(buf))) > 0) {
        write(m_fd, buf, sz);
    }

    close(m_fd);
    m_fd = -1;
    DBG_PRINT("close\n");
}
