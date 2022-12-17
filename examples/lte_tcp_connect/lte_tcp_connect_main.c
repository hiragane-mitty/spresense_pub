/****************************************************************************
 * examples/lte_tcp_connect/lte_tcp_connect_main.c
 *
 *   Copyright 2022 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sdk/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/boardctl.h>
#include <arch/board/board.h>
#include <nuttx/wireless/lte/lte.h>
#include "lte_connection.h"
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


/****************************************************************************
 * Private Data
 ****************************************************************************/

lte_quality_t lte_quality;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int tcp_connect(char *hostname, char *port)
{
  int sockfd = -1;
  struct addrinfo hints, *addr_list, *cur;

  /* Do name resolution with both IPv6 and IPv4 */
  memset( &hints, 0, sizeof( hints ) );
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  if (getaddrinfo(hostname, port, &hints, &addr_list) != 0)
    {
      return sockfd;
    }

  /* Try the sockaddrs until a connection succeeds */

  for (cur = addr_list; cur != NULL; cur = cur->ai_next)
    {
      sockfd = (int) socket(cur->ai_family,
                            cur->ai_socktype,
                            cur->ai_protocol);
      if (sockfd < 0)
        {
          continue;
        }

      if (connect(sockfd, cur->ai_addr, cur->ai_addrlen) == 0)
        {
          break;
        }

      close(sockfd);
      sockfd = -1;
    }

  freeaddrinfo(addr_list);

  return sockfd;
}

static void tcp_disconnect(int sockfd)
{
  shutdown(sockfd, 2);
  close(sockfd);
}

static void parse_argument(int argc, char *argv[],
                           struct lte_apn_setting *apn,
                           char **hostname, char **port)
{
  apn->ip_type   = LTE_IPTYPE_V4;
  apn->auth_type = LTE_APN_AUTHTYPE_CHAP;
  apn->apn_type  = LTE_APN_TYPE_DEFAULT | LTE_APN_TYPE_IA;

  if (strcmp(argv[1], "iij") == 0)
    {
      apn->apn       = "iijmobile.biz";
      apn->user_name = "mobile@iij";
      apn->password  = "iij";
    }
  else if (strcmp(argv[1], "snc") == 0)
    {
      apn->apn       = "4gn.jp";
      apn->user_name = "mb_lte@snc";
      apn->password  = "snc";
    }
  else if (strcmp(argv[1], "amari") == 0)
    {
      apn->apn       = "internet";
      apn->user_name = NULL;
      apn->password  = NULL;
      apn->auth_type = LTE_APN_AUTHTYPE_NONE;
    }
  else /* soracom */
    {
      apn->apn       = "soracom.io";
      apn->user_name = "sora";
      apn->password  = "sora";
    }

  if (strcmp(argv[2], "azure") == 0)
    {
      *hostname = "ocrcam-iothub-develop.azure-devices.net";
      *port     = "443";
    }
  else if (strcmp(argv[2], "example") == 0)
    {
      *hostname = "example.com";
      *port     = "443";
    }
  else if (strcmp(argv[2], "mitty") == 0)
    {
      *hostname = "221.132.186.199";
      *port     = "2349";
    }
  else
    {
      *hostname = argv[2];
      *port     = argv[3];
    }
}


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * lte_azureiot_main
 ****************************************************************************/

int tcptest(int argc, FAR char *argv[])
{
  int i;
  int sockfd;
  struct lte_apn_setting apn;
  char *hostname;
  char *port;

  parse_argument(argc, argv, &apn, &hostname, &port);

  printf("LTE connect...\n");
  printf("APN = %s\n", apn.apn);

  if (app_lte_connect_to_lte(&apn)) 
    {
      printf("LTE connect...NG\n\n");

      return ERROR;
    }

  printf("LTE connect...OK\n\n");
  lte_get_quality_sync(&lte_quality);

  printf("TCP connect...\n");
  printf("server= %s:%s\n", hostname, port);
  for (i = 10; 0 < i; i--)
    {
       sockfd = tcp_connect(hostname, port);
       if (sockfd > 0)
         {
           printf("TCP connect...OK\n");
           tcp_disconnect(sockfd);
           break;
         }

      printf("TCP connect...retry\n");
      sleep(1);
    }

  printf("\nLTE disconnect...\n");

  app_lte_disconnect_from_lte();

  printf("LTE disconnect...OK\n\n");

  return 0;
}

int main(int argc, FAR char *argv[])
{
  while (1)
    {
      tcptest(argc, argv);
      sleep(1);
    }
}
