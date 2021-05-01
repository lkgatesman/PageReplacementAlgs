#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <queue>

//define the namespace for printing
using namespace std;

//function declarations here
void generate(long int references[]);
void reductionOne(long int references[]);
void reductionTwo(long int references[]);
void FIFO(long int references[], int k, int pageTable[], int frame[]);
void LRU(long int references[], int k, int pageTable[], int frame[]);
void secondChance(long int references[], int k, int pageTable[], int frame[]);

bool debug = false;

int main(){
    int algorithm; 
    int k;

    cout << "Please indicate the replacement algorithm of your choice.\nWrite 0 for FIFO, 1 for LRU, or 2 for Second Chance: ";
    cin >> algorithm;

    //validate that the user chose a valid replacement algorithm option
    //if they did not, terminate program
    if (algorithm != 0 && algorithm != 1 && algorithm != 2){
        cout << "Invalid replacement algorithm chosen. Terminating program.";
        return 0;
    }

    cout << "Please indicate the number of frames you would like (integers only): ";
    cout << "\n";
    cin >> k;

    cout << "Number of frames simulated: ";
    cout << k;
    cout << "\n";

    //create array to represent all the frames
    int frame[k];
    int n; //counter variable
    for (n = 0; n < k; n++){
        frame[n] = -1;
    }

    //create the page table, and make all of its entries equal to null
    int pageTable[10];
    int m; //counter variable
    for (m = 0; m < 10; m++){
        pageTable[m] = -1;
    }

    //create array to hold all 1000 references and reduce them
    long int references[1000];
    generate(references);   //populate the array with random numbers between 0 and 2^31 - 1
    reductionOne(references);   //reduce the references by mod 1000
    reductionTwo(references);   //sort each reference accordingly (via table given by professor)

    if (debug == true){
        int j; //counter variable
        for (j = 0; j < 15; j++){
            cout << "value after2: " << references[j] << "\n";
        }
    }

    //run chosen algorithm
    if (algorithm == 0){
        cout << "You chose: FIFO\n";
        FIFO(references, k, pageTable, frame);
    }
    else if (algorithm == 1){
        cout << "You chose: LRU\n";
        LRU(references, k, pageTable, frame);
    }
    else if (algorithm == 2){
        cout << "You chose: Second Chance\n";
        secondChance(references, k, pageTable, frame);
    }

    return 0;
}

//FIFO page replacement algorithm //IS FINISHED
void FIFO(long int references[], int k, int pageTable[], int frame[]){

    //create variable to keep track of hits, misses, and evictions
    int numHits = 0;
    int numMisses = 0;
    int numEvicts = 0;

    //create tracker that will count how many times each page was referenced
    //set all values to zero initially
    int referenceTracker[10];
    int i; //counter variable
    for (i = 0; i < 10; i++){
        referenceTracker[i] = 0;
    }
    
    //create queue for resident pages
    queue<int> residents;
    
    //start iterating through the references
    int count;
    for ( count = 0 ; count < 1000; count++ ){

        int page = references[count];
        referenceTracker[page]++;       //increase number of times page was referenced

        //check if the page is loaded
        if (pageTable[page] == -1){
            //this means that the page is not loaded, which is a fault, so increase misses by 1
            numMisses++;
            //now we need to find an empty frame to put the page into
            for (int i = 0; i < k; i++){
                if (frame[i] == -1){
                    frame[i] = page;                //load the page in the frame
                    pageTable[page] = i;            //keep track of which frame the page is in
                    residents.push(page);           //queue the page in residents queue of loaded pages
                    break;                          //break the for statement since we found
                }

                //if it's the last possible frame and its taken as well, then EVICT!!
                if (i == k-1 && frame[i] != -1){
                    //if the residents frame is empty, then something is wrong
                    //print error message and terminate function
                    if (residents.empty()){
                        cout << "Error in eviction. Terminating program. \n";
                        return;
                    }
                    else{
                        int evictedPage = residents.front();    //store the first page in the queue
                        residents.pop();       //remove first page from the queue
                        frame[pageTable[evictedPage]] = page;       //set the frame that the page was in equal to the new page
                        pageTable[page] = pageTable[evictedPage];   //set the new page's value in page table to its new frame's index
                        pageTable[evictedPage] = -1;             //empty the pageTable entry for the evicted page

                        residents.push(page);       //queue the page in residents queue of currently loaded pages

                        numEvicts++;        //increase the number of evictions by 1
                    }
                }

            }
        }

        //but if the page IS loaded
        else{
            numHits++;      //increase number of hits
        }
    }

    /*
    for each reference:
    if the page is not loaded, then choose an empty frame
    load the page into that frame
    and set the frame number into the page table
    and put the page into the queue of resident pages
    */

   //at the end of the algorithm, output the stats!
   cout << "Number of hits: ";
   cout << numHits;
   cout << "\n";
   cout << "Number of misses: ";
   cout << numMisses;
   cout << "\n";
   cout << "Number of evictions: ";
   cout << numEvicts;
   cout << "\n";
   int a; //counter
   for (a = 0; a < 10; a++){
        cout << "Number of references to page ";
        cout << a;
        cout << ": ";
        cout << referenceTracker[a];
        cout << "\n";
   }



}

