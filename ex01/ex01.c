/*
 * Problema 1
 * Sterge recursiv un director cu tot continutul sau.
 * Fisierele read-only se fac mai intai normale, apoi se sterg.
 *
 * Utilizare: ex01 director
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

#define MAX_PATH_LEN 1024

static int sterge_director(const char *dir)
{
    struct _finddata_t fd;
    intptr_t h;
    char pattern[MAX_PATH_LEN];
    char cale[MAX_PATH_LEN];
    int rezultat = 0;

    if (strlen(dir) + 4 >= MAX_PATH_LEN) {
        fprintf(stderr, "Calea este prea lunga: %s\n", dir);
        return -1;
    }

    sprintf(pattern, "%s\\*", dir);

    h = _findfirst(pattern, &fd);
    if (h == -1) {
        fprintf(stderr, "Nu pot deschide directorul: %s\n", dir);
        return -1;
    }

    do {
       
        if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
            continue;

        if (strlen(dir) + strlen(fd.name) + 2 >= MAX_PATH_LEN) {
            fprintf(stderr, "Calea este prea lunga: %s\\%s\n", dir, fd.name);
            rezultat = -1;
            continue;
        }
        sprintf(cale, "%s\\%s", dir, fd.name);

        if (fd.attrib & _A_SUBDIR) {
        
            if (sterge_director(cale) != 0)
                rezultat = -1;
        } else {
           
            if (fd.attrib & _A_RDONLY) {
                if (_chmod(cale, _S_IREAD | _S_IWRITE) != 0) {
                    perror(cale);
                    rezultat = -1;
                    continue;
                }
            }
            if (remove(cale) != 0) {
                perror(cale);
                rezultat = -1;
            }
        }
    } while (_findnext(h, &fd) == 0);

    _findclose(h);   

    
    if (_rmdir(dir) != 0) {
        perror(dir);
        rezultat = -1;
    }

    return rezultat;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Utilizare: %s director\n", argv[0]);
        return 1;
    }

    if (sterge_director(argv[1]) != 0) {
        fprintf(stderr, "Stergerea nu s-a finalizat complet.\n");
        return 1;
    }

    printf("Directorul \"%s\" a fost sters.\n", argv[1]);
    return 0;
}