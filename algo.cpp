// CS-302 Analysis and design of algorithm

#include<bits/stdc++.h>
using namespace std;

#define mn 1000

int n;
int cap[mn][mn];
int flow[mn][mn];
int level[mn];
int level_matrix[mn][mn];

void initialize()
{
    for(int i=0;i<mn;i++)
    {
        level[i]=-1;
        for(int j=0;j<mn;j++)
        {
            cap[i][j]=0;
            flow[i][j]=0;
            level_matrix[i][j]=0;
        }
    }
}

void assign_level(int src)
{
    queue<int> q;
    level[src]=0;
    q.push(src);
    cout<<"YES"<<endl;
    while(!q.empty())
    {
        int x = q.front();
        q.pop();

        for(int j=1;j<=n;j++)
        {
            if(cap[x][j]!=0)
            {
                if(level[j]<level[x]+1) level[j]=level[x]+1;
                q.push(j);
            }
        }
    }

    for(int i=0;i<=n;i++)
    {
        cout<<level[i]<<" ";
    }
    cout<<endl;

}

void get_level_matrix()
{
    for(int i=0;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(level[j]==i) level_matrix[i][j]=1;
        }
    }
    for(int i=0;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            cout<<level_matrix[i][j]<<" ";
        }
        cout<<endl;
    }
}


int main()
{
    initialize();
    cout << "Enter number of vertices: ";
    cin>>n;
    int incoming_cap[n+1];
    int outgoing_cap[n+1];

    for(int i=0;i<=n+1;i++)
    {
        incoming_cap[i]=0;
        outgoing_cap[i]=0;
    }

    printf("Enter edges and capacity as three spaced integers : ");
    printf("\n\nNote That : If you inter x y z then it means egdes goes from x to y i.e x->y , having weight z \n\n");
    printf("\n\nMake sure you use 1-based indexing for graph nodes.\n\n");
    printf("Once you are done, enter -1 to quit.\n");

    while (1)
    {
        int left;  // left vertex
        int right; // right vertex
        int w;   // capacity of an edge
        cin >> left ;
        if (left == -1){
            break;
        }
        cin >> right >> w;
        cap[left][right] = w;
        //incoming_cap[right]+=w;
        //outgoing_cap[left]+=w;
        cout << "Inserted Edge !!\n";
    }
    int source, sink;
    cout << "Enter source vertex : ";
    cin >> source ;
    cout << "Enter sink vertex : ";
    cin >> sink;

    assign_level(source);
    get_level_matrix();

    int balanced[n+1];
    for(int i=0;i<=n;i++) balanced[i]=1;

    int blocked[n+1];
    for(int i=0;i<=n;i++) blocked[i]=0;

    int is_balanced=0;
    int f=1;

    while(is_balanced==0)
    {

        // Forward

        if(f==1){

        // step 1 :


        for(int i=1;i<=n;i++)
        {
            if(cap[source][i]!=0 && blocked[i]==0)
            {
                flow[source][i]=cap[source][i];
                incoming_cap[i]=cap[source][i];
                outgoing_cap[source]+=cap[source][i];
                balanced[i]=0;
            }
        }

        // step 2 : level 1 onwards
        
        for(int i=0;i<n;i++)
        {
            int lv = pr[i].level;
            int nd = pr[i].node;
            if(lv==0) continue;
            
            if(incoming_cap[nd]>outgoing_cap[nd])
            {
                int to_increase = incoming_cap[nd]-outgoing_cap[nd];
                
                for(int j=0;j<=n;j++)
                {
                    if(cap[nd][j]!=0 && blocked[j]==0)
                    {
                        int child = j;
                        
                        if(to_increase>0)
                        {
                            if(cap[nd][child]<=to_increase)
                            {
                                to_increase = to_increase - cap[nd][child];
                                flow[nd][child] = flow[nd][child] + cap[nd][child];
                                outgoing_cap[nd] = outgoing_cap[nd] + 
                                incoming_cap[child]+= cap[nd][child]-flow[nd][child];
                            }
                            else 
                            {
                                flow[nd][child] = flow[nd][child] + (cap[nd][child]-to_increase);
                                incoming_cap[child]+=cap[nd][child]-to_increase
                                to_increase=0;
                            }
                        }
                        else break;
                        
                    }
                }
                
                if(incoming_cap[nd]!=outgoing_cap[nd]) blocked[nd]=1;
                
                
            }
            else if(incoming_cap[nd]<outgoing_cap[nd])
            {
                int to_decrease = outgoing_cap[nd] - incoming_cap[nd] ;
                
                for(int j=0;j<=n;j++)
                {
                    if(cap[nd][j]!=0 && blocked[j]==0)
                    {
                        int child = j;
                        
                        if(to_decrease>0)
                        {
                            if(flow[nd][child]>=to_decrease)
                            {
                                int temp = flow[nd][child];
                                flow[nd][child] = flow[nd][child] - to_decrease;
                                outgoing_cap[nd]=outgoing_cap[nd]-to_decrease;
                                to_decrease = to_decrease - temp;
                                
                            }
                            else
                            {
                                to_decrease = to_decrease - flow[nd][child];
                                outgoing_cap[nd] = outgoing_cap[nd] - flow[nd][child];
                                flow[nd][child]=0;
                            }
                        }
                        
                    }
                }
            }
            
            
        }
        

        }

        // Backward
        else {


        // step 1 :



        }

        // Check for all balance

        int c=0;
        for(int i=1;i<=n;i++)
        {
            if(balanced[i]==0) c=1;
        }
        if(c==0) is_balanced=1;
    }

}
