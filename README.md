# Sisteme de Operare – Capitolul 2

**1.** Realizați un program care primește în linia de comandă un nume de director pe care trebuie să îl șteargă. Directorul primit ca parametru poate să nu fie vid și poate conține, la rândul său, alte directoare. Pentru parcurgerea întregului arbore de subdirectoare, se va realiza o funcție recursivă care primește ca parametru un subdirector, schimbă directorul curent în subdirector, caută toate fișierele `*.*` indiferent de atribut, pe cele obișnuite le șterge (dacă sunt read-only le modifică mai întâi atributul), iar pentru subdirectoare se apelează recursiv, iar în final schimbă directorul curent în părinte.

**2.** Realizați un program care va fi apelat astfel:
```
cpd [fanion...] director_sursa director_destinatie
```
care mută cu același nume toate fișierele din `director_sursa` în `director_destinatie` împreună cu toată structura arborescentă. Fanioanele indică următoarele opțiuni:
- `-a` – se mută și fișierele ascunse;
- `-dx` – unde x este un număr; se mută fișiere până la nivelul x de imbricare; nivel de imbricare 1 înseamnă că nu se intră în subdirectoare;
- `-lx` – unde x este un număr; se mută doar fișierele cu lungime mai mică sau egală cu x.

Se va verifica ca directorul destinație să nu fie pe arborele de subdirectoare al directorului sursă, deoarece în acest caz programul ar putea cicla.

**3.** Realizați un program de căutare de fișier, specificat printr-un nume extins (poate conține `?`, `*`) pornind de la directorul curent. Pentru fiecare fișier găsit se va afișa calea completă și numele.

**4.** Să se realizeze un program care primește în linia de comandă numele a două fișiere, sursa și dest, și realizează copierea lui sursa în dest. Se va verifica să nu coincidă sursa și dest. Se va lucra fără redirecționări, duplicări.

**5.** Se consideră un fișier existent, „test.tst”. Se cere să se adauge la sfârșitul său toate cifrele care îi aparțin.

Exemplu:
- inițial: `abc123de6fghi12jk`
- final: `abc123de6fghi12jk123612`

Se execută pe rând:
- citire cu memorarea cifrelor întâlnite într-un buffer de memorie;
- când bufferul este plin:
  - determinarea poziției curente și memorarea ei în variabila crt;
  - poziționare pe EOF;
  - scriere buffer;
  - repoziționare la crt.

Pentru ca programul să nu cicleze umplând discul, se va determina inițial poziția EOF, iar citirile nu se vor face dincolo de acest EOF inițial.

**6.** Să se implementeze un program cpd care se apelează prin:
```
cpd [fanion ...] sursa dest
```
care copiază arborele de fișiere din sursa în dest, în conformitate cu fanioanele:
- `-a` – se copiază și fișierele ascunse;
- `-o` – nu se suprascriu fișierele existente în destinație;
- `-u` – se șterg fișierele din sursa;
- `-dx` – se copiază fișierele până la nivelul x de imbricare.

Inițial, se va testa dacă dest este în arborele de subdirectoare de la sursa, caz în care programul se oprește cu mesaj de eroare.

**7.** Să se scrie un program cauta care se apelează prin:
```
cauta [fanion] ext text
```
care caută acele fișiere cu extensia ext care conțin șirul de caractere text. Căutarea se va realiza în mod recursiv, iar textul va fi căutat în conținutul fișierului, și nu în numele acestuia. Poate fi folosit și fanionul:
- `-cx` – specifică de minim câte ori trebuie să apară textul în fișier.

**8.** Realizați o aplicație care sortează conținutul unui fișier folosind algoritmul Bubble Sort. Nu se acceptă folosirea unui tablou, ci numai folosirea funcției lseek.
