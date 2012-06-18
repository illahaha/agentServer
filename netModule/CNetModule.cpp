#include "CNetModule.h"

CNetModule::CNetModule()
{
  
}

void CNetModule::startNetService()
{
  int listenfd, socketfd,epfd,curfd,nfds;
  unsigned int lisnum,myport;
  socklen_t len;
  char buff[1024];
  struct sockaddr_in my_addr,their_addr;
  
  struct epoll_event ev;
  struct epoll_event events[1000];

  //  listenfd = socket(AF_INET,SOCKET_STREAM,0);
 
  //  char *local_addr ="127.0.0.1";
  //  inet_aton(local_addr,&(serveraddr.sin_addr));
  
  if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1){
    cout<<"socket"<<endl;
    return;
  }else{
    cout<<"creat socket sucess!"<<endl;
  }
  
  // setnonblocking(listenfd);
  
  myport = 12002;

  //bzero(&my_addr,sizeof(my_addr));
  memset(&my_addr,0,sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(myport);
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  if(bind(listenfd,(struct sockaddr*)&my_addr,sizeof(my_addr))==-1){
    cout<<"bind socket err"<<endl;
    cout<<"errno"<<strerror(errno)<<endl;
  }else{
    cout<<"bind sucess!"<<endl;
  }

  if(listen(listenfd,lisnum)==-1){
    cout<<"listen err"<<endl;
  }else{
    cout<<"listen sucess"<<endl;
  }

  epfd = epoll_create(10000);
  len = sizeof(struct sockaddr);
  ev.events = EPOLLIN||EPOLLET;
  ev.data.fd = listenfd;
  if(epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev)<0){
    cout<<"epill_ctl err"<<endl;
  }else{
    cout<<"socket..create epoll sucess"<<endl;
  }
  curfd = 1;
  while(1){
    nfds=epoll_wait(epfd,events,curfd,-1);
    if(nfds == -1)
    {
      cout<<"epoll_wairt err"<<endl;
      break;
    }
    
    int new_fd;
    for(int n=0;n<nfds;n++){
      if(events[n].data.fd==listenfd){
	cout<<"listen!"<<endl;
	new_fd = accept(listenfd,(struct sockaddr*)&their_addr,&len);
	if(new_fd<0){
	  cout<<"new_fd accept err"<<endl;
	  continue;
	}else{
	  cout<<"new connect"<<endl;
	}
	
	setnonblocking(new_fd);
	ev.events = EPOLLIN|EPOLLET;
	ev.data.fd = new_fd;
	if(epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&ev)<0){
	  cout<<"epoll add err"<<endl;
	  continue;
	}
	curfd++;
      }else{
	int ret =handle_message(new_fd);
	if(ret<1&&errno!=11){
	  epoll_ctl(epfd,EPOLL_CTL_DEL,events[n].data.fd,&ev);
	  curfd--;
	}
      }
    }
  }
  close(listenfd);
  return;
}

int CNetModule::handle_message(int new_fd)
{
  char buf[1024+1];
  int len;
  bzero(buf,1024+1);
  len = recv(new_fd,buf,1024,0);
  if(len<0){
    cout<<"1111"<<endl;
  }else {
    printf("receive message.len=%d",len);
  }
  return 0;
}

int CNetModule::setnonblocking(int sockfd)
{
  if(fcntl(sockfd,F_SETFL,fcntl(sockfd,F_GETFD,0)|O_NONBLOCK)==-1){
    return -1;
  }
  return 0;
}

int main()
{
  CNetModule netModule;
  netModule.startNetService();
  return 0;
}
