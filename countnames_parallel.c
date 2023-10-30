
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Define MACRO with 100 names
#define totalNames 100

/*
    argc is the total arguments that provide when running the program.
    argv is the array that hold all the arguments.

    Ex: argc: 2
     -> argv: ["countnames_parallel", "names.txt"]
     -> argv is the array

    Note: the first element of argv will be always the program name.

*/

/// @brief
/// @param argc
/// @param argv
/// @return
int main(int argc, char *argv[])
{
    // check if argv holds more than one argument
    if (argc < 2)
    {
        printf("Usage: %s [file1] [file2] ...\n", argv[0]);
        return 1;
    }

    // initialize variables
    int i, j;
    int fd[argc - 1][2];
    int totalUniqueNames = 0;
    char name[256];
    char uniqueNames[(argc - 1) * totalNames][256];
    int counts[(argc - 1) * totalNames];

    pid_t pid, wpid;

    // loop over each file
    for (i = 1; i < argc; i++)
    {
        // open file for reading
        FILE *fp = fopen(argv[i], "r");
        // check if there is a file
        if (fp == NULL)
        {
            printf("Cannot open file %s\n", argv[i]);
            return 1;
        }

        // create pipe
        if (pipe(fd[i - 1]) == -1)
        {
            perror("pipe");
            return 1;
        }

        // fork process
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            // child process
            close(fd[i - 1][0]); // close read end
            int nameCount = 0;
            int fileUniqueNames = 0;

            char holdName[totalNames][256];
            int count[totalNames] = {0};

            while (fgets(name, 256, fp))
            {
                nameCount++;

                // remove newline character
                name[strcspn(name, "\n")] = 0;

                if (name[0] == '\0')
                {
                    // ignore empty lines
                    fprintf(stderr, "Warning - file %s line %d is empty.\n", argv[i], nameCount);
                    continue;
                }
                else
                {
                    // check if name already exists
                    int foundIndex = -1;
                    for (j = 0; j < fileUniqueNames; j++)
                    {
                        if (strcmp(holdName[j], name) == 0)
                        {
                            foundIndex = j;
                            break;
                        }
                    }

                    // add name to list if does not exist
                    if (foundIndex == -1)
                    {
                        strcpy(holdName[fileUniqueNames], name);
                        count[fileUniqueNames] = 1;
                        fileUniqueNames++;
                    }
                    else // if name already exists
                    {
                        count[foundIndex]++;
                    }
                }
            }
            // close the file pointer
            fclose(fp);

            for (j = 0; j < fileUniqueNames; j++)
            {
                if (write(fd[i - 1][1], holdName[j], sizeof(holdName[j])) == -1)
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }

                if (write(fd[i - 1][1], &count[j], sizeof(int)) == -1)
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }

            // Close write end of pipe
            close(fd[i - 1][1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            close(fd[i - 1][1]); // Close write end of pipe

            // Read the string and integer from the pipe
            char str_buf[256];
            int num_buf;
            int num;

            int foundIndex = -1;

            while ((num_buf = read(fd[i - 1][0], str_buf, 256)) > 0)
            {                                          // read from pipe
                read(fd[i - 1][0], &num, sizeof(int)); // read integer from pipe
                foundIndex = -1;

                for (j = 0; j < totalUniqueNames; j++)
                {
                    if (strcmp(uniqueNames[j], str_buf) == 0)
                    {
                        foundIndex = j;
                        break;
                    }
                }

                // add name to list if does not exist
                if (foundIndex == -1)
                {
                    strcpy(uniqueNames[totalUniqueNames], str_buf);
                    counts[totalUniqueNames] = num;
                    totalUniqueNames++;
                }
                else // if name already exists
                {
                    counts[foundIndex] += num;
                }
            }
            close(fd[i - 1][0]); // Close read end of pipe
        }
    }
    int status;
    while ((wpid = wait(&status)) > 0)
    {
    }

    for (i = 0; i < totalUniqueNames; i++)
        printf("%s: %d\n", uniqueNames[i], counts[i]);
    return 0;
}
