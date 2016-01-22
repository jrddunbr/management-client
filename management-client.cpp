/* 
 * File:   management-client.cpp
 * Author: jared
 *
 * Created on January 21, 2016, 3:50 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

class tcp_module {
private:
    string address;
    int port;
    struct sockaddr_in server;
    int sock;
public:
    tcp_module(string, int);
    void sendTcp(string);
};

tcp_module::tcp_module(string addr, int portNumber) {
    address = addr;
    port = portNumber;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Error creating the socket\nExiting\n";
        exit(1);
    }

    if (inet_addr(address.c_str()) == -1) {
        struct hostent *host;
        if ((host = gethostbyname(address.c_str()))) {
            cerr << "Error resolving host " << address << "\nPlease use an IP if you are not, exiting\n";
            exit(1);
        }
    }

    server.sin_addr.s_addr = inet_addr(address.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sock, ((struct sockaddr*) &server), sizeof (server)) < 0) {
        cerr << "Error connecting to host. Exiting.\n";
        exit(1);
    }
}

void tcp_module::sendTcp(string data) {
    if (send(sock, data.c_str(), data.length(), 0) < 0) {
        cerr << "Error sending data. Exiting\n";
        exit(1);
    }
}

bool isConnected = false;

void sendKey(tcp_module connection, string key, string data) {
    string send = "GET /" + key + "/" + data + " HTTP/1.1\r\n\r\n";
    connection.sendTcp(send);
}

int main(int argc, char** argv) {
    
    if(argc !=  5) {
        cerr << "Error, not enough arguments!\n";
        cout << "\n  Usage:\n\n./[executable] host port key value\n";
        for(int i = 0; i < argc; i++) {
            cout << argv[i] << " ";
        }
        cout << "\n";
        exit(1);
    }
    
    string key = argv[3];
    string data = argv[4];

    string host = argv[1];
    int port = atoi(argv[2]);
    tcp_module tcp(host, port);
    sendKey(tcp, key, data);

    return 0;
}


