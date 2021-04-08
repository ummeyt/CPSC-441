/****************************************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 441
 * Tutorial:    03
 * Assignment:  4
 *
 * File name: jumpingBozons.c
 * Compile & run: please read the manual
 *
 * NOTE: Most of the starting code was provided by Reza.
 *       
 * TO GET BONUS RESULTS: comment out "response_time = getExponentialVar(Y);" 
 *                       and uncomment "response_time = 10.0;"
 *****************************************************************/

/* -------------- Libraries ------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* -------------- Global variables ------------------------ */
#define MAX_INT 2147483647      /* Maximum positive integer 2^31 - 1 */
#define SIM_END_TIME 1000000.0  /* Simulation duration */
#define SEED 12345              /* Seed value for random number generator */

/* Bozon states */
#define SILENT 0
#define MELODIOUS 1
#define SCREECHY 2

/* Defining event type indicators */
#define EVENT_TYPE1 1
#define EVENT_TYPE2 2

/* Debugging flag */
#define DEBUG 1

/* -------------- Structures ------------------------------ */
// Events can be defined as structures. Each event has at least two elements: 1.time 2.type
struct Event {
  double time;
  int type;
};

// You can define your program entities (e.g. Bozon, Atmosphere and etc.) as structures.
struct Bozon {
  // State variables
  int status;                // Represents the status of the Bozon. It can be SILENT, MELODIOUS, or SCREECHY
  double silent_start_time;
  double melodious_start_time;
  double screechy_start_time;

  // Statistical counters for the Bozon
  double total_silent_time;
  double total_melodious_time;
  double total_screechy_time;
  int current_yodeling_num;
};

/* -------------------- Function declarations ------------------- */
double uniform01(void);
double getExponentialVar(double mean);

