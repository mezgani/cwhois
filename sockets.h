// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <stdlib.h>

const int MAXHOSTNAME = 255;
const int MAXRECV = 5120000;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Client initialization
  bool create();
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;
  int recv ( std::string& ) const;
  // Others usefull functions
  bool is_valid() const { return m_sock != -1; }
  void setversion(int v);
  int getversion();
  std::string resolv(char *host, int v);
  void sleep(unsigned int seconds);
 private:

  int version;
  int m_sock;
  sockaddr_in m_addr;
  sockaddr_in6 m6_addr;


};


#endif
