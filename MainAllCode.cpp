#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>   // For usleep function
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For seeding rand()

using namespace std;

// Constants
#define MAX_PKT 5 // Determines packet size in bytes
#define MAX_SEQ 7    // Maximum sequence number (based on protocol)

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

// Function prototypes
void wait_for_event(EventType *event);
void from_network_layer(Packet *p);
void to_network_layer(Packet *p, seq_nr seq_nr);
void from_physical_layer(Frame *r);
void to_physical_layer(Frame *s);
void start_timer(seq_nr k);
void stop_timer(seq_nr k);
void start_ack_timer();
void stop_ack_timer();
void enable_network_layer();
void disable_network_layer();
void fill_packet(Packet *p, const char* data, size_t length);
void protocol5(const vector<Packet>& data);
bool between(seq_nr a, seq_nr b, seq_nr c); // Check if a <= b < c circularly
void send_data(seq_nr frame_nr, seq_nr frame_expected, const vector<Packet>& buffer);
#define inc(k) if ((k) < MAX_SEQ) (k)++; else (k) = 0  // Macro to increment sequence number circularly

// Function Implementations

void wait_for_event(EventType *event) {
    usleep(200000); // Simulate waiting time (200 microseconds)
    
    srand(time(0)); // Seed the random number generator
    int randomEvent = rand() % 4; // Generate a random number between 0 and 3
    *event = static_cast<EventType>(randomEvent);

    switch (*event) {
        case FrameArrival:
            cout << "[EVENT]: Frame Arrival" << endl;
            break;
        case CksumErr:
            cout << "[EVENT]: CksumErr" << endl;
            break;
        case Timeout:
            cout << "[EVENT]: Timeout" << endl;
            break;
        case NetworkLayerReady:
            cout << "[EVENT]: NetworkLayerReady" << endl;
            break;
    }
}

void from_network_layer(Packet *p) {
    usleep(200000); // Simulate fetching (200 microseconds delay)
}

void to_network_layer(Packet *p, seq_nr seq_nr) {
    cout << "[TRANSMITTER]: Delivering packet number " << seq_nr << " to network layer..." << endl;
    usleep(200); // Simulate delivery (200 microseconds delay)
    cout << "[TRANSMITTER]: Packet " << seq_nr << " data: ";
    
    // Assuming the Packet structure has a data field that holds the data as a char array
    cout << p->data << endl;  // Print the packet's data field, with the sequence number
}

void from_physical_layer(Frame *r) {
    usleep(200000); // Simulate fetching (200 microseconds delay)
}

void to_physical_layer(Frame *s) {
    usleep(200000); // Simulate sending (200 microseconds delay)
    cout << "[TRANSMITTER]: Sending Frame To Physical Layer: Seq=" << s->seq << ", Ack=" << s->ack << ", Data=" << s->info.data << endl;
}

void start_timer(seq_nr k) {
    usleep(200000); // Simulate timer start (200 microseconds delay)
}

void stop_timer(seq_nr k) {
    usleep(200000); // Simulate timer stop (200 microseconds delay)
}

void start_ack_timer() {
    usleep(200000); // Simulate timer start (200 microseconds delay)
}

void stop_ack_timer() {
    usleep(200000); // Simulate timer stop (200 microseconds delay)
}

void enable_network_layer() {
    usleep(200000); // Simulate enabling (200 microseconds delay)
}

void disable_network_layer() {
    usleep(200000); // Simulate disabling (200 microseconds delay)
}

void fill_packet(Packet *p, const char* data, size_t length) {
    size_t copy_length = (length < MAX_PKT) ? length : MAX_PKT;
    memset(p->data, 0, MAX_PKT);
    memcpy(p->data, data, copy_length);
}

bool between(seq_nr a, seq_nr b, seq_nr c) {
    return ((a <= b && b < c) || (c < a && a <= b) || (b < c && c < a));
}

void send_data(seq_nr frame_nr, seq_nr frame_expected, const vector<Packet>& buffer) {
    Frame s;
    s.kind = Data;
    s.info = buffer[frame_nr];
    s.seq = frame_nr;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
    
    to_physical_layer(&s);
    start_timer(frame_nr);
    cout << "[TRANSMITTER]: Start Timer for sequence number " << s.seq << endl;
}

void protocol5(const vector<Packet>& data) {
    seq_nr next_frame_to_send = 0;
    seq_nr ack_expected = 0;
    seq_nr frame_expected = 0;
    vector<Packet> buffer = data;
    seq_nr nbuffered = 0;
    EventType event;

    enable_network_layer();

    while (true) {
        wait_for_event(&event);

        switch (event) {
            case NetworkLayerReady:
                from_network_layer(&buffer[next_frame_to_send]);
                nbuffered++;
                send_data(next_frame_to_send, frame_expected, buffer);
                inc(next_frame_to_send); // Use the macro to increment
                break;

            case FrameArrival:
                Frame r;
                from_physical_layer(&r);

                if (r.seq == frame_expected) {
                    to_network_layer(&r.info, r.seq);  // Print the packet number along with data
                    inc(frame_expected);
                }

                while (between(ack_expected, r.ack, next_frame_to_send)) {
                    nbuffered--;
                    stop_timer(ack_expected);
                    inc(ack_expected);
                }
                break;

            case CksumErr:
                break;

            case Timeout:
                next_frame_to_send = ack_expected;
                for (seq_nr i = 0; i < nbuffered; i++) {
                    send_data(next_frame_to_send, frame_expected, buffer);
                    inc(next_frame_to_send);
                }
                break;
        }

        if (nbuffered < MAX_SEQ) {
            enable_network_layer();
        } else {
            disable_network_layer();
        }

        // Break the loop once data is fully processed (no more data to send)
        if (frame_expected == buffer.size()) {
            cout << "[TRANSMITTER]: All data transmitted successfully." << endl;
            break;
        }
    }
}

int main() {
    const char test_data[] = "Hello, this is a test message for protocol 5 simulation.";

    vector<Packet> buffer(MAX_SEQ + 1);
    for (int i = 0; i <= MAX_SEQ; i++) {
        fill_packet(&buffer[i], test_data, strlen(test_data));
    }

    cout << "Starting Protocol 5..." << endl;
    protocol5(buffer); // Call the protocol function with the data buffer

    return 0;
}
