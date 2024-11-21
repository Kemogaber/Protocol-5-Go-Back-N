#include <iostream>
#include <vector>
#include "protocol.h"

using namespace std;

int main() {  
    // Simulate a sliding window scenario
    const int NUM_PACKETS = 20;  // Total number of packets to send
    vector<Packet> packets(NUM_PACKETS);  // Simulated packets  

    for (int i = 0; i < NUM_PACKETS; i++) {
        cout << "packet " <<i << ": ";
        for (int j = 0; j < MAX_PKT; j++) {
            packets[i].data[j] = 'A' + (i % 26)+j;  // Fill with letters
            cout << packets[i].data[j];
        }
        cout << endl;
    }
    cout << endl;
    protocol5(packets);
    cout << "\nSimulation Complete!" << endl;
    return 0;
}

