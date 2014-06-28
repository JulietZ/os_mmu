#include "Utils.h"

class ReplacementAlgorithm {
  public:
    ReplacementAlgorithm(){
    }

    virtual int selectPhysicalFrame() = 0;
    virtual void operateVirPage(int virPage) = 0;
    virtual void loadVirPage(int virPage) = 0;
    virtual void replaceVirPage(int vitPage) = 0;
};

class NRU : public ReplacementAlgorithm {
  private:
    int * NotRecentUsedVirPage;
    int length;      //length of NotRecentUsedVirPage
    int replaceCount;

  public:
    NRU(){
      NotRecentUsedVirPage = new int[PageTableSize];
      length=0;
      replaceCount=1;
    }

    void operateVirPage(int virPage){}
    void loadVirPage(int virPage){}
    void replaceVirPage(int virPage){
      replaceCount++;
    }

    int selectPhysicalFrame(){
      getPTEsWithLowestNumber();
      clearReferenced();
//      std::cout << "Got You" <<instCount << std::endl; 
      int rand=getRandomNumber(length);
//      std::cout << "Got You Again" <<instCount << std::endl; 
      int virPage=NotRecentUsedVirPage[rand];
      return PageTable[virPage].frameIndex;
    }

    void getPTEsWithLowestNumber(){
      length=0;
      int minValue=INT_MAX;
      //scan physical frames 
      int i=0;
      for (;i<PageTableSize;i++){
        if (PageTable[i].present==0){
          continue;
        }

        if (minValue>countValue(i)){
          minValue=countValue(i);
          length=0;
          NotRecentUsedVirPage[length++]=i;
        }else if (minValue==countValue(i)){
          NotRecentUsedVirPage[length++]=i;
        }
      }
    }

    /*
     * cout referenced modified
     */
    int countValue(int index){
      int referenceFlag = PageTable[index].referenced;
      int modifiedFlag = PageTable[index].modified;
      return 2*referenceFlag+modifiedFlag;
    }

    /*
     * clear reference bit
     */
    void clearReferenced(){
      //clear reference if necessary 
      int i=0;
      if (replaceCount%10 == 0){
        for (;i<PageTableSize;i++){
          PageTable[i].referenced=0;
        }
      }
    }
};

class LRU : public ReplacementAlgorithm {
  private:
    std::list<int> framelist;
  
  public:
    LRU(){}

    void operateVirPage(int virPage){
      int frameIndex = PageTable[virPage].frameIndex;
      for (std::list<int>::iterator it=framelist.begin(); it!=framelist.end(); ++it){
        if ((*it)==frameIndex) {
          framelist.erase(it);
          break;
        }
      }
      framelist.push_front(frameIndex);
    }
    
    void replaceVirPage(int virPage){
      framelist.pop_back();
      framelist.push_front(PageTable[virPage].frameIndex);
    }

    void loadVirPage(int virPage){
      framelist.push_front(PageTable[virPage].frameIndex);
    }

    int selectPhysicalFrame(){
      int i=framelist.back();
      return i;
    }
};

class FIFO : public ReplacementAlgorithm {
  private:
    int replaceCount;
  
  public:
    FIFO(){
      replaceCount=0;
    }

    void operateVirPage(int virPage){}
    void replaceVirPage(int virPage){}
    void loadVirPage(int virPage){}

    int selectPhysicalFrame(){
      return replaceCount++ % FrameTableSize;
    }
};

class SC : public ReplacementAlgorithm {
  private:
    int replaceCount;
  
  public:
    SC(){
      replaceCount=0;
    }

    void operateVirPage(int virPage){}
    void replaceVirPage(int virPage){}
    void loadVirPage(int virPage){}

    int selectPhysicalFrame(){
      int selected=replaceCount++ % FrameTableSize;
      while (PageTable[FrameTable[selected]].referenced==1){
        PageTable[FrameTable[selected]].referenced=0;
        selected=replaceCount++ % FrameTableSize;
      }
      return selected;
    }
};

class Random : public ReplacementAlgorithm {

  public:
    void operateVirPage(int virPage){}
    void loadVirPage(int virPage){}
    void replaceVirPage(int virPage){}

    int selectPhysicalFrame(){
      return getRandomNumber(FrameTableSize);
    }
};

class ClockF : public ReplacementAlgorithm {
  private:
    int ptr;
  
  public:
    ClockF(){
        ptr=0;
    }

    void operateVirPage(int virPage){}
    void replaceVirPage(int virPage){}

    void loadVirPage(int virPage){}

    int selectPhysicalFrame(){
      int selected=ptr++ % FrameTableSize;
      while (PageTable[FrameTable[selected]].referenced==1){
        PageTable[FrameTable[selected]].referenced=0;
        selected=ptr++ % FrameTableSize;
      }
      return selected;
    }
};

class ClockV : public ReplacementAlgorithm {
  private:
    int ptr;
  
  public:
    ClockV(){
      ptr=0;
    }

    void operateVirPage(int virPage){}
    void replaceVirPage(int virPage){}
    void loadVirPage(int virPage){}

    int selectPhysicalFrame(){
      int selected=ptr++ % PageTableSize;
      while (PageTable[selected].present==0 || PageTable[selected].referenced==1){
        PageTable[selected].referenced=0;
        selected=ptr++ % PageTableSize;
      }
      return PageTable[selected].frameIndex;
    }
};

class AgingF : public ReplacementAlgorithm {
  private:
    unsigned int * ages;
  
  public:
    AgingF(){
      ages = new unsigned int[FrameTableSize];
      for (int i=0; i<FrameTableSize; i++) {
        ages[i]=0;
      }
    }

    void operateVirPage(int virPage){}
    void replaceVirPage(int virPage){}
    void loadVirPage(int virPage){}

    int selectPhysicalFrame(){
      for (int i=0; i<FrameTableSize; i++) {
        ages[i]=ages[i]>>1;
        if (PageTable[FrameTable[i]].referenced){
          ages[i]=ages[i]|0x80000000;
          PageTable[FrameTable[i]].referenced=0;
        }
      }
      int tempframe=0;
      for (int i=1; i<FrameTableSize; i++) {
        if (ages[i]<ages[tempframe]) {
          tempframe=i;
        }
      }
        ages[tempframe]=0;
      return tempframe;
    }
};

class AgingV : public ReplacementAlgorithm {
  private:
    unsigned int * ages;
  
  public:
    AgingV(){
      ages = new unsigned int[PageTableSize];
      for (int i=0; i<PageTableSize; i++) {
        ages[i]=0;
      }
    }

    void operateVirPage(int virPage){}
    void replaceVirPage(int virPage){}
    void loadVirPage(int virPage){}

    int selectPhysicalFrame(){
      for (int i=0; i<PageTableSize; i++) {
        ages[i]=ages[i]>>1;
        if (PageTable[i].referenced){
          ages[i]=ages[i]|0x80000000;
          PageTable[i].referenced=0;
        }
      }
      int tempPage=0;
      while (!PageTable[tempPage].present){
        tempPage++;
      }
      for (int i=1; i<PageTableSize; i++) {
        if (PageTable[i].present==1 && ages[i]<ages[tempPage]) {
          tempPage=i;
        }
      }
        ages[tempPage]=0;
      return PageTable[tempPage].frameIndex;
    }    
};
