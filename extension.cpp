#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "ActorGraph.hpp"

using namespace std;

/**
 * main method to read and write files and find the average dist to Kevin Bacon
 */
int main(int argc, char* argv[])
{
  //make sure there are 2 arguments:program name, movie cast file,
  //weight option, pair file, output file
  if(argc < 2)
  {
    cout<<"number of arguments is incorrect, should be 2"<<endl;
    return 0;
  }

  //load file, read pair file
  ActorGraph graph;
  //update the map
  graph.loadFromFile(argv[1],false);
  //get the actorStart name, Kevin Bacon in this case
  string actorStart = "BACON, KEVIN (I)";
  
  //obtain the average dist from extension method
  double average = graph.extension(actorStart);

  //print out the dist in terminal
  cout<<"Average distance to Kevin Bacon is "<<average<<endl;

  return 0;
}
