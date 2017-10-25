/**
 * Name: Rui Deng
 *       Dadong Jing
 * Data: Mar 10, 2016
 * Overview: main program to find the actors' connections
 * Assignment number: PA4
 */

#include <iostream>
#include <fstream>
#include "ActorGraph.hpp"
#include <sstream>
#include <cstring>
#include "UnionFind.hpp"

using namespace std;

/**
 * main method to read and write files and find the conenctions
 * between actors based on the pair file
 */
int main(int argc, char* argv[])
{
  //year to be printed in out file
  int year;

  //make sure there are 5 arguments:program name, movie cast file,
  //weight option, pair file, output file
  if(argc < 4)
  {
    cout<<"number of arguments is incorrect, should be 4 or 5"<<endl;
    return 0;
  }

  //load file, read pair file and initialize output stream to write
  ActorGraph graph;

  //load the graph, create all actors and movies
  graph.load(argv[1]);

  ifstream input(argv[2]);
  ofstream output(argv[3]);

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
  output<<"Actor1"<<"	"<<"Actor2"<<"	"<<"Year"<<endl;

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

    //if default option, use union find
    if(argc == 4)
    {
      year = graph.UnionActorConnect(actorStart, actorEnd);
    }
    //when the option is bfs, using bfs to find path
    else if(strcmp(argv[4],"bfs") == 0)
    {
      year = graph.BFSActorConnect(actorStart, actorEnd);
    }
    //else when the option is union find
    else if(strcmp(argv[4],"ufind") == 0)
    {
      year = graph.UnionActorConnect(actorStart, actorEnd);
    }
    //otherwise, wrong input 
    else
    {
      cout<<"wrong input option for weighted and unweighted"<<endl;
      return 0;
    }

    //call method to write the path into output file
    //graph.writePathHelper(actorStart, actorEnd,output);
    output<< actorStart<<"	"<<actorEnd<<"	"<<year<<endl;
  }

  //close files and return
  input.close();
  output.close();
  return 0;
}
