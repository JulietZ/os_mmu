#include "ReplacementAlgorithm.h"
using namespace std;

void initPageTable(){
  int i=0;
  for (;i<PageTableSize;i++){
    PageTable[i].present=0;
    PageTable[i].modified=0;
    PageTable[i].referenced=0;
    PageTable[i].pagedOut=0;
    PageTable[i].frameIndex=0xfffffff;
  }
}

void initFrameTable(int FrameTableSize){
  FrameTable = new int[FrameTableSize];
  int i=0;
  for (;i<FrameTableSize;i++){
    FrameTable[i]=-1;
  }
}

void initStat(){
  stat.unmaps=0;
  stat.maps=0;
  stat.ins=0;
  stat.outs=0;
  stat.zeros=0;
}

void initRandVals(char * rfileName){
    //read the random value from random file
    ifstream rfile(rfileName);
    if (!rfile.is_open()){
        cout << "Error Opening \"rfile\", Please make sure correct input file name typed" << endl;
        exit(0);
    }
    int random;
    rfile >>randCount;
    while (!rfile.eof()){
      rfile >> random;
      randVals.push_back(random);
    }
}

int getRandomNumber(int range){
  if (randOfs == randCount){
    randOfs=0;
  }
  return (randVals[randOfs++] % range);
}

void printOperation(string operation, int virtualPage, int physicalFrame){
  if (operation == "ZERO"){
    cout << instCount-1 << ": " << left << setw(5) << operation << right << setw(8) << physicalFrame << endl;
  }else{
    cout << instCount-1 << ": " << left << setw(5) << operation << right << setw(4) << virtualPage << right << setw(4) <<physicalFrame << endl;
  }
}

void printPageTable(){
  int i=0;
  for (i;i<PageTableSize;i++){
    if (PageTable[i].present == 1){
      cout << i << ":";
      char RMS[3];
      RMS[0] = PageTable[i].referenced ? 'R' : '-';
      RMS[1] = PageTable[i].modified ? 'M' : '-';
      RMS[2] = PageTable[i].pagedOut ? 'S' : '-';
      cout << RMS[0] << RMS[1] << RMS[2];
    }else{
      if (PageTable[i].pagedOut==1){
        cout << "#";
      }else{
        cout << "*";
      }
    }
    cout << " ";
  }
  cout << endl;
}

void printFrameTable(){
  int i=0;
  for (;i<FrameTableSize;i++){
    if (FrameTable[i]==-1){
      cout << "*";
    }else{
      cout << FrameTable[i];
    }
    cout << " ";
  }
  cout << endl;
}

void printSummary(){
  totalCost+=instCount;
  totalCost+=(stat.unmaps+stat.maps)*400;
  totalCost+=(stat.ins+stat.outs)*3000;
  totalCost+=stat.zeros*150;
  printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",
    instCount, stat.unmaps, stat.maps, stat.ins, stat.outs, stat.zeros, totalCost);
}
