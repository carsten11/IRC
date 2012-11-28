/** Alexandra Mahlmann, Carsten Petersen, Matthias Leo Gislason  or THE AVENGERS
    IRC Client - Computer Networking - Fall 2012
    Server:  irc.freenode.net
    ServerIP (frenode example): 84.240.3.129 - cameron.freenode.net (130.239.18.172 also possible)

    How to use:
    First insert the ServerIP address or ServerName into the "Program arguements" of your project.
    Be sure to have a txt file in the same folder as the project named irc.log.
    Winsock2.h has to be added as a library.
    Compile and Run project, giving up a desired nickname.
    Answers from Server and requests from Client are written into irc.log and on console for comfort.
    Just have in mind to press return after giving the command, then the channel to join or desired message.
    Unfortunately no threads are implemented, so that the recieve-sending process together
    is not possible.
    But after joining a channel, you see the list of members in that channel, and you can use all the desired
    commands: NICK, JOIN, PART, QUIT. Wireshark shows the same dialog between client and server.
    Should there be action in the channel, occationally they appear in the log/console as well.
**/

#include <iostream>   //to output
#include <cstring>
#include <stdlib.h>
#include <winsock2.h> //for all socket things
#include <fstream>    //for writing into file
#include <iomanip>
#include <ctime>     // for the time logged
#include <unistd.h>
#include <algorithm> // to use ToUpper()

using namespace std;

//*********************************************
//Global variables needed in various functions
//*********************************************
string CRLF = "\r\n\0";	// Carriage return, Line Feed
const int BUFFER_SIZE = 15000;
fstream file;

//*********************************************
//Functions implemented
//*********************************************

/**This function checks whather the command comming from user is legal**/
bool isvalid(string comm)
{
    transform(comm.begin(),comm.end(),comm.begin(), :: toupper);
    if (comm == "JOIN" || comm == "PART" || comm == "QUIT"|| comm == "NICK"|| comm == "PRIVMSG")
        return true;
}

/**function to form the string that goes into the file irc.log (and console)
Puzzling together time with the source (here SERVER), the colons and buffer content **/
void logRecvMess(char* buffer, int size)
{
    time_t now = time(0);
    string nowTime = ctime(&now);
    nowTime.erase(nowTime.find('\n',0),1);
    string all = (nowTime + " : server : "); //compose the message all together
    cout << all;
    file << all;

    for (int i = 0; i<size; ++i) //write the message from the buffer to the file
        file << buffer[i];
    file << endl;
}

/**string that goes into the file irc.log (and console). Same as above but from the CLIENT,
and it comes as a string, so no conversion**/
void logSentMess(string comm, string mess) //logs the message sent from the client to the server
{
    string par = comm + " " + mess; //to get it going we hardcode the message now
    time_t now = time(0);
    string nowTime = ctime(&now);
    nowTime.erase(nowTime.find('\n',0),1);
    string all = (nowTime + " : client : " + par + CRLF); //compose the string
    cout << all;
    file << all;  //write the string to the file
}

/**function that reads command and message together into a buffer and completely forms
the content needed for the Server to accept the content**/
void getdata(char* buffer, string comm, string mess) //Place the request to the server into the buffer
{
    string commMess = comm + " :" + mess + CRLF; //first forming one string of all letter/symbols needed
    for (int a=0; a<=commMess.size(); a++)
    {
        buffer[a]=commMess[a]; //reading string into buffer
    }
    int len = strlen(buffer);
    buffer[len++] = '\0';       //adding termination in the end
}

/**this function is the process for sending. It calls all 3 function needed.
getdata - send (included in <winsock2.h> - logSentMess**/
void logSent(SOCKET sock, char* buffer, string comm, string mess)
{
    getdata(buffer, comm, mess);
    send(sock, buffer, strlen(buffer), 0);
    logSentMess(comm, mess);
}

/**this function is the same as getdata in principle, but is used during chatting
that is only with the command "PREVMSG", puts everything together in the buffer**/
void getDataMsg(SOCKET sock, char* buffer, string comm, string mess, string channel)
{
    string commMess = comm + " "+ channel + " :" + mess + CRLF; //first forming one string of all letter/symbols needed
    for (int a=0; a<=commMess.size(); a++)
    {
        buffer[a]=commMess[a]; //reading string into buffer
    }
    int len = strlen(buffer);
    buffer[len++] = '\0';       //adding termination in the end
}
/**clearing buffer, which was a kind of debugging as well, but is not used at the moment**/
void clear(char buffer[BUFFER_SIZE])
{
    for (int i = 0; i<BUFFER_SIZE; i++)
        buffer[i]=0;
}

