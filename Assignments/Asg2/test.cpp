#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <cstring>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
#include <numeric>
using namespace std;


//#define DEBUG cout << "LINE: " << __LINE__ << " :: " 
//#define DEBUG2(MSG) cout << "LINE: " << __LINE__ << " :: " << MSG << endl;

void sequentialIndex();
void wordSum();
void myHash();

int main() {
 
  sequentialIndex();
       //    wordSum();
       //    myHash();
  
  return 0;
}


void sequentialIndex()
{
  map<int, string> indexMap;                 // Map of string and int
  map<int, string>::iterator it;
  
  char str[100];
  cout << "Enter a string: " << endl;
  cin.getline(str, 100);
  char* point;
  point = strtok(str, " ,.?!");
  int wordNum = 0;
  //ENCODING
  while (point != NULL)
    {
      wordNum++;
      indexMap.insert(pair<int, string>(wordNum, point)); // Insert Element in map
      cout << "0x" <<wordNum << " ";
      point = strtok(NULL, " ,.?!");
    }
  cout << endl;

  //DECODING
  cout << "Enter a string of numbers: " << endl;
  cin.getline(str, 100);
  point = strtok(str, " 0x,.?!");

  //Loop through the map
  for (it = indexMap.begin(); point != NULL; it++)
    {
      wordNum = atoi(point); //char * to int
      it = indexMap.find(wordNum);
      if (it == indexMap.end()){
	cout<<"Decode '0x" << wordNum << "' not found ";
      } else {
	cout << it->second << " ";
      }
      point = strtok(NULL, "0x,.?!");
    }
  cout << endl;
}
  

void wordSum()
{
  map<int, string> myMap;
  map<int, string>::iterator it;
  int wordSum = 0;
  char str[100];
  cout << "Enter a string: " << endl;
  cin.getline(str, 100);


  //ENCODING
  char* point;
  point = strtok(str, " ,.?!");
  while (point != NULL)
    {
      wordSum = 0;
      size_t len = strlen(point);
      for (unsigned int j = 0; j < len; j++) {
	wordSum += point[j];
      }
      myMap.insert(pair<int, string>(wordSum, point));
      cout << wordSum << " ";
      point = strtok(NULL, " ,.?!");
    }
  cout << endl;
  
  //DECODING
  cout << "Decoding: " << endl;
  cin.getline(str, 100);  
  point = strtok(str, " ,.?!"); //split the lines acording to the delimiters
  
   //Loop through the map
  for (it = myMap.begin(); point != NULL; it++)
    {
      wordSum = atoi(point); //char * to int
      it = myMap.find(wordSum);
      if (it == myMap.end()){
	cout<<"Decode '" << wordSum << "' not found ";
      } else {
	cout << it->second << " ";
      }
      point = strtok(NULL, " ,.?!");
    }
  cout << endl;

}

void myHash()
{
  map<int, string> myMap;
  map<int, string>::iterator it;
  int randomNum = 0;
  char str[100];
  cout << "Encoding: " << endl;
  cin.getline(str, 100);

  
  //ENCODING
  char* point;
  point = strtok(str, " ,.?!");
  while (point != NULL)
    {
      randomNum = rand() % 1000 + 1; //generate number between 1 and 1000
      myMap.insert(pair<int, string>(randomNum, point));      
      cout << randomNum << " ";
      //      cout << "rand: " << randomNum << "\tword: " << point << endl;
      point = strtok(NULL, " ,.?!");
    }
  cout << endl;

  //DECODING
  cout << "Decoding: " << endl;
  cin.getline(str, 100);  
   point = strtok(str, " ,.?!"); //split the lines according to the delimiters    
   //Loop through the map
  for (it = myMap.begin(); point != NULL; it++)
    {
      randomNum = atoi(point); //char * to int
      it = myMap.find(randomNum);
      if (it == myMap.end()){
	cout<<"Decode '" << randomNum << "' not found ";
      } else {
	 

	cout << it->second << " ";
      }
      point = strtok(NULL, " ,.?!");
}
  cout << endl;
}

