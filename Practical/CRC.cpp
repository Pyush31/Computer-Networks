#include <iostream>
#include <string>
using namespace std;

string remainder(string dividend , string generator )
{
    int i = 0;
    int temp = 0;
    while(generator.length() <= dividend.length())
    {
        i = 0;
        for(int j = 0; j < generator.length();j++)
        {
            temp = (dividend[i] ^ generator[j]);
            switch(temp)
            {
            case 1:
                dividend[i] = '1';
                break;
            case 0:
                dividend[i] = '0';
                break;
            default:
                break;
            }
            i++;
        }
        while((generator.length() <= dividend.length()) && (dividend[0] != '1'))
        {
            dividend = dividend.substr(1);
        }
    }
    return dividend;
}

int main()
{
    cout<<"\n Simulation for CRC error detection algorithm for Noisy Channel ";
    string message,generator,dividend,crc,tmessage,emessage;
    cout<<"\n Enter the message : "; cin>>dividend;
    cout<<"\n Enter the Generator : "; cin>>generator;
    message = dividend;

    for (int i = 0; i < generator.length()-1; i++)
    {
        dividend += "0";
    }

    if((dividend.length() < generator.length()) || (generator[0] != '1' && generator[generator.length()-1] != '1'))
    {
        return -1;
    }
    crc = remainder(dividend,generator);
    tmessage = message + crc;
    cout<<"\n Transmitted Message : "<<tmessage;
    cout<<"\n Error Detection ";
    int fbits =0;
    int index =0;
    emessage = tmessage;
    cout<<"\n Enter the no of bits to flip : "; cin>>fbits;
    for (int i = 0; i < fbits; i++)
    {
        cout<<"\n Position of bit to be flipped ";
        cin>>index;
        emessage[index-1] = (emessage[index-1] == '1')?'0':'1';
    }
    crc = remainder(emessage,generator);
    for (int i = 0; i < crc.length(); i++)
    {
        if (crc[i] == '1')
        {
            cout<<"\n Error During Transmission "<<crc;
            return 1;
        }
    }
    cout<<"\n No Error";
    return 0;
}