/**this function is checking the command given, in some cases we have to call
the recv() more often than once, a situation we would have gotten rid of with threads.**/
void checkRecv(SOCKET sock, string comm, int bsize,char* recvData)
{
    transform(comm.begin(),comm.end(),comm.begin(), :: toupper);
    if (comm == "JOIN")
    {
        bsize = recv(sock,recvData,BUFFER_SIZE,0); // these 3 functions complete the reciving process
        logRecvMess(recvData, bsize);              // reveice - log - cout - memset
        cout << recvData << endl;
        memset(&recvData,0,BUFFER_SIZE); // fill the space behind the data with zeros
    }
    else if (comm == "QUIT")
    {
        bsize = recv(sock,recvData,BUFFER_SIZE,0);
        logRecvMess(recvData, bsize);
        cout << recvData << endl;
        memset(&recvData,0,BUFFER_SIZE); //fill the space behind the data with zeros
    }

}

/**Not used at moment. Func is displaying most of the important commands possible to use in the IRC client /server.
This shows where we wanted to go, instead only the four most important commands are usable.**/
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

//************************************
//main function, test client
//************************************

int main(int argc, char* argv[])
{
    file.open("irc.log",fstream::out | fstream::app);   // file to log in
    string hostname = argv[1];                          // server name or ip as arguement from command line
    int port = 6667;                                    // IRC port on Server
    char buffer [BUFFER_SIZE];                          // buffer for char array sended
    int bsize;                                          // the output of the recv() function (size of recvData)
    string mess, nickn, comm, channel;                  // message, nickname, command
    SOCKET sock;                                        // Socket
    sockaddr_in serverAddr;                             // Socket stuff
    hostent *host;                                      // host declared
    char recvData[BUFFER_SIZE];                         // char array for data recieved
    memset(&recvData,0,BUFFER_SIZE);                    // clearing data recived array.

    //display();  //show all the commands, far from all are implemented

    ////******* from UDP server********
    WSADATA wsaData;
    WORD version = MAKEWORD(2, 0);
    if (WSAStartup(version, &wsaData))
    {
        cerr << "Failure to find WinSock 2.2 or better." << endl;
        return EXIT_FAILURE;
    }

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

    cout << "Connected to server! " << endl;
    comm = "NICK";
    cout << "Please choose a NICKNAME: ";
    cin >> nickn;

    //fills buffer with both strings and sends it to server
    logSent(sock,buffer,comm, nickn);
    //revieving answer and logging
    bsize = recv(sock,recvData,BUFFER_SIZE,0);
    logRecvMess(recvData, bsize);

    //send directly USER command, where the user does not need to give information
    comm = "USER AL 0 *";
    mess = ".";             //a little hacking going on to get the space and colon into USER message
    logSent(sock,buffer,comm,mess);

    //answer (more than 1200 letters!) recieved is logged and "couted"
    bsize = recv(sock,recvData,BUFFER_SIZE,0);
    logRecvMess(recvData, bsize);
    cout << recvData << endl;

    //then the goal was to get the two things going in this following while loop,
    //listening and recieving. We ended up in trouble implementing it all, but
    //managed to work around the corner and get all of the desired commands going.
    while (true)
    {
        bsize = recv(sock,recvData,BUFFER_SIZE,0);
        logRecvMess(recvData, bsize);
        cout << recvData << endl;
        memset(&recvData,0,BUFFER_SIZE); //fill the space behind the data with zeros

        cout << "Enter next command (JOIN, QUIT, PART, NICK, PRIVMSG): ";
        cin >> comm;
        while (!isvalid(comm))//checking whether the command is legal
        {
            cout << "This is not a valid commad, please try again" << endl;
            cin >> comm;
        }
        //in this paragraphs special conditions are checked.
        transform(comm.begin(),comm.end(),comm.begin(), :: toupper);
        //if the command is privmsg, the message following is sent diffrently
        //to the server then other messages, it calles "getDataMsg" (see functions)
        if (comm == "PRIVMSG")
            {
                cout << "Please enter your message (no # required): ";
                cin.ignore(1,'\r'); //ignores the end of the comm string(enter)
                getline(cin,mess);  //getline to get the whitspaces
                getDataMsg(sock,buffer,comm,mess,channel);
                send(sock, buffer, strlen(buffer), 0);
                logSentMess(comm, mess);
                continue;
            }
        //if the command is quit, the message is sent to server, but after having
        //done so, we end the while loop with "break" and shut down gracefully
        else if (comm == "QUIT")
            {
                cout << "Please enter a quit message if you want: ";
                cin.ignore(1,'\r');//ignores the end of the comm string(enter)
                getline(cin,mess);  //getline to get the whitspaces
                logSent(sock,buffer,comm,mess);
                checkRecv(sock, comm, bsize, recvData);
                bsize = recv(sock,recvData,BUFFER_SIZE,0);
                logRecvMess(recvData, bsize);
                cout << recvData << endl;
                memset(&recvData,0,BUFFER_SIZE); //fill the space behind the data with zeros
                break;
            }
        //if the command is not quit, we keep on in th el
        cout << "Please enter the channel/message (ex for channel: #test1): ";
        cin >> mess;
        if (comm == "JOIN")
            channel = mess;
        logSent(sock,buffer,comm,mess);
        checkRecv(sock, comm, bsize, recvData);
    }

    closesocket(sock);     //closing socket
    WSACleanup();           //clean up
    file.close();          // closing file
    return EXIT_SUCCESS;
}










