#ifndef SERVER_HPP
#define SERVER_HPP

class server
{
  public:
    server(int port);
};

class tcp_server: public server
{
  public:
    tcp_server(int port): server(port) {}
}

class udp_server: public server
{
  public:
    udp_server(int port): server(port) {}
}

#endif
