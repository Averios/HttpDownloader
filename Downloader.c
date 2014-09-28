#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

int hostname_to_ip(char* domain, struct sockaddr_in *_addr);
int get_domain(char *address, char *domain, char *destionation);

int main(int argc, char** argv){
    char buffer[1025];
    int portNum, sockfd = 0, ok;
    struct sockaddr_in serv_addr;
    
    if(argc < 2){
        portNum = atoi(argv[2]);
    }
    else{
        portNum = 80;
    }
    ok = strlen(argv[1]);
    int i,separator;
    for(i = 0; i < ok; i++){
        if(argv[1][i] == '/'){
            separator = i;
            break;
        }
    }
    char *domain,*destination;
    domain = (char*)malloc(sizeof(char)*(i+1));
    
    if(i == ok){
        strcpy(domain,argv[1]);
        destination = (char*)malloc(sizeof(char)*2);
        strcpy(destination,"/");
    }
    else{
        strncpy(domain,argv[1],i);
        destination = (char*)malloc(sizeof(char)*(ok-i+1));
        int j = i,k;
        for(k = 0; j < ok; j++, k++){
            destination[k] = argv[1][j];
        }
    }
    
    hostname_to_ip(domain, &serv_addr);
    
    
    
    
    return 0;
}

//http://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
int hostname_to_ip(char *domain, struct sockaddr_in *_addr){
    
    struct addrinfo hint, *serv_addr, *iter;
    struct sockaddr_in *h;
    int rv;
    
    memset(&hint, 0, sizeof hint);
    
    hint.ai_family = AF_INET;       //Force ipv4
    hint.ai_socktype = SOCK_STREAM; //TCP
    
    if((rv = getaddrinfo(domain, "http", &hint, &serv_addr)) != 0){
        //Print the error
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    //Dummy variable for testing purpose
    char *ip;
    ip = (char*)malloc(sizeof(char)*100);
    
    //Loop through all the result
    for(iter = serv_addr; iter != NULL; iter->ai_next){
        h = (struct sockaddr_in*)iter->ai_addr;
        if(strcpy(ip,inet_ntoa(h->sin_addr)) != NULL){
            //Stop the loop when the connection is successful
            *_addr = *h;
            break;
        }
    }
    
    freeaddrinfo(serv_addr);
    free(ip);
    return 0;
    
}
