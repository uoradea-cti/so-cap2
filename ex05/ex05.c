/*
 * Problema 5
 * Adauga la sfarsitul fisierului toate cifrele care ii apartin.
 *   initial: abc123de6fghi12jk
 *   final:   abc123de6fghi12jk123612
 * Cifrele se memoreaza intr-un buffer. Cand bufferul e plin: se retine
 * pozitia curenta, se sare la EOF, se scrie bufferul, se revine.
 * Citirile nu se fac dincolo de EOF-ul initial.
 *
 * Utilizare: ex05 test.tst
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DIM_BUF     16    
#define DIM_CITIRE  64    


static int goleste_buffer(int fd, const char *cifre, int nr)
{
    long poz;
    int total = 0, scris;

    poz = _tell(fd);                         
    if (poz == -1L)
        return -1;
    if (_lseek(fd, 0L, SEEK_END) == -1L)     
        return -1;

    while (total < nr) {                     
        scris = _write(fd, cifre + total, nr - total);
        if (scris == -1)
            return -1;
        total += scris;
    }

    if (_lseek(fd, poz, SEEK_SET) == -1L)    
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    char cifre[DIM_BUF];
    char blk[DIM_CITIRE];
    int nr = 0;
    int fd, citit, i, cer;
    long ramas;

    if (argc != 2) {
        fprintf(stderr, "Utilizare: %s test.tst\n", argv[0]);
        return 1;
    }

    fd = _open(argv[1], _O_RDWR | _O_BINARY);
    if (fd == -1) {
        perror(argv[1]);
        return 1;
    }

    ramas = _filelength(fd);                 
    if (ramas == -1L) {
        perror(argv[1]);
        _close(fd);
        return 1;
    }

    while (ramas > 0) {
        cer = (ramas > DIM_CITIRE) ? DIM_CITIRE : (int)ramas;
        citit = _read(fd, blk, cer);
        if (citit == -1) {
            perror(argv[1]);
            _close(fd);
            return 1;
        }
        if (citit == 0)
            break;
        ramas -= citit;

        for (i = 0; i < citit; i++) {
            if (isdigit((unsigned char)blk[i])) {
                cifre[nr++] = blk[i];
                if (nr == DIM_BUF) {         
                    if (goleste_buffer(fd, cifre, nr) != 0) {
                        perror(argv[1]);
                        _close(fd);
                        return 1;
                    }
                    nr = 0;
                }
            }
        }
    }

    
    if (nr > 0 && goleste_buffer(fd, cifre, nr) != 0) {
        perror(argv[1]);
        _close(fd);
        return 1;
    }

    _close(fd);
    printf("Cifrele au fost adaugate la sfarsitul fisierului \"%s\".\n", argv[1]);
    return 0;
}