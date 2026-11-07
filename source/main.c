#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int is_minecraft_running() {
    /* 
     * Looks for a 'java' process that is a child or descendant of 'prismrun'.
     * The host system's pgrep can perfectly trace this process tree relationship.
     * It pipes the java PIDs into pstree to verify if 'prismrun' is in their ancestry.
     */
    int status = system("pgrep -f java | xargs -I {} pstree -p -s {} | grep -q 'prismrun' 2>/dev/null");

    /* If the exit status is 0, grep successfully found java under prismrun */
    return (status == 0);
}

int main() {
    /* 
     * Waits 5 seconds before the very first process check.
     * This provides ample time for the Java process to load into RAM and stabilize.
     */
    sleep(5);

    /* Verification loop running every 5 seconds */
    while (is_minecraft_running()) {
        sleep(5);
    }

    return 0;
}
