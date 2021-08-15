#include<bits/stdc++.h>
#define pb push_back
#define mid (l+r>>1)
#define memu(a) memset(a,0x3f,sizeof(a))
using namespace std;
const int N=4e5+5,M=4e5+5;

int n,m,Q,u,v,tp,cnt,idx,w[N<<1],dfn[N],low[N],sta[N];
int tot,id[N<<1],wn[N<<1],fa[N<<1],d[N<<1],siz[N<<1],son[N<<1],top[N<<1];
int hd[N],to[M<<1],ne[M<<1],tc;
int val[N<<3];
char op;
vector<int> G[N<<1];
multiset<int> s[N<<1];
multiset<int>::iterator it;

inline void add(int,int);
inline void tarjan(int);
inline void dfs1(int);
inline void dfs2(int,int);
inline void build(int,int,int);
inline void mfy(int,int,int,int,int);
inline int ask(int,int);
inline int qry(int,int,int,int,int);

int main() {
    memu(w);
    scanf("%d%d%d",&n,&m,&Q); cnt=n;
    for(int i=1;i<=n;i++) scanf("%d",&w[i]);
    for(int i=1;i<=m;i++) scanf("%d%d",&u,&v), add(u,v);
    tarjan(1);
    d[1]=1; dfs1(1); dfs2(1,1);
    build(1,1,cnt);
    while(Q--) {
        cin>>op;
        scanf("%d%d",&u,&v);
        if(op=='A') printf("%d\n",ask(u,v));
        else {
            int f=fa[u];
            if(f) {
                it=s[f].lower_bound(w[u]);
                s[f].erase(it);
                s[f].insert(v);
                mfy(1,1,cnt,id[f],w[f]=*s[f].begin());
            }
            mfy(1,1,cnt,id[u],w[u]=v);
        }
    }
    return 0;
}

inline void add(int x,int y) {
    to[++tc]=y; ne[tc]=hd[x]; hd[x]=tc;
    to[++tc]=x; ne[tc]=hd[y]; hd[y]=tc;
}

inline void tarjan(int x) {
    dfn[x]=low[x]=++idx; sta[++tp]=x;
    for(int i=hd[x],y,z;i;i=ne[i]) {
        if(!dfn[y=to[i]]) {
            tarjan(y);
            low[x]=min(low[x],low[y]);
            if(dfn[x]<=low[y]) {
                G[x].pb(++cnt); G[cnt].pb(x);
                do {
                    z=sta[tp--]; G[z].pb(cnt); G[cnt].pb(z);
                }while(z^y);
            }
        }
        else low[x]=min(low[x],dfn[y]);
    }
}

inline void dfs1(int x) {
    siz[x]=1;
    for(int i=0,y;i<G[x].size();i++) {
        if((y=G[x][i])==fa[x]) continue;
        fa[y]=x; d[y]=d[x]+1;
        dfs1(y);
        siz[x]+=siz[y]; 
        if(siz[y]>siz[son[x]]) son[x]=y;
        if(x>n) s[x].insert(w[y]);
    }
    if(x>n) w[x]=*s[x].begin();
}

inline void dfs2(int x,int t) {
    id[x]=++tot; top[x]=t; wn[tot]=w[x];
    if(son[x]) dfs2(son[x],t);
    for(int i=0,y;i<G[x].size();i++)
        if((y=G[x][i])!=fa[x] && y!=son[x]) dfs2(y,y);
} 

inline void build(int k,int l,int r) {
    if(l==r) { val[k]=wn[l]; return ; }
    build(k<<1,l,mid); build(k<<1|1,mid+1,r);
    val[k]=min(val[k<<1],val[k<<1|1]);
}

inline void mfy(int k,int l,int r,int p,int x) {
    if(l==r) { val[k]=x; return ; }
    if(p<=mid) mfy(k<<1,l,mid,p,x);
    else mfy(k<<1|1,mid+1,r,p,x);
    val[k]=min(val[k<<1],val[k<<1|1]);
}

inline int qry(int k,int l,int r,int L,int R) {
    if(L<=l && R>=r) return val[k];
    int ret=1e9;
    if(L<=mid) ret=min(ret,qry(k<<1,l,mid,L,R));
    if(R>mid) ret=min(ret,qry(k<<1|1,mid+1,r,L,R));
    return ret;
}

inline int ask(int x,int y) {
    int ret=1e9;
    while(top[x]!=top[y]) {
        if(d[top[x]]<d[top[y]]) swap(x,y);
        ret=min(ret,qry(1,1,cnt,id[top[x]],id[x]));
        x=fa[top[x]];
    }
    if(id[x]>id[y]) swap(x,y);
    ret=min(ret,qry(1,1,cnt,id[x],id[y]));
    if(x>n) ret=min(ret,w[fa[x]]);
    return ret;
}
