/****************************************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 441
 * Tutorial:    03
 * Assignment:  2
 *
 * File name: cryptoserver.cpp
 * Compile & run: please read the manual
 * NOTE: Most of the following code has been copied directly from 
 *       Carey's TCP wordlengthserver.c example with a few changes
 *       
 *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>

#include <time.h>
#include <iostream>
#include <cstring>
#include <map>
#include <iterator>
#include <algorithm>
#include <numeric>

using namespace std;

/* Global manifest constants */
#define MAX_MESSAGE_LENGTH 9999
#define MYPORTNUM 12346

/* Global variable */
int childsockfd;

bool isWord(char str[]);
void sequentialIndex(char str[]);
void wordSum(char str[]);
void myHash(char str[]);

/* This is a signal handler to do graceful exit if needed */
void catcher( int sig )
{
  close(childsockfd);
  exit(0);
}

/* Main program for server */
int main()
{
  struct sockaddr_in server;
  static struct sigaction act;
  char messagein[MAX_MESSAGE_LENGTH];
  char messageout[MAX_MESSAGE_LENGTH];
  int parentsockfd;
  int i, j;
  int pid;
  char c;

  /* Set up a signal handler to catch some weird termination conditions. */
  act.sa_handler = catcher;
  sigfillset(&(act.sa_mask));
  sigaction(SIGPIPE, &act, NULL);

  /* Initialize server sockaddr structure */
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(MYPORTNUM);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  /* set up the transport-level end point to use TCP */
  if( (parentsockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
    {
      fprintf(stderr, "server: socket() call failed!\n");
      exit(1);
    }
  printf("server: socket() call successful.\n");
  
  /* bind a specific address and port to the end point */
  if( bind(parentsockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in) ) == -1 )
    {
      fprintf(stderr, "server: bind() call failed!\n");
      exit(1);
    }
  printf("server: bind() call successful.\n");
  
  /* start listening for incoming connections from clients */
  if( listen(parentsockfd, 5) == -1 )
    {
      fprintf(stderr, "server: listen() call failed!\n");
      exit(1);
    }
  printf("server: listen() call successful.\n");
  
  /* initialize message strings just to be safe (null-terminated) */
  bzero(messagein, MAX_MESSAGE_LENGTH);
  bzero(messageout, MAX_MESSAGE_LENGTH);

  fprintf(stderr, "\nWelcome! I am the server end of the client-server demo!!\n");
  fprintf(stderr, "server listening on port %d...\n", MYPORTNUM);

  /* Main loop: server loops forever listening for requests */
  while (1)
    {
      /* accept a connection */
      if( (childsockfd = accept(parentsockfd, NULL, NULL)) == -1 )
	{
	  fprintf(stderr, "server: accept() call failed!\n");
	  exit(1);
	}
      printf("server: accept() call successful.\n");
      
      /* don't need the parent listener socket that was inherited */
      close(parentsockfd);

      /* obtain the message from this client */
      while( recv(childsockfd, messagein, MAX_MESSAGE_LENGTH, 0) > 0 )
	{
	  /* print out the received message */
	  printf("Received message: %s\n", messagein);


	  //ONLY ONE HASH FUNCTION CALL ALLOWED IN MAIN OUT OF THREE
	  sequentialIndex(messagein);
	  //  	  wordSum(messagein);
	  //	  myHash(messagein);

	  /* create the outgoing message (as an ASCII string) */
	  sprintf(messageout, "%d\n", strlen(messagein));

	  /* send the result message back to the client */
	  send(childsockfd, messageout, strlen(messageout), 0);

		
	  /* clear out message strings again to be safe */
	  bzero(messagein, MAX_MESSAGE_LENGTH);
	  bzero(messageout, MAX_MESSAGE_LENGTH);
	}

      /* when client is no longer sending information to us, */
      /* the socket can be closed and the child process terminated */
      close(childsockfd);
      exit(0);      
    }
}

/*********************************************************************************
 * isWord
 * The sole purpose of this function is to check if the given string consist of 
 * only digits or not. If it isn't just digits then it's a "word" so it will 
 * return true, but if the whole string is digits then it will return false so
 * that it is clear that the user is trying to decode  
 ********************************************************************************/
bool isWord(char str[]) {
  const int string_len = atoi(str);
  for(int i = 0; i < string_len; ++i) {
    if(!isdigit(str[i])) 
      return false;
  }
  return true; 
}

/*********************************************************************************
 * Sequential Index Hash
 * Server incrementally builds its lexican form the observed words & assigns the
 * next integer value to the new word seen.
 * Ex. Given a string “I am very very happy”: 1 is assigned to “I”, 2 to “am” and
 *     3 to “very”. However, since “very” is a repeated word, it should be 
 *     assigned as 3 again. Since it is a one-to-one mapping, if "I" is encoded 
 *     as 1, 1 should be decoded as "I"
 ********************************************************************************/
void sequentialIndex(char str[])
{
  map<int, string> indexMap;          //the map       
  map<int, string>::iterator it;      //an iterator to store the return value
  char* point;                        //a char pointer to store the current token
  int wordNum = 0;                    //the number assigned to the word
  bool check = false;		      //bool to check if it's a "word" or not

  check = isWord(str);                    //check if it's a word
  if (check)                              //if it is a "word"
    {
      //ENCODING
      //split the lines according to the delimiters
      point = strtok(str, " ,.?!");
      while (point != NULL)
	{
	  wordNum++;                      //increment the word number
	  //put the word number and word in map
	  indexMap.insert(pair<int, string>(wordNum, point)); 
	  cout << "0x" <<wordNum << " ";
	  point = strtok(NULL, " ,.?!");  //check the next token
	}
      cout << endl;
    }
  else {
    //DECODING
    //    cout << "Decode: " << endl;
    //    cin.getline(str, 100);
    //split the lines according to the delimiters
    point = strtok(str, " 0x,.?!");

    //Loop through the map
    for (it = indexMap.begin(); point != NULL; it++)
      {
	wordNum = atoi(point);            //char * to int
	it = indexMap.find(wordNum);      //find the wordNum in the map
	if (it == indexMap.end()){        //if it's not in the map
	  cout<<"Decode '0x" << wordNum << "' not found ";
	} else {
	  cout << it->second << " ";      //if it's in the map then print it out
	}
	point = strtok(NULL, "0x,.?!");   //check the next token
      }
    cout << endl;
  }
}
/*********************************************************************************
 * Word Sum Hash
 * The hashes are fixed size & are constructed by summing the ASCII values of the
 * chars in a word.
 * Ex. The string "how are you" should be encoded as "334 312 349". The ASCII
 *     values of the chars "h", "o", and "w" are 104, 111, and 119, which sums up
 *     to be 334. So all the word “how” should be encoded as 334, and the number 
 *     334 should be decoded as “how”.
 ********************************************************************************/
void wordSum(char str[])
{
  map<int, string> sumMap;         //the map
  map<int, string>::iterator it;   //an iterator to store the return value
  int wordSum = 0;                 //the sum of the word
  char* point;                     //a char pointer to store the current token
  bool check = false;		   //bool to check if it's a "word" or not

  check = isWord(str);                    //check if it's a word
  //  cout << "isWord: " << check << endl;//DEBUG
  if (check)                              //if it is a "word"
    {
      //ENCODING
      //split the lines according to the delimiters
      point = strtok(str, " ,.?!");       
      while (point != NULL)
	{
	  wordSum = 0;
	  size_t len = strlen(point);     
	  for (unsigned int j = 0; j < len; j++) {
	    //add up the sum with each char ASCII value
	    wordSum += point[j];           
	  }
	  //put the word and ASCII value in the map
	  sumMap.insert(pair<int, string>(wordSum, point)); 
	  cout << wordSum << " ";
	  point = strtok(NULL, " ,.?!");  //check the next token
	}
      cout << endl;
    }

  else
    {
      //DECODE
      //split the lines according to the delimiters
      point = strtok(str, " ,.?!");      
      //      cout << "current 'word': " << point << endl;  //DEBUG
      //Loop through the map
      for (it = sumMap.begin(); point != NULL; it++)
	{
	  wordSum = atoi(point);         //char * to int
	  it = sumMap.find(wordSum);     //find the pair with the given decode
	  //	  cout << "word sum:" << wordSum << endl;    //DEBUG
	  
	  if (it == sumMap.end()){       //if it's not found in the map
	  cout<< "Decode '" << wordSum << "' not found ";
	  } else {
	    cout << it->second << " ";   //else print the found pair
	  }
	  point = strtok(NULL, " ,.?!"); //check the next token
	  //	  cout << "current 'word': " << point << endl;//DEBUG  
	}
      cout << endl;
    }
}

/*********************************************************************************
 * My Hash
 * This is a scheme of my design. My hash takes in a string and encodes it as a 
 * random number from the built in rand() in the range 1 to 1000. Since it maps
 * the word to the random value, its just as easy to decode as it simply takes the
 * given random numbers and searches the map to find its pair.
 * Ex. The string "how are you" could be encoded as "415 598 305" and if you were
 *     to decode "415 598 305" then it would be "how are you".
 ********************************************************************************/
void myHash(char str[])
{
  map<int, string> myMap;
  map<int, string>::iterator it;
  int randomNum = 0;
  bool check = false;
  char* point;
    
  check = isWord(str);                    //check if it's a word
  if (check)                              //if it is a "word"
    {
      //ENCODING
      //split the lines according to the delimiters
      point = strtok(str, " ,.?!");      
      while (point != NULL)                
	{
	  randomNum = rand() % 1000 + 1; //generate number between 1 and 1000
	  //put the pair in the map
	  myMap.insert(pair<int, string>(randomNum, point)); 
	  cout << randomNum << " ";
	  //      cout << "rand: " << randomNum << "\tword: " << point << endl;
	  point = strtok(NULL, " ,.?!"); //check the next token
	}
      cout << endl;
    }
  else {
    //DECODING
    //   cout << "Decoding: " << endl;
    //    cin.getline(str, 100);
    //split the lines according to the delimiters    
    point = strtok(str, " ,.?!"); 

    //Loop through the map
    for (it = myMap.begin(); point != NULL; it++)
      {
	randomNum = atoi(point);       //char * to int
	it = myMap.find(randomNum);    //find the pair with the given random number
	if (it == myMap.end()){        //if the random number is not in the map
	  cout<<"Decode '" << randomNum << "' not found ";
	} else {
	  cout << it->second << " ";   //else print the word paired with the number
	}
	point = strtok(NULL, " ,.?!"); //check the next token
      }
    cout << endl;
  }
}
