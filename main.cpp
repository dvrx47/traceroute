// Autor:  Mikołaj Kowalik
// indeks: 283476

#include "obsluga.hpp"

int pid = getpid();

extern struct timeval timeStart[MAX_TTL+1], timeEnd[MAX_TTL+1];
extern int suma_pakietow[MAX_TTL+1];




int main(int argc, char ** argv){

    
    //inicjowanie tablic
    for(int i=0; i<MAX_TTL; ++i){
            timeStart[i].tv_sec = timeStart[i].tv_usec = 0;
            timeEnd[i].tv_sec = timeEnd[i].tv_usec = 0;
            suma_pakietow[i] = 0;
    }
    //kod z wykładu
    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    //obsługa poprawności inicjalizacji, i zmiennych startowych
    if( sockfd < 0 ) {
        fprintf(stderr, "nie udało się utworzyć gniazda (prawdopodobnie brak uprawnień administratora).\n" );
        return EXIT_FAILURE;
    }

    if( argc != 2 ) {
        fprintf(stderr, "podano nieprawidłową ilość argumentów\n" );
        return EXIT_FAILURE;
    }

    if( !inet_pton(AF_INET, argv[1], &recipient.sin_addr) ){
        fprintf(stderr, "nieprawidłowy adres ip\n" ); 
        return EXIT_FAILURE;
    }  

    
//ttl \in [1,30]
    for(int ttl = 1; ttl <= 30; ++ttl)
    {
        setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
        for(int i=0; i<3; ++i){
                
                struct timeval timeTemp;
                

               ssize_t bytes_sent = wyslijpakiet(sockfd, ttl, recipient, pid); 
               gettimeofday(&timeTemp,NULL);

               timeStart[ttl].tv_sec += timeTemp.tv_sec;
               timeStart[ttl].tv_usec += timeTemp.tv_usec;
               if( bytes_sent < 0){
                   std:: cout << "błąd: nie udało się poprawnie wysłać pakietu." << std:: endl;
                   return EXIT_FAILURE;
               }
        }


        
        fd_set descriptors;
        FD_ZERO (&descriptors);
        FD_SET (sockfd, &descriptors);
        //tv - czas oczekiwan, timeTemp - czas który zostanie dopasowany zgodnie z ttl pakietu
        struct timeval tv, timeTemp; 
        tv.tv_sec = 1; tv.tv_usec = 0;

        int ready;
        bool wyjscie = false;
        printf("%2d. ", ttl);

        //odbieranie odpowiedzi
        for(int i=0; i<3; ++i){
            ready = select (sockfd+1, &descriptors, NULL, NULL, &tv);
            
            gettimeofday(&timeTemp,NULL);
            int delta = obsluz_pakiet(sockfd, ttl, ready, timeTemp, argv[1], i, pid);

            //delta = 10 - specjalny komunikat kontaktu z docelowym komputerem
            if( delta != 10 ) 
                i+=delta;
            else
                wyjscie = true;       
        }
        //wyświetlenie średnich czasów
        if(suma_pakietow[ttl] == 3)
            std:: cout << 
            ((timeEnd[ttl].tv_sec - timeStart[ttl].tv_sec)*1000.0 + (timeEnd[ttl].tv_usec - timeStart[ttl].tv_usec)/1000.0)/3 << "ms";
        else if( suma_pakietow[ttl] != 0 ) std:: cout << "???";
        
        std::cout<< std:: endl;
        if(wyjscie) break;
    }


	return EXIT_SUCCESS;
}
