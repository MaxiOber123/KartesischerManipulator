/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <iostream>
#include <unistd.h> //contains various constants

#include "SIMPLESOCKET.H"

using namespace std;


int intLength(int input){
    int length = 1;

    if (input>0){
        for(length = 0; input>0; length++){
            input = input/10;
        }
    }
    return length;
}

string toFour(int input){
    string output;

    output = to_string(input);
    while(output.length() < 4){
        output.insert (0, 1, '0');
    }
    return output;
}

int main() {
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";
	string msg;
	int x, y, p;

	//connect to host
	c.conn(host , 2023);

	int i=0;
	bool goOn=1;
	while(goOn){ // send and receive data
		if((rand() % 20) < i++){
			msg = string("BYEBYE");
			goOn = 0;
		}else{
			x = rand() % 4000;
			y = rand() % 4000;
			p = rand() % 2;

            msg = "x" + toFour(x) + "y" + toFour(y) + "p" + to_string(p) + "r1e";
		}
		cout << "client sends:" << msg << endl;
		c.sendData(msg);
		msg = c.receive(64);
		cout << "got response:" << msg << endl;
		sleep(1);


	}
}


