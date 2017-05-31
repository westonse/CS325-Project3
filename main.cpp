// C++ program to solve Traveling Salesman Porblem 
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include<time.h>
#include<math.h>
#include<limits.h>
using namespace std;
const int MAX_ARRAY_SIZE = 16000;
//used for seths xcode environment dont use for putty
//const string path = "/Users/westonse/Library/Autosave Information/Project3/Project3/";




// Number of vertices in the graph
//#define V 15112

// A utility function to find the vertex with minimum key value, from
// the set of vertices not yet included in MST
int minKey(int key[], bool mstSet[], int V)
{
    // Initialize min value
    int min = INT_MAX;
    int min_index = -1;
    
    for (int v = 0; v < V; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
    
    return min_index;
}

// A utility function to print the constructed MST stored in parent[]
void printMST(int parent[], int n, int **graph)
{
    printf("Edge   Weight\n");
    for (int i = 1; i < n; i++)
    {
        printf("%d - %d    %d \n", parent[i], i, graph[i][parent[i]]);
    }
}

// Function to construct and print MST for a graph represented using adjacency
// matrix representation
void primMST(int **graph, int V)
{
    int parent[V]; // Array to store constructed MST
    int key[V];   // Key values used to pick minimum weight edge in cut
    bool mstSet[V];  // To represent set of vertices not yet included in MST
    
    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;
    
    // Always include first 1st vertex in MST.
    key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
    parent[0] = -1; // First node is always root of MST
    
    // The MST will have V vertices
    for (int count = 0; count < V-1; count++)
    {
        // Pick the minimum key vertex from the set of vertices
        // not yet included in MST
        int u = minKey(key, mstSet, V);
        
        // Add the picked vertex to the MST Set
        mstSet[u] = true;
        
        // Update key value and parent index of the adjacent vertices of
        // the picked vertex. Consider only those vertices which are not yet
        // included in MST
        for (int v = 0; v < V; v++)
            
            // graph[u][v] is non zero only for adjacent vertices of m
            // mstSet[v] is false for vertices not yet included in MST
            // Update the key only if graph[u][v] is smaller than key[v]
            if (graph[u][v] && mstSet[v] == false && graph[u][v] <  key[v])
                parent[v]  = u, key[v] = graph[u][v];
    }
    
    // print the constructed MST
    printMST(parent, V, graph);
}

//calculates distance between two cities given x/y cooridinates
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


//PRINT FUNCTION FOR RESULTS
/*void printResults(int distance, int tour[MAX_ARRAY_SIZE], string filename)
{
    //std::string filename2 = path + "changeResults.txt";
    //string filename2 = "./changeResults.txt";
    string filename2 = path + "testchange.txt";
    ofstream myfile2;
    //open new file if first time making change
    if(count>1)
    {
        myfile2.open (filename2.c_str(), ios::app);
        myfile2 << "\n";
    }
    //else append
    else
    {
        myfile2.open (filename2.c_str());
    }
}*/


// Driver program to test above function
int main(int argc, char** argv)
{
    //line of text that holds each array
    string line;
    //keeps track of number of cities
    int numCities = 0;
    //holds city names (numbers)
    int cityNames[MAX_ARRAY_SIZE] = {0};
    //holds city x/y coordinates
    int cityX[MAX_ARRAY_SIZE] = {0};
    int cityY[MAX_ARRAY_SIZE] = {0};
    //solution arrays
    //int TSPDistance = 0;
    //int TSPTour[MAX_ARRAY_SIZE] = {0};
    //adjacency matrix
    //int AdjacencyMatrix[16000][16000] = {0};
    //int** AdjacencyMatrix = new int[16000][16000];
    //Create double pointer
    int **AdjacenyMatrix;
    //Assign first dimension
    AdjacenyMatrix = new int*[MAX_ARRAY_SIZE];
    //Assign second dimension
    for(int i = 0; i < MAX_ARRAY_SIZE; i++)
    {
        AdjacenyMatrix[i] = new int[MAX_ARRAY_SIZE];
    }
    //holds distance between two points to populate adjacency matrix
    int distance = 0;
    //used for debugging
    //int count = 0;
    //used for seths xcode environment
  
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
    
    //increment number of cities to reflect actual number of cities (originally used for indexing)
    //numCities++;

    
    /*CALCULATE DISTANCES AND CREATE ADJACENCY MATRIX*/
    for(int i = 0; i<numCities; i++)
    {
        for(int j=i; j<numCities; j++)
        {
            distance = calcDistance(j,i,cityX,cityY);   //calculate distance and populate adjacency matrix
            AdjacenyMatrix[i][j] = distance;
            AdjacenyMatrix[j][i] = distance;
            
        }
    }

    
    /*IMPLEMENT TSP ALGORITHM HERE*/
    primMST(AdjacenyMatrix, numCities);
    
    //print TSP algorithm results to output file
    //printResults(TSPDistance,TSPTour,filename);
    
    
    //cout << count;
    return 0;
    
    
}
    
