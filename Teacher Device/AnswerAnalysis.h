/*File header class created to store data from arduino
 *Created by Suvana Rohanlal
 *Code adapted from w3schools.
 */
#ifndef AnswerAnalysis_h
#define AnswerAnalysis_h

#include "Arduino.h"

/*class containing methods and variables used in 
 *AnswerAnalysis.cpp
 */
class AnswerAnalysis
{
  public:
    AnswerAnalysis();
    String write(String answer);
  private:
    int _answer;
};

#endif