#include "ReplacementAlgorithm.h"
using namespace std;

bool OFLAG=false;
bool PFLAG=false;
bool FFLAG=false;
bool SFLAG=false;
struct STAT stat;
int PageTableSize=64;
struct PTE * PageTable = new PTE[PageTableSize];
int * FrameTable;
int FrameTableSize=32;
int PageUsedCount=0;
std::vector<int> randVals;
int randOfs=0;
int randCount=0;
int instCount=0;
unsigned long long totalCost=0;

int main(int argc, char* argv[]) {
  char* algo=NULL;
  char* outputOption=NULL;
  int c;

  while ((c = getopt (argc, argv, "a:f:o:")) != -1)
    switch (c){
      case 'a':
        algo = optarg;
        break;
      case 'f':
        FrameTableSize = atoi(optarg);
        break;
      case 'o':
        outputOption = optarg;
        break;
      /*case '?':
          if (optopt == 's')
              fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          else if (isprint (optopt))
              fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          else
              fprintf (stderr,
                       "Unknown option character `\\x%x'.\n",
                       optopt);
          return 1;*/
      default:
        abort ();
    }

  ReplacementAlgorithm *object;
  if (algo==NULL) object = new LRU();
  else if (strncmp(algo,"N",1)==0) object = new NRU();
  else if (strncmp(algo,"l",1)==0) object = new LRU();
  else if (strncmp(algo,"r",1)==0) object = new Random();
  else if (strncmp(algo,"f",1)==0) object = new FIFO();
  else if (strncmp(algo,"s",1)==0) object = new SC();
  else if (strncmp(algo,"c",1)==0) object = new ClockF();
  else if (strncmp(algo,"C",1)==0) object = new ClockV();
  else if (strncmp(algo,"a",1)==0) object = new AgingF();
  else if (strncmp(algo,"A",1)==0) object = new AgingV();

  //frame number
  initStat();
  initPageTable();
  initFrameTable(FrameTableSize);

  int index=0;
  while (outputOption[index]<='Z' && outputOption[index]>='A'){
    c = outputOption[index++];
    switch (c){
      case 'O':
        OFLAG=true;
        break;
      case 'P':
        PFLAG=true;
        break;
      case 'F':
        FFLAG=true;
        break;
      case 'S':
        SFLAG=true;
        break;
      default:
        cout << "Unknown Option" << endl;
        abort();
    }
  }

  ifstream infile(argv[optind]);
  if (!infile) {
    cout << "Fail to open infile" << endl;
  }
  initRandVals(argv[optind+1]);

  int readOrWrite;
  int virPage;
  string line;

  while (!infile.eof()) {
    getline(infile,line);
    if (line[0]=='#' || line.length()==0) {
      continue;
    }        

    //get readOrWrite and virPage
    if (OFLAG){
      cout << "==> inst: " << line << endl;
    }
    istringstream iss(line);
    iss>>readOrWrite>>virPage;

    instCount ++;
    //check if the virPage in memory
    if (PageTable[virPage].present==1){
      operateVirPage(readOrWrite, virPage);
      object->operateVirPage(virPage);
      continue;
    }

    //load new page
    if (PageUsedCount<FrameTableSize){
      loadVirPage(readOrWrite,virPage,PageUsedCount++);
      object->loadVirPage(virPage);
    //replace page
    }else{
      int physicalFrame = object->selectPhysicalFrame();
      removeVirPage(physicalFrame);
      loadVirPage(readOrWrite,virPage,physicalFrame);
      object->replaceVirPage(virPage);
    }
  }

  if (PFLAG){
    //print pageTable
    printPageTable();
  }

  if (FFLAG){
    //print frameTable
    printFrameTable();
  }

  if (SFLAG){
    //print Summaries
    printSummary();
  }
}
