/*************************TIME LIMIT FUNCTIONALITY***********************************/
/*Used for time limit for programming competition */
#define TIMED_TEST  //Comment this line out if unlimited time limit is desired
/********************************************************************************/

// C++ program to solve TSP
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include<time.h>
#include<math.h>
#include<limits.h>
#include <chrono>
#include <climits>
#include <cmath>
#include <vector>

using namespace std;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::vector;
using std::flush;

#define TIME_LIMIT 300  //Time limit for programming competition.
#define NUMBER_CHECKS 35  //Only check  NUMBER_CHECKS  vertices before optimizing.


typedef std::chrono::high_resolution_clock hr_clock;
const int MAX_ARRAY_SIZE = 16000;

/*calculates distance between two cities given x/y cooridinates*/
int calcDistance(int city1, int city2, int* citiesXaxis, int* citiesYaxis)
{
    double distance = 0;
    double x = citiesXaxis[city1] - citiesXaxis[city2]; //calculating number to square in next step
    double y = citiesYaxis[city1] - citiesYaxis[city2];
    
    distance = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    distance = round(sqrt(distance));       //round to nearest int
    int dist = (int) distance;              //convert to int
    return dist;
}


/*Checks for a first path using nearest neighbor approach. Returns best
 *path with bestSolution variable.  Stops if time limit is reached. */
bool firstPath(int **G, int numCities, int *&bestSolution, hr_clock::time_point begin){
    bool *alreadyVisited = new bool [numCities];  //Tracks vertices already visited
    bool timeOut = false;  //If time limit is reached.
    int nextV = 0; //next vertex to add.
    int minimum = 0; //minimum distance between two vertices
    int skip;  //# of verties to skip between start and end
    bestSolution = new int[numCities+1];  //Stores best path found
    bestSolution[0] = INT_MAX;  //Init to infinity
    int *temp = new int [numCities+1];  //Temp variable for possible better path
    
#ifdef TIMED_TEST
    hr_clock::time_point done;  //Time variables
    std::chrono::duration<long double> timeElapsed;  //Total time elapsed
#endif
    
    //Determines how many vertices should be checked.
    //For larger city sets only every 'NUMBER_CHECKS' vertices will be checked.
    if (numCities < 250) { skip = 1; }
    else { skip = numCities / NUMBER_CHECKS; }
    //Checks every 'skip'th vertex for nearest neighbor cycle.
    for (int i = 0; i < numCities && !timeOut; i += skip){
        //Reset variables for each vertex being examined.
        for (int j = 0; j < numCities; j++) { alreadyVisited[j] = 0; }
        alreadyVisited[i] = 1;
        temp[0] = 0;
        temp[1] = i;
        
        //Go through all cities
        for (int j = 2; j < numCities + 1 && !timeOut; j++){
            nextV = -1;
            minimum = INT_MAX;
            //Find nearest neighbor
            for (int k = 0; k < numCities; k++){
                if (!alreadyVisited[k] && G[temp[j-1]][k] < minimum) {
                    nextV = k;
                    minimum = G[temp[j-1]][k];
                }
            }
            //Add nearest neighbor
            temp[j] = nextV;
            temp[0] += minimum;
            alreadyVisited[nextV] = 1;
            
            //Checks if time limit is up
#ifdef TIMED_TEST
            done = hr_clock::now();
            timeElapsed = (std::chrono::duration_cast<std::chrono::microseconds> (done - begin));
            if (timeElapsed.count() > TIME_LIMIT) {
                cout << "Time Limit Reached ("  << TIME_LIMIT << " seconds).\n";
                timeOut = true; }
#endif
        }
        //while time remains
        if (!timeOut) {
            //Add distance
            temp[0] += G[nextV][i];
            
            //If tour found is best tour so far, save it.
            if (temp[0] < bestSolution[0]){
                for (int j = 0; j < numCities + 1; j++) { bestSolution[j] = temp[j]; }
            }
        }
    }
    
    delete [] alreadyVisited;
    delete [] temp;
    
    return timeOut;
}

/*Run a 2-opt on the best 'NUMBER_CHEKCS' path found. Runs until
 *either no optimizations are found, or time runs out*/
