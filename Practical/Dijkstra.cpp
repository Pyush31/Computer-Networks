#include<iostream>
#include<string>
using namespace std;

int main()
{
    int vertex,source,dest;
    int adjM[5][5];
    cout<<"\n Enter the number of Vertices ";
    cin>>vertex;
    for (int i = 0; i < vertex; i++)
    {
        for (int j = 0; j < vertex; j++)
        {
            cout<<"("<<i<<","<<j<<") : ";
            cin>>adjM[i][j];
        }
        cout<<endl;
    }
    cout<<"\n Adjacency Matrix "<<endl;
     for (int i = 0; i < vertex; i++)
    {
        for (int j = 0; j < vertex; j++)
        {
            cout<<adjM[i][j]<<" ";
        }
        cout<<endl;
    }

    cout<<"\n Enter the source vertex "; cin>>source;
    cout<<"\n Enter the destination vertex "; cin>>dest;

    int visited[vertex] = {0};        //for visited nodes
    int ShortDist[vertex] = {0};     //for shortest distance of each node
    int hops[vertex] = {0};
    int flag = 1;
    int traverse = source;
    for (int i = 0; i < vertex; i++)
    {
        ShortDist[i] = INT_MAX;
    }
    ShortDist[traverse] = 0;
    visited[traverse] = 1;
    while (flag)            //shortest path to node which is not visited
    {
        for (int i = 0; i < vertex; i++)
        {
            if(!visited[i])
            {
                if(ShortDist[i] < ShortDist[traverse])
                {
                    traverse = i;
                }
            }
        }
    }
    //Updating the shortest distance
    for (int i = 0; i < vertex; i++)
    {
        if (!visited[i] && adjM[traverse][i])
        {
            if (ShortDist[traverse] + adjM[traverse][i] < ShortDist[i])
            {
                ShortDist[i] = ShortDist[traverse] + adjM[traverse][i];
                hops[i] = traverse;
            }
        } 
    }
    visited[traverse] = 1;
    for(int i = 0; i < vertex; i++)
    {
        flag = 0;
        if (!visited[i])
        {
            traverse = 1;
            flag = 1;
            break;
        }
    }
    cout<<"\n Routing Table for Source ";
    cout<<"\n Vertex    Distance    Closest Node "<<endl;
    for (int i = 0; i < vertex; i++)
    {
        cout<<"    "<<i<<"     "<<ShortDist[i]<<"      "<<hops[i]<<endl;
    }
    return 0;
}