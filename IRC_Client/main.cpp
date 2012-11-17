	//Alexandra Mahlmann, Carsten Petersen, Matthías Leó Gíslason
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

	const int BUFFER_SIZE = 1024;
	fstream file;

	void display();
	void logSentMess();
	void logRecvMess(char* buffer, int size);
	void getdata(char* buffer);


	int main(int argc, char* argv[])
	{
		file.open("irc.log",fstream::out | fstream::app);
		string hostname = argv[1];
		int port = 6667;
		char buffer [BUFFER_SIZE];
		int checksent, bsize;

	   // display();

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

		host = (hostent *) gethostbyname(hostname.c_str());
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
		{
			cerr << "Error connecting" << endl;
			return EXIT_FAILURE;
		}
		cout << "Connected to server: " << endl;
		//cout << "What nick name do you want (not more than 9 letters):" << endl;
		getdata(buffer);
		while (true)
		{
			bsize=0;

			for(int i = 0; i<2;i++)
			{
				bsize = recv(sock,recvData,BUFFER_SIZE,0);
				logRecvMess(recvData, bsize);
				cout << recvData;
				memset(&recvData,0,BUFFER_SIZE);
			}

			Sleep(60000);
			checksent = send(sock, buffer, strlen(buffer), 0);
			logSentMess();

			bsize = recv(sock,recvData,BUFFER_SIZE,0);
			logRecvMess(recvData, bsize);


			if(buffer[0] == 'Q' && buffer[1] == 'U' && buffer[2] == 'I' && buffer[3] == 'T')
				break;
		}

		shutdown(sock,port);
		closesocket(sock);
		WSACleanup();
		file.close();
		return EXIT_SUCCESS;
	}

	void getdata(char* buffer)
	{
		buffer[0] = 'Q';
		buffer[1] = 'U';
		buffer[2] = 'I';
		buffer[3] = 'T';
		buffer[4] = ' ';
		buffer[5] = ':';
		buffer[6] = 'M';
		buffer[7] = '\r';
		buffer[8] = '\n';
		buffer[9] = '\0';
	}

	void logSentMess()
	{
		string par = "/QUIT \r\n"; //to get it going we hardcode the message now

		time_t now = time(0);
		string nowTime = ctime(&now);
		nowTime.erase(nowTime.find('\n',0),1);
		string all = (nowTime + " : client : " + par);
		file << all;
	}

	void logRecvMess(char* buffer, int size)
	{
		time_t now = time(0);
		string nowTime = ctime(&now);
		thetime.erase(nowTime.find('\n',0),1);
		string all = (nowTime + " : server : ");
		cout << all;
		file << all;
		for (int i = 0; i<size; ++i)
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

