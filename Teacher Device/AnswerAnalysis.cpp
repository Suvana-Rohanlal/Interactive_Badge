/*Class created to store data from arduino
 *Created by Suvana Rohanlal
 *Code adapted from w3schools.
 */
#include <iostream>
#include <fstream>
#include <String>

using namespace std;

#include "AnswerAnalysis.h"
#include "Arduino.h"

AnswerAnalysis::AnswerAnalysis() {

}
/*Method created to write to a textfile
 *
 *Textfile is created and "answer" is written to the textfile
 */
String AnswerAnalysis::write(String answer){
ofstream myfile("C:\\answer.txt");
  if (myfile.is_open())
  {
    myfile << answer;
    myfile << "\n";
    myfile.close();
    return "Done";
  }
  else return answer;
  
}
