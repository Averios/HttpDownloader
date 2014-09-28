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
void get_domain(char *address, char **domain, char **destination);

int main(int argc, char** argv){
    char buffer[1025];
    int portNum, sockfd = 0, ok;
    struct sockaddr_in serv_addr;
    char *domain,*destination;
    
    if(argc < 2){
        portNum = atoi(argv[2]);
    }
    else{
        portNum = 80;
    }

    get_domain(argv[1], &domain, &destination);
    
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

//Separate the domain and file from the url
void get_domain(char *address, char **domain, char **destination){
    int length;
    length = strlen(address);
    char *dom, *dest;
    int i,separator;
    for(i = 0; i < length; i++){
        if(address[i] == '/'){
            separator = i;
            break;
        }
    }
    dom = (char*)malloc(sizeof(char)*(i+1));
    printf("%d\n",i);
    if(i == length){
        strcpy(dom,address);
        dest = (char*)malloc(sizeof(char)*2);
        strcpy(dest,"/");
    }
    else{
        strncpy(dom,address,i);
        dest = (char*)malloc(sizeof(char)*(length-i+1));
        int j = i,k;
        for(k = 0; j < length; j++, k++){
            dest[k] = address[j];
        }
        
    }

    *domain = dom;
    *destination = dest;
}
