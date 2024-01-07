#include <stdio.h>
#include <sys/sysinfo.h>

int main() {
    struct sysinfo info;
    
    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return 1;
    }
    
    // Calculate the free memory in kilobytes
    unsigned long free_memory_kb = info.freeram / 1024;
    
    // Print the free memory
    printf("Free memory: %lu KB\n", free_memory_kb);
    
    return 0;
}

