/**
 * Name: Rui Deng
 *       Dadong Jing
 * Data: Mar 10, 2016
 * Overview: this file implements all the methods to build disjoint
 * set
 * Assignment number: PA4
 */

#include "UnionFind.hpp"

using namespace std;

ActorNode* UnionFind::Find(ActorNode* actor)
{
  //if actor node itself is the root, return that node
  if(actor->prev == 0)
  {
    return actor;  
  }
  
  //otherwise call this method recursively to find the root
  return actor->prev = Find(actor->prev);
}

void UnionFind::Union(ActorNode* actorA, ActorNode* actorB)
{
  //find the root of both actor nodes passed in
  ActorNode* rootA = Find(actorA);
  ActorNode* rootB = Find(actorB);

  //if not in the same set, union them, put second root behind first one
  if(rootA != rootB)
  {
    rootB->prev = rootA;
  }
}
