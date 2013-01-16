#include "sockets.h"
#include "string.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string>


Socket::Socket() :
  m_sock ( -1 )
{

  memset ( &m_addr, 0, sizeof ( m_addr ) );
  memset(&m6_addr, 0, sizeof(m6_addr));

}

Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}


void Socket::setversion(int v)
{
	version=v;
}

int Socket::getversion()
{
	return version;
}


bool Socket::create()
{
  if (version==4) 
  m_sock = socket ( AF_INET, SOCK_STREAM, 0);
  else 
  m_sock = socket ( AF_INET6, SOCK_STREAM, 0);

  if ( ! is_valid() )
    return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;


  return true;

}


bool Socket::send ( const std::string s ) const
{
  int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  if ( status == -1 )
    {
      return false;
    }
  else
    {
      return true;
    }
}


int Socket::recv ( std::string& s ) const
{
  char buf [ MAXRECV + 1 ];

  s = "";

  memset ( buf, 0, MAXRECV + 1 );

  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 )
    {
      std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      s = buf;
      return status;
    }
}



bool Socket::connect ( const std::string host, const int port )
{
  int status;
  if ( ! is_valid() ) return false;

  if (version==4) {
	  m_addr.sin_family = AF_INET;
	  m_addr.sin_port = htons ( port );
  	  status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );
   	  if ( errno == EAFNOSUPPORT ) return false;
          status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );
  }
  else {
	  m6_addr.sin6_family = AF_INET6;
	  m6_addr.sin6_port = htons ( port );
  	  status = inet_pton ( AF_INET6, host.c_str(), &m6_addr.sin6_addr );
   	  if ( errno == EAFNOSUPPORT ) return false;
          status = ::connect ( m_sock, ( sockaddr * ) &m6_addr, sizeof ( m6_addr ) );
  }

  if ( status == 0 )
    return true;
  else
    return false;
}


std::string Socket::resolv(char *hostname, int version)
{

char buf[INET6_ADDRSTRLEN];
struct sockaddr *sa;
struct addrinfo hints, *res;

hints.ai_socktype = SOCK_STREAM;

memset ((char *)&hints, 0, sizeof(hints));

switch (version) {
        case 4:
                hints.ai_family = AF_INET;
                break;
        case 6:
                hints.ai_family = AF_INET6;
                break;
        default:
                hints.ai_family = PF_UNSPEC;
                break;
        }

// getaddrinfo() returns a list of address structures.
int error =  getaddrinfo(hostname, NULL, &hints, &res);

if (error != 0) {
        fprintf(stderr, "%s: not found in name service database, may be the server does not support IPv6\n", hostname);
        exit(1);
}

// Read datagrams and echo them back
getnameinfo(res->ai_addr, res->ai_addrlen, buf, sizeof(buf), NULL, 0, NI_NUMERICHOST);
sa = res->ai_addr;
freeaddrinfo(res);
return buf;
}


void Socket::sleep( unsigned int seconds )
{
       std::time_t target = std::time(0) + seconds;
       while( std::time(0) < target )
            ; // Do nothing.
}


