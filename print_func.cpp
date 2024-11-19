#include "protocol.h"
#include <iostream>
#include <unistd.h>   // For usleep function
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For seeding rand()

using namespace std;

// Function Implementations
void wait_for_event(EventType *event, bool isBufferFull) {
    // Simulate waiting time (20 milliseconds)
    usleep(20000);

    // Seed the random number generator to ensure randomness
    srand(time(0)); // Seed only once at the beginning of the program

    // Generate a random event, the total probability adds up to 100
    int randomEvent;

    if (isBufferFull) {
        // If the buffer is full, increase the chances for errors and FrameArrival
        randomEvent = rand() % 100;  // Generate a random number between 0 and 99
        if (randomEvent < 10) {  // 10% chance for error (CksumErr or Timeout)
            int errorType = rand() % 2;  // 0 for CksumErr, 1 for Timeout
            if (errorType == 0) {
                cout << "Event occurred: CksumErr" << endl;
                *event = CksumErr;
            } else {
                cout << "Event occurred: Timeout" << endl;
                *event = Timeout;
            }
        } else if (randomEvent < 60) {  // 50% chance for FrameArrival
            cout << "Event occurred: FrameArrival" << endl;
            *event = FrameArrival;
        } else {  // 40% chance for `Timeout`
            cout << "Event occurred: Timeout" << endl;
            *event = Timeout;
        }
    } else {
        // If the buffer is not full, allow for `NetworkLayerReady` event and adjust other probabilities
        randomEvent = rand() % 100;  // Generate a random number between 0 and 99
        if (randomEvent < 50) {  // 50% chance for `NetworkLayerReady`
            cout << "Event occurred: NetworkLayerReady" << endl;
            *event = NetworkLayerReady;
        } else if (randomEvent < 80) {  // 30% chance for `FrameArrival`
            cout << "Event occurred: FrameArrival" << endl;
            *event = FrameArrival;
        } else {  // 20% chance for `CksumErr` or `Timeout`
            int errorType = rand() % 2;  // 0 for CksumErr, 1 for Timeout
            if (errorType == 0) {
                cout << "Event occurred: CksumErr" << endl;
                *event = CksumErr;
            } else {
                cout << "Event occurred: Timeout" << endl;
                *event = Timeout;
            }
        }
    }
}

void from_network_layer(Packet p) {
    cout << "Fetching packet from network layer..." << endl;
    usleep(20000); // Simulate fetching (200 microseconds delay)
    cout << "packet of value: " << p.data << " fetched successfully" << endl;
}

void to_network_layer(Packet p) {
    cout << "Delivering packet to network layer..." << endl;
    usleep(20000); // Simulate delivery (200 microseconds delay)
    cout << "Packet Received: " << "..." << p.data << endl;
}

void from_physical_layer(Frame *r) {
    cout << "Fetching frame from physical layer..." << endl;
    usleep(20000); // Simulate fetching (200 microseconds delay)
    cout << "Frame fetched successfully." << endl;
}

void to_physical_layer(Frame *s) {
    cout << "Sending frame to physical layer..." << endl;
    usleep(20000); // Simulate sending (200 microseconds delay)
    cout << "Frame sent. Kind: " << s->kind << ", Seq: " << s->seq << ", Ack: " << s->ack << endl;
}

void start_timer(seq_nr k) {
    cout << "Starting timer for sequence number " << k << "..." << endl;
    usleep(20000); // Simulate timer start (200 microseconds delay)
}

void stop_timer(seq_nr k) {
    cout << "Stopping timer for sequence number " << k << "..." << endl;
    usleep(20000); // Simulate timer stop (200 microseconds delay)
}

void start_ack_timer() {
    cout << "Starting acknowledgment timer..." << endl;
    usleep(20000); // Simulate timer start (200 microseconds delay)
}

void stop_ack_timer() {
    cout << "Stopping acknowledgment timer..." << endl;
    usleep(20000); // Simulate timer stop (200 microseconds delay)
}

bool enable_network_layer() {
    cout << "Enabling network layer..." << endl;
    usleep(20000); // Simulate enabling (200 microseconds delay)
    return true;
}

bool disable_network_layer() {
    cout << "Disabling network layer..." << endl;
    usleep(20000); // Simulate disabling (200 microseconds delay)
    return false;
}