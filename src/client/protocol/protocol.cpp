#include "protocol.hpp"

Client::~Client(){
    if(this->sockfd >= 0){
        close(this->sockfd);
    }
}