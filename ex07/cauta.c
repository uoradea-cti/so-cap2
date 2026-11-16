/*
 * Problema 7
 * Realizati programul cauta care cauta, recursiv, fisierele cu extensia
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
int numar_aparitii_in_fisier(const char *cale_fisier, const char *text)
{
    size_t len_text = strlen(text);
    if (len_text == 0)
    {
        return 1;
    }

    int fd = _open(cale_fisier, _O_RDONLY | _O_BINARY);
    if (fd == -1)
    {
        return 0;
    }

    char buffer[4096];
    char *fereastra = (char *)malloc(len_text + sizeof(buffer));
    if (fereastra == NULL)
    {
        _close(fd);
        return 0;
    }

    size_t ramas = 0;
    int aparitii = 0;
    int octeti;

    while ((octeti = _read(fd, buffer, sizeof(buffer))) > 0)
    {
        memcpy(fereastra + ramas, buffer, octeti);
        size_t total = ramas + (size_t)octeti;

        if (total >= len_text)
        {
            size_t limita = total - len_text;
            size_t i = 0;
            while (i <= limita)
            {
                if (memcmp(fereastra + i, text, len_text) == 0)
                {
                    aparitii++;
                    i += len_text;
                }
                else
                {
                    i++;
                }
            }

            ramas = total - i;
            if (ramas > len_text - 1)
            {
                ramas = len_text - 1;
            }
            if (ramas > 0)
            {
                memmove(fereastra, fereastra + (total - ramas), ramas);
            }
        }
        else
        {
            ramas = total;
        }
    }

    free(fereastra);
    _close(fd);
    return aparitii;
}

void cauta_recursiv(const char *director, const char *ext, const char *text, int min_aparitii)
{
    char sablon[512];
    char cale_relativa[512];
    char cale_completa[_MAX_PATH];
    struct _finddata_t fisier;
    intptr_t handle;

    if (ext[0] == '.')
    {
        snprintf(sablon, sizeof(sablon), "%s\\*%s", director, ext);
    }
    else
    {
        snprintf(sablon, sizeof(sablon), "%s\\*.%s", director, ext);
    }

    handle = _findfirst(sablon, &fisier);
    if (handle != -1)
    {
        do
        {
            if (!(fisier.attrib & _A_SUBDIR))
            {
                snprintf(cale_relativa, sizeof(cale_relativa), "%s\\%s", director, fisier.name);
                if (numar_aparitii_in_fisier(cale_relativa, text) >= min_aparitii)
                {
                    if (_fullpath(cale_completa, cale_relativa, _MAX_PATH) != NULL)
                    {
                        printf("%s\n", cale_completa);
                    }
                    else
                    {
                        printf("%s\n", cale_relativa);
                    }
                }
            }
        } while (_findnext(handle, &fisier) == 0);

        _findclose(handle);
    }

    snprintf(sablon, sizeof(sablon), "%s\\*.*", director);
    handle = _findfirst(sablon, &fisier);
    if (handle != -1)
    {
        do
        {
            if (strcmp(fisier.name, ".") == 0 || strcmp(fisier.name, "..") == 0)
            {
                continue;
            }

            if (fisier.attrib & _A_SUBDIR)
            {
                snprintf(cale_relativa, sizeof(cale_relativa), "%s\\%s", director, fisier.name);
                cauta_recursiv(cale_relativa, ext, text, min_aparitii);
            }
        } while (_findnext(handle, &fisier) == 0);

        _findclose(handle);
    }
}
int main(int argc, char *argv[])
{
    /* Scrieti solutia aici */
int min_aparitii = 1;
    const char *ext = NULL;
    const char *text = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "-c", 2) == 0 && strlen(argv[i]) > 2 && ext == NULL)
        {
            min_aparitii = atoi(argv[i] + 2);
            if (min_aparitii < 1)
            {
                min_aparitii = 1;
            }
        }
        else if (ext == NULL)
        {
            ext = argv[i];
        }
        else if (text == NULL)
        {
            text = argv[i];
        }
        else
        {
            printf("Utilizare: cauta [fanion] ext text\n");
            return 1;
        }
    }

    if (ext == NULL || text == NULL)
    {
        printf("Utilizare: cauta [fanion] ext text\n");
        return 1;
    }

    cauta_recursiv(".", ext, text, min_aparitii);
    return 0;
}
