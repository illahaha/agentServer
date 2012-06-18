#include <iostream>
#include <vector>
#include <fcntl.h>
using namespace std;

#define STRING_SIZE 100;
#define FILE_BUFF_SIZE 10000;

typedef struct tagProcess
{
  char name[100];
  char path[100];
}Process;

class CStartProcess
{
 public:
  CStartProcess();
 public:
  void startAllProcess();
  void stopAllProcess();
 private:
  int readProcessInfo();
  void setProcessName(char* name,Process* process);
  void setProcessValue(char* value,Process* process);
 private:
  vector<Process> processList;
};
