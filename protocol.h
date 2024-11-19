#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstddef> // for size_t
#include <vector> // Include the vector header

// Constants
#define MAX_PKT 2 // Determines packet size in bytes
#define MAX_SEQ 7    // Maximum sequence number (based on protocol)
using namespace std;
typedef unsigned int seq_nr; // Sequence or acknowledgment numbers

// Packet definition
struct Packet {
    unsigned char data[MAX_PKT]; // Packet data
};

// Frame kind enumeration
enum FrameKind { 
    Data,  // Data frame
    Ack,   // Acknowledgment frame
    Nak    // Negative acknowledgment frame
};

// Frame definition
struct Frame {
    FrameKind kind;  // Frame kind (data/ack/nak)
    seq_nr seq;      // Sequence number
    seq_nr ack;      // Acknowledgment number
    Packet info;     // Network layer packet
};

// Event type enumeration
enum EventType { 
    FrameArrival, 
    CksumErr, 
    Timeout, 
    NetworkLayerReady 
};

// Function Prototypes
void wait_for_event(EventType *event, bool isBufferFull);            // Wait for an event
void from_network_layer(Packet p);             // Fetch packet from network layer
void to_network_layer(Packet p);                // Deliver packet to network layer
void from_physical_layer(Frame *r);             // Get frame from physical layer
void to_physical_layer(Frame *s);                // Send frame to physical layer
void start_timer(seq_nr k);                      // Start timer
void stop_timer(seq_nr k);                       // Stop timer
void start_ack_timer();                          // Start acknowledgment timer
void stop_ack_timer();                           // Stop acknowledgment timer
bool enable_network_layer();                     // Allow network layer events
bool disable_network_layer();                    // Forbid network layer events
void protocol5(vector<Packet> data);

// Macro to increment sequence number circularly
#define inc(k) if ((k) < MAX_SEQ) (k)++; else (k) = 0

#endif // PROTOCOL_H