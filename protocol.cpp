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
   cout << "Sent frame with seq: " << s.seq << " and ack: " << s.ack << " data: " ;
for (int i = 0; i < MAX_PKT; i++) {
    cout << s.info.data[i];
}
cout << endl;
    return s;
}

// Sliding window protocol
void protocol5(vector<Packet> data) {
    queue<Frame> myQueue;
    seq_nr counter=0;
    seq_nr next_frame_to_send = 0;
    seq_nr ack_expected = 0;
    seq_nr frame_expected = 0;
    seq_nr nbuffered = 0;
    seq_nr Max_Window_Size=MAX_SEQ+1;
    bool Flag;
    EventType event;

    Flag=enable_network_layer();
    cout << "Starting protocol5 with " << data.size() << " packets." << endl;
    while (counter<data.size()) {
        wait_for_event(&event, Flag);
        switch (event) {
            case NetworkLayerReady:{
                if(myQueue.size() < MAX_SEQ){
                from_network_layer(data[counter]);
                myQueue.push(send_data(next_frame_to_send, frame_expected, counter, data));
                nbuffered++;
                counter++;
                inc(next_frame_to_send); // Use the macro to increment
                }
                else{
                    cout << "Buffer is Full" << endl;
                    Flag=disable_network_layer();
                }
                break;
        }
            case FrameArrival:
            {
                Frame r=myQueue.front();
                myQueue.pop();
                from_physical_layer(&r);
                if ( r.seq == frame_expected) {
                    to_network_layer(r.info);
                    inc(frame_expected);
                    nbuffered--;
                    if(!Flag) Flag=enable_network_layer();
                    stop_timer(ack_expected);
                    inc(ack_expected);
                }

                /*while (between(ack_expected, r.ack, next_frame_to_send)) {

                }
                */
                break;
            }

            case CksumErr:{
                cout << "Error is ignored" << endl;
                break;
            }

            case Timeout:{
                int size=myQueue.size();
                next_frame_to_send = myQueue.front().seq;
                counter=((counter-size)>0?counter-size:0);
                for (int i=0;i<size;i++){
                    from_network_layer(data[counter]);
                    send_data(next_frame_to_send,ack_expected,counter,data);
                    counter++;
                    inc(next_frame_to_send);
                }
                break;
            }
        }
    }
    cout << "Simulation Has Ended" << endl;
}