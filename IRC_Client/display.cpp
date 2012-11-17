//Alexandra Mahlmann
//IRC Client
//Fall 2012
//Servers: 130.239.18.172 - leguin.rcc.uma.se
//          84.240.3.129 - cameron.freenode.net

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <winsock2.h>
#include <fstream>
#include <iomanip>

using namespace std;

void display();

int main(int argc, char* argv[])
{
    string hostname = argv[1];
    int port = 6667;
    char buffer [256];
    int n;

    //display();

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
    //char sendData[1024];

    host = (hostent *) gethostbyname(hostname.c_str());
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << sock << endl;
        cerr << "Error opening socket! " << endl;
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr = *((struct in_addr *) host->h_addr);
    memset(&(serverAddr.sin_zero), 0, 8);

    if(connect(sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0)
        cerr << "Error connecting" << endl;

    cout << "Enter the message: \n";
    send(sock, buffer, sizeof(buffer), 0) != sizeof(strNumber)

  //numbytes = recv(sock, buf, MAXDATASIZE-1, 0)) == -1)
    shutdown(sock,port);
    closesocket(sock);
    WSACleanup();
    return EXIT_SUCCESS;
}




//Echoing the diffrent commands and parameters
void display()
{
    //cout << "****************************************************************************** " << endl;
    cout << "\n*********** [COMMANDS] and <Parameters> for the IRC Client ******************* " << endl;
    //cout << "****************************************************************************** " << endl;
    cout << "\nTo create a new channel use the command JOIN (you then are the channel operator)" << endl;
    cout << "[JOIN] <channel>{,<channel>} [<key>{,<key>}] /Example: JOIN &foo fubar " << endl;
    //cout << setw(15) << " creating " << "a new channel, you are the channel operater being able to use: " << endl;
    cout << "[KICK] <channel> <user> [<comment>] /Example: KICK #Finnish John :Spoke English" << endl;
   // cout << setw(15) << " example:" << " KICK #Finnish John :Speaking English" << endl;
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

    cout << "\n*********** Other [Commands] and <parameters> *************" <<endl;
    cout << "\n[PART] <channel>{,<channel>} /Client removed as active user/" << endl;
    cout << "[NICK] <nickname> {[+|-]|i|w|s|o}" << endl;
    cout << setw(10) << "i "<< "- marks a users as invisible" << endl;
    cout << setw(10) << "s "<< "- marks a user for receipt of server notices" << endl;
    cout << setw(10) << "w "<< "- user receives wallops" << endl;
    cout << setw(10) << "o "<< "- operator flag" << endl;
    cout << "[QUIT][<Quit message>] /Client connection closed /Example: QUIT :gone to lunch" << endl;
    cout << "PASS <password>" << endl;
    cout << "USER <username> <hostname> <servername> <realname>" << endl;
    cout << "SERVER <servername> <hopcount> <info>" << endl;
    cout << "OPER <user> <password>" << endl;
}

