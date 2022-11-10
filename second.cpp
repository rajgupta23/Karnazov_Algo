// CS-302 Analysis and design of algorithm

#include <bits/stdc++.h>
using namespace std;

#define mn 1000
int n;
int cap[mn][mn];
int flow[mn][mn];
int level[mn];
vector<int> level_matrix[mn];

void initialize()
{
    for (int i = 0; i < mn; i++)
    {
        level[i] = -1;
    }
}

void assign_level(int src)
{
    queue<int> q;
    level[src] = 0;
    q.push(src);
    cout << "YES" << endl;
    while (!q.empty())
    {
        int x = q.front();
        q.pop();

        for (int j = 1; j <= n; j++)
        {
            if (cap[x][j] > 0)
            {
                if (level[j] < level[x] + 1)
                {
                    level[j] = level[x] + 1;
                    q.push(j);
                }
            }
        }
    }

    for (int i = 0; i <= n; i++)
    {
        cout << level[i] << " ";
    }
    cout << endl;
}

void get_level_matrix()
{
    for (int i = 0; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (level[j] == i)
                level_matrix[i].push_back(j);
        }
    }
    for (int i = 0; i <= n; i++)
    {
        for (auto x : level_matrix[i])
        {
            cout << x << " ";
        }
        cout << endl;
    }
}
bool isBal(vector<int> &a)
{
    for (auto x : a)
        if (x == 0)
            return 0;
    return 1;
}

