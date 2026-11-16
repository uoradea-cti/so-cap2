/*
 * Problema 2
 * Realizati programul cpd care muta cu acelasi nume toate fisierele din
 * director_sursa in director_destinatie, impreuna cu structura arborescenta.
 *   -a   se muta si fisierele ascunse
 *   -dx  se muta fisiere pana la nivelul x de imbricare (1 = fara subdirectoare)
 *   -lx  se muta doar fisierele cu lungimea <= x
 * Se verifica sa nu fie destinatia in arborele directorului sursa.
 *
 * Utilizare: cpd [fanion...] director_sursa director_destinatie
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
int este_in_arbore(const char *sursa, const char *destinatie)
{
    char abs_sursa[_MAX_PATH];
    char abs_dest[_MAX_PATH];

    if (_fullpath(abs_sursa, sursa, _MAX_PATH) == NULL ||
        _fullpath(abs_dest, destinatie, _MAX_PATH) == NULL)
    {
        return 0;
    }

    size_t len = strlen(abs_sursa);
    if (_strnicmp(abs_sursa, abs_dest, len) == 0 &&
        (abs_dest[len] == '\0' || abs_dest[len] == '\\' || abs_dest[len] == '/'))
    {
        return 1;
    }
    return 0;
}

void muta_recursiv(const char *sursa, const char *dest, int nivel_curent,
                   int include_ascunse, int max_nivel, long max_lungime)
{
    char sablon[512];
    char cale_sursa[512];
    char cale_dest[512];
    struct _finddata_t fisier;
    intptr_t handle;

    _mkdir(dest);

    snprintf(sablon, sizeof(sablon), "%s\\*.*", sursa);
    handle = _findfirst(sablon, &fisier);
    if (handle == -1)
    {
        return;
    }

    do
    {
        if (strcmp(fisier.name, ".") == 0 || strcmp(fisier.name, "..") == 0)
        {
            continue;
        }

        if (!include_ascunse && (fisier.attrib & _A_HIDDEN))
        {
            continue;
        }

        snprintf(cale_sursa, sizeof(cale_sursa), "%s\\%s", sursa, fisier.name);
        snprintf(cale_dest, sizeof(cale_dest), "%s\\%s", dest, fisier.name);

        if (fisier.attrib & _A_SUBDIR)
        {
            if (max_nivel == -1 || nivel_curent < max_nivel)
            {
                muta_recursiv(cale_sursa, cale_dest, nivel_curent + 1,
                              include_ascunse, max_nivel, max_lungime);
                _rmdir(cale_sursa);
            }
        }
        else
        {
            if (max_lungime == -1 || fisier.size <= max_lungime)
            {
                if (fisier.attrib & _A_RDONLY)
                {
                    _chmod(cale_sursa, _S_IREAD | _S_IWRITE);
                }
                remove(cale_dest);
                if (rename(cale_sursa, cale_dest) != 0)
                {
                    perror("Eroare la mutarea fisierului");
                }
            }
        }
    } while (_findnext(handle, &fisier) == 0);

    _findclose(handle);
}
int main(int argc, char *argv[])
{
    /* Scrieti solutia aici */
int include_ascunse = 0;
    int max_nivel = -1;
    long max_lungime = -1;
    const char *sursa = NULL;
    const char *destinatie = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            include_ascunse = 1;
        }
        else if (strncmp(argv[i], "-d", 2) == 0 && strlen(argv[i]) > 2)
        {
            max_nivel = atoi(argv[i] + 2);
        }
        else if (strncmp(argv[i], "-l", 2) == 0 && strlen(argv[i]) > 2)
        {
            max_lungime = atol(argv[i] + 2);
        }
        else if (sursa == NULL)
        {
            sursa = argv[i];
        }
        else if (destinatie == NULL)
        {
            destinatie = argv[i];
        }
        else
        {
            printf("Utilizare: cpd [fanion...] director_sursa director_destinatie\n");
            return 1;
        }
    }

    if (sursa == NULL || destinatie == NULL)
    {
        printf("Utilizare: cpd [fanion...] director_sursa director_destinatie\n");
        return 1;
    }

    if (este_in_arbore(sursa, destinatie))
    {
        fprintf(stderr, "Eroare: directorul destinatie nu poate fi in arborele directorului sursa.\n");
        return 1;
    }

    muta_recursiv(sursa, destinatie, 1, include_ascunse, max_nivel, max_lungime);
    return 0;
}
