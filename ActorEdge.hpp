/**
 * Name: Rui Deng
 *       Dadong Jing
 * Data: Mar 10, 2016
 * Overview: this file is to implement the edge and its functionality 
 * of graph
 * Assignment number: PA4
 */

#ifndef ACTOREDGE_HPP
#define ACTOREDGE_HPP

using namespace std;

//initialize actor and movie class to avoid not class type case
class ActorNode;
class Movie;

/**
 * edge class to store two actors in two ends of the edge and a movie that
 * two actors both appear
 */
class ActorEdge
{
  public:

    //two actors and one movie as memeber variables
    ActorNode* startNode;
    ActorNode* endNode;
    Movie* movieName;
  
    /**
     * constructor taking in three parameters and initialize all members
     * Param: starting actor node, ending actor node, 
     *        and their movie connection
     */
    ActorEdge(ActorNode* start, 
              ActorNode* end, 
              Movie* movie) : startNode(start),endNode(end),movieName(movie){};
};

#endif //ACTOREDGE_HPP
