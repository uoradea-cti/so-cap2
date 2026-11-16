/*
 * Problema 3
 * Realizati un program de cautare de fisier, specificat printr-un nume
 * extins (poate contine ?, *), pornind de la directorul curent.
 * Pentru fiecare fisier gasit se afiseaza calea completa si numele.
 *
 * Utilizare: ex03 tipar   (ex: ex03 *.txt)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
void cauta_fisiere(const char *director, const char *tipar)
{
    char sablon[512];
    char cale_relativa[512];
    char cale_completa[_MAX_PATH];
    struct _finddata_t fisier;
    intptr_t handle;

    snprintf(sablon, sizeof(sablon), "%s\\%s", director, tipar);
    handle = _findfirst(sablon, &fisier);
    if (handle != -1)
    {
        do
        {
            if (!(fisier.attrib & _A_SUBDIR))
            {
                snprintf(cale_relativa, sizeof(cale_relativa), "%s\\%s", director, fisier.name);
                if (_fullpath(cale_completa, cale_relativa, _MAX_PATH) != NULL)
                {
                    printf("%s\n", cale_completa);
                }
                else
                {
                    printf("%s\n", cale_relativa);
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
                cauta_fisiere(cale_relativa, tipar);
            }
        } while (_findnext(handle, &fisier) == 0);

        _findclose(handle);
    }
}
int main(int argc, char *argv[])
{
    /* Scrieti solutia aici */
    if (argc != 2)
    {
        printf("Utilizare: %s tipar\n", argv[0]);
        return 1;
    }

    cauta_fisiere(".", argv[1]);

    return 0;
}
