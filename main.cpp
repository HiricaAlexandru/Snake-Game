#include <iostream>
#include <cstring>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
using namespace std;


#define WIDTH 40 //dimensiunile patratului in care se desfasoara jocul
#define HEIGHT 30

struct poz{
    int x,y;
};

struct jucator{
    poz loc;
    static int nrTot;
};

int jucator::nrTot = 2;
int scor = 0;

void flush(char harta[HEIGHT+2][WIDTH+3]){//functia sterge continutul matricei
    for(int i = 1;i<=HEIGHT;i++)
        for(int j = 1;j<=WIDTH;j++)
            harta[i][j] = ' ';
    }

void show(char harta[HEIGHT+2][WIDTH+3]){//afiseaza matricea ca sir de caractere luat pe linii din motive de performanta
    for(int i = 0;i<=HEIGHT+1;i++){
            cout<<harta[i];
        cout<<'\n';
    }

}

void punMarginile(char harta[HEIGHT+2][WIDTH+3]){//adauga marginile cu '#' si de asemenea pune NULL la sfarsitul fiecarei linii

    for(int i = 0;i<=HEIGHT+1;i++){
        harta[i][0] = '#';
        harta[i][WIDTH+1] = '#';
        harta[i][WIDTH+2] = 0;

    }

    for(int i = 0;i<=WIDTH+1;i++){
        harta[0][i] = '#';
        harta[HEIGHT+1][i] = '#';
    }

}

void init(char harta[HEIGHT+2][WIDTH+3]){//acesta este ecranul ce se afiseaza la inceput
    char intro[] = "ATESTAT 2020";
    char nume[] = "Hirica Ioan Alexandru";

    punMarginile(harta);

    char numeJOC[] = "Snake";
    char buton[] = "Apasati orice buton";
    strcpy(harta[HEIGHT/2-HEIGHT/8]+WIDTH/2-strlen(intro)/2,intro);
    strcpy(harta[HEIGHT/2-HEIGHT/8+1]+WIDTH/2-strlen(nume)/2,nume);
    strcpy(harta[HEIGHT/2-HEIGHT/8+2]+WIDTH/2-strlen(numeJOC)/2,numeJOC);
    strcpy(harta[HEIGHT/2-HEIGHT/8+6]+WIDTH/2-strlen(nume)/2,buton);


    for(int i = 0;i<=HEIGHT+1;i++){//afisez element cu element pentru ca avem 2 null uri pe linie si functia show nu ar fi functionat fara modificari
             for(int j = 0;j<=WIDTH+1;j++)
                cout<<harta[i][j];
        cout<<'\n';
    }
    while(1){//asteapta ca un buton sa fie afisat pentru a incepe jocul
        if(kbhit())
            return;
        //Sleep(1);

    }

    flush(harta);


}

void generezJucatorul(jucator *coada){//pune coordonatele jucatorului

    (coada+1)->loc.x = 5;
    (coada+1)->loc.y = 5;
    (coada+2) -> loc.x = 5;
    (coada+2) ->loc.y = 4;
    jucator::nrTot = 2;


}

void generezObiectiv(poz &obiectiv,jucator juc[]){//genereaza locatia mancarii
    obiectiv.x = -1;
    obiectiv.y = -1;
    int ok=0;
    do{
    obiectiv.x = rand() % WIDTH+1;
    obiectiv.y = rand() % HEIGHT+1;
    for(int i = 1;i<=jucator::nrTot;i++)
        if(juc[i].loc.x == obiectiv.x && juc[i].loc.y == obiectiv.y) //verifica daca locatia random este egala cu cea a unui nod din coada pentru a reseta pozitia
            ok = 1;
    }while(ok == 1);
}

void controller(int &miscare){//functia afla sensul in care sarpele se va deplasa
    if(kbhit()){
        char ch = getch();
        if(ch == 'w' || ch == 'W')
            miscare = 4;
        if(ch == 'a' || ch == 'A')
            miscare = 1;
        if(ch == 'd' || ch == 'D')
            miscare = 3;
        if(ch == 's' || ch == 'S')
            miscare = 2;
    }


}

