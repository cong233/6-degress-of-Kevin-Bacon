/**
 * Name: Rui Deng
 *       Dadong Jing
 * Data: Mar 10, 2016
 * Overview: the header file contains all the methods to build disjoint
 * set
 * Assignment number: PA4
 */

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include "ActorGraph.hpp"

using namespace std;

class UnionFind
{
  public:
    
    //a member variable storing the actors
    unordered_map<string, ActorNode*> * unionF;

    /**
     * Constructor to build a UnionFind object and initialize member
     */
    UnionFind(unordered_map<string, ActorNode*> *map) : unionF(map) {} 

    /**
     * a recursive method to find an actor's root in the set
     * Parm: actor node
     * Return: its root node
     */
    ActorNode* Find(ActorNode* actor);

    /**
     * method to union two set together based on two actor nodes
     * in the sets
     * Parm: two actor nodes
     * Return: void
     */
    void Union(ActorNode* actorA, ActorNode* actorB);
};

#endif //UNIONFIND_HPP
