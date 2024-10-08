#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "csv.h"

// function to open file
FILE *open_file(int argc, char *argv[])
{

    char *file = argv[argc - 1]; // get name of file

    FILE *f = fopen(file, "r"); // open file for reading

    if (f == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    return f;
}

// function to remove all commas within quotations
char *parse_csv_line(const char *line)
{
    bool quote_found = false;                  // flag for quotes
    char *new_row = (char *)malloc(MAXLENGTH); // allocate memory for new row
    int index = 0;                             // will be used for new string

    // loop over every character on the line, looking for quotes
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '"')
        {
            if (quote_found)
            { // toggle quote flag appropriately
                quote_found = false;
            }
            else
            {
                quote_found = true;
            }
        }

        if (line[i] == ',' && quote_found)
        { // don't add comma to new row if in quotes
            continue;
        }
        else
        { // if not in quotes add to new row and update index
            new_row[index] = line[i];
            index++;
        }
    }

    return new_row;
}

void process_single_h_tag(char *op, char *column, FILE *file)
{

    int header_index = 0;

    char row[MAXLENGTH];

    if (fgets(row, MAXLENGTH, file) != NULL)
    {

        bool has_found = false;

        char *field_name = strtok(row, ",");

        // find index of header
        while (field_name != NULL)
        {

            // trim away newlines for header name
            field_name[strcspn(field_name, "\r\n")] = 0;

            if (strcmp(field_name, column) == 0)
            {
                has_found = true;
                break;
            }

            header_index++;
            field_name = strtok(NULL, ",");
        }

        // check if valid name was given
        if (!has_found)
        {
            exit(EXIT_FAILURE);
        }
    }

    // determine if -min, max or -mean required
    if (strcmp(op, "-min") == 0)
    {

        char row[MAXLENGTH];
        double min_value = INFINITY;
        bool has_numeric_data = false; // flag to determine if it contains numeric data

        // read through csv file
        while (fgets(row, MAXLENGTH, file) != NULL)
        {

            char *cleaned_line = parse_csv_line(row); // clean line first
            char *value = strtok(cleaned_line, ",");

            // increment over to correct location on line
            for (int i = 0; i < header_index; i++)
            {
                value = strtok(NULL, ",");
            }

            // convert value to number (IDK HOW TO CHECK IF INTEGER OR NOT)
            int new_val = atoi(value);

            if (new_val == 0 && value[0] != '0')
            {
                // if true then the conversion failed. keep false
            }
            else
            {
                has_numeric_data = true;
            }

            if (new_val < min_value)
            {
                min_value = new_val;
            }

            free(cleaned_line); // free memory when done
        }

        // if no numeric data was found, exit program
        if (!has_numeric_data)
        {
            exit(EXIT_FAILURE);
        }

        // print min value
        printf("%f\n", min_value);
    }

    else if (strcmp(op, "-max") == 0)
    {

        char row[MAXLENGTH];
        double max_value = -1;
        bool has_numeric_data = false; // flag to determine if it contains numeric data

        // read through csv file
        while (fgets(row, MAXLENGTH, file) != NULL)
        {

            char *cleaned_line = parse_csv_line(row); // clean line first
            char *value = strtok(cleaned_line, ",");

            // increment over to correct location on line
            for (int i = 0; i < header_index; i++)
            {
                value = strtok(NULL, ",");
            }

            // convert value to number (IDK HOW TO CHECK IF INTEGER OR NOT)
            int new_val = atoi(value);

            if (new_val == 0 && value[0] != '0')
            {
                // if true then the conversion failed. keep false
            }
            else
            {
                has_numeric_data = true;
            }

            if (new_val > max_value)
            {
                max_value = new_val;
            }
            free(cleaned_line); // free memory when done
        }

        // if no numeric data was found, exit program
        if (!has_numeric_data)
        {
            exit(EXIT_FAILURE);
        }

        // print max value
        printf("%f\n", max_value);
    }

    else if (strcmp(op, "-mean") == 0)
    {

        char row[MAXLENGTH];
        double sum = 0.0;
        int count = 0;
        bool has_numeric_data = false; // flag to determine if it contains numeric data

        // read through csv file
        while (fgets(row, MAXLENGTH, file) != NULL)
        {

            char *cleaned_line = parse_csv_line(row); // clean line first
            char *value = strtok(cleaned_line, ",");

            // increment over to correct location on line
            for (int i = 0; i < header_index; i++)
            {
                value = strtok(NULL, ",");
            }

            // convert value to number (check if it's a valid integer)
            double new_val = atof(value);

            if (new_val == 0.0 && value[0] != '0')
            {
                // if true, the conversion failed
            }
            else
            {

                // sum together all values
                sum += new_val;
                count++;
                has_numeric_data = true;
            }

            free(cleaned_line); // free memory when done
        }

        // if no numeric data was found, exit program
        if (!has_numeric_data)
        {
            exit(EXIT_FAILURE);
        }

        // calculate mean value
        double mean = sum / count;

        // print mean value
        printf("%f\n", mean);
    }   
    else 
    {
        exit(EXIT_FAILURE); // incorrect second value given for h tag
    }
}

