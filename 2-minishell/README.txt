================================================================================
					Sandu Dorogan 335 CA
							Tema 2
================================================================================
================================================================================

Mulumesc pentru schelet si parser. Acestea au micsorat semnificativ 
timpul de rezolvare. Mi se par foarte destept facute.

Am sa incerc sa explic pe pasi rezolvarea. 

> Simple_command, in mare, face un fork si un execvp pe comanda data.
  * Aici se indeplineste formatul de eroare cerut.
  * Cand se intalneste 'exit' sau 'quit', se iese.
  * Cand stringul comenzii are un '=', apel de sistem, setare de variabila.
  * Cand e 'cd', se face fork, pentru a salva outputul, dar si un chdir intern
  shell-ului.
  * Aici au fost foarte de folos cele doua functii din util:

> Redirectarile se realizeaza inainte fork-ului din simple_command. Ca rezultat,
procesul copil are intrarile si iesirile setate dupa necesitate.
  * Sunt folosite structurile din parses, foarte destept puse acolo.
  * Operatorul &> trebuie la out sa aibe modul create, iar la err, append.
  Se intampla asta cand numele fisierelor pentru out si err coincid.

> Secventierea comenzilor ruleaza parse command pentru ambele comenzi, luand
in considerare valoarea de retur a acesteia. Fiecare secventiere este 
conditionata dupa cerinta. De exemplu, daca e operatorul ||, atunci a doua 
parsarea la a doua comanda este apelata doar atunci cand prima comanda 
nu intoarce zero.

> Paralelizarea este realizata ca procesul curent sa apeleze o parsare, iar
procesul copil al sau, sa realizeze o alta parsare de comanda.

> Pipe-ul nu tre sa interfereze cu fisierele standard a shellului, asa ca am 
ales sa formez doua procese copil, cate unul pentru fiecare comanda, iar la 
fiecare sa le modific intrarea si iesirea in dependenta de directia pipe-ului.
Foarte important: tre de inchis toate descriptor-ile de fisier, chiar si cele
duplicate.

> Paralelizarea si pipe-ul intorc valoarea de iesire a celei de-a doua comenzi.

> GitLab:
 https://gitlab.cs.pub.ro/sandu.dorogan/l3-so-assignments/tree/master/2-minishell/skel-linux