//LRU page replacement algorithm
void LRU(long int references[], int k, int pageTable[], int frame[]){

    //create variable to keep track of hits, misses, and evictions
    int numHits = 0;
    int numMisses = 0;
    int numEvicts = 0;

    //create tracker that will count how many times each page was referenced
    //set all values to zero initially
    int referenceTracker[10];
    int i; //counter variable
    for (i = 0; i < 10; i++){
        referenceTracker[i] = 0;
    }
    
    //create array for keeping track of which queue was least recently used
    int LRU[10];
    int m; //counter variable
    for (m = 0; m < 10; m++){
        LRU[m] = -1;
    }
    
    //start iterating through the references
    int count;
    for ( count = 0 ; count < 1000; count++ ){

        int page = references[count];
        referenceTracker[page]++;       //increase number of times page was referenced

        if (debug == true){
            cout << "reference: ";
            cout << page;
            cout << "\n";
        }

        //check if the page is loaded
        if (pageTable[page] == -1){
            //this means that the page is not loaded, which is a fault, so increase misses by 1
            numMisses++;
            //cout << "miss ";
            //now we need to find an empty frame to put the page into
            for (int i = 0; i < k; i++){
                if (frame[i] == -1){
                    frame[i] = page;                //load the page in the frame
                    pageTable[page] = i;            //keep track of which frame the page is in
                    LRU[page] = count;              //set the time for the last time this page was referenced
                    break;                          //break the for statement since we found
                }

                //if it's the last possible frame and its taken as well, then EVICT!!
                if (i == k-1 && frame[i] != -1){
                    //if there is no pages loaded, then we have a problem
                    //but IDK how to implement this

                    //find the frame the least recently used page via LRU[]
                    int min = 1000;
                    int evictedPage;
                    int theFrame;
                    int j; //counter variable
                    for (j = 0; j < 10; j++){
                        if (LRU[j] <= min && LRU[j] != -1){
                            min = LRU[j];
                            evictedPage = j;
                            theFrame = pageTable[j];
                        }
                    }

                    if (min == 1000){
                        cout << "Error in eviction. Terminating program. \n";
                        return;
                    }

                    frame[theFrame] = page;       //set the frame that the page was in equal to the new page
                    pageTable[page] = theFrame;   //set the new page's value in page table to its new frame's index
                    pageTable[evictedPage] = -1;             //empty the pageTable entry for the evicted page


                    LRU[page] = count;          //set the most recently reference time of the new page
                    LRU[evictedPage] = -1;      //set the evict page's most recently used time to -1

                    //cout << "evicted";

                    numEvicts++;        //increase the number of evictions by 1

                    //DEBUG: print out the page, the frame, the evictedpage, etc. 
                    if (debug == true){
                        cout << "\n min: ";
                        cout << min;
                        cout << "\n evicted page: ";
                        cout << evictedPage;
                        cout << "\n the frame: ";
                        cout << theFrame;
                        cout << "\n new page: ";
                        cout << page;
                        cout << "\n";
                    }
                }

            }
        }

        //but if the page IS loaded
        else{
            numHits++;      //increase number of hits
            //cout << "hit";

            //and need to update when this page was last referenced!
            LRU[page] = count;
        }

        //cout << "\n";
    }

    /*
    for each reference:
    if the page is not loaded, then choose an empty frame
    load the page into that frame
    and set the frame number into the page table
    and put the page into the queue of resident pages
    */

   //at the end of the algorithm, output the stats!
   cout << "Number of hits: ";
   cout << numHits;
   cout << "\n";
   cout << "Number of misses: ";
   cout << numMisses;
   cout << "\n";
   cout << "Number of evictions: ";
   cout << numEvicts;
   cout << "\n";
   int l; //counter variable
   for (l = 0; l < 10; l++){
        cout << "Number of references to page ";
        cout << l;
        cout << ": ";
        cout << referenceTracker[l];
        cout << "\n";
   }
}

