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
void get_domain(char *address, char **domain, char **destination, char **file);

int main(int argc, char** argv){
    char buffer[1025];
    int portNum, sockfd = 0;
    struct sockaddr_in serv_addr;
    char *domain,*destination,*file;
    
    if(argc < 2){
        portNum = atoi(argv[2]);
    }
    else{
        portNum = 80;
    }

    get_domain(argv[1], &domain, &destination, &file);
    
    printf("%s\n%s\n%s\n", domain, destination, file);
    hostname_to_ip(domain, &serv_addr);
    
    printf("%s resolved %s\n", domain, inet_ntoa(serv_addr.sin_addr));
    printf("Connecting to server\n");
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
      printf("\n Error : Could not create socket \n");
      return 1;
    }
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        printf("ERROR cannot connect to server\n");
        return 1;
    }
    
    printf("Connected\n");
    
    printf("Disconnecting...\n");
    close(sockfd);
    printf("Disconnected\n");
    
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
    
    //resolving domain
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
void get_domain(char *address, char **domain, char **destination, char **file){
    char *dom, *dest, *fl;
    char *temp;
    dom = strtok_r(address, "/", &dest);
    if(strcmp(dest,"") == 0){
        dest = (char*)malloc(sizeof(char)*2);
        strcpy(dest, "/");
    }
    fl = (char*)malloc(sizeof(dest));
    temp = dest;
    while(strcmp(temp, "") != 0){
        strcpy(fl, temp);
        fl = strtok_r(fl, "/", &temp);
    }
    if(fl == NULL){
        fl = (char*)malloc(sizeof(char)*4);
        strcpy(fl, "any");
    }
    
    
    *domain = dom;
    *destination = dest;
    *file = fl;
}
