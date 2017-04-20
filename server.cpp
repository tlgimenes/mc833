///////////////////////////////////////////////////////////////////////////////
//
//  authors: Tiago Lobato Gimenes (118827)
//           Ingrato
//
///////////////////////////////////////////////////////////////////////////////
//  
//  _____ _____  _   _ _____   _____ ___________ _   _ ___________ 
// |  ___/  __ \| | | |  _  | /  ___|  ___| ___ \ | | |  ___| ___ \
// | |__ | /  \/| |_| | | | | \ `--.| |__ | |_/ / | | | |__ | |_/ /
// |  __|| |    |  _  | | | |  `--. \  __||    /| | | |  __||    / 
// | |___| \__/\| | | \ \_/ / /\__/ / |___| |\ \\ \_/ / |___| |\ \ 
// \____/ \____/\_| |_/\___/  \____/\____/\_| \_|\___/\____/\_| \_|
//  
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>

///////////////////////////////////////////////////////////////////////////////

#define LISTEN_PORT 12345
#define MAX_PENDING 5
#define MAX_LINE 256

///////////////////////////////////////////////////////////////////////////////

int main()
{
  struct sockaddr_in socket_address;
  char buf[MAX_LINE];
  unsigned int len;
  int s, new_s;

  /* criação da estrutura de dados de endereço */
  bzero((char *)&socket_address, sizeof(socket_address));
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  if(socket_df <= 0) {

  }

  /* criação de socket passivo */
  assert(false);

  /* Associar socket ao descritor */
  assert(false);

  /* Criar escuta do socket para aceitar conexões */
  assert(false);

  /* aguardar/aceita conexão, receber e imprimir texto na tela, enviar eco */
  assert(false);
}

///////////////////////////////////////////////////////////////////////////////
