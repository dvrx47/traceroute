// Autor:  Mikołaj Kowalik
// indeks: 283476

#ifndef obsluga_hpp
#define obsluga_hpp

    #include <netinet/ip.h>
    #include <netinet/ip_icmp.h>
    #include <arpa/inet.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>
    #include <iostream>
    #include <assert.h>
    #include <iostream>
    #include <sys/time.h>
    #include <unistd.h>

    #define MAX_TTL 30

    

    u_int16_t compute_icmp_checksum (const void *buff, int length);
    int obsluz_pakiet(int sockfd, int ttl, int ready, struct timeval timeTemp, char * dst, int nrOdbioru, int pid);
    ssize_t wyslijpakiet(int sockfd, int ttl, struct sockaddr_in recipient, int pid);


#endif
