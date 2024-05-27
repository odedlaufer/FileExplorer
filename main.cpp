#include "navigation.h"
#include "file_operations.h"
#include "file_search.h"
#include "file_sort.h"
#include "handle_input.h"

int main() { 

    displayCurrentDirectory();

    while(true) { 
        handleUserInput();
        displayCurrentDirectory();
    }

    return 0;
   
}




