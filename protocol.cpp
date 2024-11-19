#include <iostream>
#include <vector>
#include "protocol.h"

using namespace std;

// Check if `a <= b < c` circularly
bool between(seq_nr a, seq_nr b, seq_nr c) {
    return ((a <= b && b < c) || (c < a && a <= b) || (b < c && c < a));
}

// Send a data frame
void send_data(seq_nr frame_nr, seq_nr frame_expected, const vector<Packet>& buffer) {
    Frame s;
    s.kind = Data; // Set frame kind
    s.info = buffer[frame_nr]; // Insert packet into frame
    s.seq = frame_nr; // Set sequence number
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1); // Set piggybacked ACK

    to_physical_layer(&s); // Transmit the frame
    start_timer(frame_nr); // Start the timer
    cout << "Sent frame with seq: " << s.seq << " and ack: " << s.ack << endl;
}

// Sliding window protocol
void protocol5() {
    seq_nr next_frame_to_send = 0;
    seq_nr ack_expected = 0;
    seq_nr frame_expected = 0;
    vector<Packet> buffer(MAX_SEQ + 1);
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
                    to_network_layer(&r.info);
                    inc(frame_expected);
                }

                while (between(ack_expected, r.ack, next_frame_to_send)) {
                    nbuffered--;
                    stop_timer(ack_expected);
                    inc(ack_expected);
                }
                break;

            case CksumErr:
                // Ignore bad frames
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
    }
}