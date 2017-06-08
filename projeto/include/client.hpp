#ifndef CLIENT_HPP
#define CLIENT_HPP

class client
{
  public:
    client(int port, const std::string& ip);
};

class tcp_client: public client
{
  public:
    tcp_client(int port, const std::string& ip): client(port, ip) {}
};

class udp_client: public client
{
  public:
    udp_client(int port, const std::string& ip): client(port, ip) {}
};

#endif
