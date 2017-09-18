// Knapsack Problem using Dyanamic Programming

#include<bits/stdc++.h>
using namespace std;

int K[1000][1000];
int max(int a, int b) { return (a > b)? a : b; }

int knapSack(int W, int wt[], int val[], int n)
{
  int i, w;
//   int K[n+1][W+1];

  for (i = 0; i <= n; i++)
  {
    for (w = 0; w <= W; w++)
    {
        if (i==0 || w==0)
            K[i][w] = 0;
        else if (wt[i-1] <= w)
            K[i][w] = max(val[i-1] + K[i-1][w-wt[i-1]],  K[i-1][w]);
        else
            K[i][w] = K[i-1][w];
    }
  }
  return K[n][W];
}

int main()
{
    FILE *fin1 = freopen("inputForDP.txt", "r", stdin);
    FILE *fin2 = freopen("outputByDP.txt", "w", stdout);
    
    int  W;
    int n;
    
    cout<<"Max weight of knapsack: ";
    cin>>W;
    cout<<W<<endl;

    cout<<"Total number of items: ";
    cin>>n;
    cout<<n<<endl;

    int val[n+10];
    int wt[n+10];

    cout<<"Benefit by each item: ";
    for(int i=0; i<n; i++){
        cin>>val[i];
        cout<<val[i]<<" ";
    }
    cout<<endl;
    cout<<"Weight of each item:  ";
    for(int i=0; i<n; i++){
        cin>>wt[i];
        cout<<wt[i]<<" ";
    }
    cout<<endl;
    printf("max benefit = %d", knapSack(W, wt, val, n));
    cout<<endl<<endl;
    for (int i = 0; i <= n; i++)
    {
        cout<<i<<" => ";
      for (int w = 0; w <= W; w++)
      {
        cout<<K[i][w]<<" ";
      }
      cout<<endl;
    }
    return 0;
}