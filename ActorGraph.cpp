/*
 * ActorGraph.cpp
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
 * Overview: this file is to build the actor graph
 * Assignment number: PA4
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.hpp"
#include <queue>
#include <algorithm>
#include <cstring>

using namespace std;

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges)
{
  
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;
  // keep reading lines until the end of file is reached
  while (infile)
  {
    string s;
    
    // get the next line
    if (!getline( infile, s )) break;
    
    if (!have_header)
    {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss)
    {
      string next;
      
      // get the next string before hitting a tab character and put it 
      // in 'next'
      if (!getline( ss, next, '\t' )) break;
      
      record.push_back( next );
    }
    
    if (record.size() != 3)
    {
      // we should have exactly 3 columns
      continue;
    }
    
    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    //initialize the node and moviei
    ActorNode* actor;
    Movie* movie;

    //check if actor is already exist in the actorMap
    auto findActor = actorMap->find(actor_name);

    //if actor exist, then get the actor from actor map
    if(findActor != actorMap->end())
    {
      actor = findActor->second;
    }

    //if actor does not exist, then create a new actor node and
    //insert it into the actormap
    else
    {
      actor = new ActorNode(actor_name);

      //make actor pair
      pair<string,ActorNode*> actP = make_pair(actor_name,actor);
      actorMap->insert(actP);
    }

    //check if movie exist
    auto findMovie = MovieMap->find(movie_title + to_string(movie_year));

    //if movie already exist, then get the movie from movie map
    if(findMovie != MovieMap->end())
    {
      movie = findMovie->second;

      //connect current actor with all actors from movie's vector 
      //get the actorList's iterator for looping
      vector<ActorNode*>::iterator it = movie->actorList->begin();

      //while we are not at the end of the vector, keep looping
      while(it != movie->actorList->end())
      {
        //add the edge for both way
        connectGraph(actor,*it,movie);  
        connectGraph(*it,actor,movie);

        //increment the iterator
        it++;
      }       

      //update the actorList
      movie->actorList->push_back(actor);
    }

    //if movie does not exist, then create a new Movie and
    //insert it to the movieMap
    else
    {
      movie = new Movie(movie_title, movie_year);

      //push back current actor into the movie's actorList
      movie->actorList->push_back(actor);

      //make the pair for MovieMap
      pair<string,Movie*> MoviePair = 
                       make_pair(movie->name + to_string(movie->year), movie);

      //insert into the MovieMap
      MovieMap->insert(MoviePair);
    }
  }

  if (!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  infile.close();
  return true;
}


void ActorGraph::connectGraph(ActorNode* start, ActorNode* end, Movie* movie)
{
  //create the edge
  ActorEdge* connection = new ActorEdge(start,end,movie);

  //update the actor's edge vector
  start->edge->push_back(connection);
}


void ActorGraph::BFSFind(string actorF, string actorTo)
{
  //renew graph
  auto mapite = actorMap->begin();
  ActorNode* renew;
  for( ;mapite != actorMap->end();++mapite)
  {
    renew = mapite->second;

    //change its dist and prev back to initial value for next BFS find
    renew->dist = INT_MAX;
    renew->prev = 0;
  }

  //create a queue to hold all element
  queue<ActorNode*> myQ;

  //find those two input actor
  auto findActorA = actorMap->find(actorF);
  auto findActorB = actorMap->find(actorTo);

  //if one of the two actor does not exist, return and output can't find actor
  if(findActorA == actorMap->end() || findActorB == actorMap->end())
  {
    cout<<"Actor not in the graph"<<endl;
    return;
  }

  //get the starting actor
  ActorNode* currActor = findActorA->second;

  //update its filed
  currActor->dist = 0;

  //push it onto the queue
  myQ.push(currActor);

  //keep looping when queue is not empty
  while(!myQ.empty())
  {
    //get the first node from the queue
    ActorNode* get = myQ.front();
    myQ.pop();

    //get the edge vector's iterator
    vector<ActorEdge*>::iterator it = get->edge->begin(); 
    for( ; it != get->edge->end(); ++it)
    {
      //get one of its neighbor based on edge connection
      ActorNode* neighbor = (*it)->endNode;
      Movie* mov = (*it)->movieName;
      
      //if neighbor's dist is INT_MAX, then change its fields
      if(neighbor->dist == INT_MAX)
      {
        //update neighor's dist based on get's dist + 1
        neighbor->dist = get->dist + 1;
        
        //update neighbor's prev 
        neighbor->prev = get;

        //update neighbor's movie information
        neighbor->movieInfo = mov->name + "#@" + to_string(mov->year);
              
        //if neighbor is the actorTo node, then we find the end of the path,
        //and we should stop BFS find
        if(neighbor->actorName.compare(actorTo) == 0)
        {
          return;
        }
        
        //push onto queue
        myQ.push(neighbor);
      }
    }
  }
}


int ActorGraph::BFSActorConnect(string actorF, string actorTo)
{
  //renew graph
  auto mapite = actorMap->begin();
  ActorNode* renew;
  for( ;mapite != actorMap->end();++mapite)
  {
    renew = mapite->second;
    
    //change its dist and prev back to initial value for next BFS find
    renew->dist = INT_MAX;
    renew->prev = 0;
    renew->edge->clear();
  }

  //priority queue to store movies so that pop the ealiest movie out
  priority_queue<Movie*,vector<Movie*>,movieCmp> moviePQ;
  
  //using iterator to go through movie map and store all movies to priority
  //queue so that we can use later in while loop 
  auto mmp = MovieMap->begin();
  Movie* mm;
  for( ;mmp != MovieMap->end();++mmp)
  {
    mm = mmp->second;
    moviePQ.push(mm);
  }

  //default year, when not find, return this year
  int year = 9999;
 
  //find both actors in actor map
  auto findActorA = actorMap->find(actorF);
  auto findActorB = actorMap->find(actorTo);

  //if one of the two actor does not exist, return and output can't find actor
  if(findActorA == actorMap->end() || findActorB == actorMap->end())
  {
    return year;
  }

  //while loop to add movies and link the actors
  while(!moviePQ.empty())
  {
    //get the first movie from priority queue
    Movie* currM = moviePQ.top();
    moviePQ.pop();
    Movie* nextM = moviePQ.top();
    
    int size = (int)currM->actorList->size();
    vector<ActorNode*> *myV = currM->actorList;

    //connect the actors in one movies using nested for loop
    for(int i = 0; i < size; i++)
    {
      for(int j = 0; j < size; j++)
      {
        if( ((*myV)[i]) != ((*myV)[j]))
        {
          //create the edge and link
          ActorEdge* connection1 = new ActorEdge((*myV)[i],(*myV)[j],currM); 
          (*myV)[i]->edge->push_back(connection1);
        }
      }
    }
     
    //add all movies in one specific year at a time
    if(currM->year == nextM->year && !moviePQ.empty())
    {
      continue;
    }

    //renew graph before next BFS
    mapite = actorMap->begin();
    for( ;mapite != actorMap->end();++mapite)
    {
      renew = mapite->second;
      renew->dist = INT_MAX;
    }

    //create a queue to hold all element
    queue<ActorNode*> myQ;

    //get the starting actor
    ActorNode* currActor = findActorA->second;

    //update its filed
    currActor->dist = 0;

    //push it onto the queue
    myQ.push(currActor);
    
    //keep looping when queue is not empty
    while(!myQ.empty())
    {
      //get the first node from the queue
      ActorNode* get = myQ.front();
      myQ.pop();

      //get the edge vector's iterator
      vector<ActorEdge*>::iterator it = get->edge->begin(); 
      for( ; it != get->edge->end(); ++it)
      {
        //get one of its neighbor based on edge connection
        ActorNode* neighbor = (*it)->endNode;
        Movie* mov = (*it)->movieName;

        //if neighbor's dist is INT_MAX, then change its fields
        if(neighbor->dist == INT_MAX)
        {
          //update neighor's dist based on get's dist + 1
          neighbor->dist = get->dist + 1;
         
          //if neighbor is the actorTo node, then we find the end of the path,
          //and we should stop BFS find
          if(neighbor->actorName.compare(actorTo) == 0)
          {
            year = currM->year; 
            return year;
          }
        
          //push onto queue
          myQ.push(neighbor);
        }
      }
    }
  }

  //return the default year
  return year;
}


int ActorGraph::UnionActorConnect(string actorF, string actorTo)
{
  //renew graph
  auto mapite = actorMap->begin();
  ActorNode* renew;
  for( ;mapite != actorMap->end();++mapite)
  {
    renew = mapite->second;

    //change its dist and prev back to initial value for next BFS find
    renew->dist = 0;
    renew->prev = 0;
  }
 
  //priority queue to store movies so that pop the ealiest movie out
  priority_queue<Movie*,vector<Movie*>,movieCmp> moviePQ;  

  //using iterator to go through movie map and store all movies to priority
  //queue so that we can use later in while loop 
  auto mmp = MovieMap->begin();
  Movie* mm;
  for( ;mmp != MovieMap->end();++mmp)
  {
    mm = mmp->second;
    moviePQ.push(mm);
  }

  //default year, when not find, return this year
  int year = 9999;
 
  //find both actors in actor map
  auto findActorA = actorMap->find(actorF);
  auto findActorB = actorMap->find(actorTo);
  //if one of the two actor does not exist, return and output can't find actor
  if(findActorA == actorMap->end() || findActorB == actorMap->end())
  {
    return year;
  }

  //an union find object used to call all the union methods
  UnionFind uf(actorMap);

  //while loop to add movies and link the actors
  while(!moviePQ.empty())
  {
    //get the first movie from priority queue
    Movie* currM = moviePQ.top();
    moviePQ.pop();
    Movie* nextM = moviePQ.top();
    
    int size = (int)currM->actorList->size();
    vector<ActorNode*> *myV = currM->actorList;

    //connect the actors using union so that they are put in same set
    for(int i = 0; i < size-1; i++)
    {
      //union the nodes
      uf.Union((*myV)[i],(*myV)[i+1]);
    }
     
    //add all movies in one specific year at a time
    if(currM->year == nextM->year && !moviePQ.empty())
    {
      continue;
    }
    
    //get the start actor and the actor to go
    ActorNode* ActorFrom = findActorA->second;
    ActorNode* ActorFind = findActorB->second;

    //find the root node of both actor node
    ActorNode* rootA = uf.Find(ActorFrom);
    ActorNode* rootB = uf.Find(ActorFind);

    //check to see if they are in the same set, if yes, there's a path and
    //return the year
    if(rootA == rootB)
    {
      year = currM->year; 
      return year;
    }
  }

  //return the default year
  return year;
}


void ActorGraph::DijkstraFind(string actorF, string actorTo)
{
  //renew graph
  auto mapite = actorMap->begin();
  ActorNode* renew;
  for( ;mapite != actorMap->end();++mapite)
  {
    renew = mapite->second;

    //change its dist and prev back to initial value for next find
    renew->dist = INT_MAX;
    renew->prev = 0;
    renew->done = false;
  }

  //initialize a priority queue
  priority_queue<ActorNode*,vector<ActorNode*>,actorCmp> pq;

  //find those two input actor
  auto findActorA = actorMap->find(actorF);
  auto findActorB = actorMap->find(actorTo);

  //if one of the two actor does not exist, return and output can't find actor
  if(findActorA == actorMap->end() || findActorB == actorMap->end())
  {
    cout<<"Actor not in the graph"<<endl;
    return;
  }

  //get the starting actor
  ActorNode* currActor = findActorA->second;

  //update its filed
  currActor->dist = 0;

  //update its done field to false
  currActor->done = false;

  //push it onto the priority queue
  pq.push(currActor);

  //keep looping when priority queue is not empty
  while(!pq.empty())
  {
    //get the first node from the priority queue
    ActorNode* get = pq.top();
    pq.pop();
    
    //if the actorNode we pop from the pq is the actor end we want to find,
    //then we have find the path and we should return
    if(get->actorName.compare(actorTo) == 0)
    {
      return;
    }

    if(get->done != true)
    {
      //set done filed to true
      get->done = true;

      //get the edge vector's iterator
      vector<ActorEdge*>::iterator it = get->edge->begin(); 
      for( ; it != get->edge->end(); ++it)
      {
        //get one of its neighbor based on edge connection
        ActorNode* neighbor = (*it)->endNode;
        Movie* mov = (*it)->movieName;
              
        //get the dist between neighbor and get
        int distbet = get->dist + mov->weight();
       
        //if current dist is less than dist stored in neighbor,update 
        //neighbor field
        if(distbet < neighbor->dist)
        {
          //update neighor's dist to equal distbet
          neighbor->dist = distbet;
          //update neighbor's prev 
          neighbor->prev = get;
          //update neighbor's movie name
          neighbor->movieInfo = mov->name + "#@" + to_string(mov->year);
                  
          //push neighbor onto priority queue
          pq.push(neighbor);
        }
      }
    }
  }
}


void ActorGraph::writePathHelper(string actorF, string actorTo,ofstream& out)
{
  //get the currActor from actorMap
  auto actorP = actorMap->find(actorTo);
  
  //build the actorNode
  ActorNode* endActor = actorP->second;
 
  //create a vector to hold the actorNode
  vector<ActorNode*> myV;

  //push back the endActor
  myV.push_back(endActor);

  //keep looping and write to file until we reach actorF
  while(endActor->actorName.compare(actorF) != 0)
  {
    //push back its prev into the vector
    myV.push_back(endActor->prev);
       
    //change to its prev
    endActor = endActor->prev;
  }

  //loop from the end of the vector to get the path
  //and store it backward
  vector<ActorNode*> correctPath = reverse(myV);

  //loop through the entir path vector 
  for(int i = 0; i < (int)correctPath.size() - 1; i++)
  {
    //output its name
    out<< "(" + correctPath[i]->actorName +")" + "--";

    //output its movieInfo that connect to the next link
    out<< "[" + correctPath[i+1]->movieInfo + "]" + "-->";
  }

  //print the last node
  int lastNode = (int)correctPath.size() - 1;
  out<< "(" + correctPath[lastNode]->actorName +")" <<endl;
}


/**
 * helper reverse method to get the right ording in the vector 
 */
