#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <iomanip>
using namespace std;

#define PORT 2729

struct jucatori{// informatii despre jucatori
	char nume[100];
	int punctaj;
	int cul; //0 - galben | 1 - rosu | 2 - verde | 3 - albastru
    int tabel=-1;
    int terminat=0;
}j[400];

struct finish{//pozitiile finale ale pionilor
    int x, y;
};
struct pozitie{// pozitiile pieselor(initiale,de start si curente)
    int xi, yi, xs, ys, x, y, poz,finish;
};
struct piesa{//informatiile despre piese
    char culoare;
    int iesit;
    pozitie piesa[4];
    finish pozfinal[4];
    int pioni_pe_tabla;
};
struct tabla//informatii despre tabla de joc
{
    char table[15][30];
    piesa player[4];
    char line_table[450];
    char table_copy[15][30];
    int culori[4] = {0, 0, 0, 0};
    int Clasament[4] = {0, 0, 0, 0};
    int k=0,k_2=0,okk=0,ok_2=0;
    int parcurgere_i[50];
    int parcurgere_j[50];
    int podium;
    int juc_term=0;
    int flag=1;
    int start=0;
    int juc_con=0;
    int juc_decon=0;
    int pause=0;
}t[100];

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;

pthread_t threads[4];
int Index=0;
int maxx;
int tb=0;
sqlite3 *db;
char *errmsg = 0;
char *tabel;
char comanda[500];
int bdd = sqlite3_open("players.db", &db);
void Initializare_piese()
{
    int x=tb-1;
    fflush(stdout);
    t[x].podium=0;
    t[x].player[0].culoare='G';
    t[x].player[1].culoare='R';
    t[x].player[2].culoare='V';
    t[x].player[3].culoare='A';
    t[x].player[0].iesit=0;
    t[x].player[1].iesit=0;
    t[x].player[2].iesit=0;
    t[x].player[3].iesit=0;
    t[x].player[0].pioni_pe_tabla=0;
    t[x].player[1].pioni_pe_tabla=0;
    t[x].player[2].pioni_pe_tabla=0;
    t[x].player[3].pioni_pe_tabla=0;
    t[x].player[0].piesa[0].finish=0;
    t[x].player[0].piesa[1].finish=0;
    t[x].player[0].piesa[2].finish=0;
    t[x].player[0].piesa[3].finish=0;
    t[x].player[1].piesa[0].finish=0;
    t[x].player[1].piesa[1].finish=0;
    t[x].player[1].piesa[2].finish=0;
    t[x].player[1].piesa[3].finish=0;
    t[x].player[2].piesa[0].finish=0;
    t[x].player[2].piesa[1].finish=0;
    t[x].player[2].piesa[2].finish=0;
    t[x].player[2].piesa[3].finish=0;
    t[x].player[3].piesa[0].finish=0;
    t[x].player[3].piesa[1].finish=0;
    t[x].player[3].piesa[2].finish=0;
    t[x].player[3].piesa[3].finish=0;
    //Pentru culoarea galbena
    t[x].player[0].piesa[0].xi=3;
    t[x].player[0].piesa[0].yi=22;
    t[x].player[0].piesa[0].xs=1;
    t[x].player[0].piesa[0].ys=18;
    t[x].player[0].piesa[0].x=3;
    t[x].player[0].piesa[0].y=22;
    t[x].player[0].piesa[0].poz=-1;
    t[x].player[0].piesa[1].xi=3;
    t[x].player[0].piesa[1].yi=24;
    t[x].player[0].piesa[1].xs=1;
    t[x].player[0].piesa[1].ys=18;
    t[x].player[0].piesa[1].x=3;
    t[x].player[0].piesa[1].y=24;
    t[x].player[0].piesa[1].poz=-1;
    t[x].player[0].piesa[2].xi=4;
    t[x].player[0].piesa[2].yi=22;
    t[x].player[0].piesa[2].xs=1;
    t[x].player[0].piesa[2].ys=18;
    t[x].player[0].piesa[2].x=4;
    t[x].player[0].piesa[2].y=22;
    t[x].player[0].piesa[2].poz=-1;
    t[x].player[0].piesa[3].xi=4;
    t[x].player[0].piesa[3].yi=24;
    t[x].player[0].piesa[3].xs=1;
    t[x].player[0].piesa[3].ys=18;
    t[x].player[0].piesa[3].x=4;
    t[x].player[0].piesa[3].y=24;
    t[x].player[0].piesa[3].poz=-1;
    t[x].player[0].pozfinal[0].x=2;
    t[x].player[0].pozfinal[0].y=14;
    t[x].player[0].pozfinal[1].x=3;
    t[x].player[0].pozfinal[1].y=14;
    t[x].player[0].pozfinal[2].x=4;
    t[x].player[0].pozfinal[2].y=14;
    t[x].player[0].pozfinal[3].x=5;
    t[x].player[0].pozfinal[3].y=14;


    //Pentru culoarea rosu
    t[x].player[1].piesa[0].xi=11;
    t[x].player[1].piesa[0].yi=22;
    t[x].player[1].piesa[0].xs=9;
    t[x].player[1].piesa[0].ys=26;
    t[x].player[1].piesa[0].x=11;
    t[x].player[1].piesa[0].y=22;
    t[x].player[1].piesa[0].poz=-1;
    t[x].player[1].piesa[1].xi=11;
    t[x].player[1].piesa[1].yi=24;
    t[x].player[1].piesa[1].xs=9;
    t[x].player[1].piesa[1].ys=26;
    t[x].player[1].piesa[1].x=11;
    t[x].player[1].piesa[1].y=24;
    t[x].player[1].piesa[1].poz=-1;
    t[x].player[1].piesa[2].xi=12;
    t[x].player[1].piesa[2].yi=22;
    t[x].player[1].piesa[2].xs=9;
    t[x].player[1].piesa[2].ys=26;
    t[x].player[1].piesa[2].x=12;
    t[x].player[1].piesa[2].y=22;
    t[x].player[1].piesa[2].poz=-1;
    t[x].player[1].piesa[3].xi=12;
    t[x].player[1].piesa[3].yi=24;
    t[x].player[1].piesa[3].xs=9;
    t[x].player[1].piesa[3].ys=26;
    t[x].player[1].piesa[3].x=12;
    t[x].player[1].piesa[3].y=24;
    t[x].player[1].piesa[3].poz=-1;
    t[x].player[1].pozfinal[0].x=7;
    t[x].player[1].pozfinal[0].y=24;
    t[x].player[1].pozfinal[1].x=7;
    t[x].player[1].pozfinal[1].y=22;
    t[x].player[1].pozfinal[2].x=7;
    t[x].player[1].pozfinal[2].y=20;
    t[x].player[1].pozfinal[3].x=7;
    t[x].player[1].pozfinal[3].y=18;


    //Pentru culoarea verde
    t[x].player[2].piesa[0].xi=11;
    t[x].player[2].piesa[0].yi=4;
    t[x].player[2].piesa[0].xs=13;
    t[x].player[2].piesa[0].ys=10;
    t[x].player[2].piesa[0].x=11;
    t[x].player[2].piesa[0].y=4;
    t[x].player[2].piesa[0].poz=-1;
    t[x].player[2].piesa[1].xi=11;
    t[x].player[2].piesa[1].yi=6;
    t[x].player[2].piesa[1].xs=13;
    t[x].player[2].piesa[1].ys=10;
    t[x].player[2].piesa[1].x=11;
    t[x].player[2].piesa[1].y=6;
    t[x].player[2].piesa[1].poz=-1;
    t[x].player[2].piesa[2].xi=12;
    t[x].player[2].piesa[2].yi=4;
    t[x].player[2].piesa[2].xs=13;
    t[x].player[2].piesa[2].ys=10;
    t[x].player[2].piesa[2].x=12;
    t[x].player[2].piesa[2].y=4;
    t[x].player[2].piesa[2].poz=-1;
    t[x].player[2].piesa[3].xi=12;
    t[x].player[2].piesa[3].yi=6;
    t[x].player[2].piesa[3].xs=13;
    t[x].player[2].piesa[3].ys=10;
    t[x].player[2].piesa[3].x=12;
    t[x].player[2].piesa[3].y=6;
    t[x].player[2].piesa[3].poz=-1;
    t[x].player[2].pozfinal[0].x=12;
    t[x].player[2].pozfinal[0].y=14;
    t[x].player[2].pozfinal[1].x=11;
    t[x].player[2].pozfinal[1].y=14;
    t[x].player[2].pozfinal[2].x=10;
    t[x].player[2].pozfinal[2].y=14;
    t[x].player[2].pozfinal[3].x=9;
    t[x].player[2].pozfinal[3].y=14;
   
    //Pentru culoarea albastru
    t[x].player[3].piesa[0].xi=3;
    t[x].player[3].piesa[0].yi=4;
    t[x].player[3].piesa[0].xs=5;
    t[x].player[3].piesa[0].ys=2;
    t[x].player[3].piesa[0].x=3;
    t[x].player[3].piesa[0].y=4;
    t[x].player[3].piesa[0].poz=-1;
    t[x].player[3].piesa[1].xi=3;
    t[x].player[3].piesa[1].yi=6;
    t[x].player[3].piesa[1].xs=5;
    t[x].player[3].piesa[1].ys=2;
    t[x].player[3].piesa[1].x=3;
    t[x].player[3].piesa[1].y=6;
    t[x].player[3].piesa[1].poz=-1;
    t[x].player[3].piesa[2].xi=4;
    t[x].player[3].piesa[2].yi=4;
    t[x].player[3].piesa[2].xs=5;
    t[x].player[3].piesa[2].ys=2;
    t[x].player[3].piesa[2].x=4;
    t[x].player[3].piesa[2].y=4;
    t[x].player[3].piesa[2].poz=-1;
    t[x].player[3].piesa[3].xi=4;
    t[x].player[3].piesa[3].yi=6;
    t[x].player[3].piesa[3].xs=5;
    t[x].player[3].piesa[3].ys=2;
    t[x].player[3].piesa[3].x=4;
    t[x].player[3].piesa[3].y=6;
    t[x].player[3].piesa[3].poz=-1;
    t[x].player[3].pozfinal[0].x=7;
    t[x].player[3].pozfinal[0].y=4;
    t[x].player[3].pozfinal[1].x=7;
    t[x].player[3].pozfinal[1].y=6;
    t[x].player[3].pozfinal[2].x=7;
    t[x].player[3].pozfinal[2].y=8;
    t[x].player[3].pozfinal[3].x=7;
    t[x].player[3].pozfinal[3].y=10;
}
int mi[100], mj[100];
int poz = 1;
int parcurgere_Gi[50] = {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, -2, -2, -2, -2, 0, 0, 2, 2, 2, 2};
int parcurgere_Gj[50] = {0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, -2, -2 , -2, -2, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0};
int parcurgere_Ri[50] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0};
int parcurgere_Rj[50] = {-2, -2, -2, -2, 0, 0, 0, 0, -3, -3, -3, -3, 0, 0, 0, 0, -3, -3, -3, -3, 0, 0, 0, 0, 3, 3 , 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, -3, -3, -3, -3};
int parcurgere_Vi[50] = {-1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, -2, -2, -2, -2};
int parcurgere_Vj[50] = {0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 1, 1 , 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, -3, -3, -3, -3, 0, 0, 0, 0, -3, -3, 0, 0, 0, 0};
int parcurgere_Ai[50] = {0, 0, 0, 0, -2, -2, -2, -2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, -3, -3, -3, -3, 0, 0, 0, 0, -3, -3, 0, 0, 0, 0};
int parcurgere_Aj[50] = {2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, -3, -3, -3, -3, 0, 0, 0, 0, -3, -3, -3, -3, 0, 0, 0, 0, -3, -3, -3, -3, 0, 0, 3, 3, 3, 3};

