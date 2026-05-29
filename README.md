# Pacman in C

Acesta este un joc asemanator cu Pacman, realizat in C si care ruleaza in terminal.
Jucatorul controleaza caracterul, evita fantomele si colecteaza cat mai multe puncte.


## Functionalitati

- harta afisata in terminal
- miscare continua pe baza conceptului de tic
- control cu W, A, S, D
- colectare de puncte si sistem de scor
- high score salvat in fisier
- 3 fantome care urmaresc jucatorul
- sistem de vieti
- meniu principal
- 3 niveluri de dificultate
- functie de pauza


## Controale

- 'W' - sus
- 'A' - stanga
- 'S' - jos
- 'D' - dreapta
- 'P' - pauza/reluare joc
- 'Q' - iesire joc


### Structura proiectului

- main.c - meniul principal si bucla jocului
- game.c / game.h - logica jocului
- map.c / map.h - definirea hartii
- render.c / render.h - afisarea jocului in terminal
- input.c / input.h - citirea inputului fara enter
- Makefile - compilarea proiectului
- highscore.txt - fisierul cu scorul maxim

# compilare:
    make

# rulare:
    ./pacman

