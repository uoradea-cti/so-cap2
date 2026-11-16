/*
 * Problema 4
 * Realizati un program care primeste in linia de comanda numele a doua
 * fisiere, sursa si dest, si copiaza sursa in dest.
 * Se verifica sa nu coincida sursa si dest.
 * Se lucreaza fara redirectari si duplicari.
 *
 * Utilizare: ex04 sursa dest
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
if (argc != 3)
    {
        printf("Utilizare: %s sursa dest\n", argv[0]);
        return 1;
    }

    if (_stricmp(argv[1], argv[2]) == 0)
    {
        fprintf(stderr, "Eroare: fisierul sursa si destinatie coincid.\n");
        return 1;
    }

    int fd_sursa = _open(argv[1], _O_RDONLY | _O_BINARY);
    if (fd_sursa == -1)
    {
        perror("Eroare la deschiderea fisierului sursa");
        return 1;
    }

    int fd_dest = _open(argv[2], _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd_dest == -1)
    {
        perror("Eroare la crearea fisierului destinatie");
        _close(fd_sursa);
        return 1;
    }

    char buffer[4096];
    int octeti_cititi;

    while ((octeti_cititi = _read(fd_sursa, buffer, sizeof(buffer))) > 0)
    {
        if (_write(fd_dest, buffer, octeti_cititi) != octeti_cititi)
        {
            perror("Eroare la scrierea in fisierul destinatie");
            _close(fd_sursa);
            _close(fd_dest);
            return 1;
        }
    }

    if (octeti_cititi == -1)
    {
        perror("Eroare la citirea din fisierul sursa");
    }

    _close(fd_sursa);
    _close(fd_dest);
    return 0;
}