vector<ActorNode*> ActorGraph::reverse(vector<ActorNode*> in)
{
    //create a new vector for return purpose
    vector<ActorNode*> returnV;
    //loop from the back of the input vector
    for(int i = (int)in.size() - 1; i >= 0; i--)
    {
        //get the actorNode and push back to the return vector
        ActorNode* T = in[i];
        returnV.push_back(T);
    }
    return returnV;
}


/**
 * helper deleteAll method for destructor to use
 */
void ActorGraph::deleteAll()
{
  //loop through the actorMap and delete all actor node
  for(auto it = actorMap->begin();it != actorMap->end();it++)
  {
    delete it->second;
  }

  //loop through the movieMap and delete all movie pointer
  for(auto it = MovieMap->begin();it != MovieMap->end();it++)
  {
    delete it->second;
  }
}


bool ActorGraph::load(const char* in_filename)
{
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;
  // keep reading lines until the end of file is reached
  while (infile)
  {
    string s;
    
    // get the next line
    if (!getline( infile, s )) break;
    
    if (!have_header)
    {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss)
    {
      string next;
      
      // get the next string before hitting a tab character and put it 
      // in 'next'
      if (!getline( ss, next, '\t' )) break;
      
      record.push_back( next );
    }
    
    if (record.size() != 3)
    {
      // we should have exactly 3 columns
      continue;
    }
    
    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    //initialize the node and moviei
    ActorNode* actor;
    Movie* movie;

    //check if actor is already exist in the actorMap
    auto findActor = actorMap->find(actor_name);

    //if actor exist, then get the actor from actor map
    if(findActor != actorMap->end())
    {
      actor = findActor->second;
    }

    //if actor does not exist, then create a new actor node and
    //insert it into the actormap
    else
    {
      //create a new node
      actor = new ActorNode(actor_name);

      //make actor pair
      pair<string,ActorNode*> actP = make_pair(actor_name,actor);
      actorMap->insert(actP);
    }

    //check if movie exist
    auto findMovie = MovieMap->find(movie_title + to_string(movie_year));

    //if movie already exist, then get the movie from movie map
    if(findMovie != MovieMap->end())
    {
      movie = findMovie->second;

      //update the actorList
      movie->actorList->push_back(actor);
    }

    //if movie does not exist, then create a new Movie and
    //insert it to the movieMap
    else
    {
      movie = new Movie(movie_title, movie_year);
      //push back current actor into the movie's actorList
      movie->actorList->push_back(actor);

      //make the pair for MovieMap
      pair<string,Movie*> MoviePair = 
                       make_pair(movie->name + to_string(movie->year), movie);

      //insert into the MovieMap
      MovieMap->insert(MoviePair);
    }
  }

  //if file is not ended after the while loop
  if (!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  //close file and return
  infile.close();
  return true;
}


double ActorGraph::extension(string actorStart)
{
  //create a actor end string
  string actorEnd;

  //get the total actor size
  double size = (int)actorMap->size() - 1;

  //get the actorMap iterator
  auto it = actorMap->begin();

  //initialize total dist
  double totalDist = 0;

  //loop through all the actors in actormap
  for( ; it != actorMap->end(); ++it)
  {
    //get the actornode
    ActorNode* node = it->second;

    //if it is Kevin Bacon, then we skip it
    if(node->actorName.compare(actorStart) == 0)
    {
        continue;
    }

    //get the endactor name
    actorEnd = node->actorName;

    //call bfs on those two actors
    BFSFind(actorStart, actorEnd);

    //get the end actor from actorMap
    auto actorP = actorMap->find(actorEnd);
  
    //build the actorNode
    ActorNode* endActor = actorP->second;
     
    //keep looping and count path dist until we reach Kevin Bacon
    while(endActor->actorName.compare(actorStart) != 0)
    {
        totalDist++;

        //change to its prev
        endActor = endActor->prev;
    }  
  }

  //calculate average dist to Kevin Bacon
  double average = totalDist/size;
  return average;
}
