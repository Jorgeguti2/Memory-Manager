#include "MemoryManager.h"

#include <iomanip>
#include <iostream>
using namespace std;

namespace MemoryManager
{
	// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT 
	//
	// This is the only static memory that you may use, no other global variables may be
	// created, if you need to save data make it fit in MM_pool
	//
	// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT


	const int MM_POOL_SIZE = 65536;
	char MM_pool[MM_POOL_SIZE];

	// I have provided this tool for your use
	void memView(int start, int end)
	{

		const unsigned int SHIFT = 8;
		const unsigned int MASK = 1 << (SHIFT - 1);

		unsigned int value;	// used to facilitate bit shifting and masking

		cout << endl; // start on a new line
		cout << "               Pool                     Unsignd  Unsigned " << endl;
		cout << "Mem Add        indx   bits   chr ASCII#  short      int    " << endl;
		cout << "-------------- ---- -------- --- ------ ------- ------------" << endl;

		for (int i = start; i <= end; i++)
		{
			cout << (long*)(MM_pool + i) << ':';	// the actual address in hexadecimal
			cout << '[' << setw(2) << i << ']';				// the index into MM_pool

			value = MM_pool[i];
			cout << " ";
			for (int j = 1; j <= SHIFT; j++)		// the bit sequence for this byte (8 bits)
			{
				cout << ((value & MASK) ? '1' : '0');
				value <<= 1;
			}
			cout << " ";

            if (MM_pool[i] < 32)   // non-printable character so print a blank
            	cout << '|' << ' ' << "| (";	
            else                    // characger is printable so print it
				cout << '|' << *(char*)(MM_pool + i) << "| (";		// the ASCII character of the 8 bits (1 byte)

			cout << setw(4) << ((int)(*((unsigned char*)(MM_pool + i)))) << ")";	// the ASCII number of the character

			cout << " (" << setw(5) << (*(unsigned short*)(MM_pool + i)) << ")";	// the unsigned short value of 16 bits (2 bytes)
			cout << " (" << setw(10) << (*(unsigned int*)(MM_pool + i)) << ")";	// the unsigned int value of 32 bits (4 bytes)

																				//  cout << (*(unsigned int*)(MM_pool+i)) << "|";	// the unsigned int value of 32 bits (4 bytes)

			cout << endl;
		}
	}

	// Initialize set up any data needed to manage the memory pool
	void initializeMemoryManager(void)
	{

		int freeHead = 0; // starting index of the freelist
		int inUseHead = 2; // starting index of the inUselist
		int usedHead = 4; // starting index for the used list - deallocated memory

		int nextLink = 2; // offset index of the next link
		int prevLink = 4; // offset index for the prev link

		*(unsigned short*)(MM_pool + freeHead) = 6; // freelist starts at byte 6
		*(unsigned short*)(MM_pool + inUseHead) = 0;	// nothing in the inUse list yet
		*(unsigned short*)(MM_pool + usedHead) = 0; // nothing in the used list yet

	}

	// return a pointer inside the memory pool
	void* allocate(int aSize)
	{

		// If no chunk can accommodate aSize call onOutOfMemory()
        	if ((int)(*(unsigned short*)(MM_pool)) + aSize + 6 > 65536)
            		onOutOfMemory();
		// Initialize variables to keep track of inuseHead as the old variable
		// and freeMemory as the new variable coming in. Update freeHead, inuseHead, and allocate
		// the size of the variable at the index of freeHead
		int old = *(unsigned short*)&MM_pool[2];
        	*(unsigned short*)(MM_pool + 2) = *(unsigned short*)&MM_pool[0];
		int new_ = *(unsigned short*)&MM_pool[0];
        	*(unsigned short*)(MM_pool + 0) = new_ + (aSize + 6);
        	*(unsigned short*)(MM_pool + new_) = aSize;
        	
		// Test if there is a next node (an old one that already existed).
		// If there is, update the next of current node and prev of old node
		if(old != 0){
            		*(unsigned short*)(MM_pool + new_ + 2) = old;
            		*(unsigned short*)(MM_pool + old + 4) = new_;
        	}
        	return &MM_pool[new_ + 6];

	}

	// Free up a chunk previously allocated
	void deallocate(void* aPointer)
        {
		// Initialize variables for our current node
		int cur = (char*)aPointer -MM_pool -6;
		int next_Cur = *(unsigned short*)(MM_pool + cur + 2);
		int prev_Cur = *(unsigned short*)(MM_pool + cur + 4);
		//cout << endl;
		//cout << "CUR: " << cur << endl;
		//cout << "NEXT_CUR: " << next_Cur << endl;
		//cout << "PREV_CUR: " << prev_Cur << endl;

		// Test if there is a next node
		if(next_Cur != 0){
			*(unsigned short*)(MM_pool + next_Cur + 4) = *(unsigned short*)(MM_pool + cur + 4);
		}

		// Test if there is a previous node
		if(prev_Cur == 0){
			*(unsigned short*)(MM_pool + 2) = *(unsigned short*)(MM_pool + cur + 2);
			*(unsigned short*)(MM_pool + cur + 2 + 4) = 0;
		}else{
			*(unsigned short*)(MM_pool + prev_Cur + 2) = *(unsigned short*)(MM_pool + cur + 2);
		}

		// intialize usedHead as a variable to test if it is equal to 0
		// to update the current node appropriately
		int usedHead = *(unsigned short*)(MM_pool + 4);

		if(usedHead != 0){
			*(unsigned short*)(MM_pool + cur + 2) = usedHead;
			*(unsigned short*)(MM_pool + cur + 4) = 0;
			*(unsigned short*)(MM_pool + usedHead + 4) = cur;
		}else{
			*(unsigned short*)(MM_pool + cur + 2) = 0;
			*(unsigned short*)(MM_pool + cur + 4) = 0;
		}

		// Update usedHead
		*(unsigned short*)(MM_pool + 4) = cur;

        }

