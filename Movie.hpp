/**
 * Name: Rui Deng
 *       Dadong Jing
 * Data: Mar 10, 2016
 * Overview: a class contains the information of a movie(name, actors, year)
 * Assignment number: PA4
 */

#ifndef MOVIE_HPP
#define MOVIE_HPP

#include "ActorNode.hpp"
#include <vector>
#include <string>

using namespace std;

/**
 * movie class to store the movie name, year and all the actors that
 * appear in the movie
 */
class Movie
{
  public:

    //movie name, year and actor list as memeber variables
    string name;
    int year;
    vector<ActorNode*> *actorList;

    /**
     * constructor to initialize all member variables
     * param: a string to initialize the movie name
     *        an int to initialize the year
     */
    Movie(string movName, int ye) : name(movName),year(ye)
    {
      actorList = new vector<ActorNode*>();
    }

    /**
     * weight of movie used for weighted graph
     */
    int weight()
    {
      return (1 + (2015 - year)); 
    }

    /**
     * destructor for movie object
     */
    ~Movie()
    {
      delete actorList;
    }
};

/**
 * comparator class used to compare two movies
 */
class movieCmp
{
  public:

    /**
     * override the comparator to compare two Movie pointer
     * param: two Movie pointers passed in to be compared
     */
    bool operator()(Movie* firstMov, Movie* secondMov)
    {
      //compare the year of two nodes if years are different
      int yearOne = firstMov->year;
      int yearTwo = secondMov->year;
      if(yearOne != yearTwo)
      {
        return yearOne > yearTwo;
      }
      //otherwise compare the names
      else
      {
        return (firstMov->name.compare(secondMov->name)) < 0;
      }
    }
};

#endif //MOVIE_HPP
