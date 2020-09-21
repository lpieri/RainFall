## bonus3
- Il n'y qu'une seule chose à retenir du [code source désassemblé](source.c) :
    - Il faut remplir la condition du `strcmp`, c'est-à-dire faire en sorte que `bufferOne` et `argv[1]` soient égaux. `bufferOne` est rempli avec le contenu de `/home/user/end/.pass`, et comme les autres fichiers `.pass` on peut supposer qu'il s'agit d'une chaîne hexadécimale aléatoire.
    - Mais il y a un moyen très simple de contourner cette difficulté : en passant une chaîne vide comme `argv[1]`, alors `bufferOne[0]` sera égal à `\0` (autrement dit, `bufferOne` sera une chaîne vide). Si les deux chaînes sont vides, alors le `strcmp` aboutira.
- En effet, `./bonus3 ""` permet bien de lancer le *shell*. `cat /home/user/end/.pass` pour obtenir le dernier *flag*.