static int callback(void *beUsed, int argc, char **argv, char **ColmName) 
{
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", ColmName[i], argv[i] ? argv[i] : "NULL");
        fflush(stdout);
   }
   printf("\n");
   return 1;
}

void FromMatToStr(int x){
	int i, j, cnt = 0;
    
    bzero(t[x].line_table, 450);
	for(i = 0; i <= 14; i++){
        for(j = 0; j <= 29; j++)
			t[x].line_table[cnt++] = t[x].table[i][j];
		}

	t[x].line_table[cnt] = '\0';
}

void CopyTable(char table_copy[15][30],char table[15][30]){
    for(int i = 0; i <= 14; i++)
        for(int j = 0; j <= 29; j++)
            table_copy[i][j] = table[i][j];
}

void CreateTable(int x){

    for(int i = 0; i <= 14; i++)
        for(int j = 0; j <= 29; j++)
            t[x].table[i][j] = ' ';
    t[x].table[3][4] = t[x].table[3][6] = t[x].table[4][4] = t[x].table[4][6] = 'A';
    t[x].table[11][4] = t[x].table[11][6] = t[x].table[12][4] = t[x].table[12][6] = 'V';
    t[x].table[3][22] = t[x].table[3][24] = t[x].table[4][22] = t[x].table[4][24] = 'G';
    t[x].table[11][22] = t[x].table[11][24] = t[x].table[12][22] = t[x].table[12][24] = 'R';
    for(int i = 1; i <= 14; i++)
    {
        t[x].table[i][0] = '|';
        t[x].table[i][29] = '|';
    }

    for(int i = 1; i <= 28; i++)
    {
        t[x].table[0][i] = '_';
        t[x].table[14][i] = '_';
    }

     for(int i = 10;  i <= 18; i+=2)
    {
        t[x].table[1][i] = '_';
        t[x].table[13][i] = '_';
    }
    
    for(int i = 5;  i <= 9; i++)
    {
        t[x].table[i][2] = '_';
        t[x].table[i][26] = '_';
    }

    for(int i = 4; i <= 10; i += 2)
    {
        t[x].table[5][i] =  '_';
        t[x].table[5][i + 14] = '_';
        t[x].table[9][i] = '_';
        t[x].table[9][i + 14] = '_';
    }

    for(int i = 1; i <= 3; i++)
    {
        t[x].table[i + 1][10] = '_'; 
        t[x].table[i + 9][10] = '_';
    }

    for(int i = 1; i <= 3; i++)
    {
        t[x].table[i + 1][18] = '_';
        t[x].table[i + 9][18] = '_';
    }

    for(int i = 1; i <= 4; i++){
        t[x].table[i + 1][14] = 'g';
        t[x].table[i + 8][14] = 'v';
    }

    for(int i = 4; i <= 10; i+=2)
    {
            t[x].table[7][i] = 'a';
            t[x].table[7][i + 14] = 'r';
    }
    t[x].table[1][18] = t[x].table[5][2] = t[x].table[9][26] = t[x].table[13][10] = 's';
}
void Afisare_tabla(int x){
    for(int i = 0; i <= 14; i++){
        for(int j = 0; j <= 29; j++)
            cout <<t[x].table[i][j];
        cout << endl;
        fflush(stdout);
    }
    cout << endl;
}




