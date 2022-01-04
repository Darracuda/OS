//
// Created by Diana Kolpakova on 16.12.2021.
//

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHM_NAME "/lab4.mem"

int main() {
    int value;
    int summa = 0;

    while (scanf("%d", &value) != EOF)
    {
        summa += value;
    }

    // give name to shared memory
    int map_fd = shm_open(SHM_NAME, O_RDWR, S_IREAD | S_IWRITE);
    if (map_fd < 0) {
        fprintf(stderr, "Error opening shared memory: %s\n", strerror(errno));
        return -1;
    }

    // get address of named shared memory
    char *addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, map_fd, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "Error create mapping: %s\n", strerror(errno));
        return -1;
    }

    *addr = summa; //write to shared memory
}

