#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    struct direct_cache {
        unsigned short addr_up; // 16-bit tag address.
        bool valid_flag; //  Prevents copy, flag is set if data valid
        bool lru_flag; // Least Recently Used eviction policy,. Not used in direct mapped.
        char data[4]; // 32-bit data bus.
    };

    unsigned short byte_no;
    unsigned short set_no;

    unsigned short cpu_upper; // Upper half of address.
    unsigned short cpu_lower; // Upper half of address.

    unsigned int hits_no = 0; // Counts number of hits.
    unsigned int miss_no = 0; // Counts number of misses.
    
    unsigned int cpu_addr;

    ifstream my_addr; // Address text file to be loaded.
    direct_cache x[16384]; // Creates 2^14 cache entries.
    string addr_line; // Used to store input address line.

    for (int i = 0; i <= 16384; i++) {
        x[i].valid_flag = 0; // Valid flag is set to zero.
        //x[i].lru_flag = 1;	// LRU initialised, not used in the direct mapped
    }

    my_addr.open("Addresses.txt"); // Opens input file that holds addresses to be tested.
    while (!my_addr.eof()) { // Hold the file open until the end.
        //stringstream lineIn(addr_line);		// 1st line of address file
        my_addr >> hex >> cpu_addr; // The line is initially treated as hex.
        cpu_lower = cpu_addr & 0x0000ffff; // CPU is anded with ffff.
        cpu_upper = cpu_addr >> 16; // First half is anded w/ a set of ffff, then it's bit shifted by 16.
        set_no = (cpu_lower & 0x0000ffff) >> 2;
        byte_no = cpu_lower & 3; // Lower half is anded with three.

        // The following if statement validates a hit.
        if (x[set_no].addr_up == cpu_upper && x[set_no].valid_flag) { 
            cout << "Hit:    " << hex << cpu_addr << endl; //Printing address if a hit.
            hits_no++; //Add 1 to number of hits.
        }
        // The following if statement validates a miss.
        else { 
            cout << "Miss:   " << hex << cpu_addr << endl; //Printing address if a miss.
            miss_no++; //Add 1 to number of misses.
            x[set_no].addr_up = cpu_upper;
            x[set_no].valid_flag = true;
        }
    }

    cout << "Number of hits: " << hits_no << endl; //Prints total number of hits.
    cout << "Number of misses: " << miss_no << endl; //Prints total number of misses.
}
