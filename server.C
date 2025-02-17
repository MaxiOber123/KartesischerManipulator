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
#include <iostream>
#include <fstream>


class KartServer : public TCPserver{
public:
    KartServer(int port, int bufferSize);


protected:
    string myResponse(string input);
    bool toController(int x, int y, int p);
};


int intLength(int input){
    int length = 1;

    if (input>0){
        for(length = 0; input>0; length++){
            input = input/10;
        }
    }
    return length;
}


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
    int x, y, e, p, r;
    bool coordFlag, penFlag, control;


    if (input.compare(0,5,"start") == 0){
        return string ("Warten auf Koordinaten(COORD[x,y]). \n");
    }

    if (input.compare(0,6,"COORD[") == 0){
        if (coordFlag==1){
            return string ("Koordinaten wurden bereits eingegeben! Wenn Sie die Koordinaten zurücknehmen oder überschreiben wollen, nutzen Sie 'Back', bis die Koordinaten gelöscht werden. \n");
            }
        e=sscanf(input.c_str(), "COORD[%i,%i]",&x, &y);
        if (e!= 2){
            return string ("COULD NOT READ COORDINATES \n");
        }
        coordFlag=true;

        if(coordFlag==true && penFlag==true){
            o << "x" << x << "y" << y << "p" << p << "r1e" << ", zum Bestätigen 'y', zur Bearbeitung 'back'. \n";
            return (o.str());
        }

        o << "x" << x << "y" << y << " Warten auf Stiftposition(PEN[0]:oben PEN[1]:unten) \n";
        return (o.str());

    }

    if (input.compare(0,4,"PEN[") == 0){
        if (penFlag==1){
            return string ("Stiftposition wurde bereits eingegeben! Wenn Sie die Stiftposition zurücknehmen oder überschreiben wollen, nutzen Sie 'back', bis die Stiftposition gelöscht wird. \n");
            }
        sscanf(input.c_str(), "PEN[%i]", &p);
        if(p!=0&&p!=1){
            p=0;
            return string ("Ungültige Stiftposition! Erlaubt ist 0 (oben) oder 1 (unten)! \n");
        }
        penFlag=true;

        if(coordFlag==true && penFlag==true){
           o << "x" << x << "y" << y << "p" << p << "r1e" << ", zum Bestätigen 'y', zur Bearbeitung 'back'. \n";
            return (o.str());
        }

        o << "p" << p << " Warten auf Koordinaten(COORD[x,y]). \n";
        return (o.str());

    }

    if (input.compare(0,1,"x") == 0){
        sscanf(input.c_str(), "x%i""y%i""p%i""r%i""e",&x,&y,&p,&r);
        if(intLength(x) < 4 || intLength(x) > 4){
            return string ("X-Coordinate is too small! 4 Digits required! \n");
        }
        if(intLength(y) < 4 || intLength(y) > 4){
            return string ("Y-Coordinate is too small! 4 Digits required! \n");
        }

        o << "x" << x << "y" << y << "p" << p << "r" << r << "e \n";

        //Weiterleitung an Mikrocontroller

        return (o.str());
    }

    if (input.compare(0,4,"back") == 0){
        if(coordFlag==1 && penFlag==1){
            penFlag=0;
            p=0;
            return string ("Stiftposition gelöscht. Neue Eingabe möglich. \n");
        }else if(coordFlag==1 && penFlag==0){
            coordFlag=0;
            x=0;
            y=0;
            return string ("Koordinaten gelöscht. Neue Eingabe möglich. \n");
        }else{
            return string ("Keine Eingaben zum Löschen gefunden. Warten auf Koordinaten(COORD[x,y]). \n");
        }
    }

    if (input.compare(0,1,"y") == 0){
        if(coordFlag==0 && penFlag==0){
            return string ("Keine Koordinaten und Stiftposition zum Senden. \n");
        }else if(coordFlag==1 && penFlag==0){
            return string ("Keine Stiftposition zum Senden. \n");
        }else if(coordFlag==0 && penFlag==1){
            return string ("Keine Koordinaten zum Senden. \n");
        }else{
            control=toController(x,y,p);
            if(control=true){
                o << "x" << x << "y" << y << "p" << p << "r1e" << " erfolgreich gesendet. \n";
                coordFlag=false;
                penFlag=false;
                return (o.str());
            }else if(control=false){
                return string ("Fehler beim Öffnen der Datei! \n");
            }

        }
    }


    return string ("ERROR \n");
}

bool KartServer::toController(int x, int y, int p){
    //Weiterleitung an Mikrocontroller
    std::ofstream file("../Projekt_XY_Plotter/Eckpunkte.txt", std::ios_base::app);  // Öffnen der Textdatei zum Schreiben
        if (file.is_open()) {
            file << "x" << x << "y" << y << "p" << p << "r1e" << "\n";
            file.close();  // Datei schließen
            std::cout << "Eckpunkte in 'eckpunkte.txt' gespeichert! \n" << std::endl;
            return true;
        } else {
            std::cerr << "Fehler beim Öffnen der Datei! \n" << std::endl;
            return false;
        }

}



