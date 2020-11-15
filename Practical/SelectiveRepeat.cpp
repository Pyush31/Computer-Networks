#include <iostream>
#include <string>
#include <cmath>
#define MAX_PKT 1
#define MAX_SQ_BITS 3
#define MAX_WINDOW_SIZE 5 //the data frames start from 1
typedef unsigned int seq_num;                               //sequence number or ack number
typedef enum {Data, ack, nak} frame_kind;                   //frame kind
typedef enum {frame_arrival,ESTB_CONN,DONE,RESEND} event_type;     //type of event
using namespace std;

//packet class
class Packet{
    public:
        string d;
        Packet()
        {
            d = "";
        }
};

//frame class
class Frame{
    public:
        frame_kind kind;
        seq_num seq;
        seq_num ack;
        Packet p;
        Frame(frame_kind kind,seq_num seq, seq_num ack)
        {
            this->kind = kind;
            this->ack = ack;
            this->seq = seq;
        }
        Frame *copy(Frame *f)
        {
            Frame *temp = new Frame(f->kind,f->seq,f->ack);
            return temp;
        }
};

Frame *sender_frame = 0;
Frame *reciever_frame = new Frame(ack,0,0);

Frame *sending_window[7];
Frame *recieving_window[7];

//sequence number counter
int sender_sequence = 1;
int reciever_sequence = 1;

//event 
event_type event = ESTB_CONN;

void from_network_layer(Packet *p)
{
    cout<<"Encapsulating Packet in the Frame\n";
    sender_frame = new Frame(Data,0,0);
    sender_frame->kind = Data;
    sender_frame->p = *p;
    sender_frame->ack = 0;
    sender_frame->seq = sender_sequence++;
}

void to_physical_layer(Frame *f)
{
    if(f->kind == Data)
    {
        cout<<"Sending :"<<f->seq<<" "<<f->p.d<<"\n";
        if(event != RESEND)
        {
            sending_window[f->seq] = f->copy(f);
            sending_window[f->seq]->p = f->p;
        }
        else
        {
            sender_frame = f;
        }
            
    }
    else if(f->kind == ack)
    {
        cout<<"Recieved :"<<f->ack<<" ACK"<<"\n";
        if(f->ack != 0)
        {
            sending_window[f->ack]->ack = 1;            
        }
    }
}

void from_physical_layer(Frame *f)
{
    if(sender_sequence%2 == 0 || event == RESEND)
    {
        cout<<"Recieved : "<<f->seq<<" "<<f->p.d<<"\n";
    }
    recieving_window[f->seq] = f->copy(f);
    recieving_window[f->seq]->p = f->p;    
}

void to_network_layer(Packet *p)
{
    if(sender_sequence%2 == 0 || event == RESEND)
    {
        cout<<"Decapsulating Recieved Frame\n";
        cout<<"Sending ACK \n";
    }
    else
    {
        cout<<"PACKET LOST\n";
    }
    reciever_frame->kind = ack;
    reciever_frame->ack = sender_frame->seq;
    reciever_frame->seq = 0;
    reciever_sequence++;
}

Packet *packet;

class SelectiveRepeat
{
    public:
        string s;
        int length;
        int cursor;
        string r;
        SelectiveRepeat(string s)
        {
            this->s = s;
            length = s.length();
            cursor = 0;
            r = "";
        }
        void sender();
        void reciever();
        void resend()
        {
            if(sender_sequence > MAX_WINDOW_SIZE)
            {
                cout<<"RESENDING THE NAK FRAME\n\n";
                event = RESEND;
                for (int i = 1; i < sender_sequence; i++)
                {
                    if(sending_window[i]->ack != 1)
                    {
                        to_physical_layer(sending_window[i]);
                        cout<<"\n";
                        reciever();
                        cout<<"SENDER\n";
                        to_physical_layer(reciever_frame);
                    }
                    Frame *temp = sending_window[i];
                    delete temp;
                    temp = 0;
                    sending_window[i] = 0;
                }
                sender_sequence = 1;
                reciever_sequence = 1;
                event = frame_arrival;
            }
            else if(length <= 0 && (sender_sequence-1) > 1)
            {
                cout<<"\nLOST PACKETS RESENT \n";
            }
        }
};

void SelectiveRepeat :: sender()
{
    
    cout<<"SENDER\n";
    if(length <= 0)
    {
        to_physical_layer(reciever_frame);
        resend();
        event = DONE;
        cout<<"\n";
        reciever();
        return;
    }
    while(length > 0)
    {
        if(event == frame_arrival)
        {
            if(sender_frame > 0)
            {
                if(sender_sequence % 2 == 0) 
                {
                    to_physical_layer(reciever_frame);
                }
                else if(sender_sequence % 2 != 0)
                {
                    cout<<"Recieved :"<<"NAK"<<"\n";
                }
            }
            resend();
            packet = new Packet();
            //creating the packet
            for(int i = 0; i < MAX_PKT; i++, cursor++)
            {
                packet->d += s[cursor]; 
            }
            from_network_layer(packet);
            to_physical_layer(sender_frame);
            cout<<"\n";
        }
        else if(event == ESTB_CONN)
        {
            cout<<"Establishing Connection\n\n";
        }
        reciever();
    }

}

void SelectiveRepeat :: reciever()
{
    cout<<"RECIEVER\n";
    if(event == frame_arrival)
    {
        from_physical_layer(sender_frame);
        to_network_layer(packet);
        length -= MAX_PKT;
        if(reciever_sequence > MAX_WINDOW_SIZE)
        {
            for(int i = 1; i <= MAX_WINDOW_SIZE; i++)
            {
                Frame *temp = recieving_window[i];
                r += temp->p.d;
                delete temp;
                temp = 0;
                recieving_window[i] = 0;
            }
            reciever_sequence = 1;
        }
        cout<<"\n";
        sender();
    }
    else if(event == RESEND)
    {
        from_physical_layer(sender_frame);
        to_network_layer(packet);
        cout<<"\n";
        return;
    }
    else if(event == ESTB_CONN)
    {
        cout<<"Connection Estblished\n";
        event = frame_arrival;
        cout<<"\n";
        sender();
    }
    else if(event == DONE)
    {
        if(reciever_sequence <= MAX_WINDOW_SIZE)
        {
            for(int i = 1; i < reciever_sequence; i++)
            {
                Frame *temp = recieving_window[i];
                r += temp->p.d;
                delete temp;
                temp = 0;
                recieving_window[i] = 0;
            }
            reciever_sequence = 1;
        }
        cout<<"Done\n";
        cout<<"\nDATA RECIEVED : "<<r; 
        return;
    }
}


int main()
{
    cout<<"     SELECTIVE REPEAT PROTOCOL FOR NOISY CHANNEL\n";
    cout<<"(Every Frame with the Even Sequence Number has ACK Error and Number of Packets are Greater than Window Size)\n";
    cout<<"Enter the message to be sent : ";
    string s;
    getline(cin,s);
    SelectiveRepeat *z = new SelectiveRepeat(s);
    z->sender();
}