/* -------------------- Function definitions ------------------- */
int main(int argc, char ** argv)
{
  if (argc < 3) {
    printf("Please put three arguments: <M> <Sleep time> <Yodeling time>\n");
    exit (-1);
  }
  
  int M = atoi(argv[1]);	/* Set number of bozons */             
  int S = atoi(argv[2]);        /* Set mean sleep duration */
  int Y = atoi(argv[3]);        /* Set mean yodel duration */

  /* ******************* Defining Variables *************************** */
  // Simulator variables (e.g. time, event list, entities, statistical counters and control variables)
  struct Bozon bozon;
  struct Event curr_event;    // For storing the most imminent event fetched from the event_list in each iteration
  struct Event next_event;    // A dummy variable for creating next event in each iteration
  struct Event event_list[M]; // An array of events
  int curr_event_index;       // Shows the index of the current event in the event_list 
  double curr_event_time;     // Holds the time of the most imminent event in eachiteration
  double curr_time;           // Represents the current time of the simulator
  double response_time;       // Holds the generated response time for bozon
  double attempted_yodel;
  double perfect_yodel;
  /* Control variables for calculating SILENT, MELODIOUS, and SCREECHY periods of Bozon */

  /* ************************* Initialization ************************ */
  /* Initializing all Bozon's state variables and statistical counters */
  bozon.status = SILENT;
  bozon.silent_start_time = 0.0;
  bozon.melodious_start_time = 0.0;
  bozon.screechy_start_time = 0.0;

  bozon.total_silent_time = 0.0;
  bozon.total_melodious_time = 0.0;
  bozon.total_screechy_time = 0.0;
  bozon.current_yodeling_num = 0;

  /* Initializing time and event index of the simulator */
  curr_time = 0.0;
  curr_event_index = 0;

  /* Initializing RNG (Random Number Generator) seed*/
  srandom(SEED);
  
  /* Generating initial SILENT events before loop starts */ 
  for (int i = 0; i < M; i++) {
    response_time = getExponentialVar(S);  //sleep time mean
    next_event.time = curr_time + response_time;
    next_event.type = EVENT_TYPE1;
    event_list[i] = next_event;
  }

  /* Main loop */
  while (curr_time < SIM_END_TIME) {
    // Finding the most imminent event in the event list and storing it in curr_event for execution
    double min = SIM_END_TIME;
    for (int i = 0; i < M; i++) {
      if (event_list[i].time < min) {
      	min = event_list[i].time;
      	curr_event_index = i;
      }
    }
    // Updating the time variable to the current time (time of the current event you want to execute) of the system
    curr_event = event_list[curr_event_index];
    curr_time = curr_event.time;

    switch (curr_event.type) {
    case EVENT_TYPE1:
      // Updating statistical counter
      bozon.current_yodeling_num++;
      if (bozon.status == SILENT) {
	// Updating state variable
	bozon.status = MELODIOUS;
	bozon.melodious_start_time = curr_time;
 	// Updating statistical counter
	bozon.total_silent_time += curr_time - bozon.silent_start_time;
	// Generating new event
	response_time = getExponentialVar(Y);
	//	response_time = 10.0;   //BONUS
	next_event.time = curr_time + response_time;
	next_event.type = EVENT_TYPE2;
	attempted_yodel++;
	event_list[curr_event_index] = next_event;
      }
      else if (bozon.status == MELODIOUS) {	
	// Updating statistical counter
	bozon.status = SCREECHY;
	bozon.screechy_start_time = curr_time;
 	// Updating statistical counter
	bozon.total_melodious_time += curr_time - bozon.melodious_start_time;  
	// Generating new event
	response_time = getExponentialVar(Y);
	//	response_time = 10.0;   //BONUS
	next_event.time = curr_time + response_time;	
	next_event.type = EVENT_TYPE2;
	attempted_yodel++;
	event_list[curr_event_index] = next_event;
      }
      else if (bozon.status == SCREECHY) {
 	// Updating statistical counter
	bozon.total_screechy_time += curr_time - bozon.screechy_start_time;  	
	bozon.screechy_start_time = curr_time;
	// Generating new event
	response_time = getExponentialVar(Y);
	//	response_time = 10.0;   //BONUS
	next_event.time = curr_time + response_time;
	next_event.type = EVENT_TYPE2;
	attempted_yodel++;
	event_list[curr_event_index] = next_event;
      }
      break;
    case EVENT_TYPE2:
      // Updating statistical counter
      bozon.current_yodeling_num--;
      if (bozon.status == MELODIOUS)
    	{
	  perfect_yodel++;

	  // Updating state variable
    	  bozon.status = SILENT;
	  // Updating statistical counter    	  
    	  bozon.total_melodious_time += curr_time - bozon.melodious_start_time;
    	  bozon.silent_start_time = curr_time;
	  // Generating new event
	  response_time = getExponentialVar(S);
	  next_event.time = curr_time + response_time;
          next_event.type = EVENT_TYPE1;
          event_list[curr_event_index] = next_event;
    	}
      else
	{
	  // if there is more than 1 bozon yodeling at the same time
	  if (bozon.current_yodeling_num >= 2)
	    {
	      // Updating state variable
  	      bozon.status = SCREECHY;
	      // Updating statistical counter    	  
  	      bozon.total_screechy_time += curr_time - bozon.screechy_start_time;
  	      bozon.screechy_start_time = curr_time;
	      // Generating new event
	      response_time = getExponentialVar(S);
	      next_event.time = curr_time + response_time;
  	      next_event.type = EVENT_TYPE1;
	      if (bozon.status == MELODIOUS)
		perfect_yodel++;
  	      event_list[curr_event_index] = next_event;
	    }
	  // else if only one bozon is yodeling
	  else if (bozon.current_yodeling_num == 1)
	    {
	      // Updating state variable
	      bozon.status = MELODIOUS;
	      // Updating statistical counter	  
	      bozon.total_screechy_time += curr_time - bozon.screechy_start_time;
	      bozon.melodious_start_time = curr_time;
	      // Generating new event
	      response_time = getExponentialVar(S);
	      next_event.time = curr_time + response_time;
	      next_event.type = EVENT_TYPE1;
	      if (bozon.status == MELODIOUS)
		perfect_yodel++;
	      event_list[curr_event_index] = next_event;
	    }
	}
      break;
    default:
      printf("Invalid type of event: %d", curr_event.type);
    }
  }

  // Updating statistical counters one last time based on the most recent state of the system
  if (bozon.status == SCREECHY) 
    bozon.total_screechy_time += curr_time - bozon.screechy_start_time;
  else if (bozon.status == MELODIOUS)
    bozon.total_melodious_time += curr_time - bozon.melodious_start_time;
  else if (bozon.status == SILENT)
    bozon.total_silent_time += curr_time - bozon.silent_start_time;

  // Report
  printf("\tTotal time observing channel: %.3f\n", SIM_END_TIME);
  printf("\tTotal silent time = %f\n", bozon.total_silent_time);
  printf("\tTotal melodious time = %f\n", bozon.total_melodious_time);
  printf("\tTotal screechy time = %f\n", bozon.total_screechy_time);
  printf("\tSILENT time on the channel = %.3lf%c\n", 100 * ((float)bozon.total_silent_time / curr_time), '%');
  printf("\tMELODIOUS time on the channel = %.3lf%c\n", 100 * bozon.total_melodious_time / curr_time, '%');
  printf("\tSCREECHY time on the channel = %.3lf%c\n", 100 * bozon.total_screechy_time / curr_time, '%');
  printf("\tAttempted yodel time = %f\n", attempted_yodel);
  printf("\tPerfect yodel percentage = %.3lf%c\n", 100 * (perfect_yodel / attempted_yodel), '%');
  return 0;
}

/* Generate a random floating point number uniformly distributed in [0,1] */
double uniform01()
{
  double randnum;

  /* Get a random positive integer from random() */
  randnum = (double) 1.0 * random();

  /* Divide by max integer to get something in the range 0.0 to 1.0 */
  randnum = randnum / (1.0 * MAX_INT);
  return randnum;
}

/* Generate a random floating point number exponentially distributed with mean = m */
double getExponentialVar(double m)
{
  double randnum, ans;
  randnum = uniform01();
  ans = -(m) * log(randnum);
  return ans;
}
