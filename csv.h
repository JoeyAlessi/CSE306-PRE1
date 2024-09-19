#include <stdio.h>
#include <stdlib.h>


#ifndef CSV
#define CSV

//TODO: decided value for INT_MAX
#define MAXLENGTH 1024
#define INT_MAX


FILE *open_file(int argc, char *argv[]);

char *parse_csv_line(const char *line);

void process_single_h_tag(char *op, char *column, FILE *file);

void process_multi_h_tag(int argc, char *argv[], FILE *file);

void calculateMinimumValue(char *field, FILE *file);

void process_first_arg(int argc, char *argv[], FILE *file);

#endif