void putonline(char nume[20], sqlite3 *db,int fd)//pune player online
{
    char comanda[1024];
	int bdd;
	char *errmsg = 0;
    char msg[100];
    struct sqlite3_stmt *selectstmt;
    const char* data = "Callback function called";
    fflush(stdout);
    bzero(comanda,1024);
    strcpy(comanda , "UPDATE JUCATORI SET ONLINE=1 WHERE USERNAME='user';");
    sprintf(comanda,  "UPDATE JUCATORI SET ONLINE=1 WHERE USERNAME='%s';",nume);
    bdd=sqlite3_exec(db, comanda, callback, (void*)data, &errmsg);
    if (bdd == SQLITE_OK)
        printf("Operation done successfully\n");
    sqlite3_close(db);
}
void putoffline(char nume[20], sqlite3 *db,int fd)// pune player offline
{
    char comanda[1024];
	int bdd;
	char *errmsg = 0;
    char msg[100];
    struct sqlite3_stmt *selectstmt;
    const char* data = "Callback function called";
    fflush(stdout);
    bzero(comanda,1024);
    strcpy(comanda , "UPDATE JUCATORI SET ONLINE=0 WHERE USERNAME='user';");
    sprintf(comanda,  "UPDATE JUCATORI SET ONLINE=0 WHERE USERNAME='%s';",nume);
    bdd=sqlite3_exec(db, comanda, callback, (void*)data, &errmsg);
    //sqlite3_prepare_v2(db, comanda, -1, &selectstmt, NULL);
    if (bdd == SQLITE_OK)
        printf("Operation done successfully\n");
    sqlite3_close(db);
}

int verifonline(char nume[20], sqlite3 *db,int fd)//verifica daca playerul este online, pentru a nu fi 2 jucatori online in acelasi timp
{
    sqlite3_stmt* stmt;
    char* errMesg = 0;
    char sql_stmt[200];
    strcpy(sql_stmt,"SELECT ONLINE from JUCATORI WHERE USERNAME='user';");
    sprintf(sql_stmt,"SELECT ONLINE from JUCATORI WHERE USERNAME='%s';",nume);
    int bdd;
    bdd = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, 0);

    if (bdd != SQLITE_OK) {
        printf("\nUnable to fetch data");
        sqlite3_close(db);
    }
    char result[200];
    bzero(result,200);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        strcat(result,(char*)sqlite3_column_text(stmt, 0));
        strcat(result,"\0");
    }
    if(result[0]=='1')
        return 1;
    else
        return 0;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void updateparola(char nume[20],char parola[100], sqlite3 *db,int fd)//in cazul in care un jucator isi reseteaza parola, atunci cand isi introduce o parola noua, acesta functie
//va face update in baza de date
{
    char comanda[1024];
	int bdd;
	char *errmsg = 0;
    char msg[100];
    struct sqlite3_stmt *selectstmt;
    const char* data = "Callback function called";
    fflush(stdout);
    bzero(comanda,1024);
    strcpy(comanda , "UPDATE JUCATORI SET PAROLA='parola' WHERE USERNAME='user';");
    sprintf(comanda,  "UPDATE JUCATORI SET PAROLA='%s' WHERE USERNAME='%s';",parola,nume);
    bdd=sqlite3_exec(db, comanda, callback, (void*)data, &errmsg);
    if (bdd == SQLITE_OK)
        printf("Operation done successfully\n");
    sqlite3_close(db);
}

void Insertnume(char nume[20], sqlite3 *db,int fd,int i)//conecteaza/inregistreaza jucatorul
{
	char comanda[1024];
	int bdd;
	char *errmsg = 0;
    char msg[100];
    struct sqlite3_stmt *selectstmt;
    strcpy(j[i].nume,nume);
    bzero(comanda,1024);
    strcpy(comanda , "SELECT * from JUCATORI WHERE USERNAME='user';");
    sprintf(comanda, "SELECT * from JUCATORI WHERE USERNAME='%s';", nume);
    sqlite3_prepare_v2(db, comanda, -1, &selectstmt, NULL);
        if (sqlite3_step(selectstmt) != SQLITE_ROW)
        {
            bzero(msg, 100);
	        strcpy(msg, "Am observat ca este prima conectare cu acest nume. Pentru a va inregistra o introduceti o parola:");
            if (write (fd, msg, 100) < 0){
			    perror ("[Server]Eroare la write() spre client.\n");
		    }
            bzero(msg, 100);
            if (read (fd, msg, 100) < 0){
               perror ("[Server]Eroare la read() spre client.\n");
            }
            char msg2[100];
            bzero(msg2, 100);
	        strcpy(msg2, "Introduceti cod de 4 cifre/litere, pe care puteti sa il folositi in cazul in care v-ati uitat parola");
            if (write (fd, msg2, 100) < 0){
			    perror ("[Server]Eroare la write() spre client.\n");
		    }
            bzero(msg2, 100);
            if (read (fd, msg2, 100) < 0){
               perror ("[Server]Eroare la read() spre client.\n");
            }
            bzero(comanda,1024);
            strcpy(comanda, "INSERT INTO JUCATORI(USERNAME,PAROLA,PUNCTE,P1_X,P1_Y,P2_X,P2_Y,P3_X,P3_Y,P4_X,P4_Y,CASTIG,MECIURI_JUCATE,COD,ONLINE) VALUES('user','parola',0,0,0,0,0,0,0,0,0,0,0,'cod',1);");
	        sprintf(comanda, "INSERT INTO JUCATORI(USERNAME,PAROLA,PUNCTE,P1_X,P1_Y,P2_X,P2_Y,P3_X,P3_Y,P4_X,P4_Y,CASTIG,MECIURI_JUCATE,COD,ONLINE) VALUES('%s','%s',0,0,0,0,0,0,0,0,0,0,0,'%s',1);", nume,msg,msg2);
            bdd = sqlite3_prepare_v2(db, comanda, -1, &selectstmt, NULL);
            if (sqlite3_step(selectstmt) == SQLITE_ROW)
            {
                bzero(msg, 100);
	            strcpy(msg, "Felicitari v-ati conectat cu succes!");
                if (write (fd, msg, 100) < 0){
			        perror ("[Server]Eroare la write() spre client.\n");
		        }
            }
            else
                {
                    bzero(msg, 100);
	                strcpy(msg, "Felicitari v-ati conectat cu succes!");
                    if (write (fd, msg, 100) < 0){
			            perror ("[Server]Eroare la write() spre client.\n");
		            }
                }
        } 
        else 
        {
            if(verifonline(nume,db,fd)==1)
            {
                bzero(msg, 100);
                strcpy(msg, "Acest user este deja conectat intr-o alta sesiune de joc. Incercati un alt username.");
                if (write (fd, msg, 100) < 0){
                    perror ("[Server]Eroare la write() spre client.\n");
                }
                bzero(nume, 100);
                if (read (fd,nume, 100) < 0){
                    perror ("[Server]Eroare la read() spre client.\n");
                }
                Insertnume(nume,db,fd,i);
            }
            else
                {
                    bzero(msg, 100);
                    strcpy(msg, "Te rugam sa iti introduci parola:");
                    if (write (fd, msg, 100) < 0){
                        perror ("[Server]Eroare la write() spre client.\n");
                    }
                    bzero(msg, 100);
                    if (read (fd, msg, 100) < 0){
                        perror ("[Server]Eroare la read() spre client.\n");
                    }
                    bzero(comanda,1024);
                    strcpy(comanda , "SELECT * from JUCATORI WHERE PAROLA='parola' AND USERNAME='user';");
                    sprintf(comanda, "SELECT * from JUCATORI WHERE PAROLA='%s' AND USERNAME='%s';", msg,nume);

                    bdd = sqlite3_prepare_v2(db, comanda, -1, &selectstmt, NULL);
                    fflush(stdout);
                    if( bdd == SQLITE_OK)
                    {
                        if (sqlite3_step(selectstmt) != SQLITE_ROW)
                            while(sqlite3_step(selectstmt) != SQLITE_ROW)
                            {
                                bzero(msg, 100);
                                strcpy(msg, "Parola gresita! Introduceti parola corecta. Pentru resetarea parolei scrieti /resetpassword");
                                if (write (fd, msg, 100) < 0){
                                    perror ("[Server]Eroare la write() spre client.\n");
                                }
                                bzero(msg, 100);
                                if (read (fd, msg, 100) < 0){
                                    perror ("[Server]Eroare la read() spre client.\n");
                                }
                                if(strstr(msg,"/resetpassword"))
                                {
                                    bzero(msg, 100);
                                    strcpy(msg, "Introduceti codul de resetare a parolei, introdus la prima inregistrare:");
                                    if (write (fd, msg, 100) < 0){
                                        perror ("[Server]Eroare la write() spre client.\n");
                                    }
                                    bzero(msg, 100);//aici se ia codul
                                    if (read (fd, msg, 100) < 0){
                                        perror ("[Server]Eroare la read() spre client.\n");
                                    }
                                    bzero(comanda,1024);
                                    strcpy(comanda , "SELECT * from JUCATORI WHERE COD='cod' AND USERNAME='user';");
                                    sprintf(comanda, "SELECT * from JUCATORI WHERE COD='%s' AND USERNAME='%s';", msg,nume);
                                    bdd = sqlite3_prepare_v2(db, comanda, -1, &selectstmt, NULL);
                                    if( bdd == SQLITE_OK)
                                    {
                                        while(sqlite3_step(selectstmt) != SQLITE_ROW)
                                        {
                                            bzero(msg, 100);
                                            strcpy(msg, "Cod gresit, incercati dinou!");
                                            if (write (fd, msg, 100) < 0){
                                                perror ("[Server]Eroare la write() spre client.\n");
                                            }
                                            bzero(msg, 100);//aici se ia codul
                                            if (read (fd, msg, 100) < 0){
                                                perror ("[Server]Eroare la read() spre client.\n");
                                            }
                                            bzero(comanda,1024);
                                            strcpy(comanda , "SELECT * from JUCATORI WHERE COD='cod' AND USERNAME='user';");
                                            sprintf(comanda, "SELECT * from JUCATORI WHERE COD='%s' AND USERNAME='%s';", msg,nume);
                                            bdd = sqlite3_prepare_v2(db, comanda, -1, &selectstmt, NULL);
                                        }

                                    }
                                    bzero(msg,100);
                                    strcpy(msg, "Introduceti parola noua:");
                                    if (write (fd, msg, 100) < 0){
                                        perror ("[Server]Eroare la write() spre client.\n");
                                            //return errno;
                                    }
                                    bzero(msg, 100);
                                    if (read (fd, msg, 100) < 0){
                                        perror ("[Server]Eroare la read() spre client.\n");
                                            //return errno;
                                    }
                                    updateparola(nume,msg,db,fd);

                                    bzero(msg,100);
                                    strcpy(msg,"Te rugam sa iti introduci parola:");
                                    if (write (fd, msg, 100) < 0){
                                        perror ("[Server]Eroare la write() spre client.\n");
                                            //return errno;
                                    }

                                    bzero(msg, 100);
                                    if (read (fd, msg, 100) < 0){
                                        perror ("[Server]Eroare la read() spre client.\n");
                                            //return errno;
                                    }

                                }
                                bzero(comanda,1024);
                                strcpy(comanda , "SELECT * from JUCATORI WHERE PAROLA='parola' AND USERNAME='user';");
                                sprintf(comanda, "SELECT * from JUCATORI WHERE PAROLA='%s' AND USERNAME='%s';", msg,nume);

                                bdd = sqlite3_prepare_v2(db, comanda, -1, &selectstmt, NULL);
                            }
                            bzero(msg, 100);
                            strcpy(msg, "Felicitari v-ati conectat cu succes!");
                            if (write (fd, msg, 100) < 0){
                                perror ("[Server]Eroare la write() spre client.\n");
                                    //return errno;
                            }   
                            putonline(nume,db,fd);
                            fflush(stdout);
                        }
                        else
                            {
                                bzero(msg, 100);
                                strcpy(msg, "Felicitari v-ati conectat cu succes!");
                                if (write (fd, msg, 100) < 0){
                                    perror ("[Server]Eroare la write() spre client.\n");
                                        //return errno;
                                }
                                putonline(nume,db,fd);
                            }
                    }

            }
        sqlite3_finalize(selectstmt);
    }


