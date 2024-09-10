#include <stdio.h>
#include <stdlib.h>

// function to open file
FILE* open_file(int argc, char *argv[]) {

    char *file = argv[argc - 1]; // get name of file

    FILE *f = fopen(file, "r"); // open file for reading

    if (f == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    return f;
}

int main(int argc, char *argv[]) {

    printf("Hello World\n");

    return 0;  
}