/*  This is the driver for the memory manager. Do not make any changes to this file
    Compile it with the MemeryManager.h and MemoryManager.cpp files.
    The output that I got is at the bottom.
    Notice that Total Memory = Free Memory + InUse Memory + Used Memory
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <climits>

#include "MemoryManager.h"

using namespace std;
using namespace MemoryManager;  // allow Memory Manager functions to be used without prefix

void memStats();  //helper function

int main(void)
{

    initializeMemoryManager();
    cout << "\nmemView immediately after initializeMemoryManager:\n";
    memView(0,10);

    cout<<"Program Starting...";
    memStats();

    cout<<"\n....allocate some memory:";
    
    short* shortPtr = (short*) allocate(sizeof(short));
    char* charPtr = (char*)allocate(sizeof(char));
    int* intPtr = (int*)allocate(sizeof(int)*3);
    char* lname = (char*)allocate(6);
    char *fname = (char*)allocate(6);
    int * maxIntPtr = (int*) allocate(sizeof(int));

    memStats();
    
    cout << "\n....assign some values:";

    *maxIntPtr = INT_MAX;  // largest positive 4 byte integer
    strcpy(fname, "Steve"); 
    strcpy(lname, "Gold");
    *shortPtr = SHRT_MAX;  // largest positive 2 byte integer
    *charPtr = 'Z';
    *intPtr = 1000000;

	cout << "\n\nProper traversalInUse() worth 10 points:";
    traverseInUse();

    deallocate(fname);
    deallocate(lname);
    deallocate (shortPtr);
    
    cout << "\n....deallocated 3 variables";

	cout << "\n\nProper traversalInUse() and traversalUsed() worth 10 points:";
    traverseInUse();
    traverseUsed();

    memView(0,75);

    memStats();

    cout << "\nFree Memory Avaialable before *DEATH:"<<(unsigned short)freeMemory()<<endl;
    char *death = (char*)allocate(freeMemory()-10);

    cout << "\nFree Memory Avaialable after *DEATH:"<<(unsigned short)freeMemory()<<endl;

    cout << "Size of lname: "<<size(lname)<<endl;
    cout << "Size of Death Star: "<<(unsigned short)size(death)<<endl;

    memStats();

    cout << "\n....forcing out-of-memory condition:";
    int *zPtr = (int *) allocate(4 * 5);  // try to allocate array of 5 integers
    
    cout << "\nYou will never see this line";

    return 0;
}

// this function gives you a summary of what memory looks like now.
void memStats()
{
    int total = freeMemory()+inUseMemory()+usedMemory();
    cout << "\n#####.....memStats.....################";
    cout << "\n#Total:"<<total<<" Free:"<<freeMemory()<<" InUse:"<<inUseMemory()<<" Used:"<<usedMemory();
    cout << "\n#Press return to continue...";
    cout << "\n#######################################";
    cin.get();
}

// output from this program:
/*


memView immediately after initializeMemoryManager:

               Pool                     Unsignd  Unsigned 
Mem Add        indx   bits   chr ASCII#  short      int    
-------------- ---- -------- --- ------ ------- ------------
0x1089fb100:[ 0] 00000110 | | (   6) (    6) (         6)
0x1089fb101:[ 1] 00000000 | | (   0) (    0) (         0)
0x1089fb102:[ 2] 00000000 | | (   0) (    0) (         0)
0x1089fb103:[ 3] 00000000 | | (   0) (    0) (         0)
0x1089fb104:[ 4] 00000000 | | (   0) (    0) (         0)
0x1089fb105:[ 5] 00000000 | | (   0) (    0) (         0)
0x1089fb106:[ 6] 00000000 | | (   0) (    0) (         0)
0x1089fb107:[ 7] 00000000 | | (   0) (    0) (         0)
0x1089fb108:[ 8] 00000000 | | (   0) (    0) (         0)
0x1089fb109:[ 9] 00000000 | | (   0) (    0) (         0)
0x1089fb10a:[10] 00000000 | | (   0) (    0) (         0)
Program Starting...
#####.....memStats.....################
#Total:65530 Free:65530 InUse:0 Used:0
#Press return to continue...
#######################################
....allocate some memory:
#####.....memStats.....################
#Total:65530 Free:65463 InUse:67 Used:0
#Press return to continue...
#######################################
....assign some values:

Proper traversalInUse() worth 10 points:
Traversing InUse Memory....
      InUse Head:63
        Index:63 Size:4 Next:51 Prev:0
        Index:51 Size:6 Next:39 Prev:63
        Index:39 Size:6 Next:21 Prev:51
        Index:21 Size:12 Next:14 Prev:39
        Index:14 Size:1 Next:6 Prev:21
        Index:6 Size:2 Next:0 Prev:14
....deallocated 3 variables

Proper traversalInUse() and traversalUsed() worth 10 points:
Traversing InUse Memory....
      InUse Head:63
        Index:63 Size:4 Next:21 Prev:0
        Index:21 Size:12 Next:14 Prev:63
        Index:14 Size:1 Next:0 Prev:21
Traversing Used Memory....
      Used Head:6
        Index:6 Size:2 Next:39 Prev:0
        Index:39 Size:6 Next:51 Prev:6
        Index:51 Size:6 Next:0 Prev:39
               Pool                     Unsignd  Unsigned 
Mem Add        indx   bits   chr ASCII#  short      int    
-------------- ---- -------- --- ------ ------- ------------
0x1089fb100:[ 0] 01001001 |I| (  73) (   73) (   4128841)
0x1089fb101:[ 1] 00000000 | | (   0) (16128) ( 100679424)
0x1089fb102:[ 2] 00111111 |?| (  63) (   63) (    393279)
0x1089fb103:[ 3] 00000000 | | (   0) ( 1536) (  33555968)
0x1089fb104:[ 4] 00000110 | | (   6) (    6) (    131078)
0x1089fb105:[ 5] 00000000 | | (   0) (  512) ( 654311936)
0x1089fb106:[ 6] 00000010 | | (   2) (    2) (   2555906)
0x1089fb107:[ 7] 00000000 | | (   0) ( 9984) (      9984)
0x1089fb108:[ 8] 00100111 |'| (  39) (   39) (        39)
0x1089fb109:[ 9] 00000000 | | (   0) (    0) (4278190080)
0x1089fb10a:[10] 00000000 | | (   0) (    0) (2147418112)
0x1089fb10b:[11] 00000000 | | (   0) (65280) (  25165568)
0x1089fb10c:[12] 11111111 | | ( 255) (32767) (     98303)
0x1089fb10d:[13] 01111111 || ( 127) (  383) (       383)
0x1089fb10e:[14] 00000001 | | (   1) (    1) (         1)
0x1089fb10f:[15] 00000000 | | (   0) (    0) ( 352321536)
0x1089fb110:[16] 00000000 | | (   0) (    0) (   1376256)
0x1089fb111:[17] 00000000 | | (   0) ( 5376) (1509954816)
0x1089fb112:[18] 00010101 | | (  21) (   21) ( 207224853)
0x1089fb113:[19] 00000000 | | (   0) (23040) (    809472)
0x1089fb114:[20] 01011010 |Z| (  90) ( 3162) ( 234884186)
0x1089fb115:[21] 00001100 | | (  12) (   12) (    917516)
0x1089fb116:[22] 00000000 | | (   0) ( 3584) (1056968192)
0x1089fb117:[23] 00001110 | | (  14) (   14) (   4128782)
0x1089fb118:[24] 00000000 | | (   0) (16128) (1073757952)
0x1089fb119:[25] 00111111 |?| (  63) (   63) (1111490623)
0x1089fb11a:[26] 00000000 | | (   0) (16384) ( 256000000)
0x1089fb11b:[27] 01000000 |@| (  64) (16960) (   1000000)
0x1089fb11c:[28] 01000010 |B| (  66) ( 3906) (      3906)
0x1089fb11d:[29] 00001111 | | (  15) (   15) (        15)
0x1089fb11e:[30] 00000000 | | (   0) (    0) (         0)
0x1089fb11f:[31] 00000000 | | (   0) (    0) (         0)
0x1089fb120:[32] 00000000 | | (   0) (    0) (         0)
0x1089fb121:[33] 00000000 | | (   0) (    0) (         0)
0x1089fb122:[34] 00000000 | | (   0) (    0) (         0)
0x1089fb123:[35] 00000000 | | (   0) (    0) (         0)
0x1089fb124:[36] 00000000 | | (   0) (    0) ( 100663296)
0x1089fb125:[37] 00000000 | | (   0) (    0) (    393216)
0x1089fb126:[38] 00000000 | | (   0) ( 1536) ( 855639552)
0x1089fb127:[39] 00000110 | | (   6) (    6) (   3342342)
0x1089fb128:[40] 00000000 | | (   0) (13056) ( 100676352)
0x1089fb129:[41] 00110011 |3| (  51) (   51) (    393267)
0x1089fb12a:[42] 00000000 | | (   0) ( 1536) (1191183872)
0x1089fb12b:[43] 00000110 | | (   6) (    6) (1866924038)
0x1089fb12c:[44] 00000000 | | (   0) (18176) (1819232000)
0x1089fb12d:[45] 01000111 |G| (  71) (28487) (1684827975)
0x1089fb12e:[46] 01101111 |o| ( 111) (27759) (   6581359)
0x1089fb12f:[47] 01101100 |l| ( 108) (25708) (     25708)
0x1089fb130:[48] 01100100 |d| ( 100) (  100) ( 100663396)
0x1089fb131:[49] 00000000 | | (   0) (    0) (    393216)
0x1089fb132:[50] 00000000 | | (   0) ( 1536) (      1536)
0x1089fb133:[51] 00000110 | | (   6) (    6) (         6)
0x1089fb134:[52] 00000000 | | (   0) (    0) ( 654311424)
0x1089fb135:[53] 00000000 | | (   0) (    0) (   2555904)
0x1089fb136:[54] 00000000 | | (   0) ( 9984) (1392518912)
0x1089fb137:[55] 00100111 |'| (  39) (   39) (1951596583)
0x1089fb138:[56] 00000000 | | (   0) (21248) (1702122240)
0x1089fb139:[57] 01010011 |S| (  83) (29779) (1986360403)
0x1089fb13a:[58] 01110100 |t| ( 116) (25972) (1702258036)
0x1089fb13b:[59] 01100101 |e| ( 101) (30309) (   6649445)
0x1089fb13c:[60] 01110110 |v| ( 118) (25974) (  67134838)
0x1089fb13d:[61] 01100101 |e| ( 101) (  101) (    262245)
0x1089fb13e:[62] 00000000 | | (   0) ( 1024) ( 352322560)
0x1089fb13f:[63] 00000100 | | (   4) (    4) (   1376260)
0x1089fb140:[64] 00000000 | | (   0) ( 5376) (      5376)
0x1089fb141:[65] 00010101 | | (  21) (   21) (        21)
0x1089fb142:[66] 00000000 | | (   0) (    0) (4278190080)
0x1089fb143:[67] 00000000 | | (   0) (    0) (4294901760)
0x1089fb144:[68] 00000000 | | (   0) (65280) (4294967040)
0x1089fb145:[69] 11111111 | | ( 255) (65535) (2147483647)
0x1089fb146:[70] 11111111 | | ( 255) (65535) (   8388607)
0x1089fb147:[71] 11111111 | | ( 255) (32767) (     32767)
0x1089fb148:[72] 01111111 || ( 127) (  127) (       127)
0x1089fb149:[73] 00000000 | | (   0) (    0) (         0)
0x1089fb14a:[74] 00000000 | | (   0) (    0) (         0)
0x1089fb14b:[75] 00000000 | | (   0) (    0) (         0)

#####.....memStats.....################
#Total:65530 Free:65463 InUse:35 Used:32
#Press return to continue...
#######################################
Free Memory Avaialable before *DEATH:65463

Free Memory Avaialable after *DEATH:4
Size of lname: 6
Size of Death Star: 65453

#####.....memStats.....################
#Total:65530 Free:4 InUse:65494 Used:32
#Press return to continue...
#######################################
....forcing out-of-memory condition:
Memory pool out of memory


---Press "Enter" key to end program---:


*/


