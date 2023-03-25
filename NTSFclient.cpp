#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <iomanip>
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define YELLOW  "\033[33m"      /* Yellow */
using namespace std;

//extern int errno;

int port;

char table_line[450], table[15][30];
void StrToMatrice(){
	int lin = 0, col = 0;
	for(int i = 0; i <= 450; i++){
        if(i % 30 == 0 && i!=0){
			lin ++;
			col = 0;
		}
		table[lin][col] = table_line[i];
		col ++;
	}	
}
void Afisare_tabla(){
    setw (25);
    for(int i = 0; i <= 14; i++){
        cout<<"                        ";
        for(int j = 0; j <= 29; j++)
        {
            switch (table[i][j])
            {
            case 'G':
            case 'g':
                cout << YELLOW<< table[i][j]<< RESET;
                break;
            case 'R':
            case 'r':
                cout << RED<< table[i][j]<< RESET;
                break;
            case 'V':
            case 'v':
                cout << GREEN<< table[i][j]<< RESET;
                break;
            case 'A':
            case 'a':
                cout << BLUE<< table[i][j]<< RESET;
                break;
            case 's':
                if(i==1 && j==18)
                    cout << YELLOW<< table[i][j]<< RESET;
                else
                    if(i==9 && j==26)
                        cout << RED<< table[i][j]<< RESET;
                    else
                        if(i==13 && j==10)
                            cout << GREEN<< table[i][j]<< RESET;
                        else
                            if(i==5 && j==2)
                                cout << BLUE<< table[i][j]<< RESET;
                break;

            default:
                cout<< table[i][j];
                break;
            }
        }
            fflush (stdout);
        cout << endl;
        fflush (stdout);
    }
}
int aruncazar(){
    srand (time(NULL));
    int zar = rand() % 6 + 1;
    return zar;
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
void Introducere()
{
    cout<<"                        Bine ai venit la jocul 'Nu te supara, frate!'           "<<endl;
    cout<<"      Daca doresti sa continui, scrie c sau daca doresti sa stii regulile jocului scrie /help."<<endl;
}
void help()
{
    FILE * fPtr;
    char ch;
    fPtr = fopen("reguli.txt", "r");
    if(fPtr == NULL)
    {
        printf("Unable to open file.\n");
    }
    do 
    {
        ch = fgetc(fPtr);
        putchar(ch);

    } while(ch != EOF);
    cout<<endl;
    fclose(fPtr);   
}
int main (int argc, char *argv[]){
    struct sockaddr_in server;
    int sd;
    char msg[100], username[100],parola[100];
    sqlite3 *db;
    char *errmsg = 0;
    char *tabel;
    char comanda[500];
     int bdd = sqlite3_open("players.db", &db);
    if(bdd)
    {
      cout<<"Eroare la open baza de date: "<<sqlite3_errmsg(db)<<endl;
    fflush(stdout);
      return 0;
    }
    else
    {
        cout<<"Succes la deschiderea bazei de date\n";
    }
    if (argc != 3){
        cout<<"[client] Sintaxa: "<<argv[0]<<" <adresa_server> <port>\n";
            fflush(stdout);
        return -1;
    }

    port = atoi (argv[2]);

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
        perror ("[client] Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;

    server.sin_addr.s_addr = inet_addr(argv[1]);

    server.sin_port = htons (port);

    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1){
        perror ("[client]Eroare la connect().\n");
        return errno;
    }
    int okk=0,maxx=0,index;
    if(read(sd, &index, 4) < 0){
                perror("[client] Eroare la read\n");
                return errno;
            }
    if(read(sd, &maxx, 4) < 0){
                perror("[client] Eroare la read\n");
                return errno;
            }
    if(read(sd, &okk, 4) < 0){
                perror("[client] Eroare la read\n");
                return errno;
            }
    if(okk==0)
        cout<<"Se asteapta conectarea celorlalti jucatori!\n";
    fflush (stdout);
    if(okk==0)
    {
        while(okk==0)
        {
            fflush (stdin);
            if(read(sd, &okk, 4) < 0){
                perror("[client] Eroare la read\n");
                return errno;
            }
        }
    }
    cout<<endl;
    int delay=5;
        do {
            printf("\033[AJocul va incepe in: %d secunde\n", delay);
            setTimeout(1000);
            delay--;

        } while (delay >= 0);
    char com[10];
    Introducere();
    cin>>com;
    if(strstr(com,"/help"))
        help();
    else
        if(strstr(com,"c")==0)
            while(strstr(com,"/help")==0 && strstr(com,"c")==0)
            {
                cout<<"Ati introdus o comanda gresita! Incercati dinou: ";
                cin>>com;
                cout<<endl;
                if(strstr(com,"/help"))
                    help();
            }
    fflush (stdout);

        bzero(msg, 100);
        if(read(sd, msg, 100) < 0){
            perror("[client] Eroare la read\n");
            return errno;
        }
        cout<<msg<<endl;
        
        fflush (stdout);
        cout << "Introduceti username-ul: ";
        fflush (stdout);
        bzero(username, 100);
        cin >> username;
        fflush(stdin);

        if (write (sd, username, 100) < 0){
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
        }
        cout<<username<<endl;
        bzero(msg, 100);
        if(read (sd, msg, 100) < 0){
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
            }
        while(strstr(msg,"Incercati"))
        {
            cout<<msg;
            bzero(username, 100);
            cin >> username;
            fflush(stdin);

            if (write (sd, username, 100) < 0){
                perror ("[client]Eroare la write() spre server.\n");
                return errno;
            }
            cout<<username<<endl;
            bzero(msg, 100);
            if(read (sd, msg, 100) < 0){
                    perror ("[client]Eroare la read() de la server.\n");
                    return errno;
                }
        }
        cout<<msg<<endl;
        fflush (stdout);
        char cod[100];
        if(strstr(msg,"observat"))
        {
            bzero(parola, 100);
            cin >> parola;
            fflush(stdin);
            if (write (sd, parola, 100) < 0){
                perror ("[client]Eroare la write() spre server.\n");
                return errno;
            }
            bzero(msg, 100);
            if(read (sd, msg, 100) < 0){
                    perror ("[client]Eroare la read() de la server.\n");
                    return errno;
                }
            cout<<msg<<" (!ATENTIE: salvati acest cod!)"<<endl;
            cin>>cod;
            while(strlen(cod)!=4)
            {
                cout<<"Introduceti un cod de 4 cifre/litere.";
                cin>>cod;
            }
            if (write (sd, cod, 100) < 0){
                perror ("[client]Eroare la write() spre server.\n");
                return errno;
            }
        }
        else
        {
            bzero(parola, 100);
            cin >> parola;
            fflush(stdin);
            if (write (sd, parola, 100) < 0){
                perror ("[client]Eroare la write() spre server.\n");
                return errno;
            }
        }
        bzero(msg, 100);
        if(read (sd, msg, 100) < 0){
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
            }
        cout<<msg<<endl;
        fflush (stdout);
        if(strstr(msg, "gresita"))
        {
            while(strstr(msg, "gresita"))
            {
                bzero(parola, 100);
                cin >> parola;
                fflush(stdin);
                if (write (sd, parola, 100) < 0){
                    perror ("[client]Eroare la write() spre server.\n");
                    return errno;
                }
                if(strstr(parola,"/resetpassword"))
                {
                    bzero(msg, 100);
                    if(read (sd, msg, 100) < 0){
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                        }
                    cout<<msg<<endl;
                    bzero(cod,100);
                    cin>>cod;
                    if (write (sd, cod, 100) < 0){
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                    }
                    bzero(msg, 100);
                    if(read (sd, msg, 100) < 0){
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                        }
                    while(strstr(msg,"gresit"))
                    {
                        cout<<msg<<endl;
                        cin>>cod;
                        if (write (sd, cod, 100) < 0){
                            perror ("[client]Eroare la write() spre server.\n");
                            return errno;
                        }
                        bzero(msg, 100);
                        if(read (sd, msg, 100) < 0){
                                perror ("[client]Eroare la read() de la server.\n");
                                return errno;
                            }
                    }
                    cout<<msg<<endl;
                    bzero(parola,100);
                    cin>>parola;
                    if (write (sd, parola, 100) < 0){
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                    }
                    bzero(msg, 100);
                    if(read (sd, msg, 100) < 0){
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                        }
                    cout<<msg<<endl;
                    bzero(parola,100);
                    cin>>parola;
                    if (write (sd, parola, 100) < 0){
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                    }
                }
                bzero(msg, 100);
                if(read (sd, msg, 100) < 0){
                        perror ("[client]Eroare la read() de la server.\n");
                        return errno;
                    }
                cout<<msg<<endl;
                fflush (stdout);
            }   
        }
        int ok = 0;
        bzero(msg, 100);
        if(read (sd, &okk, 4) < 0){
              perror ("[client]Eroare la read() de la server.\n");
              return errno;
        }
        int flg=0;
        while(flg==0)
        {
            if(okk==0)
            {
           
                cout<<"Trebuie sa asteptati ca ceilalti jucatori dinaintea dumneavoastra sa isi aleaga o culoare."<<endl;
                fflush (stdout);
                while(okk!=2)
                {
                    if(read (sd, &okk, 4) < 0){
                                perror ("[client]Eroare la read() de la server.\n");
                                return errno;
                            }
                    
                }
            }
            char culoare[1];
            
            while(okk==2){
                fflush (stdout);
                bzero (culoare, 1);
                cout<<"G-galben \nR-rosu \nV-verde \nA-albastru\nDin lista de mai sus alegeti o litera corespunzatoare unei culori:"<<endl;
                fflush (stdout);
                cin >> culoare;
                fflush(stdin);
                if (write (sd, culoare, 1) < 0){
                    perror ("[client]Eroare la write() spre server.\n");
                    return errno;
                }
                bzero(msg, 100);
                if(read (sd, msg, 100) < 0){
                    perror ("[client]Eroare la read() de la server.\n");
                    return errno;
                }
                cout<<msg<<endl;
                fflush (stdout);
                if(strstr(msg, "Ati ales culoarea") != 0){
                    okk = 0;
                }
            }
            if(okk!=1)
            {
                cout<<"Trebuie sa asteptati ca ceilalti jucatori sa isi aleaga o culoare."<<endl;
                fflush (stdout);
                while(okk==0)
                {
                    if(read (sd, &okk, 4) < 0){
                                perror ("[client]Eroare la read() de la server.\n");
                                return errno;
                            }
                }
                flg=1;
            }
        }

        int pornit=1,zar;
         fflush(stdin); 
        cout<<"Incepe jocul"<<endl;
        fflush (stdout);
        int exit=0;
       while(pornit==1)
       {
            int rand;
            if(read(sd, &rand, 4) < 0){
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
            }
            if(rand==0 || rand/10==2)
            {
                int contor=0;
                while(rand==0 || rand/10==2)
                {
                    if(read(sd, &rand, 4) < 0){
                        perror ("[client]Eroare la read() de la server.\n");
                        return errno;
                    }
                    if(rand/10==2 && contor==0)
                    {
                        int color;
                        char c[15];
                        switch (rand%10)
                        {
                        case 0: 
                            strcpy(c,"galben");
                            break;
                        case 1: 
                            strcpy(c,"rosu");
                            break;
                        case 2: 
                            strcpy(c,"verde");
                            break;
                        case 3: 
                            strcpy(c,"albastru");
                            break;
                        default:
                            break;
                        }
                        contor++;
                        cout<<"Jocul a fost oprit temporar de catre jucatorul de culoare "<<c<<" ."<<endl;
                    }
                }
            }
            bzero(msg, 100);
            if(read(sd, msg, 100) < 0){
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
            }
            cout<<msg<<endl;
            if(strstr(msg,"pacate"))
            {

                pornit=0;
            }
            else
            {   fflush (stdout);
                bzero(table_line, 450);
                if(read(sd, table_line, 450) < 0){
                    perror ("[client]Eroare la read() de la server.\n");
                    return errno;
                }
                StrToMatrice();
                Afisare_tabla();
                char x;
                cout << "Pentru a da cu zarul scrieti 'z', pentru a intrerupe temporar jocul scrieti 'p', sau pentru a iesit din joc scrieti 'q.";
                fflush (stdout);
                cin>>x;
                fflush(stdin);
                while(x!='z' && x!='p' && x!='q')
                {
                    cout<<"Ati introdus o comanda gresita! Pentru a da cu zarul scrieti 'z' sau pentru a intrerupe temporar jocul scrieti 'p'.";
                    fflush (stdout);
                    cin>>x;
                    fflush(stdin);
                }
                if(x=='q')
                {
                    zar=8;
                    if (write (sd, &zar, 4) < 0){
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                    }
                    bzero(msg,100);
                    if(read(sd,msg,100) < 0)
                    {
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                    }
                    cout<<msg<<endl;
                    cout<<"Y/N"<<endl;
                    char r[3];
                    cin>>r;
                    while(strstr(r,"Y")==0 && strstr(r,"N")==0)
                    {
                        cout<<"Ati introdus un raspuns gresit"<<endl;
                        cin>>r;
                    }
                    int rasp;
                    if(strstr(r,"Y"))
                    {
                        rasp=1;
                        pornit=0;
                        exit=1;
                        if (write (sd, &rasp, 4) < 0){
                            perror ("[client]Eroare la write() spre server.\n");
                            return errno;
                        }
                        close(sd);
                    }
                    else
                    {
                        rasp=0;
                        if (write (sd, &rasp, 4) < 0){
                            perror ("[client]Eroare la write() spre server.\n");
                            return errno;
                        }
                    
                        cout << "Pentru a da cu zarul scrieti 'z', pentru a intrerupe temporar jocul scrieti 'p'.";
                        fflush (stdout);
                        cin>>x;
                        fflush(stdin);
                        while(x!='z' && x!='p' && x!='q')
                        {
                            cout<<"Ati introdus o comanda gresita! Pentru a da cu zarul scrieti 'z' sau pentru a intrerupe temporar jocul scrieti 'p'.";
                            fflush (stdout);
                            cin>>x;
                            fflush(stdin);
                        }
                    }
                }
                if(x=='p' && exit==0)
                {
                    cout<<"Ati intrerupt temporar jocul."<<endl;
                    zar=7;
                    if (write (sd, &zar, 4) < 0){
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                    }
                    cout<<"Pentru a intrerupe pauza aruncati cu zarul, scriind 'z'"<<endl;
                    cin>>x;
                    while(x!='z')
                    {
                        cout<<"Ati introdus o comanda gresita! Pentru a intrerupe pauza aruncati cu zarul, scriind 'z'."<<endl;
                        cin>>x;
                    } 
                }
                if(x=='z'&& exit==0)
               {
                    zar=aruncazar();
                    fflush(stdin);  
                    cout<<"Ati aruncat "<<zar<<endl;
                    fflush (stdout);
                    if (write (sd, &zar, 4) < 0){
                        perror ("[client]Eroare la write() spre server.\n");
                        return errno;
                    } 
                }
                if(x=='z' && exit==0) 
                {
                    int pion;
                    bzero(msg, 100);
                    if(read(sd, msg, 100) < 0){
                        perror ("[client]Eroare la read() de la server.\n");
                        return errno;
                    }
                    cout<<msg<<endl;
                    
                    fflush(stdout);
                    if(strstr(msg,"Alegeti"))
                    {
                        cin>>pion;
                        fflush(stdin);
                        if (write (sd, &pion, 4) < 0){
                            perror ("[client]Eroare la write() spre server.\n");
                            return errno;
                        } 
                        bzero(msg, 100);
                        if(read(sd, msg, 100) < 0){
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                        }
                        while(strstr(msg,"gresita"))
                        {
                            cout<<msg<<endl;
                            fflush(stdout);
                            cin>>pion;
                            fflush(stdin);
                            if (write (sd, &pion, 4) < 0){
                                perror ("[client]Eroare la write() spre server.\n");
                                return errno;
                            } 
                            bzero(msg, 100);
                            if(read(sd, msg, 100) < 0){
                                perror ("[client]Eroare la read() de la server.\n");
                                return errno;
                            }
                            
                        }
                        bzero(msg, 100);
                        if(read(sd, msg, 100) < 0){
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                        }
                        while(strstr(msg,"nu se afla"))
                        {
                            cout<<msg<<endl;
                            fflush(stdout);
                            cin>>pion;
                            fflush(stdin);
                            if (write (sd, &pion, 4) < 0){
                                perror ("[client]Eroare la write() spre server.\n");
                                return errno;
                            } 
                            bzero(msg, 100);
                            if(read(sd, msg, 100) < 0){
                                perror ("[client]Eroare la read() de la server.\n");
                                return errno;
                            }
                            
                        }
                        bzero(msg, 100);
                        if(read(sd, msg, 100) < 0){
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                        }
                        cout<<msg<<endl;;
                        if(strstr(msg,"succes") || strstr(msg,"nu este"))
                        {
                            if(read(sd, table_line, 450) < 0){
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                            }
                            StrToMatrice();
                            Afisare_tabla();
                            if(strstr(msg,"ai terminat"))
                            {
                                bzero(msg, 100);
                                if(read(sd, msg, 100) < 0){
                                    perror ("[client]Eroare la read() de la server.\n");
                                    return errno;
                                }
                                cout<<msg<<endl;
                                pornit=0;
                            }   
                        }
                    }
              }  
            } 
        }
        if(exit==0)
        {
            char msg2[400];
            bzero(msg2, 400);
            if(read(sd, msg2, 400) < 0){
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
            }
            cout<<msg2<<endl;
            char com2[100];
            bzero(com2,100);
            cin>>com2;
            char result[300];
            
            while(strstr(com2,"q")==0)
            {    while(strstr(com2,"/mystats")==0 && strstr(com2,"/top")==0 && strstr(com2,"q")==0 && strstr(com2,"/viewstats")==0)
                {
                    cout<<com2<<endl;
                    cout<<"Ati introdus o comanda gresita! Incercati dinou"<<endl;
                    cin>>com2;
                }
                if(write(sd, com2, 100) < 0){
                    perror ("[client]Eroare la read() de la server.\n");
                    return errno;
                }
                bzero(result,300);
                if(read(sd, result, 300) < 0){
                    perror ("[client]Eroare la read() de la server.\n");
                    return errno;
                }
                cout<<result<<endl;
                cin>>com2;
            }
        }
        close (sd);

    
}