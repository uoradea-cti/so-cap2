/*
 * Problema 5
 * Se considera un fisier existent, "test.tst". Adaugati la sfarsitul lui
 * toate cifrele care ii apartin.
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
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    /* Scrieti solutia aici */
if (argc != 2)
    {
        printf("Utilizare: %s test.txt\n", argv[0]);
        return 1;
    }

    int fd = _open(argv[1], _O_RDWR | _O_BINARY);
    if (fd == -1)
    {
        perror("Eroare la deschiderea fisierului");
        return 1;
    }

    long eof_initial = _lseek(fd, 0L, SEEK_END);
    _lseek(fd, 0L, SEEK_SET);

    char buf_citire[256];
    char buf_cifre[256];
    int idx_cifre = 0;
    long poz_curenta = 0;

    while (poz_curenta < eof_initial)
    {
        int de_citit = sizeof(buf_citire);
        if (eof_initial - poz_curenta < de_citit)
        {
            de_citit = (int)(eof_initial - poz_curenta);
        }

        int octeti = _read(fd, buf_citire, de_citit);
        if (octeti <= 0)
        {
            break;
        }

        poz_curenta += octeti;

        for (int i = 0; i < octeti; i++)
        {
            if (buf_citire[i] >= '0' && buf_citire[i] <= '9')
            {
                buf_cifre[idx_cifre++] = buf_citire[i];

                if (idx_cifre == sizeof(buf_cifre))
                {
                    long poz_salvata = _lseek(fd, 0L, SEEK_CUR);
                    _lseek(fd, 0L, SEEK_END);
                    _write(fd, buf_cifre, idx_cifre);
                    _lseek(fd, poz_salvata, SEEK_SET);
                    idx_cifre = 0;
                }
            }
        }
    }

    if (idx_cifre > 0)
    {
        _lseek(fd, 0L, SEEK_END);
        _write(fd, buf_cifre, idx_cifre);
    }

    _close(fd);
    return 0;
}
