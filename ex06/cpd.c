/*
 * Problema 6
 * Realizati programul cpd care copiaza arborele de fisiere din sursa in dest.
 *   -a   se copiaza si fisierele ascunse
 *   -o   nu se suprascriu fisierele existente in destinatie
 *   -u   se sterg fisierele din sursa
 *   -dx  se copiaza fisierele pana la nivelul x de imbricare
 * Daca dest este in arborele de subdirectoare al lui sursa, programul
 * se opreste cu mesaj de eroare.
 *
 * Utilizare: cpd [fanion...] sursa dest
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

int copiaza_fisier(const char *cale_sursa, const char *cale_dest, int nu_suprascrie)
{
    if (nu_suprascrie && _access(cale_dest, 0) == 0)
    {
        return 1; 
    }

    int fd_s = _open(cale_sursa, _O_RDONLY | _O_BINARY);
    if (fd_s == -1)
    {
        perror("Eroare la deschiderea fisierului sursa");
        return 0;
    }

    int fd_d = _open(cale_dest, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd_d == -1)
    {
        perror("Eroare la crearea fisierului destinatie");
        _close(fd_s);
        return 0;
    }

    char buffer[4096];
    int octeti;
    int succes = 1;

    while ((octeti = _read(fd_s, buffer, sizeof(buffer))) > 0)
    {
        if (_write(fd_d, buffer, octeti) != octeti)
        {
            perror("Eroare la scrierea in destinatie");
            succes = 0;
            break;
        }
    }

    if (octeti == -1)
    {
        perror("Eroare la citirea din sursa");
        succes = 0;
    }

    _close(fd_s);
    _close(fd_d);
    return succes;
}

void copiaza_recursiv(const char *sursa, const char *dest, int nivel_curent,
                      int include_ascunse, int nu_suprascrie, int sterge_sursa, int max_nivel)
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
                copiaza_recursiv(cale_sursa, cale_dest, nivel_curent + 1,
                                 include_ascunse, nu_suprascrie, sterge_sursa, max_nivel);
                if (sterge_sursa)
                {
                    _rmdir(cale_sursa);
                }
            }
        }
        else
        {
            if (copiaza_fisier(cale_sursa, cale_dest, nu_suprascrie))
            {
                if (sterge_sursa)
                {
                    if (fisier.attrib & _A_RDONLY)
                    {
                        _chmod(cale_sursa, _S_IREAD | _S_IWRITE);
                    }
                    remove(cale_sursa);
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
    int nu_suprascrie = 0;
    int sterge_sursa = 0;
    int max_nivel = -1;
    const char *sursa = NULL;
    const char *destinatie = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            include_ascunse = 1;
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            nu_suprascrie = 1;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            sterge_sursa = 1;
        }
        else if (strncmp(argv[i], "-d", 2) == 0 && strlen(argv[i]) > 2)
        {
            max_nivel = atoi(argv[i] + 2);
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
            printf("Utilizare: cpd [fanion...] sursa dest\n");
            return 1;
        }
    }

    if (sursa == NULL || destinatie == NULL)
    {
        printf("Utilizare: cpd [fanion...] sursa dest\n");
        return 1;
    }

    if (este_in_arbore(sursa, destinatie))
    {
        fprintf(stderr, "Eroare: dest nu poate fi in arborele de subdirectoare al lui sursa.\n");
        return 1;
    }

    copiaza_recursiv(sursa, destinatie, 1, include_ascunse, nu_suprascrie, sterge_sursa, max_nivel);
    return 0;
}
