#include <stdio.h>
#include <sys/sysinfo.h>


void convertSeconds(int totalSeconds, int *days, int *hours, int *minutes, int *seconds) {
    *days = totalSeconds / (24 * 3600);
    totalSeconds = totalSeconds % (24 * 3600);
    *hours = totalSeconds / 3600;
    totalSeconds %= 3600;
    *minutes = totalSeconds / 60 ;
    *seconds = totalSeconds % 60;
}	


int main() {
    struct sysinfo info;
    int days,hours,minutes,seconds;

    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return 1;
    }
    
    // Calculate the uptime in seconds
    unsigned long uptime_seconds = info.uptime;
    
    // Print the uptime in seconds
    printf("System uptime: %lu seconds\n", uptime_seconds);
    
    // You can convert uptime_seconds to a more human-readable format if needed
    convertSeconds(uptime_seconds, &days, &hours, &minutes, &seconds);
    
    // Output the result
    printf("Days: %d\n", days);
    printf("Hours: %d\n", hours);
    printf("Minutes: %d\n", minutes);
    printf("Seconds: %d\n", seconds);	
    printf("%d days %d hrs %d min %d sec\n",days,hours,minutes,seconds);

    return 0;
}

