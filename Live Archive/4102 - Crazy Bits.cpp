#include <cstdio>
#include <cstring>
#include <climits>
#include <algorithm>
#include <vector>

using namespace std;

#define MAX_V 100

int V,cost[MAX_V][MAX_V];
int lx[MAX_V],ly[MAX_V];
int max_match,xy[MAX_V],yx[MAX_V],prev[MAX_V];
bool S[MAX_V],T[MAX_V];
int slack[MAX_V],slackx[MAX_V];
int q[MAX_V],head,tail;

void init_labels(){
    memset(lx,0,sizeof(lx));
    memset(ly,0,sizeof(ly));
  
    for(int x = 0;x<V;++x)
        for(int y = 0;y<V;++y)
            lx[x] = max(lx[x],cost[x][y]);
}

void update_labels(){
    int x,y,delta = INT_MAX;
  
    for(y = 0;y<V;++y) if(!T[y]) delta = min(delta,slack[y]);
    for(x = 0;x<V;++x) if(S[x]) lx[x] -= delta;
    for(y = 0;y<V;++y) if(T[y]) ly[y] += delta;
    for(y = 0;y<V;++y) if(!T[y]) slack[y] -= delta;
}

void add_to_tree(int x, int prevx){
    S[x] = true;
    prev[x] = prevx;
  
    for(int y = 0;y<V;++y){
        if(lx[x]+ly[y]-cost[x][y]<slack[y]){
            slack[y] = lx[x]+ly[y]-cost[x][y];
            slackx[y] = x;
        }
    }
}

void augment(){
    int x,y,root;
    head = tail = 0;
    memset(S,false,sizeof(S));
    memset(T,false,sizeof(T));
    memset(prev,-1,sizeof(prev));
    
    for(x = 0;x<V;++x){
        if(xy[x]==-1){
            q[tail++] = root = x;
            prev[root] = -2;
            S[root] = true;
            break;
        }
    }
  
    for(y = 0;y<V;++y){
        slack[y] = lx[root]+ly[y]-cost[root][y];
        slackx[y] = root;
    }
  
    while(true){
        while(head<tail){
            x = q[head++];
      
            for(y = 0;y<V;++y){
                if(cost[x][y]==lx[x]+ly[y] && !T[y]){
                    if(yx[y]==-1) break;
          
                    T[y] = true;
                    q[tail++] = yx[y];
                    add_to_tree(yx[y],x);
                }
            }
      
            if(y<V) break;
        }
    
        if(y<V) break;
    
        update_labels();
        head = tail = 0;
    
        for(y = 0;y<V;++y){
            if(!T[y] && slack[y]==0){
                if(yx[y]==-1){
                    x = slackx[y];
                    break;
                }
        
                T[y] = true;
        
                if(!S[yx[y]]){
                    q[tail++] = yx[y];
                    add_to_tree(yx[y],slackx[y]);
                }
            }
        }
    
        if(y<V) break;
    }
  
    ++max_match;
  
    for(int cx = x,cy = y,ty;cx!=-2;cx = prev[cx],cy = ty){
        ty = xy[cx];
        yx[cy] = cx;
        xy[cx] = cy;
    }
}

int hungarian(){
    int ret = 0;
    max_match = 0;
    memset(xy,-1,sizeof(xy));
    memset(yx,-1,sizeof(yx));
  
    init_labels();
    for(int i = 0;i<V;++i) augment();
    for(int x = 0;x<V;++x) ret += cost[x][xy[x]];
  
    return ret;
}

int main(){
    int n,a[16],b[16];
    int M[16][12],M2[16][12];
    
    while(true){
        scanf("%d",&n);
        if(n == 0) break;
        
        for(int i = 0;i < n;++i) scanf("%d",&a[i]);
        for(int i = 0;i < n;++i) scanf("%d",&b[i]);
        
        for(int i = 0;i < n;++i){
            for(int j = 0;j < 12;++j){
                M[i][j] = (a[i] & (1 << j))? 1 : 0;
                M2[i][j] = (b[i] & (1 << j))? 1 : 0;
            }
        }
        
        vector<int> r1,c1,r2,c2;
        
        for(int i = 0;i < n;++i){
            for(int j = 0;j < 12;++j){
                if(M[i][j] != M2[i][j]){
                    if(M[i][j] == 0){
                        r1.push_back(i);
                        c1.push_back(j);
                    }else{
                        r2.push_back(i);
                        c2.push_back(j);
                    }
                }
            }
        }
        
        V = r1.size();
        
        if(r2.size() != V) puts("Impossible");
        else{
            for(int i = 0;i < V;++i)
                for(int j = 0;j < V;++j)
                   cost[i][j] = -abs(r1[i] - r2[j]) - abs(c1[i] - c2[j]);
            
            printf("%d\n",-hungarian());
        }
    }
    
    return 0;
}
