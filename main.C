//============================================================================
// Name        : INF3_Prak.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <string>
#include <iostream>
#include <sstream>

#include <cstdio>      /* printf, NULL */
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <unistd.h>

#include "SHA256.H"


using namespace std;


int main(){
    int x, y;

    x = 12;
    y = 89;

    stringstream cord;
    string output;

    cord << "CORD [" << x << "," << y << "]";

    output = cord.str();

    cout << output << endl;
	//srand(time(nullptr));



    return 0;
}





