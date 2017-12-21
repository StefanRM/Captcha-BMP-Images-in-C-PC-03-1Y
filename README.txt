.___________. _______ .___  ___.      ___          ____  
|           ||   ____||   \/   |     /   \        |___ \ 
`---|  |----`|  |__   |  \  /  |    /  ^  \         __) |
    |  |     |   __|  |  |\/|  |   /  /_\  \       |__ < 
    |  |     |  |____ |  |  |  |  /  _____  \      ___) |
    |__|     |_______||__|  |__| /__/     \__\    |____/ 



===Task1===

	Functiile readBMP() si writeBMP() citesc, respectiv scriu un fisier BMP
dat ca parametru functiilor respective, impreuna cu structura din bmp_header.h .

	Dupa ce s-a deschis fisierul input.txt, se citeste numele captcha-ului in
variabila captcha_name si se construieste numele pentru fisierul output. Pentru
numele output-ului se copiaza toate caracterele fara ultimele 4 din captcha_name
(".bmp" are 4 caractere) si se alipeste "_task1.bmp". Apoi se deschide si fisierul
output, dar in scriere.
	Se citesc cele 3 canale BGR din input.txt si se citeste header-ul si
infoheader-ul din captcha. Pentru ca fiecare linie din datele fisierului BMP trebuie
sa aiba un multiplu de 4 byte se calculeaza un padding care trebuie adaugat la fiecare
linie ca sa se completeze la un multiplu de 4. Fiecare linie are un pixel a cate 3 bytes
(3 canale BGR), astfel daca numarul de pixeli (width) inmultita cu 3 (3 canale) nu e
multiplu de 4, se atribuie unei variabile padding diferenta pentru a fi multiplu de 4.
	Se construieste matricea de pixeli in felul urmator: daca e pixel alb 
(BGR: 255 255 255) atunci se pune '.', altfel se pune 'X'. In acelasi timp daca e 
pixel colorat ('X') se scire in output culoarea data de valorile citite din input.txt.
De mentionat ca la citirea datelor se sare peste padding-ul din captcha, iar la scriere
se adauga fiecarei linii valoarea 0 in locul unde trebuie sa fie padding.
	La final se afiseaza matricea si se inchid fisierele. 
	


===Task2===

	Functiile readBMP() si writeBMP() citesc, respectiv scriu un fisier BMP
dat ca parametru functiilor respective, impreuna cu structura din bmp_header.h .

	Se deschide fisierul input.txt si, prin acelasi procedeu ca la Task1, se
creeaza numele output-ului (acum se adauga la final "_task2.txt"), creandu-se 
matricea de pixeli, la fel ca la Task1.
	Folosind observatia ca, in afara de cifra 1, toate celalalte au in coltul
din stanga sus un 'X' se afla ce fel de cifra e (s-au definit la inceputul programului
siruri de caractere pentru fiecare cifra). Pentru cifra 1 se verifica daca intr-un bloc
de 5x5 la punctele din prima coloana ii corespund un 'X' pe ultima coloana; apoi se pune
'.' in tot blocul ca sa nu afecteze urmatoarele cifre. Pentru restul cifrelor se 
construieste un sir de caractere care se compara cu cele definite initial. Apoi se pune
'.' in tot blocul 5x5 in care s-a gasit cifra.
	La fiecare cifra gasita se scrie valoarea ei in output-ul text creat. La final se
inchid fisierele.



===Task3===

	Functiile readBMP() si writeBMP() citesc, respectiv scriu un fisier BMP
dat ca parametru functiilor respective, impreuna cu structura din bmp_header.h .

	Se deschide fisierul input.txt si, prin acelasi procedeu ca la Task1, se
creeaza numele output-ului (acum se adauga la final "_task3.bmp"), creandu-se 
matricea de pixeli, la fel ca la Task1, dar in acelasi timp se creeaza matricea de
culori pe 3 canale BGR.
	Fiecare linie din fisier s-a citit intr-un sir de caractere collector. Dupa
ce s-a citit linia 3, se parcurge sirul de caractere collector, se convertesc cifrele 
in integer prin diferenta de cod ASCII si se adauga valoarea cifrei intr-un vector vector_vals.
	Se construieste vectorul de culori, parcurgandu-se matricea de pixeli, iar la
fiecare cifra gasita folosind procedeul de la Task2, se adauga culoarea acelei cifre in
vectorul colors. Apoi se elimina cifra din matricea de pixeli, ca sa nu influenteze in
continuare. Dupa se reface matricea de pixeli folosind copia din matrice_aux.
	Se cauta cifrele in matricea de pixeli, folosind procedeul de la Task2, apoi se
cauta cifra in vectorul pentru eliminat. Daca trebuie eliminata si pune un indice 'Y' in
coltul din stanga-sus al blocului de 5x5 al cifrei, si in rest se pune '.', iar culoarea
din vectorul colors e eliminata.
	Apoi pentru fiecare 'Y' din matricea de pixeli se suprapune cifra din dreapta ei.
Daca sunt doi 'Y' unul dupa altul se trece la cel de-al doilea 'Y' si se reia algoritmul.
Totul se face de un nr. ales (20) astfel incat sa fie sigur ca s-au mutat toate cifrele
la stanga, fara sa fi ramas vreun 'Y'. Apoi se sterg toti 'Y' din matrice.
	Se reinitializeaza matricea de culori ca fiind alba. Apoi se parcurge concomitent
cu matricea de pixeli astfel incat la fiecare cifra intalnita in matricea de pixeli sa se
coloreze pixelii corespunzatori in matricea de culori, folosindu-se vectorul de culori colors,
care are in ordine culorile fiecarei cifre de dupa eliminare.
	Apoi se scrie fisierul BMP output, folosind matricea de pixeli si matricea de culori.
La fel ca la Task1, se trece 0 dupa fiecare linie pentru padding-ul calculat. La final se 
inchid fisierele.



===Bonus===

	Functiile readBMP() si writeBMP() citesc, respectiv scriu un fisier BMP
dat ca parametru functiilor respective, impreuna cu structura din bmp_header.h .

	Se deschide fisierul input.txt si, prin acelasi procedeu ca la Task1, se
creeaza numele output-ului (acum se adauga la final "_bonus.txt"), creandu-se 
matricea de pixeli, la fel ca la Task1.
	De aceasta data, se observa ca fiecare cifra are in coltul din dreapta-jos un
'X'. Astfel se parcurge matricea de pixeli invers (de la ultima coloana la prima).
Pentru fiecare bloc de 5x5 se compara daca e o cifra, folosindu-se siruri de caractere
predefinite pentru fiecare cifra. Intai se compara cu sirurile de caractere, iar daca
e o cifra se adauga in vectorul vec_nr. Daca nu s-a gasit cifra, se calculeaza o diferenta
diff fata de sirul de caractere predefinit. Daca aceasta diferenta e 1 atunci se adauga cifra
in vectorul vec_nr. Dupa fiecare cifra gasita se atribuie unei variabile valoarea 1 care
inseamna ca nu mai are rost sa se caute in continuare; in acelasi timp, dupa ce s-a gasit
cifra, aceasta se elimina din matricea de pixeli (tot blocul 5x5 va avea doar caracterul '.')
pentru a nu influenta cautarea in continuare.
	Apoi se parcurge vectorul vec_nr invers, afisandu-se in fisierul text output cifrele
gasite. La final se inchid fisierele.

