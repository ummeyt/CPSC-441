/**********************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 441
 * Tutorial:    03
 * Assignment:  1
 * Usage: Please read manual.txt
 *
 * File name: proxy.c
 *********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <time.h>
#define PORT 12345
#define BACKLOG 10
#define MAX_BUFF_SIZE 6000  //maximum buffer size

#define bool int
#define TRUE 1
#define FALSE 0


int main()
{
  //Address initialization
  struct sockaddr_in server_addr;  //server address to connect to
  int mysocket1, s_socket2;
  
  memset(&server_addr,0,sizeof(server_addr));
  server_addr.sin_family = AF_INET; //Address family
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // or  =inet_addr("127.0.0.1");

  
  //Socket creation
  mysocket1 = socket(AF_INET,SOCK_STREAM,0); //Type: INT //socket(domain, type, protocol)
  if(mysocket1==-1)
    {
      printf("mysocket1 creation failed.\n");
      close(mysocket1);  
      exit(-1);
    }
    else
    printf("mysocket1 created.\n");

  
  //Binding 
  int status =bind(mysocket1, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if(status==-1)
    {
      printf("Binding failed.\n");
      close(mysocket1);  
      exit(-1);
    }
  else
    printf("Binded\n");

  
  int count;
  int mysocket2;

  while (1)
    {
      //Listening
      status=listen(mysocket1,5);//5 connections can listen
      if(status==-1)
	{
	  printf("Listening failed.\n");
	  close(mysocket1);  
	  exit(-1);
	}
      else
	printf("Listening...\n");

      
      // Accept a connection
      s_socket2 = accept(mysocket1,NULL,NULL);
    
      if (s_socket2==-1)
	{
	  printf("Error in accepting\n");
	  close(s_socket2);  
	  exit(-1);
	}
      else
	printf("Connection established\n");

      
      //Receive data
      char rcv_message[MAX_BUFF_SIZE];
      count = recv(s_socket2, rcv_message, sizeof(rcv_message), 0);
      if (count==-1)
	{
	  printf("Error in receving\n");
	  close(mysocket1);
	  close(s_socket2);  
	  exit(-1);
	}
	else
	{
	  printf("Receiving.\n");
	  printf("Client said: %s\n", rcv_message);
	}

      
      //Address initialization
      struct addrinfo client_addr, *res;
      memset(&client_addr, 0, sizeof(client_addr));
      client_addr.ai_family = AF_UNSPEC;
      client_addr.ai_socktype = SOCK_STREAM;
      client_addr.ai_flags = AI_PASSIVE;

      if ((status = getaddrinfo("pages.cpsc.ucalgary.ca", "http", &client_addr, &res)) != 0) {
	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	exit(1);
      }

      status = getaddrinfo("pages.cpsc.ucalgary.ca/~carey/CPSC441/test1.html", "http", &client_addr, &res);
      mysocket2= socket(res->ai_family, res->ai_socktype, res->ai_protocol);

      if(mysocket2==-1)
	{
	  printf("Socket2 creation failed!!\n");
	  close(mysocket1);
	  close(s_socket2);
	  close(mysocket2);
	  exit(-1);
	}
      else
	printf("Socket2 created.\n");
      
      
      //Connect cilent
      status=connect(mysocket2, res->ai_addr, res->ai_addrlen);
      if(status==-1)
	{
	  printf("Accept() Failed.\n");
	  close(mysocket1);
	  close(s_socket2);
	  close(mysocket2);
	  exit(-1);
	}
      else
	printf("Socket2 connection established.\n");

      
      //Send data
      count = send(mysocket2, rcv_message, sizeof(rcv_message), 0);
      if (count==-1)
	{
	  printf("Error in send()\n");
	  close(mysocket1);
	  close(s_socket2);
	  close(mysocket2);
	  exit(-1);
	}
	else
	  printf("Sent\n");

	
      //Receive data
      char snd_message[MAX_BUFF_SIZE];
      count = recv(mysocket2, snd_message, sizeof(snd_message), 0);
      if (count==-1)
	{
	  printf("Error in recv()\n");
	  close(mysocket1);
	  close(s_socket2);
	  close(mysocket2);
	  exit(-1);
	}
      else 
	printf("%s\n", snd_message);

      send(s_socket2, snd_message, sizeof(snd_message), 0);

      //      contentLength(snd_message);
      
      //      printf("Parsing snd_message:\n");
      //      parseHTML(snd_message);
      //      bzero(snd_message, MAX_BUFF_SIZE);

      //      printf("Parsing html:\n");      
      //      parseHTTP(snd_message);
      //      bzero(snd_message, MAX_BUFF_SIZE);
      close(mysocket2);  
    }	
  //Close all the sockets
  close(s_socket2);
  close(mysocket1);
  exit(-1);
  return 0;
}


  //Parsing through the HTML pages
void parseHTML(char str[])
{
  char *token = NULL;
  //get the first token
  token = strtok(str, "\n\n");  

  //walk through other tokens
  while (token) {
    printf("%s", token);
    token = strtok(NULL, " ");
  }
}


//The length of the content - "Content-Length"
void contentLength(char str[])
{
  bool next=FALSE;
  long length=0;
  // get the first token
  char *token=strtok(str,"\r\n\t: ");
  while(token){
    //if next is true, convert the string token to a long, and save the value, else remain zero.
    length=next?atol(token):length;
    //set next to true/false, depending on whether we see content length or not
    next= !strcmp(token,"Content-Length");
    //    printf("length: %ld, next: %d, strcmp(): %d,  !strcmp(): %d, token: %s\n",
    //            length,next,strcmp(token,"content-length"),!strcmp(token,"content-length"),token);
    // get the next token
    token=strtok(NULL,"\r\n\t: ");
  }
  printf("The length is %ld\n\n", length);
}


void parseText(char str[])
{
  bool text = FALSE;
  char randomletter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[random () % 52];

  char *token=strtok(str, "\r\n\r\n");
  while (randomletter)
    {
     printf("%s", token);
//     token=strtok(NULL, "\n");
     text = TRUE;
    }
}

//Parsing HTTP requests + responses
void parseHTTP(char str[])
{
   char *token = NULL;

   const char begin = ' ';
   char* lineptr = strchr(str,begin);
   
   //get the first token
   token = strtok(str, " ");
   
   //walk through other tokens
   while (token) {
     printf("%s", token);
     token = strtok(NULL, " ");
     printf(" ");
   }
   printf("%s\n", lineptr);
}


//Prints a random character
void randomChar()
{
  time_t t;
  srand((unsigned) time(&t));
  char randomletter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[random () % 52];
  printf("%c", randomletter);
}

  /*  char *token = NULL;
  const char begin = 'C';
  char* lineptr = strchr(str,begin); //returns a pointer to the first occurrence of the character C in the string str, or NULL if char not found
  printf("lineptr: %s\n", lineptr);    }
    */

//Prints random characters to form "words"
void rword (char *word)
{
  int len = rand () % 6 + 1;
  word [len] = 0;
    while (len)
      word [--len] = 'a' + rand () % 26;
}