void Verificare_mancat(int X,int Y,int cul,int x)//verifica daca un pion este mancat de altul
{
    int ok=1;
    if(strchr("GRVA",t[x].table[X][Y]))
        for(int i=0;i<4 && ok==1;i++)
            if(t[x].player[i].culoare==t[x].table[X][Y] && i!=cul)
              {
                for(int j=0;j<4;j++)
                    if(t[x].player[i].piesa[j].x ==  X  && t[x].player[i].piesa[j].y == Y)
                    {
                        t[x].player[i].piesa[j].poz=-1;
                        t[x].player[i].piesa[j].x= t[x].player[i].piesa[j].xi;
                        t[x].player[i].piesa[j].y=t[x].player[i].piesa[j].yi;
                        fflush(stdout);
                        t[x].table[t[x].player[i].piesa[j].x][t[x].player[i].piesa[j].y]=t[x].player[i].culoare;
                        int k=0;
                        for(int p=0;p<4;p++)
                            if(t[x].player[i].piesa[p].poz<0)
                                k++;
                        //cout<<k<<"----------"<<endl;
                        if(k==4)
                            t[x].player[i].iesit=0;
                        ok=0;
                    }
               }
}

void updatepozitii(piesa p, sqlite3 *db,int fd,char nume[20])//introducerea pozitiilor pieselor in baza de date
{
    char comanda[1024];
	int bdd;
	char *errmsg = 0;
    char msg[100];
    struct sqlite3_stmt *selectstmt;
    const char* data = "Callback function called";
    fflush(stdout);
    bzero(comanda,1024);
    strcpy(comanda , "UPDATE JUCATORI SET P1_X=p1_x,P1_Y=p1_y,P2_X=p2_x,P2_Y=p2_y,P3_X=p3_x,P3_Y=p3_y,P4_X=p4_x,P4_Y=p4_y WHERE USERNAME='user';");
    sprintf(comanda, "UPDATE JUCATORI SET P1_X=%d,P1_Y=%d,P2_X=%d,P2_Y=%d,P3_X=%d,P3_Y=%d,P4_X=%d,P4_Y=%d WHERE USERNAME='%s';", p.piesa[0].x,p.piesa[0].y,p.piesa[1].x,p.piesa[1].y,p.piesa[2].x,p.piesa[2].y,p.piesa[3].x,p.piesa[3].y,nume);
    bdd=sqlite3_exec(db, comanda, callback, (void*)data, &errmsg);
        if (bdd == SQLITE_OK)
        printf("Operation done successfully\n");
    sqlite3_close(db);
}

