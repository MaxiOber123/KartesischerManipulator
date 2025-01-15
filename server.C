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
    KartServer srv(2024,25);
    srv.run();

	return 0;
}


KartServer::KartServer(int port, int bufferSize) : TCPserver(port, bufferSize){

}

string KartServer::myResponse(string input){

    stringstream o;
    int x, y, e, p, r;

    if (input.compare(0,6,"COORD[") == 0){
        e=sscanf(input.c_str(), "COORD[%i,%i]",&x, &y);
    if (e!= 2){
        return string ("COULD NOT READ COORDINATES \n");

    }
        o << "SUMME [" << (x+y) << "]";
        return (o.str());

    }

    if (input.compare(0,2,"PEN[") == 0){


    }

    if (input.compare(0,1,"x") == 0){
        sscanf(input.c_str(), "x%iy%ip%ir%ie",&x,&y,&p,&r);
        if(sizeof(x) < 4 || sizeof(x) > 4){
            return ("X-Coordinate is too small! 4 Digits required! \n");
        }
        if(sizeof(y) < 4 || sizeof(y) > 4){
            return ("Y-Coordinate is too small! 4 Digits required! \n");
        }

        o << "x" << x << "y" << y << "p" << p << "r" << r << "e \n";

        //Hier Weiterleitung an Mikrocontroller

        return (o.str());
    }

    return string ("ERROR \n");
}



