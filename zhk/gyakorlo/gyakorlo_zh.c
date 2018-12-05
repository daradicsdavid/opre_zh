#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork, pipe
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h> // for pipe()
#include <string.h>


/*
Feladat: Választunk! Egy választási körzetben 3 tagból áll a választási bizottság: az elnökből (szülő)
és két tagból (gyerekek).
Az első gyerek ellenőrzi a választók adatait, a második pedig lepecsételi és kiadja a választási lapot.

1) Az elnök megvárja, amíg mindkét tag nem jelzi, hogy kész a munkára (jelzéssel), ezután a
parancssori argumentumokon keresztül
"fogadja" a szavazókat (hány darab érkezik). A szülő generáljon minden szavazóhoz egy véletlen azonosítószámot!
A választók azonosítószámait egy csövön keresztül továbbítja az adatellenőrző tagnak, aki kiírja,
hogy fogadta az adatokat és kiírja
a képernyőre. A szülő bevárja a gyerekek befejezését.

2) Az ellenőrzést végző tag a választói azonosításhoz adatellenőrzést végez. Minden azonosító 20% eséllyel
hibás. A hibás azonosítójú választó nem szavazhat. Az ellenőrzést végző gyerek így a szavazhat/nem szavazhat
jelzéssel csővezetékbe írja az adatokat, majd erről jelzést küld a választási lapokat kezelő testvérének,
aki ezt a képernyőre írja. A választási lapokat kezelő testvér egy összesítést végez, megadja, hogy hány
érvényes/érvénytelen szavazó jelent meg, majd ezt az összesített adatot továbbítja csövön keresztül a szülőnek,
aki erről jegyzőkönyvet (érvényes-érvénytelenek százalékos aránya) készít egy szöveges fájlba. A szülő ezt a
képernyőre is kiírja.

*/
void handler(){

}



int main(int argc,char* argv[]){

    pid_t child1, child2;
    signal(SIGUSR1,handler);
    signal(SIGUSR2,handler);
    signal(SIGTERM,handler);

    int pipefd[2];
    int pipefd2[2];


    if (pipe(pipefd) == -1){
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd2) == -1){
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    child1=fork();

    if(child1>0){
        //printf("En vagyok az elnok.\n");

        child2 = fork();
        if(child2 == 0){

            //printf("Lepecsetelem es kiadom a valasztok adatait.\n");
            kill(getppid(),SIGUSR1);
            int n = atoi(argv[1]);
            int i;
            int counter=0;
            char buffer[20];
            pause();
            for(i=0;i<n;++i){

                read(pipefd[0],buffer,sizeof(buffer));
                printf("%i.szavazo statusza: %s\n",i+1,buffer);
                if(strcmp(buffer,"Szavazhat") == 0){
                    ++counter;
                }
            }


            sprintf(buffer,"%i",counter);
            write(pipefd2[1],buffer,sizeof(buffer));


            //printf("Pecsetelo bizottsag vegzett.\n");
            close(pipefd[0]);
            close(pipefd[1]);
            close(pipefd2[0]);
            close(pipefd2[1]);


        }else{

            printf("Varok a bizottsagokra.\n");
            pause();
            pause();
            printf("Mindket bizottsag keszen all!\n");

            if(argc != 2){
                printf("Adja meg, hany szavazo erkezik!\n");
            }else{
                int n = atoi(argv[1]);
                int id[n];
                srand(time(NULL));
                int status;
                int i;
                for(i=0;i<n;++i){
                    id[i] = rand()%100;
                    char idbuffer[10];
                    sprintf(idbuffer,"%i",id[i]);
                    write(pipefd[1],idbuffer,sizeof(idbuffer));
                }

                char buffer[20];
                read(pipefd2[0],buffer, sizeof(buffer));
                int szavazok = atoi(buffer);
                printf("%i szavazatbol %i volt ervenyes, %i pedig ervenytelen.\n",n,szavazok,n-szavazok);

                close(pipefd[1]);
                close(pipefd[0]);
                close(pipefd2[0]);
                close(pipefd2[1]);

                waitpid(child1,&status,0);
                waitpid(child2,&status,0);
                printf("Elnok vegzett.\n");

            }
        }
    }else if(child1 == 0){

        kill(getppid(),SIGUSR1);
        char buffer[10];
        int n = atoi(argv[1]);
        int i;
        int ervenyes;
        srand(time(NULL));
        for(i=0;i<n;++i){
            read(pipefd[0],buffer,sizeof(buffer));
            printf("%i. valaszto azonositoja:%s\n", i+1, buffer);
            ervenyes = rand()%5;
            if(ervenyes == 0){
                write(pipefd[1],"Nem szavazhat",20);
            }else{
                write(pipefd[1],"Szavazhat",20);
            }
        }
        kill(child2,SIGUSR2);

        close(pipefd[1]);
        close(pipefd[0]);
        close(pipefd2[0]);
        close(pipefd2[1]);
        //printf("Ellenorzo bizottsag vegzett.\n");
    }


    return 0;

}

//waitpid(child,&status,0);