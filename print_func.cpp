#include "protocol.h"
#include <iostream>
#include <unistd.h>   // For usleep function
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For seeding rand()

using namespace std;

// Function Implementations
void wait_for_event(EventType *event) {
    cout << "Waiting for an event..." << endl;
    usleep(200); // Simulate waiting time (200 microseconds)
    
    // Randomly select an event
    srand(time(0)); // Seed the random number generator
    int randomEvent = rand() % 4; // Generate a random number between 0 and 3
    *event = static_cast<EventType>(randomEvent);

    // Print the selected event
    switch (*event) {
        case FrameArrival:
            cout << "Event occurred: FrameArrival" << endl;
            break;
        case CksumErr:
            cout << "Event occurred: CksumErr" << endl;
            break;
        case Timeout:
            cout << "Event occurred: Timeout" << endl;
            break;
        case NetworkLayerReady:
            cout << "Event occurred: NetworkLayerReady" << endl;
            break;
    }
}

void from_network_layer(Packet *p) {
    cout << "Fetching packet from network layer..." << endl;
    usleep(200); // Simulate fetching (200 microseconds delay)
    cout << "Packet fetched successfully." << endl;
}

void to_network_layer(Packet *p) {
    cout << "Delivering packet to network layer..." << endl;
    usleep(200); // Simulate delivery (200 microseconds delay)
    cout << "Packet delivered: ";
   
    cout << "..." << endl;
}

void from_physical_layer(Frame *r) {
    cout << "Fetching frame from physical layer..." << endl;
    usleep(200); // Simulate fetching (200 microseconds delay)
    cout << "Frame fetched successfully." << endl;
}

void to_physical_layer(Frame *s) {
    cout << "Sending frame to physical layer..." << endl;
    usleep(200); // Simulate sending (200 microseconds delay)
    cout << "Frame sent. Kind: " << s->kind << ", Seq: " << s->seq << ", Ack: " << s->ack << endl;
}

void start_timer(seq_nr k) {
    cout << "Starting timer for sequence number " << k << "..." << endl;
    usleep(200); // Simulate timer start (200 microseconds delay)
}

void stop_timer(seq_nr k) {
    cout << "Stopping timer for sequence number " << k << "..." << endl;
    usleep(200); // Simulate timer stop (200 microseconds delay)
}

void start_ack_timer() {
    cout << "Starting acknowledgment timer..." << endl;
    usleep(200); // Simulate timer start (200 microseconds delay)
}

void stop_ack_timer() {
    cout << "Stopping acknowledgment timer..." << endl;
    usleep(200); // Simulate timer stop (200 microseconds delay)
}

void enable_network_layer() {
    cout << "Enabling network layer..." << endl;
    usleep(200); // Simulate enabling (200 microseconds delay)
}

void disable_network_layer() {
    cout << "Disabling network layer..." << endl;
    usleep(200); // Simulate disabling (200 microseconds delay)
}