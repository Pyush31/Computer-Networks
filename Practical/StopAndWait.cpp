#include <iostream>
#include <string>
#define max_pkt  5
typedef unsigned int sq_no;
typedef enum {Data, ack, nak} fr_kind;
typedef enum {fr_arr,Estb_Conn,Done} event_type;
using namespace std;

class Packet
{
public:
    string d;
    Packet()
    {
        d = "";
    }
};

class Frame
{
public:
    fr_kind kind;
    sq_no seq;
    sq_no ack;
    Packet p;
    Frame(fr_kind,sq_no seq,sq_no ack)
    {
        this->kind = kind;
        this->ack = ack;
        this->seq = seq;
    }
};

Frame *sen_fr = new Frame(Data,0,0);
Frame *rec_fr = new Frame(Data,0,0);

int sen_seq = 1;
int rec_seq = 1;

event_type event = Estb_Conn;

void fr_net_layer(Packet *p)
{
    cout<<"\n Encapsulating Packet in the frame";
    sen_fr->kind = Data;
    sen_fr->p = *p;
    sen_fr->seq = sen_seq++;
}

void to_phy_layer(Frame *f)
{
    if (f->kind == Data)
    {
        cout<<"\n Sending :"<<f->seq<<" "<<f->p.d<<endl;
    }
    else if (f->kind == ack)
    {
        cout<<"\n Recieved : "<<f->ack<<" Acknowlegment"<<endl;
    }
}

void fr_phy_layer(Frame *f)
{
    if (sen_seq != rec_seq)
    {
        cout<<"\n Recieved : "<<f->seq<<" "<<f->p.d<<endl;
    }
    else
    {
        cout<<"\n Frame Duplication Detected ";
    }
}

void to_net_layer(Packet *p)
{
    cout<<"\n Decapsulating recieved frame ";
    cout<<"\n Sending Acknowlegment ";
    rec_fr->kind = ack;
    rec_fr->ack = sen_fr->seq;
    rec_fr->seq = 0;
    rec_seq++;
}

int Error = 0;
Packet *packet;
class StopAndWait
{
public:
    string s;
    int l;
    int cursor;
    string r;
    StopAndWait(string s)
    {
        this->s = s;
        l = s.length();
        cursor = 0;
        r = "";
    }
    void sender();
    void reciever();

};

void StopAndWait :: sender()
{
    cout<<"\n SENDER ";
    if (l <= 0)
    {
        to_phy_layer(rec_fr);
        event = Done;
        cout<<endl;
        reciever();
        return;
    }
    while (l > 0)
    {
        if (event == fr_arr)
        {
            if (sen_fr->seq == rec_fr->ack)
            {
                to_phy_layer(rec_fr);
                packet = new Packet();
                for (int i = 0; i < max_pkt; i++,cursor++)
                {
                    packet->d += s[cursor];   
                }
                fr_net_layer(packet);
                to_phy_layer(sen_fr);
                cout<<endl;
            }
            else
            {
                cout<<"\n Acknowlegment Error\nResending>>>>>";
                Error = 0;   
            }
        }
        else if (event == Estb_Conn)
        {
            cout<<"\n Establishing Connection......>>>";
        }
        reciever();
    }     
}

void StopAndWait :: reciever()
{
    cout<<"\n RECIEVER";
    if (event == fr_arr)
    {
        if (!Error )
        {
            if (sen_fr->seq == rec_seq)
            {
                fr_phy_layer(sen_fr);
                to_net_layer(packet);
                l -= max_pkt;
                r += packet->d;
                cout<<endl;
                if (rec_seq%2 == 0)
                {
                    Error = 1;   
                }   
            }   
        }
        else
        {
            fr_phy_layer(sen_fr);
            cout<<"\n Decapsulating Recieved Frame ";
            cout<<"\n Sending Acknowlegment";
        }
        sender();
    }
    else if (event == Estb_Conn)
    {
        cout<<"\n Connection Established ";
        event = fr_arr;
        cout<<endl;
        sender();
    }
    else if (event == Done)
    {
        cout<<"\n <<<<<DONE>>>>";
        cout<<"\n Data Recieved : "<<r;
        return;   
    }
}

int main()
{
    cout<<"\n STOP & WAIT PROTOCOL FOR NOISY CHANNEL";
    cout<<"\nNOTE>>>\nEvery even sequence number frame has ACK Error";
    cout<<"\n Enter the message to be sent : ";
    string S;
    getline(cin,S);
    StopAndWait *z = new StopAndWait(S);
    z->sender();
    return 0;
}