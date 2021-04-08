/****************************************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 441
 * Tutorial:    03
 * Assignment:  2
 *
 * File name: cryptoclient.c
 * Compile & run: please read the manual
 *
 * NOTE: Most of the following code has been copied directly from 
 *       Carey's TCP wordlengthclient.c example with a few changes
 *       
 *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

/* Some generic error handling stuff */
extern int errno;
void perror(const char *s);

/* Manifest constants used by client program */
#define MAX_HOSTNAME_LENGTH 64
#define MAX_WORD_LENGTH 9999
#define MYPORTNUM 12346   /* must match the server's port! */

/* Menu selections */
#define ALLDONE 0
#define ENCODE 1
#define DECODE 2

/* Prompt the user to enter a word */
void printmenu()
{
  printf("\n");
  printf("Please choose from the following selections:\n");
  printf("  1 - Encode a message\n");
  printf("  2 - Decode a message\n");
  printf("  0 - Exit program\n");
  printf("Your desired menu selection? ");
}

/* Main program of client */
int main()
{
  int sockfd, sockfd2;
  char c;
  struct sockaddr_in server;
  struct hostent *hp;
  char hostname[MAX_HOSTNAME_LENGTH];
  char message[MAX_WORD_LENGTH];
  char messageback[MAX_WORD_LENGTH];
  int choice, len, bytes;

  /* Initialization of server sockaddr data structure */
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(MYPORTNUM);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  /* hard code the IP address so you don't need hostname resolver */
  server.sin_addr.s_addr = inet_addr("136.159.5.27");


  /* create the client socket for its transport-level end point */
  if( (sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
    {
      fprintf(stderr, "client: socket() call failed!\n");
      exit(1);
    }

  /* connect the socket to the server's address using TCP */
  if( connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1 )
    {
      fprintf(stderr, "client: connect() call failed!\n");
      perror(NULL);
      exit(1);
    }

  /* Print welcome banner */
  printf("Welcome! I am the client end of the client-server demo!!\n\n");
  printmenu();
  scanf("%d", &choice);

  /* main loop: read a word, send to server, and print answer received */
  while( choice != ALLDONE )
    {
      if( choice == ENCODE)
	{
	  /* get rid of newline after the (integer) menu choice given */
	  c = getchar();

	  /* prompt user for the input */
	  printf("Enter your encoding message: ");
	  len = 0;
	  while( (c = getchar()) != '\n' )
	    {
	      message[len] = c;
	      len++;
	    }
	  /* make sure the message is null-terminated in C */
	  message[len] = '\0';

	  /* send it to the server via the socket */
	  send(sockfd, message, len, 0);

	  /* see what the server sends back */
	  if( (bytes = recv(sockfd, messageback, len, 0)) > 0 )
	    {
	      /* make sure the message is null-terminated in C */
	      messageback[bytes] = '\0';
	      //	      printf("Encoded: ");
	      //	      printf("%s\n", messageback);
	    }
	}
      else if (choice == DECODE)
	{
	  /* get rid of newline after the (integer) menu choice given */
	  c = getchar();

	  /* prompt user for the input */
	  printf("Enter your decoding message: ");
	  len = 0;
	  while( (c = getchar()) != '\n' )
	    {
	      message[len] = c;
	      len++;
	    }
	  /* make sure the message is null-terminated in C */
	  message[len] = '\0';

	  /* send it to the server via the socket */
	  send(sockfd, message, len, 0);

	  /* see what the server sends back */
	  if( (bytes = recv(sockfd, messageback, len, 0)) > 0 )
	    {
	      /* make sure the message is null-terminated in C */
	      messageback[bytes] = '\0';
	      //	      printf("Decoding: ");
	      //	      printf("%s\n", messageback);
	    }
	
	  else
	    {
	      /* an error condition if the server dies unexpectedly */
	      printf("Server failed!\n");
	      close(sockfd);
	      exit(1);
	    }
	}
      else printf("Invalid menu selection. Please try again.\n");
      printmenu();
      scanf("%d", &choice);
    }

/* Program all done, so clean up and exit the client */
  close(sockfd);
  exit(0);
}
