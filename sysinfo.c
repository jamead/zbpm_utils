#include <stdio.h>
#include <sys/sysinfo.h>

int main() {
    struct sysinfo info;

    // Get system information
    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return 1;
    }

    // Print out all members of the sysinfo structure
    printf("System Information:\n");
    printf("  Uptime: %ld seconds\n", info.uptime);
    printf("  Load Average (1 min): %lu\n", info.loads[0]);
    printf("  Load Average (5 min): %lu\n", info.loads[1]);
    printf("  Load Average (15 min): %lu\n", info.loads[2]);
    printf("  Total RAM: %lu bytes\n", info.totalram);
    printf("  Free RAM: %lu bytes\n", info.freeram);
    printf("  Shared RAM: %lu bytes\n", info.sharedram);
    printf("  Buffer RAM: %lu bytes\n", info.bufferram);
    printf("  Total Swap: %lu bytes\n", info.totalswap);
    printf("  Free Swap: %lu bytes\n", info.freeswap);
    printf("  Number of Processes: %hu\n", info.procs);
    printf("  Total High Memory: %lu bytes\n", info.totalhigh);
    printf("  Free High Memory: %lu bytes\n", info.freehigh);
    printf("  Memory Unit Size: %u bytes\n", info.mem_unit);

    return 0;
}

