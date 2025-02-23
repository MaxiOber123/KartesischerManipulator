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


class KartServer : public TCPserver
{
public:
    KartServer(int port, int bufferSize);


protected:
    string myResponse(string input);
    bool toController(int x, int y, int p);
};

bool lessThanFour(int input) // Prueft ob Laenge der Eingabe kleiner gleich 4 ist
{
    string s;

    s = to_string(input);
    while(s.length() < 4){
        s.insert (0, 1, '0');
    }
    if(s.length() <= 4)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isBool(int input) // Prueft ob Eingabe 0 oder 1 ist
{
    if(input == 0 || input == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool inRange(int inputX, int inputY){ // Schauen ob Koordinaten im vorgegeben Bereich sind
    int rangeX, rangeY;

    rangeX = 4000;
    rangeY = 4000;

    if(inputX <= rangeX && inputY <= rangeY){
        return true;
    }else{
        return false;
    }

}

int main() //Serverinitialisierung
{
    srand(time(nullptr));
    KartServer srv(2023,25);
    srv.run();

    return 0;
}

string toFour(int input){ // Wandelt int zu string mit der Länge 4
    string output;

    output = to_string(input);
    while(output.length() < 4){
        output.insert (0, 1, '0');
    }
    return output;
}

KartServer::KartServer(int port, int bufferSize) : TCPserver(port, bufferSize)
{

}

string KartServer::myResponse(string input) // Rueckgaben
{

    stringstream o; // output-string
    int x, y, e, p, r; // x- Koordinate, Y-Koordinate, Anzahl Eingaben, Stiftposition, Reset
    bool coordFlag, penFlag, control; // Koordinatenflagge, Stiftflagge, Erfolgreiche Weiterleitung an Controller


    if (input.compare(0,5,"start") == 0) // Start
    {
        return string ("Warten auf Koordinaten(COORD[x,y]). \n");
    }

    if (input.compare(0,6,"COORD[") == 0) // Koordinaten
    {
        if (coordFlag==1) // Wenn Koordinaten schon eingegeben wurden, die noch nicht weitergeleitet wurden
        {
            return string ("Koordinaten wurden bereits eingegeben! Wenn Sie die Koordinaten zurücknehmen oder überschreiben wollen, nutzen Sie 'Back', bis die Koordinaten gelöscht werden. \n");
        }
        e=sscanf(input.c_str(), "COORD[%i,%i]",&x, &y);
        if (e!= 2) // Wenn keine 2 Werte gelesen wurden
        {
            return string ("COULD NOT READ COORDINATES \n");
        }
        if (inRange(x,y) == false){ // Wenn die gegebenen Koordinaten nicht im vorgegebenen Bereich sind
            return string ("Koordinaten sind nicht im vorgegeben Bereich! \n");
        }

        coordFlag=true; // Setzen der Koordinatenflagge

        if(coordFlag==true && penFlag==true) // Wenn Koordinaten und Stiftposition eingegeben sind
        {
            o << "x" << x << "y" << y << "p" << p << "r1e" << ", zum Bestätigen 'y', zur Bearbeitung 'back'. \n";
            return (o.str());
        }

        o << "x" << x << "y" << y << " Warten auf Stiftposition(PEN[0]:oben PEN[1]:unten) \n";
        return (o.str());

    }

    if (input.compare(0,4,"PEN[") == 0) // Stiftposition
    {
        if (penFlag==1) // Wenn Stiftposition schon eingegeben wurde
        {
            return string ("Stiftposition wurde bereits eingegeben! Wenn Sie die Stiftposition zurücknehmen oder überschreiben wollen, nutzen Sie 'back', bis die Stiftposition gelöscht wird. \n");
        }
        sscanf(input.c_str(), "PEN[%i]", &p);
        if(isBool(p) == false) // Pruefen ob eingegebene Stiftposition 0 oder 1 ist
        {
            p=0;
            return string ("Ungültige Stiftposition! Erlaubt ist 0 (oben) oder 1 (unten)! \n");
        }
        penFlag=true; // setzen der Stiftpositionsflagge

        if(coordFlag==true && penFlag==true) // Wenn Koordinaten und Stiftposition eingegeben sind
        {
            o << "x" << x << "y" << y << "p" << p << "r1e" << ", zum Bestätigen 'y', zur Bearbeitung 'back'. \n";
            return (o.str());
        }

        o << "p" << p << " Warten auf Koordinaten(COORD[x,y]). \n";
        return (o.str());

    }

    if (input.compare(0,1,"x") == 0) // Kombinierte Eingabe
    {
        sscanf(input.c_str(), "x%d""y%d""p%i""r%i""e",&x,&y,&p,&r); // Lesen der X- und Y-Koordinaten, sowie Stiftposition und reset
        if(lessThanFour(x) == false) // X-Koordinate kleiner gleich 4
        {
            return string ("X-Coordinate is too big! Maximum of 4 Digits! \n");
        }
        if(lessThanFour(y) == false) // Y-Koordinate kleiner gleich 4
        {
            return string ("Y-Coordinate is too big! Maximum of 4 Digits! \n");
        }
        if(isBool(p) == false) // Stiftposition 0 oder 1
        {
            return string ("Invalid pen position! Allowed are 0 (up) or 1 (down). \n");
        }
        if(inRange(x,y) == false) // Wenn die gegebenen Koordinaten nicht im vorgegebenen Bereich sind
        {
            return string ("Koordinaten sind nicht im vorgegeben Bereich! \n");
        }

        o << "x" << toFour(x) << "y" << toFour(y) << "p" << p << "r" << r << "e gespeichert \n";

        //Weiterleitung an Mikrocontroller (Schreiben in die Datei)
        toController(x,y,p);

        return (o.str());
    }

    if (input.compare(0,4,"back") == 0) // Rücknahme der letzten Eingabe
    {
        if(coordFlag==1 && penFlag==1) // Wenn beides eingegeben wurde
        {
            penFlag=0;
            p=0;
            return string ("Stiftposition gelöscht. Neue Eingabe möglich. \n");
        }
        else if(coordFlag==1 && penFlag==0) // Wenn nur Koordinaten eingegeben wurden
        {
            coordFlag=0;
            x=0;
            y=0;
            return string ("Koordinaten gelöscht. Neue Eingabe möglich. \n");
        }
        else
        {
            return string ("Keine Eingaben zum Löschen gefunden. Warten auf Koordinaten(COORD[x,y]). \n");
        }
    }

    if (input.compare(0,1,"y") == 0) // Bestaetigung der Eingabe fuer Weiterleitung an Mikrocontroller
    {
        if(coordFlag==0 && penFlag==0) // Wenn nichts eingegeben wurde
        {
            return string ("Keine Koordinaten und Stiftposition zum Senden. \n");
        }
        else if(coordFlag==1 && penFlag==0) // Wenn nur Koordinaten eingegeben wurden
        {
            return string ("Keine Stiftposition zum Speichern. \n");
        }
        else if(coordFlag==0 && penFlag==1) // Wenn nur Stiftposition eingegeben wurde
        {
            return string ("Keine Koordinaten zum Speichern. \n");
        }
        else
        {
            control=toController(x,y,p); // Weiterleitung an Mikrocontroller
            if(control=true)
            {
                o << "x" << x << "y" << y << "p" << p << "r1e" << " erfolgreich gespeichert. \n"; // Zusammengesetzter string als bestaetigung an Client senden
                coordFlag=false; // Zuruecksetzung der Flags
                penFlag=false;
                return (o.str());
            }
            else if(control=false) // Wenn die Datei nicht geoeffnet werden kann
            {
                return string ("Fehler beim Öffnen der Datei! \n");
            }

        }
    }


    return string ("ERROR \n"); // Sonstige Fehler, die nicht spezifisch abgefangen werden
}

bool KartServer::toController(int x, int y, int p) // Schreiben der Koordinaten und Stiftposition in die gemeinsame Datei
{
    //Weiterleitung an Mikrocontroller
    std::ofstream file("../KartesischerManipulator/Eckpunkte.txt", std::ios_base::app);  // Öffnen der Textdatei zum Schreiben (Daten werden angehangen (keine Löschung von vorherigen Daten in der Datei))
    if (file.is_open())
    {
        file << "x" << toFour(x) << "y" << toFour(y) << "p" << p << "r1e" << "\n";
        file.close();  // Datei schließen
        std::cout << "Eckpunkte in 'eckpunkte.txt' gespeichert! \n" << std::endl;
        return true;
    }
    else // Fehler beim Oeffnen oder Schreiben
    {
        std::cerr << "Fehler beim Öffnen der Datei! \n" << std::endl;
        return false;
    }

}



