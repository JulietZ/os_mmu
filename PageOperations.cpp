#include "Utils.h"
using namespace std;

void operateVirPage(int readOrWrite, int virPage){
  /*
   * Workflow of operate virPage in memory
   */
  PageTable[virPage].referenced=1;    
  if (readOrWrite == WRITE){
    PageTable[virPage].modified=1;    
  }
}

void removeVirPage(int physicalFrame){
  /*
   * workflow of removing page from current physicalFrame
   *  UNMAP the currentVirPage
   *  OUT the currentVirPage if modified
   */
  int currentVirPage=FrameTable[physicalFrame];
  //set page table
  PageTable[currentVirPage].frameIndex=0xfffffff;
  PageTable[currentVirPage].present=0;
  PageTable[currentVirPage].referenced=0;    

  if (OFLAG){
    printOperation("UNMAP",currentVirPage,physicalFrame); 
  }

  //collecting statistics
  stat.unmaps++;

  //if modified, pageOut
  if(PageTable[currentVirPage].modified==1){
    PageTable[currentVirPage].modified=0;    
    PageTable[currentVirPage].pagedOut=1;

    if (OFLAG){
      printOperation("OUT",currentVirPage,physicalFrame); 
    }

    //collecting statistics
    stat.outs++;
  }

  //update FrameTable
  FrameTable[physicalFrame]=-1;
}

void loadVirPage(int readOrWrite, int newVirPage, int physicalFrame){
  /*
   * workflow of loading new page to physicalFrame
   *  IN if new page has been pageOut
   *  ZERO if new page has not been pageOut
   *  MAP the new page
   */
  //if pageOut, pageIn
  if(PageTable[newVirPage].pagedOut==1){

    if (OFLAG){
      printOperation("IN",newVirPage,physicalFrame); 
    }

    //collecting statistics
    stat.ins++;
  }
  //if never pageOut, ZERO it
  else{

    if (OFLAG){
      printOperation("ZERO",newVirPage,physicalFrame); 
    }

    //collecting statistics
    stat.zeros++;
  }

  if (OFLAG){
    printOperation("MAP",newVirPage,physicalFrame); 
  }

  //collecting statistics
  stat.maps++;

  //set PageTable value
  PageTable[newVirPage].frameIndex=physicalFrame;
  PageTable[newVirPage].present=1;
  PageTable[newVirPage].referenced=1;    
  //set modified flag
  if (readOrWrite == WRITE){
    PageTable[newVirPage].modified=1;    
  }

  //update FrameTable
  FrameTable[physicalFrame]=newVirPage;
}
