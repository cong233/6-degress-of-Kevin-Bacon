/*
 * ActorGraph.hpp
 * Author: Adrian Guthals
 * Date: 2/24/2015
 *
 * This file is meant to exist as a container for starter code that you 
 * can use to read the input file format defined in movie_casts.tsv. Feel 
 * free to modify any/all aspects as you wish.
 */

/**
 * Name: Rui Deng
 *       Dadong Jing
 * Data: Mar 10, 2016
 * Overview: this file contains all the methods to build actor graph
 * Assignment number: PA4
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include "ActorNode.hpp"
#include "ActorEdge.hpp"
#include "Movie.hpp"
#include <unordered_map>
#include <climits>
#include <queue>
#include "UnionFind.hpp"

// Maybe include some data structures here

using namespace std;

class ActorGraph {
protected:

   //variable for ActorGraph 
   unordered_map<string,ActorNode*> *actorMap;
   unordered_map<string,Movie*> *MovieMap;


public:

  /**
   * ActorGraph constructor
   */
  ActorGraph(void)
  {
    actorMap = new unordered_map<string,ActorNode*>();
    MovieMap = new unordered_map<string,Movie*>();
    //moviePQ = new priority_queue<Movie*,vector<Movie*>,movieCmp>();
  }
  
  /**
   * destructor 
   */
  ~ActorGraph()
  {
    deleteAll();
    delete actorMap;
    delete MovieMap;
  }

  /* delete all method for destructor */
  void deleteAll();
  
  /*add the edge connection for two actoNode and a movie 
   * Param: the actorA and actorB and their connection movie
   * Return: void
   */
  void connectGraph(ActorNode* start, ActorNode* end, Movie* movie);

  /**
   * BFS find method for finding the shortest connection in unweighted
   * graph
   * Param: the starting actor name and ending actor name
   * Return: void
   */
  void BFSFind(string actorF, string actorTo);

  /**
   * Dijkstra find method for finding the shortest path in weighted graph
   * Param: the starting actor name and ending actor name
   * Return: void
   */
  void DijkstraFind(string actorF, string actorTo);

  /* a helper method to write path to a output file
   * Param: starting actor name, ending actor name, and the output file
   * Return: void
   */
  void writePathHelper(string actorF, string actorTo,ofstream& output);

  /** 
   * a helper method to load just actors and movies without connecting
   * the edges when doing actor connection
   * Param: input file that we need to read
   * Return: bool, true for successfully read and false for unsuccessfully read
   */
  bool load(const char* in_filename);

  /**
   * a helper method to BFS the graph when doing action connection
   * Param: starting actor name, ending actor name
   * Return: int year which two actors starting become connected
   */
  int BFSActorConnect(string actorF, string actorTo);

  /**
   * a helper method to using disjoint set to union nodes and then
   * find the path between actors
   * Param: starting actor name, ending actor name
   * Return: int year which two actors starting become connected
   */
  int UnionActorConnect(string actorF, string actorTo);

  /* reverse vector helper method to get the correct path
   * Parm: vector 
   * Return: vector in the reverse order
   */
  vector<ActorNode*> reverse(vector<ActorNode*> in);

  /** You can modify this method definition as you wish
   *
   * Load the graph from a tab-delimited file of actor->movie relationships.
   *
   * in_filename - input filename
   * use_weighted_edges - if true, compute edge weights as 
   * 1 + (2015 - movie_year), otherwise all edge weights will be 1
   *
   * return true if file was loaded sucessfully, false otherwise
   */
  bool loadFromFile(const char* in_filename, bool use_weighted_edges);

  /* extension method to find the average dist to Kevin Bacon
   * Param: actorStart name(which is Kevin Bacon)
   * Return: double, average dist to Kevin Bacon
   */
  double extension(string actorStart);
  
};


#endif //ACTORGRAPH_HPP
