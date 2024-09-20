#include <stdio.h>
#include <stdlib.h>


#ifndef CSV
#define CSV

//TODO: decided value for INT_MAX

// jchen526 - I decided on maxing the INT MAX and INT MIN to the minimum and maximum an integer can hold

#define MAXLENGTH 1024
#define INT_MAX 2147483647
#define INT_MIN -2147483648

FILE *open_file(int argc, char *argv[]);

char *parse_csv_line(const char *line);

void process_single_h_tag(char *op, char *column, FILE *file);

void process_multi_h_tag(int argc, char *argv[], FILE *file);

void calculateMinimumValue(char *field, FILE *file);

void calculateMaximumValue(char *field, FILE *file);

void calculateMeanValue(char *field, FILE *file);

void findRecordsValue(char *field, char *value, FILE *file);

void process_first_arg(int argc, char *argv[], FILE *file);

#endif
