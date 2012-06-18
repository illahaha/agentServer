#include "CStartProcess.h"

CStartProcess::CStartProcess()
{

}

void CStartProcess::startAllProcess()
{
  readProcessInfo();
  for(int i=0;i<processList.size();i++)
  {
    Process process = processList[i];
    pid_t pid = fork();
    switch(pid)
    {
    case -1:
      cout<<"fork_err"<<endl;
      break;
    case 0:
      cout<<process.name<<endl;
      execl(process.path,process.name,NULL);
      cout<<"execl..err"<<endl;
      break;
    default:
      cout<<"pid="<<pid<<endl;
      process.pid = pid;
      break;
    }
  }
}

void CStartProcess::stopAllProcess()
{
  
}

int CStartProcess::readProcessInfo()
{
  int config_fd;
  char buf[10000];

  config_fd=open("processList.conf",O_RDONLY);
  if(config_fd<0)
  {
    return -1;
  }
  
  int size = read(config_fd,buf,sizeof(buf));
  bool bName = true;
  char name[100];
  char path[100];
  int nameIndex=0;
  int pathIndex=0;

  for(int i=0;i<size;i++)
  {
    if(buf[i]=='=')
    {
      bName = false;
    }else if(buf[i]=='\n'||i==size-1){
      if(i==size-1&&buf[i]!='\n')
      {
	path[pathIndex] = buf[i];
	path[pathIndex+1] = '\0';
      }else{
	path[pathIndex]='\0';
      }
      name[nameIndex]='\0';
     
      Process process;
      setProcessName(name,&process);
      setProcessValue(path,&process);
      processList.push_back(process);
      
      nameIndex=0;
      pathIndex=0;
    }else if(bName){
      name[nameIndex] = buf[i];
      nameIndex++;
    }else{
      path[pathIndex] = buf[i];
      pathIndex++;
    }
  }
  return 0;
}

void CStartProcess::setProcessName(char* pName,Process* pProcess)
{
  for(int i=0;i<100;i++)
  {
    char chName = *(pName+i);
    (*pProcess).name[i]=chName;
    if(chName=='\0')
    {
      break;
    }
  }
}

void CStartProcess::setProcessValue(char* pValue,Process* pProcess)
{
  for(int i=0;i<100;i++)
  {
    char chValue = *(pValue+i);
    (*pProcess).path[i]=chValue;
    if(chValue=='\0')
    {
      break;
    }
  }
}
int main()
{
  CStartProcess startProcess;
  startProcess.startAllProcess();
  return 0;
}
