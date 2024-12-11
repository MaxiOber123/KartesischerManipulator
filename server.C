/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
#include <cstdio> // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <ctime> //contains various functions for manipulating date and time

#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include <sstream>
#include "SIMPLESOCKET.H"


class KartServer : public TCPserver{
public:
    KartServer(int port, int bufferSize);


protected:
    string myResponse(string input);
};





int main()
{
	srand(time(nullptr));
    KartServer srv(2023,25);
    srv.run();

	return 0;
}


KartServer::KartServer(int port, int bufferSize) : TCPserver(port, bufferSize){

}

string KartServer::myResponse(string input){

    stringstream o;
    int x, y, e;

    if( 0 == input.compare(0,6,"COORD[")){
        e = sscanf(input.c_str(), "COORD[%i,%i]",&x, &y);
    if (e!= 2){
        return string ("COULD NOT READ COORDINATES");
    }

    o << "SUMME [" << (x+y) << "]";
    return (o.str());

    }

    return string ("ERROR");
}



