/* remove example: remove myfile.txt */
#include <stdio.h>
#include <iostream>

using namespace std;
int main (int argsc, char * argsv[])
{
    if(argsv[1]){
        if( remove( argsv[1] ) != 0 )
            cout << "Error Deleting File," << argsv[1] << " was not found." << endl;
        else
            cout << "File successfully deleted" << endl;
    } else {
        cout << "Usage: rm <filename.ext>" << endl;
    }
    return 0;
}