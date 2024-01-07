#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

int main() {
    FILE *fp;
    char line[MAX_LINE_LENGTH];

    // Open a pipe to the command
    fp = popen("/usr/bin/uptime", "r");
    if (fp == NULL) {
        perror("Error executing command");
        return 1;
    }

    // Read output line by line and print it
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);  // Print the line
    }

    // Close the pipe
    pclose(fp);

    return 0;
}