	int size(void *ptr)
	{
		// Get index for variable location
		int index = (char*)ptr - MM_pool - 6;
		// Get size of variable
		int variable = *(unsigned short*)(MM_pool + index);
		return variable;
	}
	
	//---
	//--- support routines
	//--- 

	// Will scan the memory pool and return the total free space remaining
	int freeMemory(void)
	{
		return (65536 - *(unsigned short*)(MM_pool + 0));
	}


	// Will scan the memory pool and return the total used memory - memory that has been deallocated
	int usedMemory(void)
	{
		int total = 0;
		int cur = *(unsigned short*)&MM_pool[4];
		int size = *(unsigned short*)&MM_pool[cur];
		int next = *(unsigned short*)&MM_pool[cur+2];
		int prev = *(unsigned short*)&MM_pool[cur+4];
		//cout << "\nTraversing InUse Memory....";
		//cout << "\n      InUse Head:"<<cur;
		while (cur != 0)
		{
			total += (size + 6);
			//cout << "\n        Index:"<<cur<<" Size:"<<size<<" Next:"<<next<<" Prev:"<<prev;
			cur = next;
			size = *(unsigned short*)&MM_pool[cur];
			next = *(unsigned short*)&MM_pool[cur+2];
			prev = *(unsigned short*)&MM_pool[cur+4];
		}
		return total;
	}


	// Will scan the memory pool and return the total in use memory - memory being curretnly used
	int inUseMemory(void)
	{
		int total = 0;
		int cur = *(unsigned short*)&MM_pool[2];
		int size = *(unsigned short*)&MM_pool[cur];
		int next = *(unsigned short*)&MM_pool[cur+2];
		int prev = *(unsigned short*)&MM_pool[cur+4];
		//cout << "\nTraversing InUse Memory....";
		//cout << "\n      InUse Head:"<<cur;
		while (cur != 0)
		{
			total += (size + 6);
			//cout << "\n        Index:"<<cur<<" Size:"<<size<<" Next:"<<next<<" Prev:"<<prev;
			cur = next;
			size = *(unsigned short*)&MM_pool[cur];
			next = *(unsigned short*)&MM_pool[cur+2];
			prev = *(unsigned short*)&MM_pool[cur+4];
		}
		return total;
	}	

	// helper function to see teh InUse list in memory
	void traverseInUse()
	{
		int cur = *(unsigned short*)&MM_pool[2];
		int size = *(unsigned short*)&MM_pool[cur];
		int next = *(unsigned short*)&MM_pool[cur+2];
		int prev = *(unsigned short*)&MM_pool[cur+4];
		cout << "\nTraversing InUse Memory....";
		cout << "\n      InUse Head:"<<cur;
		while (cur != 0)
		{
			cout << "\n        Index:"<<cur<<" Size:"<<size<<" Next:"<<next<<" Prev:"<<prev;
			cur = next;
			size = *(unsigned short*)&MM_pool[cur];
			next = *(unsigned short*)&MM_pool[cur+2];
			prev = *(unsigned short*)&MM_pool[cur+4];
		}
	}

	//Helper function to seet eh Used list in memory
	void traverseUsed()
	{
		int cur = *(unsigned short*)&MM_pool[4];
		int size = *(unsigned short*)&MM_pool[cur];
		int next = *(unsigned short*)&MM_pool[cur+2];
		int prev = *(unsigned short*)&MM_pool[cur+4];
		cout << "\nTraversing Used Memory....";
		cout << "\n      Used Head:"<<cur;
		while (cur != 0)
		{
			cout << "\n        Index:"<<cur<<" Size:"<<size<<" Next:"<<next<<" Prev:"<<prev;
			cur = next;
			size = *(unsigned short*)&MM_pool[cur];
			next = *(unsigned short*)&MM_pool[cur+2];
			prev = *(unsigned short*)&MM_pool[cur+4];
		}
		
	}


	// this is called from Allocate if there is no more memory availalbe
	void onOutOfMemory(void)
  	{
    	std::cout << "\nMemory pool out of memory\n" << std::endl;
    	std::cout << "\n---Press \"Enter\" key to end program---:";

		cin.get();

    	exit( 1 );
  }
}
