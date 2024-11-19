#include <iostream>
#include <vector>
#include "protocol.h"

using namespace std;

int main() {
    // Simulate a sliding window scenario
    const int NUM_PACKETS = 10;  // Total number of packets to send
    const int ERROR_PACKET = 3; // Packet with checksum error
    const int TIMEOUT_PACKET = 5; // Packet that causes a timeout
    vector<Packet> packets(NUM_PACKETS);  // Simulated packets

    // Fill packets with dummy data
    for (int i = 0; i < NUM_PACKETS; i++) {
        for (int j = 0; j < MAX_PKT; j++) {
            packets[i].data[j] = 'A' + (i % 26);  // Fill with letters
        }
    }

    seq_nr next_frame_to_send = 0;  // Next frame to send
    seq_nr ack_expected = 0;        // Oldest unacknowledged frame
    seq_nr frame_expected = 0;      // Next frame expected at receiver
    seq_nr nbuffered = 0;           // Number of frames currently in use
    vector<Frame> window(MAX_SEQ + 1);  // Sliding window

    cout << "Starting Go-Back-N ARQ Simulation..." << endl;

    // Simulate sending and receiving packets
    for (int i = 0; i < NUM_PACKETS; i++) {
        cout << "\n--- Sending Packet " << i << " ---" << endl;

        // Construct frame
        Frame s;
        s.info = packets[i];
        s.seq = next_frame_to_send;
        s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

        // Simulate sending to physical layer
        cout << "Sending frame with Seq: " << s.seq << ", Ack: " << s.ack << endl;

        // Simulate acknowledgment or error
        if (i == ERROR_PACKET) {
            cout << "Simulated Checksum Error on frame " << i << endl;
            // Ignore the frame; no acknowledgment sent
        } else if (i == TIMEOUT_PACKET) {
            cout << "Simulated Timeout for frame " << i << endl;
            // Retransmit all outstanding frames (Go-Back-N)
            cout << "Retransmitting frames starting from Seq: " << ack_expected << endl;
            for (seq_nr j = ack_expected; j != next_frame_to_send; j = (j + 1) % (MAX_SEQ + 1)) {
                cout << "Retransmitting frame with Seq: " << j << endl;
            }
        } else {
            cout << "Acknowledgment received for frame " << s.seq << endl;
            ack_expected = (ack_expected + 1) % (MAX_SEQ + 1);
        }

        // Move sliding window forward if there's no error or timeout
        if (i != ERROR_PACKET && i != TIMEOUT_PACKET) {
            next_frame_to_send = (next_frame_to_send + 1) % (MAX_SEQ + 1);
            frame_expected = (frame_expected + 1) % (MAX_SEQ + 1);
        }

        // Simulate the state of the sender's window
        nbuffered = (next_frame_to_send >= ack_expected) 
                    ? next_frame_to_send - ack_expected 
                    : (MAX_SEQ + 1) - ack_expected + next_frame_to_send;
        cout << "Sliding Window: Ack_Expected = " << ack_expected 
             << ", Next_Frame_To_Send = " << next_frame_to_send 
             << ", Buffered Frames = " << nbuffered << endl;
    }

    cout << "\nSimulation Complete!" << endl;
    return 0;
}