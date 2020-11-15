#include<iostream>
#include<iomanip>
using namespace std;

int main()
{
     cout<<"\n DISTANCE VECTOR PROTOCOL SIMULATION";
    int vertex = 5;
    cout<<"Enter the Number of Routers : "; cin>>vertex;
    int distTable[vertex][vertex] = {0};

    int hopTable[vertex][vertex] = {0};

    //Input
    for(int i = 0; i < vertex; i++)
    {
        for(int j = 0; j < vertex; j++)
        {
            cout<<"("<<i<<","<<j<<") : "; cin>>distTable[i][j];
            hopTable[i][j] = j;
            if(i!=j && distTable[i][j] == 0)
            {
                distTable[i][j] = 1000;
            }
        }
        cout<<"\n";
    }

    //Sharing the Routing Table and Recalculating the Cost 
    for(int i = 0; i < vertex; i++)
    {
        for(int j = 0; j < vertex; j++)
        {
            for(int k = 0; k < vertex; k++)
            {
                if(distTable[i][j] > (distTable[i][k] + distTable[k][j]))
                {
                    distTable[i][j] = distTable[i][k] + distTable[k][j];
                    if(hopTable[i][j] == j)
                    {
                        hopTable[i][j] = k;
                    }
                    else
                    {
                        hopTable[i][j] = hopTable[i][j]*10 + k;
                    }
                }
            }
        }
    }
    cout<<"Routing Tables\n";
    for(int i = 0; i < vertex; i++)
    {
        cout<<"Router : "<<i<<"\n";  
        cout<<left;
        cout<<setw(12)<<"Destiantion"<<setw(10)<<"Distance"<<setw(10)<<"Next Hop"<<"\n";
        for(int j = 0; j < vertex; j++)
        {
            cout<<"     "<<setw(10)<<j<<setw(10)<<distTable[i][j]<<setw(10)<<hopTable[i][j]<<"\n";
        }
        cout<<right;
        cout<<"\n";
    }

    return 0;
}