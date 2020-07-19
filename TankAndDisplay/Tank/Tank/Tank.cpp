
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>  
#include <SFML/Network/UdpSocket.hpp>
#include <thread>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>


void senderM(void*);
void checkVolumePer();  

using namespace std;

int volumePer=0;   // the volume percentage for tank
bool isFill = 0;    // is fill button clicked
bool isDrain = 0;   // is fill drain clicked


char ip[50];
int port;
int port2;

int main()
{
    // Initialization settings 
    std::cout << "Display's IP(e.g. PC's IP) is : ";
    std::cin >> ip;
    std::cout << "Display's port(1024 ~ 65535)(e.g. 55001) is : ";
    std::cin >> port;
    std::cout << "The port number(1024 ~ 65535)(e.g. 55002) which you wanna bind with is : ";
    std::cin >> port2;

    if (port < 1024 && port>65535 && port2 < 1024 && port2 > 65535) {
        system("Ple try again");
        return 0 ;
    }


    // Socket Setup
    sf::UdpSocket socket;
    sf::IpAddress sender;
    unsigned short port;
    socket.bind(port2);
    // Receive a message from anyone
    char buffer[1024];
    std::size_t received = 0;

    socket.receive(buffer, sizeof(buffer), received, sender, port);

    //run a thread to count the volume percentage
    sf::Thread t1(checkVolumePer);
    t1.launch();

    //run a thread to send message to display
    sf::Thread t(senderM,&socket);
    t.launch();
    
    //Receive meesage about the status of button
    while (true) {

        socket.receive(buffer, sizeof(buffer), received, sender, port);
        std::cout << buffer << std::endl;
        if (buffer[0] == '0' & isFill ==0) {
            std::cout << "Sart Filling" << std::endl;
            isFill = 1;
        
        }
        else if (buffer[0] == '0' & isFill == 1) {
            std::cout << "Stop Filling" << std::endl;
            isFill = 0;
        }
        else if (buffer[0] == '1' & isDrain == 0) {
            std::cout << "Sart Draining" << std::endl;
            isDrain = 1;

        }
        else if (buffer[0] == '1' & isDrain == 1) {
            std::cout << "Stop Draining" << std::endl;
            isDrain = 0;
        }
        else if (buffer[0] == '2') {
            std::cout << "Stop Filling and Draining" << std::endl;
            isDrain = 0;
            isFill = 0;
        }
        

    }
}

//Count Vloume percentage for tank
void checkVolumePer()
{
    while (true) {
        if (isFill&& volumePer>=0&& volumePer<100) {
            volumePer++;
        }
        if (isDrain && volumePer > 0 && volumePer <= 100) {
            volumePer--;
        }


        Sleep(500);
    }
}

//send message to display
void senderM(void* socket1 )
{
    while (true) {
        
        std::stringstream  stream;
        sf::UdpSocket* socket = (sf::UdpSocket*) socket1;
        char result = volumePer;
        std::string message;
        stream << volumePer;
        stream >> message;
        std::cout << volumePer << std::endl;
        socket->send(message.c_str(), sizeof(message) + 1, ip, port);
       
        if (isDrain) {
            if (isFill) {
                message = "a";
                socket->send(message.c_str(), sizeof(message) + 1, ip, port);
            }
            else {
                message = "b";
                socket->send(message.c_str(), sizeof(message) + 1, ip, port);
            }
        }
        else {
            if (isFill) {
                message = "c";
                socket->send(message.c_str(), sizeof(message) + 1, ip, port);
            }
            else {
                message = "d";
                socket->send(message.c_str(), sizeof(message) + 1, ip, port);

            }
        
        }

        Sleep(300);
    }
}