void Optimize(int **G, int numCities, int *&bestPath, hr_clock::time_point begin){
    int betterSolution = 0;
    int check = 0;
    int swap1 = 0;
    int swap2 = 0;
    int turn = 0;
    int *temp = NULL;
    bool improvementFound = true;
    
#ifdef TIMED_TEST
    hr_clock::time_point done;  //Start and finish times.
    std::chrono::duration<long double> timeElapsed;  //Variable to hold diff in start & finish.
#endif
    
    while (improvementFound){
        improvementFound = false;  //Assume no improvement found.
        betterSolution = 0; //best improvement possible.
        for (int i = 0; i < (numCities - 2); i++) {  //For every vertex in path
            for (int j = i + 2; j < numCities - 1; j++) {  //Check every neihboring vertex.
                //ID any crossing paths
                check = ((G[bestPath[i + 1]][bestPath[i + 2]] + G[bestPath[j + 1]][bestPath[j + 2]]) -
                         (G[bestPath[i + 1]][bestPath[j + 1]] + G[bestPath[i + 2]][bestPath[j + 2]]));
                if (check > betterSolution) {  //Get best path
                    betterSolution = check;
                    swap1 = i+1;
                    swap2 = j+1;
                    improvementFound = true;  //Run again.
                }
            }
        }
        
        //If a positive swap is found make swap.
        if (improvementFound) {
            turn = swap2 - swap1;  //Get length of subarray
            temp = new int[turn];
            for (int i = 0; i < turn; i++){  //Sort in reverse order
                temp[i] = bestPath[swap2];
                swap2--;
            }
            for (int i = 0; i < turn; i++){  //Insert reversed subarray into bestSolution
                bestPath[swap1 + 1] = temp[i];
                swap1++;
            }
            bestPath[0] -= betterSolution;  //Subtract the distance of the improvement.
            delete[] temp;
        }
        
    
#ifdef TIMED_TEST
        done = hr_clock::now();
        timeElapsed = (std::chrono::duration_cast<std::chrono::microseconds> (done - begin));
        if (timeElapsed.count() > TIME_LIMIT) {  //if time limit reached return
            cout << "Time Limit Reached (" << TIME_LIMIT << " seconds).\n";
            return; }
#endif
    }
    
    cout << "Done.\n\n";
    
#ifdef TIMED_TEST
    done = hr_clock::now();
    timeElapsed = (std::chrono::duration_cast<std::chrono::microseconds> (done - begin));
    cout << "Completed in " << timeElapsed.count() << " seconds.\n"; //print time elapsed
#endif
}


/*Writes final result to 'input_file.tour' for evaluation.*/
void printResult(char *argv[], int *bestSolution, int numCities){
    string fileIn = argv[1];
    string fileName = "./" + fileIn;
    
    cout << "\nDone processing " << fileName << " for <1.25*OPT Solution\n\n";
    
    fileName.append(".tour");
    ofstream outputStream;
    outputStream.open(fileName);
    
    //Prints total distance, followed by list of vertices in order.
    if (outputStream.is_open()){
        for (int i = 0; i < numCities + 1; i++){ outputStream << bestSolution[i] << endl; }
        outputStream.close();
    }
    else{
        cout << "Unable to write to " << fileName << endl << endl;
    }
    
    cout << "TSP results written to " << fileName << "\n\n";
}




int main(int argc, char** argv)
{
    hr_clock::time_point timer;
    timer = hr_clock::now();  //start timer.
    cout << "Beginning Timer\n\n";
    bool timeOut = false;
    int* Solution = NULL;  //Array to house "best" result.
    
    //line of text that holds each array
    string line;
    //keeps track of number of cities
    int numCities = 0;
    //holds city names (numbers)
    int cityNames[MAX_ARRAY_SIZE] = {0};  //holds city x/y coordinates
    int cityX[MAX_ARRAY_SIZE] = {0};
    int cityY[MAX_ARRAY_SIZE] = {0};
    int distance = 0;  //holds distance between two points to populate graph
    int **G;      //complete graph representing distances between cities
    //Assign first dimension
    G = new int*[MAX_ARRAY_SIZE];
    //Assign second dimension
    for(int i = 0; i < MAX_ARRAY_SIZE; i++)
    {
        G[i] = new int[MAX_ARRAY_SIZE];
    }
    //open file (user inputs file name from command line)
    string fileIn = argv[1];
    string filename = "./" + fileIn;
    ifstream myfile(filename.c_str());
    
    //if file is valid open it
    if(myfile)
    {
        //parse the file line by line
        while(getline(myfile,line))
        {
            //convert line to a stream object
            std::stringstream stream(line);
            //parse each line for city name and x/y coordinates
            for(int i = 0; i<3; i++)
            {
                int n;
                stream >> n;
                switch(i) {
                    case 0 : cityNames[numCities] = n;
                    case 1 : cityX[numCities] = n;
                    case 2 : cityY[numCities] = n;
                }
            }
            //increment city count
            numCities++;
        }
    }
    else
    {
        myfile.close();
        cout<<"File not valid exiting program\n";
        return -1;
    }
    //close the file
    myfile.close();
    
    /*CALCULATE DISTANCES AND CREATE GRAPH*/
    for(int i = 0; i<numCities; i++)
    {
        for(int j=i; j<numCities; j++)
        {
            distance = calcDistance(j,i,cityX,cityY);   //calculate distance and populate adjacency matrix
            G[i][j] = distance;
            G[j][i] = distance;
            
        }
    }
    
    cout << "Distances Calculated, begin TSP algorithm...\n\n";
    /*TSP IMPLEMENTATION*/
    timeOut = firstPath(G, numCities, Solution, timer);  //Calculates path.
    cout << "Optimizing...\n\n";
    if (!timeOut) { Optimize(G, numCities, Solution, timer); } //Optimize path until time limit reached
    printResult(argv, Solution, numCities);  //output to file.
    
    return 0;
    
    
}

    
