#include "execute.h"

#include <iostream>
#include <unistd.h>

using namespace std;

int execute(const string& program, const int parametersLength, const string parameters[])
{
    int processId = fork();
    if (processId == -1)  // error
        return -1;
    else if (processId == 0)  // child process
    {
        char** argv = new char* [parametersLength + 2]; // extra 2 items
        argv[0] = (char*)program.c_str();      // the 1st extra item
        for (int i = 0; i < parametersLength; i++)
            argv[i + 1] = (char*)parameters[i].c_str();
        argv[parametersLength + 1] = nullptr;       // the 2nd extra item

        execv(program.c_str(), argv);
        exit(0);
    }
    else    //  parent process
        return processId;
}
