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
#include <dos.h>
#include <dir.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct _finddata_t fileinfo;
    char path[260], newPath[260];
    intptr_t value;

    if (argc != 2)
        return 1;

    strcpy(path, argv[1]);
    chdir(path);

    value = _findfirst("*.*", &fileinfo);

    if (value != -1)
    {
        do
        {
            if (strcmp(fileinfo.name, ".") != 0 &&
                strcmp(fileinfo.name, "..") != 0)
            {
                if (fileinfo.attrib & _A_SUBDIR)
                {
                    strcpy(newPath, fileinfo.name);
                    chdir(newPath);
                    /* Apel recursiv */
                    chdir("..");
                }
                else
                {
                    if (fileinfo.attrib & FA_RDONLY)
                        _chmod(fileinfo.name, 1, FA_ARCH);

                    remove(fileinfo.name);
                }
            }
        } while (_findnext(value, &fileinfo) == 0);

        _findclose(value);
    }

    chdir("..");
    rmdir(path);

    return 0;
}
