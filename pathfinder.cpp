/**
 * Name: Rui Deng
 *       Dadong Jing
 * Data: Mar 10, 2016
 * Overview: main program to find shortest path between actors
 * Assignment number: PA4
 */

#include <iostream>
#include <fstream>
#include "ActorGraph.hpp"
#include <sstream>
#include <cstring>

using namespace std;

/**
 * main method to read and write files and find the shortest paths
 * between actors based on the pair file
 */
int main(int argc, char* argv[])
{
  //make sure there are 5 arguments:program name, movie cast file,
  //weight option, pair file, output file
  if(argc < 5)
  {
    cout<<"number of arguments is incorrect, should be 5"<<endl;
    return 0;
  }

  //load file, read pair file and initialize output stream to write
  ActorGraph graph;
  graph.loadFromFile(argv[1],false);
  ifstream input(argv[3]);
  ofstream output(argv[4]);

  //check if the files are open
  if(input.is_open() == false || output.is_open() == false)
  {
    cout<<"files are not open"<<endl;
    return 0;
  }

  //initalize some strings used to read file and store actor names
  string line;
  string actorStart;
  string actorEnd;

  //read the header of pair file and it is useless
  getline(input, line);

  //write the first line of output file
  output<<"(actor)--[movie#@year]-->(actor)--..."<<endl;

  //while loop go through the pair file and find shortest paths
  while(1)
  {
    //get each line containing a pair of actors
    getline(input, line);

    //end of file, break
    if(input.eof())
      break;

    //using string stream to split the line with tab 
    istringstream iss(line);
    getline(iss, actorStart, '\t');
    getline(iss, actorEnd, '\t');

    //when choosing unweighted option, using BFS and print path to 
    //output file
    if(strcmp(argv[2],"u") == 0)
    {
      graph.BFSFind(actorStart, actorEnd);
    }
    //when choosing weighted option, using dijkstra method
    else if(strcmp(argv[2],"w") == 0)
    {
      graph.DijkstraFind(actorStart, actorEnd);
    }
    //otherwise wrong input
    else
    {
      cout<<"wrong input option for weighted and unweighted"<<endl;
      return 0;
    }

    //call method to write the path into output file
    graph.writePathHelper(actorStart, actorEnd,output);
  }

  //close files and return
  input.close();
  output.close();
  return 0;
}
