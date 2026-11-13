/*
 * Problema 3
 * Cautare de fisiere dupa un tipar (poate contine ?, *), pornind de la
 * directorul curent, in tot arborele. Pentru fiecare fisier gasit se
 * afiseaza calea completa si numele.
 *
 * Utilizare: ex03 tipar   (ex: ex03 *.txt)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

#define MAX_PATH_LEN 1024


static int se_potriveste(const char *tipar, const char *nume)
{
    while (*tipar) {
        if (*tipar == '*') {
            while (*tipar == '*')
                tipar++;
            if (*tipar == '\0')
                return 1;
            for (; *nume; nume++)
                if (se_potriveste(tipar, nume))
                    return 1;
            return 0;
        }
        if (*nume == '\0')
            return 0;
        if (*tipar != '?' &&
            tolower((unsigned char)*tipar) != tolower((unsigned char)*nume))
            return 0;
        tipar++;
        nume++;
    }
    return *nume == '\0';
}


static int cauta(const char *dir, const char *tipar)
{
    struct _finddata_t fd;
    intptr_t h;
    char pattern[MAX_PATH_LEN];
    char cale[MAX_PATH_LEN];
    int gasite = 0;

    if (strlen(dir) + 3 >= MAX_PATH_LEN) {
        fprintf(stderr, "Calea este prea lunga: %s\n", dir);
        return 0;
    }

    sprintf(pattern, "%s\\*", dir);
    h = _findfirst(pattern, &fd);
    if (h == -1)
        return 0;   

    do {
        if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
            continue;

        if (fd.attrib & _A_SUBDIR) {
            if (strlen(dir) + strlen(fd.name) + 2 >= MAX_PATH_LEN) {
                fprintf(stderr, "Calea este prea lunga: %s\\%s\n", dir, fd.name);
                continue;
            }
            sprintf(cale, "%s\\%s", dir, fd.name);
            gasite += cauta(cale, tipar);
        } else if (se_potriveste(tipar, fd.name)) {
            printf("%s\\%s   (nume: %s)\n", dir, fd.name, fd.name);
            gasite++;
        }
    } while (_findnext(h, &fd) == 0);

    _findclose(h);
    return gasite;
}

int main(int argc, char *argv[])
{
    char curent[MAX_PATH_LEN];
    int gasite;

    if (argc != 2) {
        fprintf(stderr, "Utilizare: %s tipar\n", argv[0]);
        return 1;
    }

    if (_getcwd(curent, MAX_PATH_LEN) == NULL) {
        perror("getcwd");
        return 1;
    }

    gasite = cauta(curent, argv[1]);

    if (gasite == 0)
        printf("Nu a fost gasit niciun fisier pentru tiparul \"%s\".\n", argv[1]);
    else
        printf("Total: %d fisier(e).\n", gasite);

    return 0;
}