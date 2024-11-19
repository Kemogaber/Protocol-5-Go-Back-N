#include "protocol.h"
#include <iostream>
#include <unistd.h>   // For usleep function
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For seeding rand()

using namespace std;

// Function Implementations
void wait_for_event(EventType *event, bool isBufferFull) {
    // Predefined hardcoded event sequence
    static const EventType eventSequence[] = {
        NetworkLayerReady, FrameArrival,NetworkLayerReady ,FrameArrival, NetworkLayerReady, NetworkLayerReady , NetworkLayerReady, NetworkLayerReady,FrameArrival,Timeout, FrameArrival,FrameArrival, NetworkLayerReady,CksumErr,NetworkLayerReady ,Timeout, FrameArrival, 
        NetworkLayerReady, FrameArrival, Timeout,NetworkLayerReady ,CksumErr, NetworkLayerReady,FrameArrival
    };
    
    static int eventIndex = 0;  // Keep track of the current event index

    // Loop through the predefined sequence of events
    *event = eventSequence[eventIndex];
    cout << endl ;
    switch (*event) {
        case FrameArrival:
            cout << "FrameArrival Event occurred!" << endl;
            break;
        case CksumErr:
            cout << "CksumErr Event occurred!" << endl;
            break;
        case Timeout:
            cout << "Timeout Event occurred!" << endl;
            break;
        case NetworkLayerReady:
            cout << "NetworkLayerReady Event occurred!" << endl;
            break;
    }
    cout << endl ;

    // Increment the event index to loop through the array
    eventIndex = (eventIndex + 1) % (sizeof(eventSequence) / sizeof(eventSequence[0]));
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