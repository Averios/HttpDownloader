#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

int hostname_to_ip(char*, char*);

int main(int argc, char** argv){
    char buffer[1025];
    int portNum, sockfd = 0, ok;
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
    printf("%s\n",domain);
    printf("%s\n",destination);

    return 0;
}

int hostname_to_ip(char *domain, char* ip){
    
}
