#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char buffer[1024];

    // Open a pipe to the command
    fp = popen("/sbin/ifconfig | grep -oP '(?<=inet\\s)\\d+(\\.\\d+){3}'", "r");
    if (fp == NULL) {
        perror("Error executing command");
        return 1;
    }

    // Read output line by line and print it
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("IP Address: %s", buffer);  // Print the line
    }

    // Close the pipe
    pclose(fp);

    return 0;
}

