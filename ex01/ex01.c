/*
 * Problema 1
 * Realizati un program care primeste in linia de comanda un nume de
 * director pe care trebuie sa il stearga, impreuna cu tot continutul
 * (fisiere si subdirectoare). Parcurgerea se face cu o functie recursiva.
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

void sterge_director_recursiv(const char *cale_director)
{
    char sablon[512];
    char cale_completa[512];
    struct _finddata_t fisier;
    intptr_t handle;

    snprintf(sablon, sizeof(sablon), "%s\\*.*", cale_director);

    handle = _findfirst(sablon, &fisier);
    if (handle != -1)
    {
        do
        {
            if (strcmp(fisier.name, ".") == 0 || strcmp(fisier.name, "..") == 0)
            {
                continue;
            }

            snprintf(cale_completa, sizeof(cale_completa), "%s\\%s", cale_director, fisier.name);

            if (fisier.attrib & _A_SUBDIR)
            {
                sterge_director_recursiv(cale_completa);
            }
            else
            {
                if (fisier.attrib & _A_RDONLY)
                {
                    _chmod(cale_completa, _S_IREAD | _S_IWRITE);
                }

                if (remove(cale_completa) != 0)
                {
                    perror("Eroare la stergerea fisierului");
                }
            }
        } while (_findnext(handle, &fisier) == 0);

        _findclose(handle);
    }

    if (_rmdir(cale_director) != 0)
    {
        perror("Eroare la stergerea directorului");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Utilizare: %s director\n", argv[0]);
        return 1;
    }

    sterge_director_recursiv(argv[1]);

    return 0;
}
