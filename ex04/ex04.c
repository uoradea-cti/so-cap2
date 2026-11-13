/*
 * Problema 4
 * Copiaza fisierul sursa in dest. Se verifica sa nu coincida sursa si dest.
 * Se lucreaza fara redirectari si duplicari.
 *
 * Utilizare: ex04 sursa dest
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_PATH_LEN 1024
#define DIM_BUF 65536

int main(int argc, char *argv[])
{
    char abs_src[MAX_PATH_LEN], abs_dst[MAX_PATH_LEN];
    static char buf[DIM_BUF];
    int fs, fd;
    int citit, scris, total;
    int rezultat = 0;

    if (argc != 3) {
        fprintf(stderr, "Utilizare: %s sursa dest\n", argv[0]);
        return 1;
    }

    
    if (!_fullpath(abs_src, argv[1], MAX_PATH_LEN) ||
        !_fullpath(abs_dst, argv[2], MAX_PATH_LEN)) {
        fprintf(stderr, "Nu pot determina calea absoluta.\n");
        return 1;
    }
    if (_stricmp(abs_src, abs_dst) == 0) {
        fprintf(stderr, "Eroare: sursa si destinatia coincid.\n");
        return 1;
    }

    
    fs = _open(argv[1], _O_RDONLY | _O_BINARY);
    if (fs == -1) {
        perror(argv[1]);
        return 1;
    }

    fd = _open(argv[2], _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY,
               _S_IREAD | _S_IWRITE);
    if (fd == -1) {
        perror(argv[2]);
        _close(fs);
        return 1;
    }

    while ((citit = _read(fs, buf, DIM_BUF)) > 0) {
        total = 0;
        while (total < citit) {              
            scris = _write(fd, buf + total, citit - total);
            if (scris == -1) {
                perror(argv[2]);
                rezultat = 1;
                goto final;
            }
            total += scris;
        }
    }
    if (citit == -1) {
        perror(argv[1]);
        rezultat = 1;
    }

final:
    _close(fs);
    _close(fd);

    if (rezultat == 0)
        printf("Fisierul \"%s\" a fost copiat in \"%s\".\n", argv[1], argv[2]);
    return rezultat;
}