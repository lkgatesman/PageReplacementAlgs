# PageReplacementAlgs
Lab 2 for Operating Systems : Simulating page replacement algorithms

Lauren Gatesman
Operating Systems, Lab 2
Professor Allan Gottlieb
Due May 1, 2021


DESCRIPTION OF PROGRAM

    This is a C++ program.

    This file simulates the page replacement algorithms FIFO (First In First Out), LRU (Least Recently Used), and Second
    Chance. The number of references is automatically 1,000, and the number of pages is 10, as per professor's instructions.
    The user is asked which page replacement algorithm they would like to use, followed by the number of
    frames they would like to use. After both of these inputs are given by the user, the following output information 
    is printed: the algorithm chosen, the number of frames used, the number of hits, the number of misses, the number
    of evictions, and the number of references to each page, 0 through 9. This is the end of the program.

HOW TO RUN

    How to run on LINSERV: 


WHAT THE PROGRAM DOES & DOES NOT ACCOUNT FOR (in terms of user input)

    The program does not account for users inputting non-integers or negative integers as input.
    This is per Professor Gottlieb's instructions.    