int main()
{
    initialize();
    cout << "Enter number of vertices: ";
    cin >> n;
    int incoming_flow[n + 1];
    int outgoing_flow[n + 1];

    for (int i = 0; i <= n + 1; i++)
    {
        incoming_flow[i] = 0;
        outgoing_flow[i] = 0;
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
        cap[right][left] = -w; // for par. identification
        cout << "Inserted Edge !!\n";
    }
    int source, sink;
    cout << "Enter source vertex : ";
    cin >> source;
    cout << "Enter sink vertex : ";
    cin >> sink;

    assign_level(source);
    get_level_matrix();
    vector<int> balanced(n + 1, 1), pf_blocked(n + 1), bf_blocked(n + 1);

    int is_balanced = 0;
    int f = 1;
    int maxLev = 0;
    for (int i = 0; i < mn; i++)
    {
        if (level_matrix[i].size() == 0)
        {
            maxLev = i - 1;
            break;
        }
    }
    bool flag = 0;
    while (!flag) // n-1
    {
        // forward
        outgoing_flow[source] = 0;
        for (int i = 1; i <= n; i++)
        {
            if (cap[source][i] > 0 && bf_blocked[i] == 0 and pf_blocked[i] == 0)
            {
                flow[source][i] = cap[source][i];
                incoming_flow[i] = cap[source][i];
                outgoing_flow[source] += cap[source][i];
                balanced[i] = 0;
            }
        }
        for (int lv = 1; lv < maxLev; lv++)
        {
            cout << "lv = " << lv << " \n";
            for (int i : level_matrix[lv])
            {
                cout << "i = " << i << endl;
                if (bf_blocked[i] == 0)
                {
                    int needed = incoming_flow[i] - outgoing_flow[i];
                    if (needed > 0)
                    {
                        int got = 0;
                        for (int child = 1; child <= n; child++)
                        {
                            if (cap[i][child] > 0 and pf_blocked[child] == 0 and bf_blocked[child] == 0) // child which is not blocked
                            {

                                cout << "child " << child << " "
                                     << "cap" << cap[i][child];
                                int available = cap[i][child] - flow[i][child];
                                if (available > 0)
                                {
                                    int took = min(available, max(0, needed - got));
                                    got += took;
                                    flow[i][child] += took;
                                    incoming_flow[child] += took;
                                    cout << "took " << took << endl;
                                }
                            }
                        }

                        outgoing_flow[i] += got;

                        if (outgoing_flow[i] < incoming_flow[i])
                        {
                            pf_blocked[i] = 1;
                        }
                        balanced[i] = (incoming_flow[i] - outgoing_flow[i]);
                    }
                    else
                    {
                        balanced[i] = (incoming_flow[i] - outgoing_flow[i]);
                    }
                }
                else // bf_blocked case ie, gin<gout or needed<0
                {
                    int needed = incoming_flow[i] - outgoing_flow[i];
                    if (needed < 0)
                    {
                        int got = 0;
                        for (int child = 0; child <= n; child++)
                        {
                            if (cap[i][child] > 0) // valid child
                            {
                                int available = flow[i][child];
                                int took = min(available, max(0, got - needed));
                                got -= took;
                                flow[i][child] -= took;
                                incoming_flow[child] -= took;
                            }
                        }
                        outgoing_flow[i] += got;
                        balanced[i] = (incoming_flow[i] - outgoing_flow[i]);
                    }
                }
            }
        }
        flag = 1;

        // check for pruning
        for (int i = 2; i <= n - 1; i++)
        {
            if (incoming_flow[i] != outgoing_flow[i])
                flag = 0;
        }
        if (flag)
            break;
        // backflow

        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (cap[i][j] > 0)
                    cout << i << " " << j << " "
                         << " = " << flow[i][j] << endl;
            }
        }

        incoming_flow[sink] = 0;
        for (int i = 1; i <= n; i++)
        {

            if (cap[i][sink] > 0 && bf_blocked[i] == 0 and pf_blocked[i] == 0)
            {
                incoming_flow[sink] += cap[i][sink];
                outgoing_flow[i] += (cap[i][sink] - flow[i][sink]);
                flow[i][sink] = cap[i][sink];
                balanced[i] = incoming_flow[i] == outgoing_flow[i];
            }
        }

        for (int lv = maxLev - 1; lv >= 1; lv--)
        {

            for (auto i : level_matrix[lv])
            {
                if (pf_blocked[i] == 0)
                {
                    cout << "GG\n";

                    int needed = outgoing_flow[i] - incoming_flow[i];
                    if (needed > 0)
                    {
                        int got = 0;
                        for (int child = 0; child <= n; child++)
                        {

                            if (cap[child][sink] and pf_blocked[child] == 0 and bf_blocked[child] == 0)
                            {
                                int available = -flow[child][i] + cap[child][i];
                                if (available > 0)
                                {
                                    int took = min(available, max(0, needed - got));
                                    got += took;
                                    flow[child][i] += took;
                                    outgoing_flow[child] += took;
                                    cout << "took " << took << endl;
                                }
                            }
                        }
                        incoming_flow[i] += got;

                        if (outgoing_flow[i] > incoming_flow[i])
                        {
                            bf_blocked[i] = 1;
                        }
                        balanced[i] = (incoming_flow[i] - outgoing_flow[i]);
                    }
                    else
                    {
                        balanced[i] = (incoming_flow[i] - outgoing_flow[i]);
                    }
                }
                else // pf block gin>gout
                {
                    cout << "2GG " << i << endl;
                    int needed = outgoing_flow[i] - incoming_flow[i];
                    if (needed < 0)
                    {
                        int got = 0;
                        for (int child = 1; child <= n; child++)
                        {
                            cout << "Child " << child << " ";
                            if (cap[child][i] > 0) // valid child
                            {
                                int available = flow[child][i];
                                int took = min(available, max(0, got - needed));
                                got -= took;
                                flow[child][i] -= took;
                                cout << "took " << took << "avai " << available << endl;
                                outgoing_flow[child] -= took;
                            }
                        }
                        incoming_flow[i] += got;
                        balanced[i] = (incoming_flow[i] - outgoing_flow[i]);
                    }
                }
            }
        }

        flag = 1;

        // check for pruning
        for (int i = 2; i <= n - 1; i++)
        {
            if (incoming_flow[i] != outgoing_flow[i])
                flag = 0;
        }
        if (flag)
            break;
        break;
    }
    cout << "cnjdsn\n";
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (cap[i][j] > 0)
                cout << i << " " << j << " "
                     << " = " << flow[i][j] << endl;
        }
    }
    int ans = 0;
    for (int i = 0; i <= n; i++)
    {
        if (cap[1][i] > 0)
            ans += flow[1][i];
    }
    cout << "ans " << ans << endl;
}
