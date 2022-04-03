/**
 * @file bozon.cpp
 * @author Tyler WItzke
 * @brief This is a fun program using imaginary creatures known as bozons screache to mimic
 * a aloha access control protocol
 * @version 0.1
 * @date 2022-04-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//***Imports***//
#include <iostream>
using namespace std;
#include <cmath>
#include <vector>

//***Constants***//
#define M 128    /* Number of bozons    */
#define S 200.0  /* Mean sleep duration */
#define Y 10.0  /* Mean yodel duration */
#define END_TIME 1000000.0
#define MAX_INT 2147483647.0       /* Maximum positive integer 2^31 - 1 */

//***Global Variables***//
double idleTime = 0;
double melodiousTime = 0;
double screechTime = 0;
int attemptedYodels = 0;
int perfectYodels = 0;
double perfectYodelTime = 0;
double currTime = 0;



/**
 * @brief This function generates a random floating point number
 * @return double A random floating point number between 0, and 1
 */
double Uniform01()
  {
    double randnum;
    /* get a random positive integer from random() */
    randnum = (double) 1.0 * rand();
    /* divide by max int to get something in the range 0.0 to 1.0  */
    randnum = randnum / (1.0 * MAX_INT);
    return( randnum );
  }

/**
 * @brief This function takes in a parameter and generates a random number based on it
 * 
 * @param mu The mean of the value
 * @return double The random number being generated
 */
double Exponential(double mu)
 {
    double randnum, ans;

    randnum = Uniform01();
    ans = -(mu) * log(randnum);
    return( ans );
  }



/**
 * @brief Class Bozon holds the valueable information about the yodeling creatures
 */
class Bozon {
    public:
        Bozon() {
            this->isYodeling = false;
            this->nextAction = Exponential(S);
        }

        //Getter function for if creature is yodeling
        bool getYodel(){ return this->isYodeling; } 

        //Toggles the bozon to yodel or not and performs logic with time
        void toggleYodel() { 
            this->isYodeling = !(this->isYodeling); 
            double set = S;

            //Bozon is now yodeling
            if(this->isYodeling){
                this->perfectYodel = true;
                this->startYodel = currTime;
                attemptedYodels++;
                set = Y;
            }
            //Bozon had a perfect yodel 
            else if(this->perfectYodel) {
                perfectYodels++;
                this->perfectYodel = false;
                perfectYodelTime += (currTime-(this->startYodel));
                } 
            this->setNextAction(set); //Set the next action
            }

        //Returns the next action of the bozon
        double getNextAction() { return this->nextAction; }

        //Sets the time of the next Action for the bozon
        void setNextAction(double val) {    this->nextAction = Exponential(val) + currTime; }

        //Switches the perfect yodel off
        void notPerfect() { 
	    if(this->perfectYodel) perfectYodelTime += (currTime-(this->startYodel));	
            this->perfectYodel = false; 
            }


    private:
        bool isYodeling; //Determines if bozon is yodeling or not
        double nextAction; //Tells you when the bozon needs to be toggled next
        bool perfectYodel; //Determines if the current yodel the bozon is on is perfect
        double startYodel; //Shows when the current yodel started for this bozon
};

/**
 * @brief This function will perform the simulation we are trying to perform
 * 
 * @param bozons This vector of all bozons being used 
 */
void runSimulation(vector<Bozon> &bozons)
{
    if( M <= 0) return; //Unusable Simulation
    
    double nextEvent = 0;
    int yodelers = 0;
    //Infinite Loop to run simulation
    while(1)
    {
        //Terminating case
        if(currTime >= END_TIME) break;

        yodelers = 0; 
        nextEvent = END_TIME;
        //Check which bozons need to be toggled and when the next event occurs
        for( int i = 0; i < bozons.size(); i++){

            //Either wake bozon up, or put bozon to sleep
            if(bozons.at(i).getNextAction() <= currTime) bozons.at(i).toggleYodel(); 
    
            //Determine when next event occurs
            if(bozons.at(i).getNextAction() < nextEvent) nextEvent = bozons.at(i).getNextAction();
            
            //See if there is screeching occuring
            if(bozons.at(i).getYodel()) yodelers++;
        }

        //If the number of yodelers is greater than 2, we know screaching is going on  
        if(yodelers >= 2){
            for( int i = 0; i < bozons.size(); i++){
                bozons.at(i).notPerfect();
            }
            screechTime += (nextEvent - currTime);
        }
        //If the numbers of yodelers is equal to 1 its melodious
        else if(yodelers > 0) {
            melodiousTime += (nextEvent - currTime);
        }
        //Otherwise the channel is idle
        else {
            idleTime += (nextEvent - currTime);
        }
        
        //Update the current time
        currTime = nextEvent;
    }
}

/**
 * @brief This function prints the results from the simulation function
 */
void printResults()
{
    printf("M = %d,  S = %f,  Y = %f\n", M, S, Y);
    printf("Total time observing the channel: %f\n", END_TIME);
    printf("Idle time on the channel: %f   %f%\n", idleTime, 100*(idleTime/END_TIME));
    printf("Melodious time on the channel: %f   %f%\n", melodiousTime, 100*(melodiousTime/END_TIME));
    printf("Screech time on the channel: %f   %f%\n\n", screechTime, 100*(screechTime/END_TIME));
    printf("Attempted Yodels: %d\n", attemptedYodels);
    printf("Perfect Yodels: %d\n", perfectYodels);
    if(attemptedYodels == 0) {
        printf("Perfect Yodels/Attempted Yodels: nan\n");
    } else {
        printf("Perfect Yodels/Attempted Yodels: %f\n", (double)perfectYodels/attemptedYodels);
    }
    printf("Perfect Yodel time on the channel: %f   %f%\n\n", perfectYodelTime, 100*(perfectYodelTime/END_TIME));
}



/**
 * @brief This is the main function taht drives the entire program, first populate
 * a vector with bozons then run the simulation
 */
int main()
{   
    srand(time(NULL)); //Set a new seed for the randum function
    vector<Bozon> bozons(M);
    runSimulation(bozons);
    printResults();
    return 0;
}