void miscCorp(int miscare,jucator *coada){//functia muta capul sarpelui
    poz curent;
    if(miscare == 1){
        curent.x = -1;
        curent.y = 0;
    }
    if(miscare == 2){
        curent.x = 0;
        curent.y = 1;
    }

    if(miscare == 3){
        curent.x = 1;
        curent.y = 0;
    }

    if(miscare == 4){
        curent.x = 0;
        curent.y = -1;
    }

    if(miscare == 0){
        curent.x = 0;
        curent.y = 0;
    }


    jucator prev=*(coada+1);
    jucator prev2;
    for(int i = 2;i<=jucator::nrTot;i++){//nodul i se duce pe vechea pozitie a nodului i-1
        prev2 = *(coada+i);
        *(coada+i) = prev;
        prev =prev2;

    }
    (coada+1)->loc.x += curent.x;
    (coada+1)->loc.y += curent.y;//se muta capul


}

void punEntitatilePeHarta(jucator *coada, poz obiectiv,char harta[HEIGHT+2][WIDTH+3]){//pune in matricea "harta" sarpele precum si mancarea
    char cap = '8';
    char corp = '0';
    char prize = 'M';



    harta[obiectiv.y][obiectiv.x] = prize;

    harta[(coada+1)->loc.y][(coada+1)->loc.x] = cap;


    for(int i = 2;i <= jucator::nrTot;i++)
        harta[(coada+i)->loc.y][(coada+i)->loc.x] = corp;


}

void hit(jucator *coada, poz &obiectiv,bool &running,int& scor){//verifica coliziunea sarpelui cu a celorlalte entitati
    if((coada+1)->loc.x == 0 || (coada+1)->loc.x == WIDTH+1 || (coada+1)->loc.y == 0 || (coada+1)->loc.y == HEIGHT+1)//cauta daca pozitia curenta a capului se lovieste de ziduri
        running = 0;//jocul ia sfarsit



    if((coada+1)->loc.x == obiectiv.x && (coada+1)->loc.y == obiectiv.y){//daca pozitia curenta a capului este = cu pozitia mancarii
        jucator::nrTot++;


        generezObiectiv(obiectiv,coada);
        scor+=10;
    }
    for(int i = 2;i<=jucator::nrTot;i++)//daca pozitia curenta a capului se loveste de vreun nod
        if((coada+1)->loc.x == (coada+i)->loc.x && (coada+1)->loc.y == (coada+i)->loc.y)
            running = 0;
}

void desfasurareJoc(jucator *coada, poz obiectiv,char harta[HEIGHT+2][WIDTH+3]){
    bool running = 1;

    int miscare = 2; //valoarea 0 inseamna ca sta pe loc,1 ca se va duce la stanga, 2 ca se va duce in jos, 3 se va duce la dreapta, 4 se va duce in sus;
    while(running){

        controller(miscare);//ia starea curenta a  controller-ului
        punEntitatilePeHarta(coada,obiectiv,harta);//pune entitatile pe harta

        miscCorp(miscare,coada);//misca pozitia sarpelui
        hit(coada,obiectiv,running,scor);//cauta daca exista coliziuni


        system("CLS");//sterge continutul consolei
        show(harta);//afiseaza harta
        flush(harta);//sterge continutul hartii
        cout<<"SCORUL ESTE "<<scor<<endl;

        //Sleep(100);

    }


    system("CLS");
}

void ecranSfarsit(){
    cout<<"GAME OVER";
    cout<<endl<<"SCORUL ESTE DE: "<<scor;
    Sleep(1000);
    cout<<endl<<"Apasa-ti orice buton pentru a inchide fereastra";

    while(1){
        if(kbhit())
            break;
    }


}

void context(){

    jucator coada[1000];
    poz obiectiv;

    char harta[HEIGHT+2][WIDTH+3]; //variabila retine harta pe care sarpele se misca iar dimensiunile sunt identate cu 2 deoarece pe margini sunt adaugati pereti. Din motive de eficienta coloanele sunt identate cu 3 pt ca la afisare sa scrie un sir de caractere direct
    init(harta); //afisez ecranul de inceput
    system("CLS"); //sterg ecranul de inceput
    generezJucatorul(coada); //pun jucatorul in cord stanga sus

    generezObiectiv(obiectiv,coada);//generez bucata de mancare

    desfasurareJoc(coada,obiectiv,harta);//desfasoara jocul

    ecranSfarsit();

}

int main()
{
    context();
}
