#include <iostream>
#include <vector>
#include "protocol.h"

using namespace std;

// Check if `a <= b < c` circularly
bool between(seq_nr a, seq_nr b, seq_nr c) {
    return ((a <= b && b < c) || (c < a && a <= b) || (b < c && c < a));
}

// Send a data frame
Frame send_data(seq_nr frame_nr, seq_nr frame_expected,seq_nr counter, const vector<Packet>& data) {
    Frame s;
    s.kind = Data; // Set frame kind
    s.info = data[counter]; // Insert packet into frame
    s.seq = frame_nr; // Set sequence number
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1); // Set piggybacked ACK

    to_physical_layer(&s); // Transmit the frame
    start_timer(frame_nr); // Start the timer
   cout << "Sent frame with seq: " << s.seq << " and ack: " << s.ack << " data: ";
for (int i = 0; i < MAX_PKT; i++) {
    cout << s.info.data[i];
}
    return s;
}

// Sliding window protocol
void protocol5(vector<Packet> data) {
    Frame temp_frame;
    seq_nr counter=0;
    seq_nr next_frame_to_send = 0;
    seq_nr ack_expected = 0;
    seq_nr frame_expected = 0;
    seq_nr nbuffered = 0;
    seq_nr Max_Window_Size=MAX_SEQ+1;
    EventType event;

    enable_network_layer();

    while (counter<data.size()) {
        wait_for_event(&event);

        switch (event) {
            case NetworkLayerReady:{
                if(nbuffered < MAX_SEQ){
                from_network_layer(data[counter]);
                temp_frame=send_data(next_frame_to_send, frame_expected, counter, data);
                nbuffered++;
                counter++;
                inc(next_frame_to_send); // Use the macro to increment
                break;
                }
                else{
                    cout << "Buffer is Full" << endl;
                    break;
                }
        }
            case FrameArrival:
                from_physical_layer(&temp_frame);

                if (temp_frame.seq == frame_expected) {
                    to_network_layer(temp_frame.info);
                    inc(frame_expected);
                }

                while (between(ack_expected, temp_frame.ack, next_frame_to_send)) {
                    nbuffered--;
                    stop_timer(ack_expected);
                    inc(ack_expected);
                }
                break;

            case CksumErr:
                cout << "Error is being corrected at Receiver" << endl;
                break;

            case Timeout:
                next_frame_to_send = ack_expected;
                nbuffered=0;
                counter=(counter-Max_Window_Size)>0?counter-Max_Window_Size:0;
                for (int i=0;i<Max_Window_Size;i++){
                    from_network_layer(data[counter]);
                    temp_frame=send_data(next_frame_to_send,ack_expected,counter,data);
                    nbuffered++;
                    counter++;
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