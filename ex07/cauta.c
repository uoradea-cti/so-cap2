/*
 * Problema 7
 * cauta - cauta recursiv, de la directorul curent, fisierele cu extensia
 * ext care contin sirul text (in continutul fisierului, nu in nume).
 *   -cx  textul trebuie sa apara de cel putin x ori in fisier
 *
 * Utilizare: cauta [fanion] ext text
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_PATH_LEN 1024
#define DIM_BLOC     65536


static int are_extensia(const char *nume, const char *ext)
{
    const char *p = strrchr(nume, '.');
    return p != NULL && _stricmp(p + 1, ext) == 0;
}


static int contine_text(const char *cale, const char *text, size_t len, long minim)
{
    int fd;
    char *buf;
    size_t n = 0, i;
    int citit;
    long gasite = 0;

    buf = (char *)malloc(DIM_BLOC + len);
    if (!buf) {
        fprintf(stderr, "Memorie insuficienta.\n");
        return -1;
    }

    fd = _open(cale, _O_RDONLY | _O_BINARY);
    if (fd == -1) {
        perror(cale);
        free(buf);
        return -1;
    }

    while ((citit = _read(fd, buf + n, DIM_BLOC)) > 0) {
        n += citit;
        i = 0;
        while (i + len <= n) {
            if (memcmp(buf + i, text, len) == 0) {
                if (++gasite >= minim) {         
                    _close(fd);
                    free(buf);
                    return 1;
                }
                i += len;                        
            } else {
                i++;
            }
        }
        memmove(buf, buf + i, n - i);           
        n -= i;
    }

    if (citit == -1) {
        perror(cale);
        _close(fd);
        free(buf);
        return -1;
    }

    _close(fd);
    free(buf);
    return 0;
}


static void parcurge(const char *dir, const char *ext, const char *text,
                     size_t len, long minim)
{
    struct _finddata_t fd;
    intptr_t h;
    char pattern[MAX_PATH_LEN];
    char cale[MAX_PATH_LEN];

    if (strlen(dir) + 3 >= MAX_PATH_LEN) {
        fprintf(stderr, "Calea este prea lunga: %s\n", dir);
        return;
    }

    sprintf(pattern, "%s\\*", dir);
    h = _findfirst(pattern, &fd);
    if (h == -1)
        return;                                  

    do {
        if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
            continue;

        if (strlen(dir) + strlen(fd.name) + 2 >= MAX_PATH_LEN) {
            fprintf(stderr, "Calea este prea lunga: %s\\%s\n", dir, fd.name);
            continue;
        }
        sprintf(cale, "%s\\%s", dir, fd.name);

        if (fd.attrib & _A_SUBDIR) {
            parcurge(cale, ext, text, len, minim);
        } else if (are_extensia(fd.name, ext)) {
            if (contine_text(cale, text, len, minim) == 1)
                printf("%s\n", cale);
        }
    } while (_findnext(h, &fd) == 0);

    _findclose(h);
}

int main(int argc, char *argv[])
{
    long minim = 1;
    int arg = 1;
    const char *ext, *text;
    char curent[MAX_PATH_LEN];
    char *end;

    
    if (argc > 1 && argv[1][0] == '-') {
        if (argv[1][1] != 'c' || argv[1][2] == '\0') {
            fprintf(stderr, "Fanion necunoscut: %s\n", argv[1]);
            return 1;
        }
        minim = strtol(argv[1] + 2, &end, 10);
        if (*end != '\0' || minim < 1) {
            fprintf(stderr, "Fanion invalid: %s\n", argv[1]);
            return 1;
        }
        arg = 2;
    }

    if (argc - arg != 2) {
        fprintf(stderr, "Utilizare: %s [-cx] ext text\n", argv[0]);
        return 1;
    }

    ext  = argv[arg];
    text = argv[arg + 1];
    if (ext[0] == '.')                          
        ext++;

    if (text[0] == '\0') {
        fprintf(stderr, "Textul cautat nu poate fi vid.\n");
        return 1;
    }

    if (_getcwd(curent, MAX_PATH_LEN) == NULL) {
        perror("getcwd");
        return 1;
    }

    parcurge(curent, ext, text, strlen(text), minim);
    return 0;
}