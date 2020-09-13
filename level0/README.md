# Level 0

Lorsqu'on lance le binaire `level0` sans argument, le programme segfault. Avec différents arguments, le programme nous répond "No !" :
```shell
~ ./level0
Segmentation fault (core dumped)
~ ./level0 abc
No !
~ ./level0 123
No !
```

Vu qu'il segfault sans argument, je peux regarder avec gdb à quelle instruction il segfault :
```shell
~ gdb level0
[...]
(gdb) r
Starting program: /home/user/level0/level0

Program received signal SIGSEGV, Segmentation fault.
0x08049aff in ____strtol_l_internal ()
(gdb) backtrace
#0  0x08049aff in ____strtol_l_internal ()
#1  0x08049aaa in strtol ()
#2  0x0804972f in atoi ()
#3  0x08048ed9 in main ()
```

En examinant de plus près le code on peut voir qu'il segfault au niveau d'un [`atoi`](https://linux.die.net/man/3/atoi) non protégé. Il exécute `atoi` en lui passant le premier argument et compare la sortie d'`atoi` pour savoir si il est égal à `0x1a7` (soit `423` en décimal).
```
(gdb) disassemble main
Dump of assembler code for function main:
[...]
   0x08048ecc <+12>:	add    $0x4,%eax
   0x08048ecf <+15>:	mov    (%eax),%eax
   0x08048ed1 <+17>:	mov    %eax,(%esp)
   0x08048ed4 <+20>:	call   0x8049710 <atoi>
   0x08048ed9 <+25>:	cmp    $0x1a7,%eax
[...]
End of assembler dump.
```

Si le retour d'`atoi` est bien égal à 423, alors le programme exécute `/bin/sh` avec l'utilisateur `level1`.  
Il ne nous reste donc plus qu'à exécuter le programme avec pour argument `423` :
```shell
level0@RainFall:~$ ls -la level0
-rwsr-x---+ 1 level1 users 747441 Mar  6  2016 level0
level0@RainFall:~$ ./level0 423
$ whoami
level1
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```