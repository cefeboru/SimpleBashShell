/* remove example: remove myfile.txt */
#include <stdio.h>
#include <iostream>

using namespace std;
int main (int argsc, char * argsv[])
{
    if(argsv[1]){
        if( remove( argsv[1] ) != 0 )
            printf("Error Deleting File, '%s' was not found.", argsv[1]);
        else
            puts( "File successfully deleted" );
    } else {
        cout << "Usage: rm <filename.ext>" << endl;
    }
    return 0;
}