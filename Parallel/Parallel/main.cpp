// ============================================================================
//     Author: K Perkins
//     Date:   Aug 19, 2013
//     Taken From: http://programmingnotes.org/
//     File:  Parallel.cpp
//     Description: File downloaders are programs used for downloading files
//       from the Internet. Using the fork() & execlp("wget") command, the
//       following is a multi-process parallel file downloader which reads an
//       input file containing url file download links as a commandline
//       argument and downloads the files located on the internet all at once.
// ============================================================================
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

// compile & run
// g++ Parallel.cpp -o Parallel
// ./Parallel urls.txt

int main(int argc, char* argv[])
{
    // declare variables
    pid_t pid = -1;
    int urlNumber = 0;
    char urlName[256];
    ifstream infile;
    
    // check if theres enough command line args
    if(argc < 2)
    {
        cout <<"nERROR -- NOT ENOUGH ARGS!"
        <<"nnUSAGE: "<<argv[0]<<" <file containing url downloads>nn";
        exit(1);
    }
    
    // try to open the file containing the download url links
    // exit if the url file is not found
    infile.open(argv[1]);
    if(infile.fail())
    {
        cout <<"nERROR -- "<<argv[1]<<" NOT FOUND!nn";
        exit(1);
    }
    
    // get download url links from the file
    while(infile.getline(urlName, sizeof(urlName)))
    {
        ++urlNumber;
        
        // fork another process
        pid = fork();
        
        if(pid < 0)
        {   // ** error occurred
            cerr << "Fork Failed";
            exit(1);
        }
        else if(pid == 0)
        {   // ** child process
            cout <<endl<<"** URL #"<<urlNumber
            <<" is currently downloading... **nn";
            execlp("/usr/bin/wget", "wget", urlName, NULL);
        }
    }
    infile.close();
    
    while(urlNumber > 0)
    {   // ** parent process
        // parent will wait for the child to complete
        wait(NULL);
        cout <<endl<<"-- URL #"<<urlNumber<<" is complete! --n";
        --urlNumber;
    }
    
    cerr<<"nThe parent process is now exiting...n";
    
    return 0;
}// http://programmingnotes.org/