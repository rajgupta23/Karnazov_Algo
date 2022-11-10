// CS-302 Analysis and design of algorithm

#include <bits/stdc++.h>
using namespace std;

#define mn 1000

int n;
int cap[mn][mn];
int flow[mn][mn];
int level[mn];
pair<int, int> pr[mn];

void initialize()
{
    for (int i = 0; i < mn; i++)
    {
        level[i] = -1;
        for (int j = 0; j < mn; j++)
        {
            cap[i][j] = 0;
            flow[i][j] = 0;
        }
    }
}

void assign_level(int src)
{
    queue<int> q;
    level[src] = 0;
    q.push(src);
    while (!q.empty())
    {
        int x = q.front();
        q.pop();

        for (int j = 1; j <= n; j++)
        {
            if (cap[x][j]>0)
            {
                if (level[j] < level[x] + 1)
                    level[j] = level[x] + 1;
                q.push(j);
            }
        }
    }

    for (int i = 0; i <= n; i++)
    {
        cout << level[i] << " ";
    }
    cout << endl;
}

int main()
{
    initialize();
    cout << "Enter number of vertices: ";
    cin >> n;
    int incoming_cap[n + 1];
    int outgoing_cap[n + 1];

    for (int i = 0; i <= n ; i++)
    {
        incoming_cap[i] = 0;
        outgoing_cap[i] = 0;
    }

    printf("Enter edges and capacity as three spaced integers : ");
    printf("\n\nNote That : If you inter x y z then it means egdes goes from x to y i.e x->y , having weight z \n\n");
    printf("\n\nMake sure you use 1-based indexing for graph nodes.\n\n");
    printf("Once you are done, enter -1 to quit.\n");

    while (1)
    {
        int left;  // left vertex
        int right; // right vertex
        int w;     // capacity of an edge
        cin >> left;
        if (left == -1)
        {
            break;
        }
        cin >> right >> w;
        cap[left][right] = w;
        cap[right][left] = -w; // for par identification

        cout << "Inserted Edge !!\n";
    }
    int source, sink;
    cout << "Enter source vertex : ";
    cin >> source;
    cout << "Enter sink vertex : ";
    cin >> sink;

    assign_level(source);

    for (int i = 1; i <= n; i++)
    {
        pr[i] = {level[i], i};
    }

    int balanced[n + 1];
    for (int i = 0; i <= n; i++)
        balanced[i] = 0;

    int blocked[n + 1];
    for (int i = 0; i <= n; i++)
        blocked[i] = 0;

    int is_balanced = 0;
    int f = 1;
    sort(pr, pr + n + 1);

    balanced[source]=1;
    balanced[sink]=1;
    blocked[source]=0;
    blocked[sink]=0;

    while (!is_balanced)
    {
        // Forward

        if (f == 1)
        {
            // step 1 :

            for (int i = 1; i <= n; i++)
            {
                if (cap[source][i] > 0 && blocked[i] == 0)
                {
                    int increment = 0;
                    int oldflow = flow[source][i];
                    flow[source][i] = cap[source][i];
                    increment = flow[source][i] - oldflow;
                    incoming_cap[i] +=increment;
                    outgoing_cap[source] += increment;
                }
            }

            // step 2 : level 1 onwards

            for (int i = 1; i <= n; i++)
            {
                int lv = pr[i].first;
                int nd = pr[i].second;

                if (nd==source || nd==sink)
                    continue;

                if (incoming_cap[nd] > outgoing_cap[nd])
                {
                    int to_increase = incoming_cap[nd] - outgoing_cap[nd];

                    for (int j = 0; j <= n; j++)
                    {
                        if (cap[nd][j] > 0 && blocked[j] == 0)
                        {
                            int child = j;

                            if (to_increase > 0)
                            {
                                if (cap[nd][child]-flow[nd][child] >= to_increase)
                                {

                                    int incremented = 0;
                                    int oldflow = flow[nd][child];
                                    flow[nd][child] = flow[nd][child] + to_increase;
                                    incremented = flow[nd][child] - oldflow;
                                    outgoing_cap[nd] = outgoing_cap[nd] + incremented;
                                    incoming_cap[child] += incremented;
                                    to_increase = to_increase - incremented;

                                }
                                else
                                {
                                    int incremented=0;
                                    int oldflow = flow[nd][child];
                                    flow[nd][child]=cap[nd][child];
                                    incremented = flow[nd][child] - oldflow;
                                    outgoing_cap[nd] = outgoing_cap[nd] + incremented;
                                    incoming_cap[child] += incremented;
                                    to_increase = to_increase - incremented;
                                }
                            }
                            else
                                break;
                        }
                    }

                    if(incoming_cap[nd]!=outgoing_cap[nd])
                    {
                        blocked[nd]=1;
                        balanced[nd]=0;
                    }
                    else balanced[nd]=1;

                }
                else if (incoming_cap[nd] < outgoing_cap[nd])
                {
                    int to_decrease = outgoing_cap[nd] - incoming_cap[nd];

                    for (int j = 0; j <= n; j++)
                    {
                        if (cap[nd][j] > 0 && blocked[j] == 0)
                        {
                            int child = j;

                            if (to_decrease > 0)
                            {
                                if (flow[nd][child] >= to_decrease)
                                {
                                    int decremented =0;
                                    int oldflow = flow[nd][child];
                                    flow[nd][child] = flow[nd][child] - to_decrease;
                                    decremented = oldflow - flow[nd][child];
                                    outgoing_cap[nd]-=decremented;
                                    incoming_cap[child]-=decremented;

                                }
                                else
                                {
                                    int decremented =0;
                                    int oldflow = flow[nd][child];
                                    decremented = flow[nd][child];
                                    flow[nd][child]=0;
                                    outgoing_cap[nd]-=decremented;
                                    incoming_cap[child]-=decremented;
                                }
                            }
                            else break;
                        }
                    }

                    if(incoming_cap[nd]!=outgoing_cap[nd])
                    {
                        blocked[nd]=1;
                        balanced[nd]=0;
                    }
                    else balanced[nd]=1;

                }
                else
                {
                    balanced[nd]=1;
                }
            }

            f=0;
        }

        ///// Backward
        else
        {
             // step 1 :
            for (int i = 1; i <= n; i++)
            {
                if (cap[sink][i] < 0 && blocked[i]==0)
                {
                    int increment =0;
                    int oldflow = flow[i][sink];
                    flow[i][sink] = cap[i][sink];
                    increment = flow[i][sink] - oldflow;
                    outgoing_cap[i] += increment;
                    incoming_cap[sink]+=increment;
                }
            }

            // step 2 : from n-1 level

            for (int i = n; i >= 1; i--)
            {
                int nd = pr[i].second;
                int lv = pr[i].first;

                if(nd==source || nd==sink) continue;

                if (incoming_cap[nd] < outgoing_cap[nd])  ///////////////////////////////////////////
                {
                    int to_increase = outgoing_cap[nd] - incoming_cap[nd];

                    for (int j = 0; j <= n; j++)
                    {

                        if(cap[nd][j]<0 && blocked[j]==0)
                        {
                            int parent = j ;

                            if(to_increase>0)
                            {
                                if(cap[parent][nd]-flow[parent][nd]>=to_increase)
                                {
                                    int incremented =0 ;
                                    int oldflow = flow[parent][nd];
                                    flow[parent][nd] += to_increase;
                                    to_increase=0;
                                    incremented = flow[parent][nd] - oldflow;
                                    outgoing_cap[parent]+=incremented;
                                    incoming_cap[nd]+=incremented;
                                }
                                else
                                {
                                    int incremented =0 ;
                                    int oldflow = flow[parent][nd];
                                    flow[parent][nd] = cap[parent][nd];
                                    incremented = flow[parent][nd] - oldflow;
                                    outgoing_cap[parent]+=incremented;
                                    incoming_cap[nd]+=incremented;
                                    to_increase-=incremented;
                                }
                            }
                            else break;

                        }

                    }
                    if (incoming_cap[nd] != outgoing_cap[nd])
                    {
                        blocked[nd] = 1;
                        balanced[nd]=0;
                    }
                    else balanced[nd]=1;

                }
                else if (incoming_cap[nd] > outgoing_cap[nd])
                {
                    int to_decrease = incoming_cap[nd]-outgoing_cap[nd];

                    for(int j = n ; j>=0 ;j--)
                    {

                        if(cap[nd][j]<0 && blocked[j]==0)
                        {
                            int parent = j;
                            if(to_decrease>0)
                            {
                                if(flow[parent][nd]>=to_decrease)
                                {
                                    int decrement = 0;
                                    int oldflow = flow[parent][nd];
                                    flow[parent][nd] = flow[parent][nd] - to_decrease;
                                    decrement = oldflow - flow[parent][nd];
                                    outgoing_cap[parent]-=decrement;
                                    incoming_cap[nd]-=decrement;
                                    to_decrease=0;
                                }
                                else
                                {
                                    int decrement = 0;
                                    int oldflow = flow[parent][nd];
                                    flow[parent][nd]=0;
                                    decrement = oldflow - flow[parent][nd];
                                    outgoing_cap[parent]-=decrement;
                                    incoming_cap[nd]-=decrement;
                                    to_decrease = to_decrease - decrement;
                                }
                            }
                            else break;
                        }
                    }

                    if(incoming_cap[nd]!=outgoing_cap[nd])
                    {
                        blocked[nd]=1;
                        balanced[nd]=0;
                    }
                    else
                    {
                        balanced[nd]=1;
                    }

                }
            }
            f=1;

        }

        // Check for all balance

        int c = 0;
        for (int i = 1; i <= n; i++)
        {
            if (balanced[i] == 0)
            {
                c = 1;
                break;
            }

        }
        if (c == 0) is_balanced = 1;
    }
    cout<<"Complete....huehue"<<endl;

}
