/****************************************************************
 * Last Name:   Tashnim
 * First Name:  Ummey Zarin
 * Student ID:  30034931
 * Course:      CPSC 441
 * Tutorial:    03
 * Assignment:  3
 *
 * File name: qos.cpp
 * Compile & run: please read the manual
 *
 * NOTE: Please read the manual.
 *       
 *****************************************************************/

#include <stdio.h>
#include <iostream>
#include <queue>
#define FILENAME   "soccer.txt"            // input data file 
#define START      0.0
using namespace std;

double GetArrivalTime(FILE *fp)            // read the time stamp
{ 
  double a;

  fscanf(fp, "%lf", &a);
  return a;
}

double GetPacketSize(FILE *fp)             // read the packet size (in bytes)
{ 
  double s;

  fscanf(fp, "%lf\n", &s);
  return s;
}

int main(void)
{
  FILE *fp;                                // input data file 
  double initialarrival;
  double index;                            // job index
  double arrival;                          // arrival time of the packet
  double departure;                        // departure time of the time
  double delay;                            // queuing delay        
  double service = 0;                          // service time
  double packetsize;                       // packet size in bytes
  double packetloss;                       // number of packet lost
  double bandwidth = 0;                    // bandwidth (Mbps for now)
  double buffersize = 0;                   // size of the buffer
  queue<double> buffer;                    // queu to keeping track of the buffer status

  struct {                                 // sum of ...
    double delay;                          // queuing delay        
    double service;                        // service time
    double departure;                      // departure time of the time
  } sum = {0.0, 0.0, 0.0};

  // file handling
  fp = fopen(FILENAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open input file %s\n", FILENAME);
    return 1;
  }

  // printing the options
  cout <<
    "0 - WiFi Capacity: 11 Mbps, buffer size: 100\n" <<
    "1 - WiFi Capacity: 6 Mbps, buffer size: 100\n" <<
    "2 - WiFi Capacity: 54 Mbps, buffer size: 100\n" <<
    "3 - WiFi Capacity: 30 Mbps, buffer size: 100\n";
  
  int event_type;
  // prompt user to choose an option 
  cout << "Choose an event type: ";
  cin >> event_type;

  switch(event_type) {
  case 0:
    cout << "WiFi Capacity: 11 Mbps, buffer size: 100" << endl;
    buffersize = 100;
    bandwidth = (11*125000);                    // 11 bytes per sec
    break;
  case 1:
    cout << "WiFi capacity: 6 Mbps, buffer size: 100" << endl;
    buffersize = 100;
    bandwidth = (6*125000);                     // 6 bytes per sec
    break;
  case 2:
    cout << "WiFi capacity: 54 Mbps, buffer size: 100" << endl;
    buffersize = 100;
    bandwidth = (54*125000);                    // 54 bytes per sec
    break;
  case 3:
    cout << "WiFi capacity: 30 Mbps, buffer size: 100" << endl;
    buffersize = 100;
    bandwidth = (30*125000);                    // 30 bytes per sec
    break;
  default:
    cout << "Invalid option\n";
  }
 
  while (!feof(fp)) {
    index++;                                // increment index    
    arrival = GetArrivalTime(fp);           // get arrival time
    packetsize = GetPacketSize(fp);         // get packet size in bytes
    service = packetsize/bandwidth;         // calculate service time

    // while buffer is not empty and arrival >= next element in queue
    while(!buffer.empty() && arrival >= buffer.front()) {
      buffer.pop();                         // pop the packet from queue
    }

    if (buffer.size() < buffersize)         // if the buffer is occupied but not full
      {
        initialarrival = arrival;
        if(departure > arrival) {
          arrival = departure;
        }
  departure = arrival + service;            // departure time = arrival time + current service time
  buffer.push(departure);
  delay = departure - initialarrival;
      }
    
    if (buffer.size() == buffersize)        // if the buffer is full        
      {
	packetloss++;                       // increase packet loss counter
      }

    sum.delay += delay;                     // update delay    
  }
  
  printf("\tnumber of incoming packets  = %.2f packets\n", index);            // total packets
  printf("\tnumber of delivered packets = %.2f packets\n", index-packetloss); // delivered packets = total packets - packets lost
  printf("\tnumber of lost packets .... = %.2f packets\n", packetloss);       // lost packets
  printf("\tpacket loss percentage .... = %.2f%\n", (packetloss/index)*100);   // 100*(failed packets/total packets)
  printf("\taverage queueing delay ..... = %f sec\n", sum.delay/index);       // average delay in the buffer
  
  fclose(fp);
  return 0;
}