//Second Chance replacement algorithm
void secondChance(long int references[], int k, int pageTable[], int frame[]){

    //create variable to keep track of hits, misses, and evictions
    int numHits = 0;
    int numMisses = 0;
    int numEvicts = 0;

    //create tracker that will count how many times each page was referenced
    //set all values to zero initially
    int referenceTracker[10];
    int i; //counter variable
    for (i = 0; i < 10; i++){
        referenceTracker[i] = 0;
    }

    //create queue to hold currently loaded pages
    queue<int> residents;

    //create array of R bits to check each page's current R bit, and set all R bits to 0 initially
    int refBit[10];
    int z; //counter variable
    for (z = 0; z < 10; z++ ){
        refBit[z] = 0;
    }

    //iterate through all the references
    int count = 0; 
    for ( ; count < 1000; count++){

        //set the current page number
        int page = references[count];
        referenceTracker[page]++;       //increase number of times page was referenced

        //cout << "page referenced: " << page << '\n';

        //check if the page is loaded

        //if the page is not loaded...
        if (pageTable[page] == -1){
            //this means that the page is not loaded, which is a fault, so increase misses by 1
            numMisses++;

            //cout << "missed\n";
            
            //now we need to find an empty frame to put the page into
            for (int i = 0; i < k; i++){
                if (frame[i] == -1){
                    frame[i] = page;                //load the page in the frame
                    pageTable[page] = i;            //keep track of which frame the page is in via page table
                    refBit[page] = 1;               //ref bit is set to 1, since the page was just referenced

                    //and make sure the page is loaded into the queue of resident pages
                    residents.push(page);

                    //cout << "put into empty frame\n";

                    break;                          //break the for statement since we found a frame to load the page into
                }

                //if it's the last possible frame and its taken as well, then EVICT!!
                if (i == k-1 && frame[i] != -1){
                    //if there is no pages loaded, then we have a problem
                    //but IDK how to implement this
                    if (residents.empty()){
                        //cout << "Error in eviction. Terminating program. \n";
                        return;
                    }

                    //look through the residents queue until we find a victim to evict
                    while(true){
                        int current = residents.front();    //look at the front page in the queue

                        //cout << "need to evict... " << current << '\n';

                        //if the that page has a bit of 0, we evict it
                        if(refBit[current] == 0){ 
                            
                            //cout << "evicted page: " << current << '\n';

                            residents.pop();        //remove the first element from the queue

                            frame[pageTable[current]] = page;       //set the frame that the evicted page was in to the new page
                            pageTable[page] = pageTable[current];   //set the page table entry for the new page to the frame number
                            pageTable[current] = -1;        //set the page table entry for the evicted page to 1

                            residents.push(page);       //queue the page in residents queue of currently loaded pages

                            numEvicts++;    //increase the number of evictions BY 1

                            refBit[page] = 1;       //make sure the ref bit of the newly loaded page is set to 1

                            break;      //break the while loop since an eviction victim was found and evicted
                        }
                        //if the refBit of the page at the front of the queue is equal to 1, then we "reload" this page
                        else if (refBit[current] == 1){
                            refBit[current] = 0;                 //change the ref bit of the page at the front of queue to 0
                            residents.push(current);            //push the current page onto the end of the queue
                            residents.pop();                       //pop the front of the queue off (because its now at the end)   
                        }
                        else{
                            cout << "Error in page eviction. Terminating program.\n";
                            return;
                        }
                    }
                }
            }
        }
        //but if the page IS loaded
        else {
            numHits++;      //increase number of hits
            //cout << "hit\n";
            
            //and need to make sure the R bit is set to 1
            refBit[page] = 1;
        }

        //if we are every 100th reference, reset all R bits to 0
        if ((count + 1) % 100 == 0){
            //cout << "cleared!\n";
            //cout << "ref num: " << count;
            for (i = 0; i < 10; i++){
                refBit[i] = 0;
            }
        }
    }

    //at the end of the algorithm, output the stats!
    cout << "Number of hits: ";
    cout << numHits;
    cout << "\n";
    cout << "Number of misses: ";
    cout << numMisses;
    cout << "\n";
    cout << "Number of evictions: ";
    cout << numEvicts;
    cout << "\n";
    int l; //counter variable
    for (l = 0; l < 10; l++){
        cout << "Number of references to page ";
        cout << l;
        cout << ": ";
        cout << referenceTracker[l];
        cout << "\n";
    }

}

