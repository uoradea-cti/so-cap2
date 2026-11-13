/*
 * Problema 2
 * cpd - muta cu acelasi nume fisierele din director_sursa in
 * director_destinatie, pastrand structura arborescenta.
 *   -a   se muta si fisierele (si directoarele) ascunse
 *   -dx  se muta fisiere pana la nivelul x de imbricare (1 = fara subdirectoare)
 *   -lx  se muta doar fisierele cu lungimea <= x
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

#define MAX_PATH_LEN 1024

typedef struct {
    int  ascunse;   
    long nivel_max; 
    long lung_max;  
} Optiuni;


static int copiaza_fisier(const char *src, const char *dst)
{
    FILE *fs, *fd;
    char buf[65536];
    size_t n;
    int rez = 0;

    fs = fopen(src, "rb");
    if (!fs) { perror(src); return -1; }
    fd = fopen(dst, "wb");
    if (!fd) { perror(dst); fclose(fs); return -1; }

    while ((n = fread(buf, 1, sizeof(buf), fs)) > 0) {
        if (fwrite(buf, 1, n, fd) != n) {
            perror(dst);
            rez = -1;
            break;
        }
    }
    fclose(fs);
    fclose(fd);
    return rez;
}


static int muta_fisier(const char *src, const char *dst)
{
    if (_access(dst, 0) != 0 && rename(src, dst) == 0)
        return 0;

    if (copiaza_fisier(src, dst) != 0)
        return -1;

    _chmod(src, _S_IREAD | _S_IWRITE);   
    if (remove(src) != 0) {
        perror(src);
        return -1;
    }
    return 0;
}


static int muta_arbore(const char *src, const char *dst, long nivel, const Optiuni *opt)
{
    struct _finddata_t fd;
    intptr_t h;
    char pattern[MAX_PATH_LEN];
    char cs[MAX_PATH_LEN], cd[MAX_PATH_LEN];
    int rezultat = 0;

    sprintf(pattern, "%s\\*", src);
    h = _findfirst(pattern, &fd);
    if (h == -1) {
        fprintf(stderr, "Nu pot deschide directorul: %s\n", src);
        return -1;
    }

    do {
        if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
            continue;

        
        if ((fd.attrib & _A_HIDDEN) && !opt->ascunse)
            continue;

        if (strlen(src) + strlen(fd.name) + 2 >= MAX_PATH_LEN ||
            strlen(dst) + strlen(fd.name) + 2 >= MAX_PATH_LEN) {
            fprintf(stderr, "Calea este prea lunga: %s\\%s\n", src, fd.name);
            rezultat = -1;
            continue;
        }
        sprintf(cs, "%s\\%s", src, fd.name);
        sprintf(cd, "%s\\%s", dst, fd.name);

        if (fd.attrib & _A_SUBDIR) {
           
            if (opt->nivel_max == 0 || nivel < opt->nivel_max) {
                if (_mkdir(cd) != 0 && _access(cd, 0) != 0) {
                    perror(cd);
                    rezultat = -1;
                    continue;
                }
                if (muta_arbore(cs, cd, nivel + 1, opt) != 0)
                    rezultat = -1;
            }
        } else {
            if (opt->lung_max >= 0 && (long)fd.size > opt->lung_max)
                continue;
            if (muta_fisier(cs, cd) != 0)
                rezultat = -1;
        }
    } while (_findnext(h, &fd) == 0);

    _findclose(h);
    return rezultat;
}


static void taie_separator(char *p)
{
    size_t n = strlen(p);
    while (n > 3 && (p[n - 1] == '\\' || p[n - 1] == '/'))
        p[--n] = '\0';
}


static int dest_in_sursa(const char *src, const char *dst)
{
    size_t n = strlen(src);

    if (_strnicmp(src, dst, n) != 0)
        return 0;
    return dst[n] == '\0' || dst[n] == '\\' || src[n - 1] == '\\';
}

int main(int argc, char *argv[])
{
    Optiuni opt;
    const char *dir_src = NULL, *dir_dst = NULL;
    char abs_src[MAX_PATH_LEN], abs_dst[MAX_PATH_LEN];
    struct _stat st;
    char *end;
    int i;

    opt.ascunse = 0;
    opt.nivel_max = 0;
    opt.lung_max = -1;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-a") == 0) {
                opt.ascunse = 1;
            } else if (argv[i][1] == 'd' && argv[i][2] != '\0') {
                opt.nivel_max = strtol(argv[i] + 2, &end, 10);
                if (*end != '\0' || opt.nivel_max < 1) {
                    fprintf(stderr, "Fanion invalid: %s\n", argv[i]);
                    return 1;
                }
            } else if (argv[i][1] == 'l' && argv[i][2] != '\0') {
                opt.lung_max = strtol(argv[i] + 2, &end, 10);
                if (*end != '\0' || opt.lung_max < 0) {
                    fprintf(stderr, "Fanion invalid: %s\n", argv[i]);
                    return 1;
                }
            } else {
                fprintf(stderr, "Fanion necunoscut: %s\n", argv[i]);
                return 1;
            }
        } else if (!dir_src) {
            dir_src = argv[i];
        } else if (!dir_dst) {
            dir_dst = argv[i];
        } else {
            fprintf(stderr, "Prea multe argumente.\n");
            return 1;
        }
    }

    if (!dir_src || !dir_dst) {
        fprintf(stderr, "Utilizare: %s [-a] [-dx] [-lx] director_sursa director_destinatie\n", argv[0]);
        return 1;
    }

    
    if (_stat(dir_src, &st) != 0 || !(st.st_mode & _S_IFDIR)) {
        fprintf(stderr, "Sursa nu este un director valid: %s\n", dir_src);
        return 1;
    }

    
    if (!_fullpath(abs_src, dir_src, MAX_PATH_LEN) ||
        !_fullpath(abs_dst, dir_dst, MAX_PATH_LEN)) {
        fprintf(stderr, "Nu pot determina calea absoluta.\n");
        return 1;
    }
    taie_separator(abs_src);
    taie_separator(abs_dst);

    if (dest_in_sursa(abs_src, abs_dst)) {
        fprintf(stderr, "Eroare: destinatia se afla in arborele directorului sursa.\n");
        return 1;
    }

    
    if (_mkdir(abs_dst) != 0 && _access(abs_dst, 0) != 0) {
        perror(abs_dst);
        return 1;
    }

    if (muta_arbore(abs_src, abs_dst, 1, &opt) != 0) {
        fprintf(stderr, "Mutarea nu s-a finalizat complet.\n");
        return 1;
    }

    printf("Mutare finalizata.\n");
    return 0;
}