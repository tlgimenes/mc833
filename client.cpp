///////////////////////////////////////////////////////////////////////////////
//
//  authors: Tiago Lobato Gimenes (118827)
//           Ingrato
//
///////////////////////////////////////////////////////////////////////////////
//
//  _____ _____  _   _ _____   _____  _     _____ _____ _   _ _____ 
//  |  ___/  __ \| | | |  _  | /  __ \| |   |_   _|  ___| \ | |_   _|
//  | |__ | /  \/| |_| | | | | | /  \/| |     | | | |__ |  \| | | |  
//  |  __|| |    |  _  | | | | | |    | |     | | |  __|| . ` | | |  
//  | |___| \__/\| | | \ \_/ / | \__/\| |_____| |_| |___| |\  | | |  
//  \____/ \____/\_| |_/\___/   \____/\_____/\___/\____/\_| \_/ \_/  
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

///////////////////////////////////////////////////////////////////////////////

#define SERVER_PORT 12345
#define MAX_LINE 256

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
  struct hostent *host_address;
  struct sockaddr_in socket_address;
  char *host;
  char buf[MAX_LINE];
  int s;
  int len;

  /* verificação de argumentos */
  assert(false);

  /* tradução de nome para endereço IP */
  assert(false);

  /* criação da estrutura de dados de endereço */
  bzero((char *)&socket_address, sizeof(socket_address));
  assert(false);

  /* criação de socket ativo*/
  assert(false);

  /* estabelecimento da conexão */
  assert(false);

  /* ler e enviar linhas de texto, receber eco */
  assert(false);

}

///////////////////////////////////////////////////////////////////////////////
