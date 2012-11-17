//Alexandra Mahlmann, Carsten Petersen, Matthias Leo Gislason, Einar Orn Gissuarsson
//IRC Client
//Fall 2012
//Server:  irc.freenode.net
//ServerIP: 84.240.3.129 - cameron.freenode.net (130.239.18.172 also possible)

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <winsock2.h>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <unistd.h>

using namespace std;

string CRLF = "\r\n";	// Carriage return, Line Feed
const int BUFFER_SIZE = 1024;
fstream file;

void display(); //display's all the commands
void logSentMess(); //logs the message from the client
void logRecvMess(char* buffer, int size); //log the server response
void getdata(char* buffer, string comm, string mess); //Place the request to the server into the buffer
string commMess;


int main(int argc, char* argv[])
{
    file.open("irc.log",fstream::out | fstream::app);
    string hostname = "";
    int port = 6667;
    char buffer [BUFFER_SIZE];
    int checksent, bsize;
    string mess, comm;

    // display();  //show all the commands

    ////******* from UDP server********
    WSADATA wsaData;
    WORD version = MAKEWORD(2, 0);
    if (WSAStartup(version, &wsaData))
    {
        cerr << "Failure to find WinSock 2.2 or better." << endl;
        return EXIT_FAILURE;
    }

    SOCKET sock;
    sockaddr_in serverAddr;
    hostent *host;
    char recvData[BUFFER_SIZE];
    memset(&recvData,0,BUFFER_SIZE);
    char sendData[BUFFER_SIZE];

    cout << "Enter the address of the server :" << endl;
    cout << "Example : 84.240.3.129" << endl;
    cin >> hostname;

    //open socket
    host = (hostent *) gethostbyname(hostname.c_str());
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        cerr << sock << endl;
        cerr << "Error opening socket! " << endl;
        return EXIT_FAILURE;
    }

    //server info
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr = *((struct in_addr *) host->h_addr);
    memset(&(serverAddr.sin_zero), 0, 8);

    if(connect(sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0)//check if connection was sucessful
    {
        cerr << "Error connecting" << endl;
        return EXIT_FAILURE;
    }

    cout << "Connected to server: " << endl;
    cout << "Please make your legal command: " << endl;
    cin >> comm;
    cout << "Please enter a message if following the command: " << endl;
    cin >> mess;

    getdata(buffer, comm, mess);
    //cout << "*****" << strlen(buffer);
    send(sock, buffer, strlen(buffer), 0);

    while (true)
    {
        bsize=0;

        for(int i = 0; i<2; i++) //get the servers response
        {
            bsize = recv(sock,recvData,BUFFER_SIZE,0);
            logRecvMess(recvData, bsize);
            cout << recvData;
            memset(&recvData,0,BUFFER_SIZE); //fill the space behind the data with zeros
        }

        Sleep(60000); //sleep gracefully for 60 seconds

        checksent = send(sock, buffer, strlen(buffer), 0);
        logSentMess();

        bsize = recv(sock,recvData,BUFFER_SIZE,0);
        logRecvMess(recvData, bsize);


        if(buffer[0] == 'Q' && buffer[1] == 'U' && buffer[2] == 'I' && buffer[3] == 'T') //if quit command is recived then break the loop
            break;
    }

    //close all resources
    shutdown(sock,port);
    closesocket(sock);
    WSACleanup();
    file.close();
    return EXIT_SUCCESS;
}

void getdata(char* buffer, string comm, string mess) //Place the request to the server into the buffer
{
    string commMess = comm + " :" + mess + CRLF;
    //cout<< "commMess: "<< commMess << commMess.size()<< endl;
    for (int a=0;a<=commMess.size();a++)
        {
            buffer[a]=commMess[a];
        }
    int len = strlen(buffer);
    buffer[len++] = '\0';
    //cout << "buffer"<< buffer << len <<endl;
}

void logSentMess() //logs the message sent from the client to the server
{
    string par = "/QUIT \r\n"; //to get it going we hardcode the message now

    time_t now = time(0);
    string nowTime = ctime(&now);
    nowTime.erase(nowTime.find('\n',0),1);
    string all = (nowTime + " : client : " + par); //compose the string
    cout << all;
    file << all;  //write the string to the file
}

void logRecvMess(char* buffer, int size)  //log the server response
{
    time_t now = time(0);
    string nowTime = ctime(&now);
    nowTime.erase(nowTime.find('\n',0),1);
    string all = (nowTime + " : server : "); //compose the message
    cout << all;
    file << all;

    for (int i = 0; i<size; ++i) //write the response from the buffer to the file
        file << buffer[i];
    file << endl;

}

//Echoing the diffrent commands and parameters
void display()
{
    cout << "\n*********** [COMMANDS] and <Parameters> for the IRC Client ******************* " << endl;
    cout << "\nTo create a new channel use the command JOIN (you then are the channel operator)" << endl;
    cout << "[JOIN] <channel>{,<channel>} [<key>{,<key>}] /Example: JOIN &foo fubar " << endl;
    cout << "[KICK] <channel> <user> [<comment>] /Example: KICK #Finnish John :Spoke English" << endl;
    cout << "[MODE] <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>][<user>][<ban mask>] " << endl;
    cout << setw(10) << "o "<< "- give/take channel operator privileges" << endl;
    cout << setw(10) << "p "<< "- private channel flag" << endl;
    cout << setw(10) << "s "<< "- secret channel flag" << endl;
    cout << setw(10) << "i "<< "- invite-only channel flag" << endl;
    cout << setw(10) << "t "<< "- no messages to channel from clients on the outside" << endl;
    cout << setw(10) << "n "<< "- private channel flag" << endl;
    cout << setw(10) << "m "<< "- moderated channel" << endl;
    cout << setw(10) << "l "<< "- set the user limit to channel" << endl;
    cout << "[INVITE] <nickname> <channel> /Example: INVITE Wiz #Twilight_Zone" << endl;
    cout << "[TOPIC] <channel> [<topic>]" << endl;
    cout << "[NICK] <nickname> {[+|-]|i|w|s|o}" << endl;
    cout << "[QUIT][<Quit message>] /Client connection closed /Example: QUIT :gone to lunch" << endl;

}
