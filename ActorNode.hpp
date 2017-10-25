/**
 * Name: Rui Deng
 *       Dadong Jing
 * Data: Mar 10, 2016
 * Overview: this file is to implement the actorNode and its functionality
 * of graph
 * Assignment number: PA4
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <unordered_map>
#include <string>
#include "ActorEdge.hpp"
#include <climits>
#include <vector>
#include <cstring>

using namespace std;

/**
 * actor class to store a specific actor name and all the edges connected
 * to other actors. When two actors appear in the same movie, there's an
 * edge between these two actors.
 */
class ActorNode
{
  public:
    
    //actor name and edges connect to other actors
    string actorName;
    vector<ActorEdge*> *edge;

    //previous actor used for BFS and dist used to find in weighted graph
    ActorNode* prev = 0;
    int dist = INT_MAX;

    //a string to store connection between two actors used to output
    string movieInfo = "";

    //done field marks node as visited
    bool done;

    /**
     * constructor to initialize name and vector containing edges
     * param: a string used to initialize the actor name
     */
    ActorNode(string name) : actorName(name)
    {
      edge = new vector<ActorEdge*>();
    }

    /**
     * destructor for actor object
     */
    ~ActorNode()
    {
      deleteAll(); 
      delete edge;
    }

    /**
     * helper method for destructor, used to delete all edges
     */
    void deleteAll()
    {
      //loop through all edge and delete the edge pointer 
      for (auto it = edge->begin();it != edge->end();++it)
      {
        delete (*it);
      }
    }
};

/**
 * comparator class used to compare two nodes
 */
class actorCmp
{
  public:

    /**
     * override the comparator to compare two ActorNode pointer
     * param: two ActorNode pointers passed in to be compared
     */
    bool operator()(ActorNode* firstActor, ActorNode* secondActor)
    {
      //compare the dist of two nodes if dists are different
      int distOne = firstActor->dist;
      int distTwo = secondActor->dist;
      if(distOne != distTwo)
      {
        return distOne > distTwo;
      }
      //otherwise compare the names
      else
      {
        return (firstActor->actorName.compare(secondActor->actorName)) < 0;
      }
    }
};

#endif //ACTORNODE_HPP
