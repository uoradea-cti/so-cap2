/*
 * Problema 8
 * Realizati o aplicatie care sorteaza continutul unui fisier folosind
 * algoritmul Bubble Sort. Nu se accepta folosirea unui tablou,
 * ci numai folosirea functiei lseek.
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
    /* Scrieti solutia aici */
if (argc != 2)
    {
        printf("Utilizare: %s fisier\n", argv[0]);
        return 1;
    }

    int fd = _open(argv[1], _O_RDWR | _O_BINARY);
    if (fd == -1)
    {
        perror("Eroare la deschiderea fisierului");
        return 1;
    }

    long lungime = _lseek(fd, 0L, SEEK_END);
    if (lungime > 1)
    {
        int sortat;
        long limita = lungime - 1;
        char c1, c2;

        do
        {
            sortat = 1;
            for (long i = 0; i < limita; i++)
            {
                _lseek(fd, i, SEEK_SET);
                if (_read(fd, &c1, 1) != 1 || _read(fd, &c2, 1) != 1)
                {
                    perror("Eroare la citire");
                    _close(fd);
                    return 1;
                }

                if ((unsigned char)c1 > (unsigned char)c2)
                {
                    _lseek(fd, i, SEEK_SET);
                    if (_write(fd, &c2, 1) != 1 || _write(fd, &c1, 1) != 1)
                    {
                        perror("Eroare la scriere");
                        _close(fd);
                        return 1;
                    }
                    sortat = 0;
                }
            }
            limita--;
        } while (!sortat);
    }

    _close(fd);
    return 0;
}
