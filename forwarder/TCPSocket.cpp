#include"TCPSocket.h"

TCPSocket::TCPSocket(int family,int protocol)
    :Socket(SOCK_STREAM,family,protocol)
{

}

TCPSocket::~TCPSocket(){

}

void TCPSocket::connect(const SocketAddress &serverAddr){
    sockaddr_in sa=static_cast<sockaddr_in>(serverAddr);
    CHECK<SocketException>(0==::connect(sockfd_,(sockaddr *)&sa,sizeof(sa)),"fail to connect");
}

void TCPSocket::connect(unsigned short port,const std::string &addr){
    SocketAddress sockAddr(port,addr,family_);
    connect(sockAddr);
}

void TCPSocket::listen(int maxConnNum){
    CHECK<SocketException>(-1!=::listen(sockfd_,maxConnNum),"fail to listen");
}

void TCPSocket::accept(TCPSocket &connectedSocket,SocketAddress &clientAddr){
    sockaddr_in sa;
    socklen_t addrlen;
    int connfd=::accept(sockfd_,(sockaddr *)&sa,&addrlen);
    CHECK<SocketException>(-1!=connfd,"fail to accept");
    connectedSocket=*this;
    connectedSocket.setSockfd(connfd);
    clientAddr=sa;
}

int TCPSocket::send(const void *buf,size_t len,int flags){
    int num=::send(sockfd_,buf,len,flags);
    CHECK<SocketException>(-1!=num,"fail to send");
    return num;
}

int TCPSocket::send(const SocketBuffer &sockBuf,int flags){
    int num=::send(sockfd_,static_cast<void *>(sockBuf),sockBuf.getSize(),flags);
    CHECK<SocketException>(-1!=num,"fail to send");
    return num;
}

int TCPSocket::receive(void *buf,size_t max_len,int flags){
    int num=::recv(sockfd_,buf,max_len,flags);
    CHECK<SocketException>(-1!=num,"fail to recv");
    return num;
}

void TCPSocket::receive(SocketBuffer &sockBuf,int flags){
    int num=::recv(sockfd_,static_cast<void *>(sockBuf),sockBuf.getMaxSize(),flags);
    sockBuf.setSize(num);
    CHECK<SocketException>(-1!=num,"fail to recv");
}

void TCPSocket::close(){
    CHECK<SocketException>(0==::close(sockfd_),"fail to close");
}
