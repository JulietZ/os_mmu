#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <climits>
#include <string>
#include <cstring>
#include <iomanip>

#define READ 0
#define WRITE 1

struct PTE {
  unsigned int present:1;
  unsigned int modified:1;
  unsigned int referenced:1;
  unsigned int pagedOut:1;
  unsigned int frameIndex:28;
};

struct STAT {
  int unmaps;
  int maps;
  int ins;
  int outs;
  int zeros;
};

extern struct STAT stat;
extern struct PTE * PageTable;
extern int PageTableSize;
extern int * FrameTable;
extern int FrameTableSize;
extern bool OFLAG;
extern bool PFLAG;
extern bool FFLAG;
extern bool SFLAG;

extern int instCount;
extern unsigned long long totalCost;
extern int PageUedCount;

extern std::vector<int> randVals;
extern int randOfs;
extern int randCount;

void initPageTable();
void initFrameTable(int FrameTableSize);
void initStat();
void initAlgo();
void initRandVals(char* rfileName);

void operateVirPage(int readOrWrite, int physicalFrame);
void removeVirPage(int physicalFrame);
void loadVirPage(int readOrWrite, int newVirPage, int physicalFrame);

void printOperation(std::string operation, int virtualPage, int physicalFrame);

int getRandomNumber(int range);
void printPageTable();
void printFrameTable();
void printSummary();
void initAlgoAndOptionMap();
