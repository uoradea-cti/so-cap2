/*
 * Problema 8
 * Sorteaza continutul unui fisier cu Bubble Sort, fara tablou,
 * folosind doar _lseek (sortare direct pe fisier, octet cu octet).
 *
 * Utilizare: ex08 fisier
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int fd;
    long n, i, limita;
    unsigned char a, b;
    int schimbat;

    if (argc != 2) {
        fprintf(stderr, "Utilizare: %s fisier\n", argv[0]);
        return 1;
    }

    fd = _open(argv[1], _O_RDWR | _O_BINARY);
    if (fd == -1) {
        perror(argv[1]);
        return 1;
    }

    n = _filelength(fd);                      
    if (n == -1L) {
        perror(argv[1]);
        _close(fd);
        return 1;
    }

    
    for (limita = n - 1; limita > 0; limita--) {
        schimbat = 0;

        for (i = 0; i < limita; i++) {
            
            if (_lseek(fd, i, SEEK_SET) == -1L ||
                _read(fd, &a, 1) != 1 ||
                _read(fd, &b, 1) != 1) {
                perror(argv[1]);
                _close(fd);
                return 1;
            }

            if (a > b) {
                
                if (_lseek(fd, i, SEEK_SET) == -1L ||
                    _write(fd, &b, 1) != 1 ||
                    _write(fd, &a, 1) != 1) {
                    perror(argv[1]);
                    _close(fd);
                    return 1;
                }
                schimbat = 1;
            }
        }

        if (!schimbat)                       
            break;
    }

    _close(fd);
    printf("Fisierul \"%s\" a fost sortat.\n", argv[1]);
    return 0;
}