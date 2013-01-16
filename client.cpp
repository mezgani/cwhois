// Implementation of the ClientSocket class

#include "client.h"
#include "exception.h"


ClientSocket::ClientSocket ( char* host, int port, int value )
{
  std::string address;
  Socket::setversion(value);
  address=Socket::resolv(host, value);

  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }
  if ( ! Socket::connect ( address, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }
}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;
}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }
  return *this;
}