void process_multi_h_tag(int argc, char *argv[], FILE *file)
{

    // loop through and manually change the argv values
    for (int i = 2; i < argc - 1; i += 2)
    {
        char *op = argv[i];
        char *col = argv[i + 1];

        // have to open the file brand new each time
        FILE *file = open_file(argc, argv);

        // call indiviual h_tag for each operation and column given
        process_single_h_tag(op, col, file);
    }
}

void calculateMinimumValue(char *field, FILE *file)
{
    char row[MAXLENGTH];
    int min_value = INT_MAX;
    bool has_numeric_data = false; // flag to determine if it contains numeric data
    int header_index = -1;

    // Check if the field is a numeric index
    if (isdigit(field[0]))
    {
        header_index = atoi(field); // Convert to integer index
    }
    else
    {
        // Read the header to find the index of the specified field
        if (fgets(row, MAXLENGTH, file) != NULL)
        {
            char *field_name = strtok(row, ",");
            int index = 0;

            while (field_name != NULL)
            {
                field_name[strcspn(field_name, "\r\n")] = 0; // trim newlines
                if (strcmp(field_name, field) == 0)
                {
                    header_index = index; // store the index of the field
                    break;
                }
                index++;
                field_name = strtok(NULL, ",");
            }
        }
    }

    // Check if the field was found
    if (header_index == -1)
    {
        fprintf(stderr, "Field '%s' not found in header.\n", field);
        exit(EXIT_FAILURE);
    }

    // Read through the CSV file to find the minimum value
    while (fgets(row, MAXLENGTH, file) != NULL)
    {
        char *cleaned_line = parse_csv_line(row); // clean line first
        char *value = strtok(cleaned_line, ",");

        // Increment to the correct location on line
        for (int i = 0; i < header_index; i++)
        {
            value = strtok(NULL, ",");
        }

        // Convert value to number
        double new_val = atoi(value);
        if (new_val != 0.0 || value[0] == '0')
        { // check if conversion was successful
            has_numeric_data = true;
            if (new_val < min_value)
            {
                min_value = new_val;
            }
        }

        free(cleaned_line); // free memory when done
    }

    // If no numeric data was found, exit program
    if (!has_numeric_data)
    {
        exit(EXIT_FAILURE);
    }

    // Print minimum value
    printf("%d\n", min_value);
}

void calculateMaximumValue(char *field, FILE *file)
{
    char row[MAXLENGTH];
    int max_value = INT_MIN;
    bool has_numeric_data = false;
    int header_index = -1;

    if (isdigit(field[0]))
    {
        header_index = atoi(field);
    }
    else
    {
        if (fgets(row, MAXLENGTH, file) != NULL)
        {
            char *field_name = strtok(row, ",");
            int index = 0;

            while (field_name != NULL)
            {
                field_name[strcspn(field_name, "\r\n")] = 0;
                if (strcmp(field_name, field) == 0)
                {
                    header_index = index;
                    break;
                }
                index++;
                field_name = strtok(NULL, ",");
            }
        }
    }

    if (header_index == -1)
    {
        fprintf(stderr, "Field '%s' not found in header.\n", field);
        exit(EXIT_FAILURE);
    }

    while (fgets(row, MAXLENGTH, file) != NULL)
    {
        char *cleaned_line = parse_csv_line(row);
        char *value = strtok(cleaned_line, ",");

        for (int i = 0; i < header_index; i++)
        {
            value = strtok(NULL, ",");
        }

        double new_val = atoi(value);
        if (new_val != 0.0 || value[0] == '0')
        {
            has_numeric_data = true;
            if (new_val > max_value)
            {
                max_value = new_val;
            }
        }

        free(cleaned_line);
    }

    if (!has_numeric_data)
    {
        exit(EXIT_FAILURE);
    }

    printf("%d\n", max_value);
}

void calculateMeanValue(char *field, FILE *file)
{
    char row[MAXLENGTH];
    double sum = 0.0;
    int count = 0;
    bool has_numeric_data = false;
    int header_index = -1;

    if (isdigit(field[0]))
    {
        header_index = atoi(field);
    }
    else
    {
        if (fgets(row, MAXLENGTH, file) != NULL)
        {
            char *field_name = strtok(row, ",");
            int index = 0;

            while (field_name != NULL)
            {
                field_name[strcspn(field_name, "\r\n")] = 0;
                if (strcmp(field_name, field) == 0)
                {
                    header_index = index;
                    break;
                }
                index++;
                field_name = strtok(NULL, ",");
            }
        }
    }

    if (header_index == -1)
    {
        fprintf(stderr, "Field '%s' not found in header.\n", field);
        exit(EXIT_FAILURE);
    }

    while (fgets(row, MAXLENGTH, file) != NULL)
    {
        char *cleaned_line = parse_csv_line(row);
        char *value = strtok(cleaned_line, ",");
        for (int i = 0; i < header_index; i++)
        {
            value = strtok(NULL, ",");
        }

        double new_val = atof(value);
        if (new_val != 0.0 || value[0] == '0')
        {
            has_numeric_data = true;
            sum += new_val;
            count++;
        }

        free(cleaned_line);
    }

    if (!has_numeric_data)
    {
        exit(EXIT_FAILURE);
    }

    double mean = sum / count;
    printf("%f\n", mean);
}

