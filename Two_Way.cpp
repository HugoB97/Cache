#include <iostream>

#include <fstream>

#include <sstream>

using namespace std;

int main() {
    struct two_way_cache {
        unsigned short addr_up; // 16-bit tag address.
        bool valid_flag; //  Prevents copy, flag is set if data valid.
        bool lru_flag; // Least Recently Used eviction policy
        char data[4]; // 32-bit data bus
    };

    unsigned short byte_no;
    unsigned short set_no;

    unsigned short cpu_upper; // Upper half of address.
    unsigned short cpu_lower; // Upper half of address.

    unsigned int hits_no = 0; // Counts number of hits.
    unsigned int miss_no = 0; // Counts number of misses.

    unsigned int cpu_addr;

    ifstream my_addr; // Address text file to be loaded.
    two_way_cache x[16384]; // Creates 2^14 cache entries.
    two_way_cache y[16384]; // Creates 2^14 cache entries.

    for (int i = 0; i <= 16384; i++) {
        x[i].valid_flag = 0; // Valid flag is set to zero.
        x[i].lru_flag = 1; // LRU initialised
        y[i].valid_flag = 0; // Valid flag is set to zero.
        y[i].lru_flag = 1; // LRU initialised.
    }

    my_addr.open("Addresses.txt"); // Opens input file that holds addresses to be tested.
    while (!my_addr.eof()) { // Hold the file open until the end.
        my_addr >> hex >> cpu_addr; // The line is initially treated as hex.
        cpu_lower = cpu_addr & 0x0000ffff; // CPU is anded with ffff.
        cpu_upper = cpu_addr >> 16; // First half is anded w/ a set of ffff, then it's bit shifted by 16.
        set_no = (cpu_addr & 0x0000ffff) >> 2;
        byte_no = cpu_lower & 3; // Lower half is anded with three.

        // The following if statement validates a hit.
        if (x[set_no].addr_up == cpu_upper && x[set_no].valid_flag) {
            cout << "Full address: 0x" << hex << cpu_addr; //Printing address if a hit.
            cout << " Hit:    " << hex << cpu_addr << endl;
            hits_no++; //Add 1 to number of hits.
            x[set_no].lru_flag = 0; //LRU flag set to 0.
            y[set_no].lru_flag = 1; //LRU flag set to 1.
        } else if (y[set_no].addr_up == cpu_upper && y[set_no].valid_flag) {
            cout << "Full address: 0x" << hex << cpu_addr;
            cout << " Hit:  0x" << hex << cpu_upper << endl;
            hits_no++; //Add 1 to number of hits.
            x[set_no].lru_flag = 1; //LRU flag set to 1.
            y[set_no].lru_flag = 0; //LRU flag set to 0.
        }

        // The following if statement validates a miss.
        else {
            cout << "Full address: 0x" << hex << cpu_addr; //Printing address if a miss.
            cout << " Miss:   " << hex << cpu_addr << endl;
            miss_no++; //Add 1 to number of misses.
            if (x[set_no].lru_flag) {
                x[set_no].addr_up = cpu_upper;
                x[set_no].valid_flag = 1;
                x[set_no].lru_flag = 0; //LRU flag set to 0.
                y[set_no].lru_flag = 1; //LRU flag set to 1.
            } else {
                y[set_no].addr_up = cpu_upper;
                y[set_no].valid_flag = 1;
                y[set_no].lru_flag = 0; //LRU flag set to 0.
                x[set_no].lru_flag = 1; //LRU flag set to 1.
            }
        }
    }
    cout << "Number of hits: " << hits_no << endl; //Prints total number of hits.
    cout << "Number of misses: " << miss_no << endl; //Prints total number of misses.
}