//function to generate random numbers in the array between 0 and 2^31 -1
//uses pass by VALUE
void generate(long int references[]){

    int i;  //int to iterate through the references array

    //populate the array of references
    //each value is a random integer between 0...2^31 - 1
    for (i = 0; i < 1000; i++){
        references[i] = random();
    }

    return;
}

//function to reduce each reference value to a number between 0...99
void reductionOne(long int references[]){

    int i;  //int to iterate through the references array

    //reduce each number in the references by mod 100
    //this makes each value in between the range of 0...99
    for (i = 0; i < 1000; i++){
        references[i] = references[i] % 100;
    }

    return;
}

//function to reduce each reference value to a number between 0...9
void reductionTwo(long int references[]){

    reductionOne(references);   //make sure each number is in the range 0...99

    int i;  //int to iterate through the references array

    //based on the value stored in references[], choose a new page value
    for (i = 0; i < 1000; i++){

        if (0 <= references[i] && references[i] < 1){
            references[i] = 0;
        }
        else if (1 <= references[i] && references[i] < 4){
            references[i] = 1;
        }
        else if (4 <= references[i] && references[i] < 9){
            references[i] = 2;
        }
        else if (9 <= references[i] && references[i] < 16){
            references[i] = 3;
        }
        else if (16 <= references[i] && references[i] < 25){
            references[i] = 4;
        }
        else if (25 <= references[i] && references[i] < 36){
            references[i] = 5;
        }
        else if (36 <= references[i] && references[i] < 49){
            references[i] = 6;
        }
        else if (49 <= references[i] && references[i] < 64){
            references[i] = 7;
        }
        else if (64 <= references[i] && references[i] < 81){
            references[i] = 8;
        }
        else if (81 <= references[i] && references[i] < 100){
            references[i] = 9;
        }
        else{
            cout << "Invalid value in reduction #2. Terminating program. \n";
        }

    }

    return;
}
