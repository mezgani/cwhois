// Definition of the ClientSocket class

#ifndef ClientSocket_class
#define ClientSocket_class

#include "sockets.h"


class ClientSocket : private Socket
{
 public:

  ClientSocket ( char* host, int port, int value );
  virtual ~ClientSocket(){};

  const ClientSocket& operator << ( const std::string& ) const;
  const ClientSocket& operator >> ( std::string& ) const;

};


#endif
