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

    return 0;
}
