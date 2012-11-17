*****        Read-me-file for IRC client       *****  
* The Avengers: Alexandra, Carsten, Einar, Mattías *

Part I 

- argument to be put into command line is given in the upper comment part of the main file, for ex. 84.240.3.129 or irc.freenode.net

Many of the features we implemented in the first part will be refactorated and thought through during Part II and Part III.

We restricted our work to the following tasks:

1. get the socket connection to the Server into action
2. log the commands coming from Server into a file called irc.log and on the screen
3. send the message QUIT to Server
4. close file and socket appropriately

We were told that it should not be nesseccary to implement diffrent threads in Part I, so we hardcoded some information in order to get the log going. Looping through to get all lines from the server will amomg others be changed in Part II.
