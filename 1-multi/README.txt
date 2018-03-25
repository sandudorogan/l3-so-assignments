Tema 1 SO
===========


Solutia:
	Ideea consta in a parsa liniile de string in comenzi, a vedea ce comanda e, 
iar aceasta sa fie executata. Sunt cat de multe functii utilitare in 
modulul listei si a hashmap-ului, folosite la fiecare posibilitate in programul
de baza. 
De notat, de exemplu, resize-ul. Acesta foloseste o functie de lista, 
take_first. Functia returneaza pointerul spre primul element din lista, 
stergand orice link de la si catre lista a elementului returnat. Elementul este
apoi introdus cu functia add_at_end. 

	Hashmap-ul a fost implementat cu ajutorul unei liste dublu inlantuite, cu o
santinela. Santinela tine mereu minte primul si ultimul element al listei, 
altfel stau ambele pe NULL. In orice moment, primul emelent are prev-ul NULL,
iar ultimul next-ul NULL.


Structura:
	Am incercat sa o impart cat mai mult pe module.
Rezultatul consta in fisiere dedicate cerintei temei, listei si hashmap-ului.
Ultimele doua am incercat sa le fac pe cat de generice. 
Am mai ajuns si cu un fisier de utilitati. Toate functiile ce nu le-am gasit in
API-ul standardului ISO, sunt introduse acolo. 

Sursa la my_getline:
https://stackoverflow.com/questions/735126/
are-there-alternate-implementations-of-gnu-getline-interface/735472#735472


GitLab:
	https://gitlab.cs.pub.ro/sandu.dorogan/l3-so-assignments