void findRecordsValue(char *field, char *value, FILE *file)
{
    char row[MAXLENGTH];
    bool has_numeric_data = false;
    int header_index = -1;

    if (isdigit(field[0]))
    {
        header_index = atoi(field);
    }
    else 
    {
        if (fgets(row, MAXLENGTH, file) != NULL)
        {
            char *field_name = strtok(row,",");
            int index = 0;

            while (field_name != NULL)
            {
                field_name[strcspn(field_name, "\r\n")] = 0;
                if (strcmp(field_name, field) == 0)
                {
                    header_index = index;
                    break;
                }
                index++;
                field_name = strtok(NULL, ",");
            }
        }
    }
    if (header_index == -1)
    {
        fprintf(stderr, "Field '%s' not found in header.\n", field);
        exit(EXIT_FAILURE);
    }
    while (fgets(row, MAXLENGTH, file) != NULL)
    {
        char *cleaned_line = parse_csv_line(row);
        char *csv_val = strtok(cleaned_line,",");
        for (int i = 0; i < header_index; i++){csv_val = strtok(NULL,",");}
        csv_val[strcspn(csv_val, "\r\n")] = 0;
        if (strcmp(csv_val,value) == 0)
        {
            printf("%s", row);
        }
        free(cleaned_line);
    }

}

void process_first_arg(int argc, char *argv[], FILE *file)
{
    char *first_arg = argv[1]; // get first argument
    int invalid_flag = -1;

    // compare argument with (f, r, h)
    if (strcmp(first_arg, "-f") == 0)
    {

        char row[MAXLENGTH];

        // get first line
        if (fgets(row, MAXLENGTH, file) != NULL)
        {

            int word_count = 0;
            char *word = strtok(row, ",");

            // loop through all values on first row
            while (word != NULL)
            {
                word_count++;
                word = strtok(NULL, ",");
            }
            printf("%d\n", word_count);
        }
    }
    else if (strcmp(first_arg, "-r") == 0)
    {

        char row[MAXLENGTH];
        int row_count = 0;

        while (fgets(row, MAXLENGTH, file) != NULL)
        {
            row_count++;
        }
        printf("%d\n", row_count);
    }

    else if (strcmp(first_arg, "-h") == 0)
    {

        if (argc <= 3)
        {

            char *second_arg = argv[2]; // second argument
            char *third_arg = argv[3];  // third

            process_single_h_tag(second_arg, third_arg, file); // call h_tag function
        }
        else if (strcmp(argv[2],"-records") == 0) // This is bad, and I shouldn't do this >:)
        {
            FILE *records_file = open_file(argc, argv);
            findRecordsValue(argv[3], argv[4], records_file);
            invalid_flag = 0;
        }
        else
        {
            process_multi_h_tag(argc, argv, file); // if more than 3 command line arguments
        }
    }

    else
    {

        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-min") == 0)
            {
                FILE *min_file = open_file(argc, argv);
                calculateMinimumValue(argv[i + 1], min_file);
                i += 1; // Skip the field associated with min as its being entered into the fucntion.
                invalid_flag = 0;
            }
            if (strcmp(argv[i], "-mean") == 0)
            {
                FILE *mean_file = open_file(argc, argv);
                calculateMeanValue(argv[i + 1], mean_file);
                i += 1; // Skip the field associated with min as its being entered into the fucntion.
                invalid_flag = 0;
            }
            if (strcmp(argv[i], "-max") == 0)
            {
                FILE *max_file = open_file(argc, argv);
                calculateMaximumValue(argv[i + 1], max_file);
                i += 1; // Skip the field associated with min as its being entered into the fucntion.
                invalid_flag = 0;
            }
            if (strcmp(argv[i], "-records") == 0)
            {
                FILE *records_file = open_file(argc, argv);
                findRecordsValue(argv[i + 1], argv[i + 2], records_file);
                i += 2; // Skip the field and value associated with records.
                invalid_flag = 0;
            }
        }

        // CAN BE -recrods, -mean, or -max (LATER)
        if (invalid_flag < 0)
        {
            printf("Invalid first argument.\n");
        }
    }
}

int main(int argc, char *argv[])
{

    FILE *file = open_file(argc, argv); // open file for reading

    process_first_arg(argc, argv, file); // process first command-line arg.

    return 0;
}