void restabilire_pozitii(piesa p, sqlite3 *db,int fd,char nume[20])//in cazul in care se pune pauza acesta functie este apelata pentru a restabili pozitiile pieselor
//luand informatiile din baza de date
{
    sqlite3_stmt* stmt;
    char* errMesg = 0;
    char sql_stmt[200];
    strcpy(sql_stmt,"SELECT P1_X,P1_Y,P2_X,P2_Y,P3_X,P3_Y,P4_X,P4_Y from JUCATORI WHERE USERNAME='user';");
    sprintf(sql_stmt,"SELECT P1_X,P1_Y,P2_X,P2_Y,P3_X,P3_Y,P4_X,P4_Y from JUCATORI WHERE USERNAME='%s';",nume);
    int bdd;
    bdd = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, 0);

    if (bdd != SQLITE_OK) {
        printf("\nUnable to fetch data");
        sqlite3_close(db);
        //return 1;
    }
    char result1[100];
    bzero(result1,100);
    char result2[100];
    bzero(result2,100);
    int i=0;
    int j=0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        while (j<4) {
            bzero(result1,100);
            bzero(result2,100);
            strcat(result1,(char*)sqlite3_column_text(stmt, 0));
            strcat(result1,"\0");
            strcat(result2,(char*)sqlite3_column_text(stmt, i+1));
            strcat(result2,"\0");
            p.piesa[j].x=result1[0]-'0';
            p.piesa[j].y=result2[0]-'0';
            i+=2;
            j++;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void updateclasament(int poz,int nr_juc, sqlite3 *db,int fd,char nume[20])//dupa ce un jucator a terminat, i se va updata nr de puncte/meciuri jucate/meciuri castigate
{
    char comanda[1024];
	int bdd;
	char *errmsg = 0;
    char msg[100];
    struct sqlite3_stmt *selectstmt;
    const char* data = "Callback function called";
    fflush(stdout);
    bzero(comanda,1024);
    int puncte=0;
    switch (poz)
    {
    case 1:
        puncte=50*nr_juc;//locul 1
        break;
    case 2:
        puncte=35*nr_juc;//locul 2
        break;
    case 3:
        puncte=20*nr_juc;//locul 3
        break;
    case 4:
        puncte=10*nr_juc;//locul 4
        break;
    
    default:
        break;
    }
    if(poz==1)
    {
        strcpy(comanda , "UPDATE JUCATORI SET PUNCTE=PUNCTE+pct,CASTIG=CASTIG+1,MECIURI_JUCATE=MECIURI_JUCATE+1 WHERE USERNAME='user';");
        sprintf(comanda,  "UPDATE JUCATORI SET PUNCTE=PUNCTE+%d,CASTIG=CASTIG+1,MECIURI_JUCATE=MECIURI_JUCATE+1 WHERE USERNAME='%s';", puncte,nume);
    }
    else
        {
            strcpy(comanda , "UPDATE JUCATORI SET PUNCTE=PUNCTE+pct,MECIURI_JUCATE=MECIURI_JUCATE+1 WHERE USERNAME='user';");
            sprintf(comanda,  "UPDATE JUCATORI SET PUNCTE=PUNCTE+%d,MECIURI_JUCATE=MECIURI_JUCATE+1 WHERE USERNAME='%s';", puncte,nume);
        }
    bdd=sqlite3_exec(db, comanda, callback, (void*)data, &errmsg);
        if (bdd == SQLITE_OK)
        printf("Operation done successfully\n");
    sqlite3_close(db);
}

int Verif_finish(int X,int Y,int cul,int pion,int x)//verifica daca pionul curent a ajuns la finish
{
    if(strchr("grva",t[x].table[X][Y]))
            if(t[x].player[cul].pozfinal[pion].x==X && t[x].player[cul].pozfinal[pion].y==Y)
                return 1;
    return 0;
}
void verificare_win(int x,int id)//verifica daca playerul a ajuns cu toti pionii la finish
{
    int c=j[id].cul;
    int piese_finish=0;
    if(t[x].Clasament[id%4]==0)
        for(int i=0;i<4;i++)
            {
                if(t[x].player[c].piesa[i].x==t[x].player[c].pozfinal[i].x && t[x].player[c].piesa[i].y==t[x].player[c].pozfinal[i].y)
                piese_finish++;//a ajuns in pozitie finala
            }
    if(piese_finish==4)
    {
        t[x].podium++;
        t[x].Clasament[id%4]=t[x].podium;//ii acorda pozitia pe podium
    }
}
void top10(sqlite3 *db,int fd)//afiseaza top 10 jucatori dupa nr de puncte
{
    sqlite3_stmt* stmt;
    char* errMesg = 0;

    char* sql_stmt = "SELECT * from(select username,puncte FROM jucatori order by puncte desc) limit 10;";
    int bdd;
    bdd = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, 0);

    if (bdd != SQLITE_OK) {
        printf("\nUnable to fetch data");
        sqlite3_close(db);
        //return 1;
    }
    char result[300];
    bzero(result,300);
    strcpy(result,"");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        strcat(result,"Username-");
        strcat(result,(char*)sqlite3_column_text(stmt, 0));
        strcat(result," ");
        strcat(result,(char*) sqlite3_column_text(stmt, 1));
        strcat(result,"-puncte\n");
    }
    if (write (fd, result, 300) < 0){
        perror ("[Server]Eroare la write() spre client.\n");
        //return errno;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
void mystats(sqlite3 *db,int fd, char nume[20])//afiseaza stats jucatorului
{
    sqlite3_stmt* stmt;
    char* errMesg = 0;
    char sql_stmt[300];
    char result[300];
            bzero(result,300);
    strcpy(sql_stmt,"SELECT 'Username-'||username,'puncte-'||puncte,castig||'-meciuri catigate', meciuri_jucate||'-meciuri jucate' from JUCATORI WHERE USERNAME='user';");
    sprintf(sql_stmt,"SELECT 'Username-'||username,'puncte-'||puncte,castig||'-meciuri catigate', meciuri_jucate||'-meciuri jucate' from JUCATORI WHERE USERNAME='%s';",nume);
    int bdd;
    bdd = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, 0);

    if (bdd != SQLITE_OK) {
        strcpy(result,"Acest utilizator nu exista, incearca un alt username.");
        sqlite3_close(db);
        //cout<<11<<endl;
    }
    else
        {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                strcat(result,(char*)sqlite3_column_text(stmt, 0));
                strcat(result," ");
                strcat(result,(char*) sqlite3_column_text(stmt, 1));
                strcat(result," ");
                strcat(result,(char*) sqlite3_column_text(stmt, 2));
                strcat(result," ");
                strcat(result,(char*) sqlite3_column_text(stmt, 3));
                strcat(result,"\n");
            }
        }
    if(strlen(result)==0)
        strcpy(result,"Acest utilizator nu exista, incearca un alt username.");
    if (write (fd, result, 300) < 0){
        perror ("[Server]Eroare la write() spre client.\n");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
int play(jucatori j, int fd,int x, int juc) {//muta pionul ales si verifica daca poate face miscarea respectiva
    int zar;
    fflush(stdout);
    Afisare_tabla(x);
    char msg[100];
    FromMatToStr(x);
    fd_set actfds;
    if (write(fd, t[x].line_table, 450) < 0) {
        perror("[Server]Eroare la write() spre client.\n");
        return errno;
    }
    int id = j.cul;
    if (id == 0)
        for (int i = 0; i < 50; i++) {
            t[x].parcurgere_i[i] = parcurgere_Gi[i];
            t[x].parcurgere_j[i] = parcurgere_Gj[i];
        }
    else
        if (id == 1)
            for (int i = 0; i < 50; i++) {
                t[x].parcurgere_i[i] = parcurgere_Ri[i];
                t[x].parcurgere_j[i] = parcurgere_Rj[i];
            }
        else
            if (id == 2)
                for (int i = 0; i < 50; i++) {
                    t[x].parcurgere_i[i] = parcurgere_Vi[i];
                    t[x].parcurgere_j[i] = parcurgere_Vj[i];
                }
            else
                if (id == 3)
                    for (int i = 0; i < 50; i++) {
                        t[x].parcurgere_i[i] = parcurgere_Ai[i];
                        t[x].parcurgere_j[i] = parcurgere_Aj[i];
                    }
    CopyTable(t[x].table_copy,t[x].table);
    if (read(fd, &zar, 4) < 0) {
        perror("[server] Eroare la read() de la client.\n");
        return errno;
    }
    if(zar==8)
    {
        bzero(msg,100);
        strcpy(msg,"Sunteti sigur ca doriti sa parasiti jocul?");
        if (write(fd, msg, 100) < 0) {
            perror("[server] Eroare la read() de la client.\n");
            return errno;
        }
        int rasp;
        if (read(fd, &rasp, 4) < 0) {
            perror("[server] Eroare la read() de la client.\n");
            return errno;
        }
        if(rasp==1)
        {
            return -1;
        } 
        else
            if (read(fd, &zar, 4) < 0) {
            perror("[server] Eroare la read() de la client.\n");
            return errno;
        }
    }
    if(zar==7)
    {
        t[x].pause=1;
        if (read(fd, &zar, 4) < 0) {
            perror("[server] Eroare la read() de la client.\n");
            return errno;
        }
        t[x].pause=0;  
        restabilire_pozitii(t[x].player[id], db, fd, j.nume);
    }
    fflush(stdout);
    CopyTable(t[x].table,t[x].table_copy);
    if (t[x].player[id].iesit == 0 && zar != 6)
    {
        bzero(msg, 100);
        strcpy(msg, "Nu puteti face nicio miscare, deoarece nu aveti pioni valabili.");
        if (write(fd, msg, 100) < 0) {
            perror("[Server]Eroare la write() spre client.\n");
            return errno;
        }
    }
    else
    {
        int X, Y,X_vechi,Y_vechi;
        bzero(msg, 100);
        strcpy(msg, "Alegeti pionul pe care doriti sa il mutati(1,2,3,4):");
        fflush(stdout);
        if (t[x].player[id].iesit == 0 && zar == 6)
        {
            t[x].player[id].iesit = 1;
        }

        if (write(fd, msg, 100) < 0) {
            perror("[Server]Eroare la write() spre client.\n");
            return errno;
        }
        int pion;

        if (read(fd, &pion, 100) < 0) {
            perror("[Server]Eroare la write() spre client.\n");
            return errno;
        }
        if(pion<1 || pion >4)
        {
            while(pion<1 || pion >4)
            {
                bzero(msg, 100);
                strcpy(msg, "Ati introdus o valoare gresita! Alegeti pionul pe care doriti sa il mutati(1,2,3,4):");
                if (write(fd, msg, 100) < 0) {
                    perror("[Server]Eroare la write() spre client.\n");
                    return errno;
                }
                if (read(fd, &pion, 100) < 0) {
                    perror("[Server]Eroare la write() spre client.\n");
                    return errno;
                }
            }
            strcpy(msg, "Ati introdus o valoare corecta!");
                if (write(fd, msg, 100) < 0) {
                    perror("[Server]Eroare la write() spre client.\n");
                    return errno;
                }
        }
        else
        {
            strcpy(msg, "Ati introdus o valoare corecta!");
                if (write(fd, msg, 100) < 0) {
                    perror("[Server]Eroare la write() spre client.\n");
                    return errno;
                }
        }
        if(zar!=6 && t[x].player[id].piesa[pion-1].poz<0)
        {
            while(t[x].player[id].piesa[pion-1].poz<0)
            {
                bzero(msg, 100);
                strcpy(msg, "Acest pion nu se afla pe tabla de joc. Va rugam sa alegeti unul valabil.");
                if (write(fd, msg, 100) < 0) {
                    perror("[Server]Eroare la write() spre client.\n");
                    return errno;
                }
                if (read(fd, &pion, 100) < 0) {
                    perror("[Server]Eroare la write() spre client.\n");
                    return errno;
                }
            }
            strcpy(msg, "Ati introdus o valoare corecta!");
                if (write(fd, msg, 100) < 0) {
                    perror("[Server]Eroare la write() spre client.\n");
                    return errno;
                }
        }
        else
        {
            strcpy(msg, "Ati introdus o valoare corecta!");
                if (write(fd, msg, 100) < 0) {
                    perror("[Server]Eroare la write() spre client.\n");
                    return errno;
                }
        }
        X_vechi=t[x].player[id].piesa[pion - 1].x;
        Y_vechi=t[x].player[id].piesa[pion - 1].y;
        int ok=0,ok2=0;
        bzero(msg,100);
        if (zar == 6)
        {
            if (t[x].player[id].piesa[pion - 1].poz < 0)
            {
                t[x].player[id].piesa[pion - 1].x = t[x].player[id].piesa[pion - 1].xs;
                X=t[x].player[id].piesa[pion - 1].xs;
                t[x].player[id].piesa[pion - 1].y = t[x].player[id].piesa[pion - 1].ys;
                Y=t[x].player[id].piesa[pion - 1].ys;
                t[x].player[id].piesa[pion - 1].poz = 0;
                ok=1;
                t[x].player[id].pioni_pe_tabla++;
            }
            else {
                if (t[x].player[id].piesa[pion - 1].poz + zar <= 50) {
                    X = t[x].player[id].piesa[pion - 1].x;
                    Y = t[x].player[id].piesa[pion - 1].y;
                    int k=t[x].player[id].piesa[pion - 1].poz;
                    while(k< t[x].player[id].piesa[pion - 1].poz + zar)
                    {
                        X += t[x].parcurgere_i[k];
                        Y += t[x].parcurgere_j[k];
                        k++;
                    }
                    if(Verif_finish(X,Y,id,pion-1,x)==1)
                    {
                        t[x].player[id].piesa[pion-1].finish=1;
                        t[x].player[id].pioni_pe_tabla--;
                        if(t[x].player[id].pioni_pe_tabla==0)
                            t[x].player[id].iesit=0;                    
                    }
                    else
                        {
                            if(t[x].player[id].piesa[pion - 1].poz + zar>46)    
                                ok2=1;                    
                        }
                }
                else
                    ok2=1;
            }
        }
        else
            if (t[x].player[id].piesa[pion - 1].poz >= 0)
            {
                if (t[x].player[id].piesa[pion - 1].poz + zar <= 50) {
                    X = t[x].player[id].piesa[pion - 1].x;
                    Y = t[x].player[id].piesa[pion - 1].y;
                    int k=t[x].player[id].piesa[pion - 1].poz;
                    while(k< t[x].player[id].piesa[pion - 1].poz + zar)
                    {
                        X += t[x].parcurgere_i[k];
                        Y += t[x].parcurgere_j[k];
                        k++;
                    }
                    if(Verif_finish(X,Y,id,pion-1,x)==1)
                    {
                        t[x].player[id].piesa[pion-1].finish=1;
                        t[x].player[id].pioni_pe_tabla--;
                        if(t[x].player[id].pioni_pe_tabla==0)
                            t[x].player[id].iesit=0;                    
                    }
                    else
                        {
                            if(t[x].player[id].piesa[pion - 1].poz + zar>46)    
                                ok2=1;                    
                        }
                }
                else
                    ok2=1;
            }
                if(ok2==0)
                {
                    Verificare_mancat(X,Y,id,x);
                        for(int i=0;i<4;i++)
                            if(t[x].player[id].piesa[i].xs==X_vechi && t[x].player[id].piesa[i].ys==Y_vechi)
                                t[x].table[X_vechi][Y_vechi] = 's';
                            else
                                t[x].table[X_vechi][Y_vechi] = '_';
                    if(ok==0)
                    {
                        t[x].player[id].piesa[pion-1].x=X;
                        t[x].player[id].piesa[pion-1].y=Y;
                        t[x].player[id].piesa[pion-1].poz+=zar;
                    }
                    for(int i = 0 ; i < 4; i++)
                        {t[x].table[t[x].player[id].piesa[i].x][t[x].player[id].piesa[i].y] = (i+1) + '0';
                        //cout<<t[x].player[id].piesa[i].x<<" "<<t[x].player[id].piesa[i].y<<endl;
                        }
                        verificare_win(x,juc);
                        if(t[x].Clasament[juc%4]==0)
                            strcpy(msg, "Mutarea a fost efectuata cu succes!");
                        else
                            strcpy(msg, "Mutarea a fost efectuata cu succes si ai terminat!");
                }
                    else
                        strcpy(msg,"Aceasta miscare nu este posibila.");

                    if (write (fd, msg, 100) < 0){
                        perror ("[Server]Eroare la write() spre client.\n");
                        return errno;
                    }
                    Afisare_tabla(x);
                    FromMatToStr(x);

                    if (write (fd, t[x].line_table, 450) < 0){
                        perror ("[Server]Eroare la write() spre client.\n");
                        return errno;
                    }
                    for(int i = 0 ; i < 4; i++)
                        t[x].table[t[x].player[id].piesa[i].x][t[x].player[id].piesa[i].y] = t[x].player[id].culoare;
                }
            fflush(stdout);
    return 1;
    }
void setTimeout(int milliseconds)
{
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    int end = milliseconds_since + milliseconds;
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}
void finalmenu(sqlite3 *db,int fd, int i)//afiseaza meniul final 
{
    char com[10];
    fd_set actfds;
    char msg2[400];
    putoffline(j[i].nume,db,fd);
    bzero(msg2,400);
    strcat(msg2,"MENU:\nTastati /mystats pentru a va vedea statisticile\nTastati /top pentru a vedea primii 10 jucatori dupa numarul de puncte\nTastati /viewstats:nume pentru a vedea statisticile unui jucator.\nTastati q pentru a iesi din joc\n");
    if (write (fd, msg2, 300) < 0){
        perror ("[Server]Eroare la write() spre client.\n");
    }
    char com2[100];
    bzero(com2,100);
    if (read(fd, com2, 100) < 0){
        perror ("[Server]Eroare la write() spre client.\n");
    }
    while(strstr(com2,"q")==0)
    {
        if(strstr(com2,"/top"))
            top10(db,fd);
        else
            if(strstr(com2,"/mystats"))
                mystats(db,fd,j[i].nume);
                else
                    if(strstr(com2,"/viewstats"))
                    {
                        char *p=strtok(com2,":");
                        char getname[20];
                        while(p!=NULL)
                        {
                            strcpy(getname,p);
                            p=strtok(NULL,"");
                        }
                        mystats(db,fd,getname);
                    }
        bzero(com2,100);
        if (read(fd, com2, 100) < 0){
            perror ("[Server]Eroare la write() spre client.\n");
        }
    }
    close(fd);
    FD_CLR(fd, &actfds);
}

void *Threads_exec(void *arg)//executia jucatorului
{
    struct thData tdL; 
	tdL= *((struct thData*)arg);
    int fd=*(int *)arg;
    char raspuns[100];
    char msg[100];
    char culoare[100],raspcul[1];
    int k2=-1;
    int i=Index;
    int tab=tb-1;
    t[tab].juc_con++;
    if(i%4==0)
        Initializare_piese();
    if (write (tdL.cl, &i, 4) < 0){
        perror ("[Server]Eroare la write() spre client.\n");
    }
    if (write (tdL.cl, &t[tab].juc_con, 4) < 0){
        perror ("[Server]Eroare la write() spre client.\n");
    }
    fd_set actfds;
            Index++;
    if(i%4==1)
    {
        int delay=30;
        if (write (tdL.cl, &t[tab].start, 4) < 0){
                    perror ("[Server]Eroare la write() spre client.\n");
                }
        do {
            printf("\033[AJocul va incepe in: %d secunde\n", delay);
            setTimeout(1000);
            if(t[tab].juc_con==4)
                delay=0;
            delay--;

        } while (delay >= 0);
        t[tab].start=1;
        if (write (tdL.cl, &t[tab].start, 4) < 0){
                    perror ("[Server]Eroare la write() spre client.\n");
                }
    }
    while(t[tab].start==0)
    {
        if (write (tdL.cl, &t[tab].start, 4) < 0){
            perror ("[Server]Eroare la write() spre client.\n");

        }       
    }
    while(Index%4!=0)
        Index++;

            strcpy(msg, "Inainte de a incepe jocul, trebuie sa alegeti username-ul si culoarea cu care veti juca!");
            if (write (tdL.cl, msg, 100) < 0){
                perror ("[Server]Eroare la write() spre client.\n");
            }
            bzero(j[i].nume, 100);
            if (read (tdL.cl, j[i].nume, 100) < 0){
                perror ("[Server]Eroare la read() spre client.\n");
            }
            Insertnume(j[i].nume,db,tdL.cl,i);
            //cout<<j[i].nume<<endl;
            cout << "S-a conectat " << j[i].nume << " la server cu succes\n"; 
            fflush(stdout); 
            int fl=0,flg=0;
            while (t[tab].okk!=1)
            {
                if (t[tab].k == i%4)
                {
                    if(fl==0)
                    {
                //cout<<t[tab].k<<"-----"<<i<<endl;
                fflush(stdout);
                        flg=2;
                //cout<<flg<<endl;
                fflush(stdout);
                        if (write(tdL.cl, &flg, 4) < 0) {
                            perror("[Server]Eroare la write() spre client.\n");
                        }
                        fl=1;
                    }
                    if (read(tdL.cl, raspcul, 1) < 0) {
                        perror("[server] Eroare la read() de la client.\n");
                        //return errno;
                    }

                    if (strchr("RGVA", raspcul[0]) != 0) {
                        if (raspcul[0] == 'G') {
                            k2 = 0;
                            strcpy(culoare, "galben");
                        }
                        else
                            if (raspcul[0] == 'R') {
                                k2 = 1;
                                strcpy(culoare, "rosu");
                            }
                            else
                                if (raspcul[0] == 'V') {
                                    k2 = 2;
                                    strcpy(culoare, "verde");
                                }
                                else
                                    if (raspcul[0] == 'A') {
                                        k2 = 3;
                                        strcpy(culoare, "albastru");
                                    }

                        if (t[tab].culori[k2] == 0) {
                            t[tab].k++;
                            flg=0;
                            bzero(raspuns, 100);
                            strcpy(raspuns, "Ati ales culoarea ");
                            strcat(raspuns, culoare);
                            t[tab].culori[k2] = 1;

                            j[i].cul = k2;
                        }
                        else {
                            strcpy(raspuns, "Ne pare rau, dar culoarea a fost deja aleasa!");
                        }
                    }
                    else {
                        strcpy(raspuns, "Nu ati ales o culoare valida! Va rugam sa incercati o alta culoare.");
                    }
                    
                    fflush(stdout);
                    if (write(tdL.cl, raspuns, 100) < 0) {

                        perror("[Server]Eroare la write() spre client.\n");
                        //return errno;
                    }
                    else
                        cout << raspuns << endl;
                        fflush(stdout);
                    updatepozitii(t[tab].player[k2], db, tdL.cl, j[i].nume);
                }
                else
                {

                    if (write(tdL.cl, &t[tab].okk, 4) < 0) {
                        perror("[Server]Eroare la write() spre client.\n");
                        //return errno;
                    }
                }
                if(t[tab].k==t[tab].juc_con && (i%4)==t[tab].juc_con-1 )
                {
                    t[tab].okk=1;
                    if (write(tdL.cl, &t[tab].okk, 4) < 0) {
                        perror("[Server]Eroare la write() spre client.\n");
                        //return errno;
                    }
                }
                    
                
            }
            fflush(stdout);
            jucatori aux=j[i];
            int final=0;
            while(t[tab].flag)
            {
                int rand=0;
                int contor=0;
                while(t[tab].k_2<t[tab].juc_con )
                {
                    if(t[tab].pause==1)
                    {
                        rand=2;
                        rand*=10;
                        switch (t[tab].player[t[tab].k_2].culoare)
                        {
                        case 'G':
                            rand+=0;
                            break;
                        case 'R':
                            rand+=1;
                            break;
                        case 'V':
                            rand+=2;
                            break;
                        case 'A':
                            rand+=3;
                            break;
                        default:
                            break;
                        }
                        if (write (tdL.cl, &rand, 4) < 0){
                            perror ("[Server]Eroare la write() spre client.\n");
                            //return errno;
                        }
                        restabilire_pozitii(t[tab].player[i], db, tdL.cl, j[i].nume);
                    }
                    else
                        rand=0; 
                       if(t[tab].k_2==i%4)
                        {
                                fflush(stdout);
                            rand=1;
                            if (write (tdL.cl, &rand, 4) < 0){
                                        perror ("[Server]Eroare la write() spre client.\n");
                                    }
                            if(t[tab].juc_term==t[tab].juc_con-1)
                            {
                                bzero(msg,100);
                                if(t[tab].podium==t[tab].juc_con-1)
                                    strcpy(msg, "Din pacate, ati terminat pe ultimul loc.");
                                else
                                {
                                    bzero(msg,100);
                                        strcpy(msg, "Din pacate, ceilalti jucatori ramasi s-au deconectat. Felicitari ati terminat pe locul ");
                                        char loc[1];
                                            t[tab].podium++;
                                        loc[0]=t[tab].podium+'0';
                                        loc[1]='\0';
                                        strcat(msg,loc);
                                }
                                if (write (tdL.cl, msg, 100) < 0){
                                    perror ("[Server]Eroare la write() spre client.\n");
                                    //return errno;
                                }
                                t[tab].Clasament[i%4]=t[tab].juc_con-t[tab].juc_decon;
                                updateclasament(t[tab].Clasament[i%4],t[tab].juc_con,db, tdL.cl, j[i].nume);
                                finalmenu(db,tdL.cl,i);
                                t[tab].flag=0;
                            }
                            if(t[tab].Clasament[i%4] == 0){
                                bzero(msg,100);
                                strcpy(msg, "A venit randul tau");
                                if (write (tdL.cl, msg, 100) < 0){
                                    perror ("[Server]Eroare la write() spre client.\n");
                                }
                                j[i]=aux;
                                int val = play(j[i], tdL.cl,tab,i);
                                
                                fflush(stdout);
                                if(val==-1)
                                {
                                    t[tab].Clasament[t[tab].k_2]=-1;
                                    t[tab].juc_term++;
                                    t[tab].juc_decon++;
                                    putoffline(j[i].nume,db,tdL.cl);
                                    while(t[tab].Clasament[t[tab].k_2]!=0)
                                        t[tab].k_2++;
                                    close(tdL.cl);
                                    FD_CLR(tdL.cl, &actfds);
                                }
                                else
                                {
                                    updatepozitii(t[tab].player[k2], db, tdL.cl, j[i].nume);
                                    if(t[tab].Clasament[i%4] != 0){
                                        bzero(msg,100);
                                        strcpy(msg, "Felicitari ati terminat pe locul ");
                                        char loc[1];
                                        loc[0]=t[tab].Clasament[i%4]+'0';
                                        loc[1]='\0';
                                        strcat(msg,loc);
                                        fflush(stdout);
                                        j[i].terminat=1;
                                        if (write (tdL.cl, msg, 100) < 0){
                                            perror ("[Server]Eroare la write() spre client.\n");
                                        }
                                            fflush(stdout);
                                            t[tab].juc_term++;
                                            updateclasament(t[tab].Clasament[i%4],t[tab].juc_con,db, tdL.cl, j[i].nume);
                                        rand=0;
                                        fflush(stdout);
                                        while(t[tab].Clasament[t[tab].k_2]!=0)
                                            t[tab].k_2++;
                                        finalmenu(db,tdL.cl,i);
                                    }
                                    else
                                        {
                                            rand=0;
                                            fflush(stdout);
                                            t[tab].k_2++;
                                        }
                                }
                            } 
                            else
                                {
                                    if(t[tab].Clasament[i%4]!=0)
                                        t[tab].k_2++;
                                    else
                                        if (write (tdL.cl, &rand, 4) < 0){
                                                perror ("[Server]Eroare la write() spre client.\n");
                                            }
                                    
                                }
                            while(t[tab].Clasament[t[tab].k_2]!=0)
                                t[tab].k_2++;
                        }  
                }
                t[tab].k_2=0;
                while(t[tab].Clasament[t[tab].k_2]!=0)
                    t[tab].k_2++;
                fl=0;
            }
        
           
    }    
static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);	
		printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		Threads_exec((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
};	
int main (){
    if(bdd)
    {
        fprintf(stderr, "Eroare la open baza de date: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }
    else
    {
        fprintf(stdout, "Succes la deschiderea bazei de date\n");
        fflush(stdout);
    }

  /* AICI CREAM TABELUL JOCULUI */
    tabel = "CREATE TABLE IF NOT EXISTS JUCATORI("  \
       		"USERNAME       CHAR(20)     PRIMARY KEY," \
            "PAROLA       CHAR(30)     ," \
      		"PUNCTE           INT," \
            "P1_X      INT," \
            "P1_Y      INT," \
            "P2_X      INT," \
            "P2_Y      INT," \
            "P3_X      INT," \
            "P3_Y      INT," \
            "P4_X      INT," \
            "P4_Y      INT," \
			"CASTIG      INT," \
			"MECIURI_JUCATE       INT,"\
            "COD       CHAR(5),"\
            "ONLINE       INT);";
	bdd = sqlite3_exec(db, tabel, callback, 0, &errmsg);
   
    if( bdd != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", errmsg);
        fflush(stdout);
      sqlite3_free(errmsg);
    } 
    else 
    {
      fprintf(stdout, "Table created successfully\n");
        fflush(stdout);
    }
	struct sockaddr_in server;	
	struct sockaddr_in from;
	fd_set readfds;	
	fd_set actfds;
	struct timeval tv;
	int sd, client;	
	int optval=1; 
	int fd;	
	int nfds;
	unsigned int len;

	int  in = 0, on = 1;
	int p[4];
    char msg[100];

	if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
		perror ("[server] Eroare la socket().\n");
		return errno;
	}
    
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

	bzero (&server, sizeof (server));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	server.sin_port = htons (PORT);

	if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1){
		perror ("[server] Eroare la bind().\n");
		return errno;
	}

	if (listen (sd, 5) == -1){
		perror ("[server] Eroare la listen().\n");
		return errno;
	}

    cout<<"[server] Asteptam la portul "<<PORT<<"...\n";
    fflush(stdout);
    int counter=0;
	while (1){
        int client;
      thData * td; //parametru functia executata de thread     
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);
	  		client = accept (sd, NULL, NULL);
            if(t[tb-1].start==1)
                while(counter%4!=0)
                    counter++;
            int threadIndex=0;
            while(threads[threadIndex] !=0 )
            {
                threadIndex++;

            }
            if(counter%4==0)
            {
                CreateTable(tb);
                tb++;
                if(tb==100)
                    tb=0;
                cout<<"S-a pornit jocul "<<tb<<endl;
                fflush(stdout);
            }
            td=(struct thData*)malloc(sizeof(struct thData));	
            td->idThread=threadIndex++;
            td->cl=client;
                pthread_create(&threads[threadIndex],NULL,&treat,td);
            counter++;
            
    }
    close(sd);	
}

