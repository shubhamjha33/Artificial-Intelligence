#include<cstdio>
#include<vector>
#include<queue>
#include<set>
using namespace std;
#define FUTILITY 20000
class edge{
public:
    vector<int> succ;
};
vector< vector<edge> > succ_list;
vector<int> expanded,cost;
set<int> soln;
int solved;
int get_min_cost(int init){
    int mincost=FUTILITY,minarc,i,j;
    if(soln.find(init)!=soln.end())
    {
        solved=1;
        expanded[init]=1;
        return 0;
    }
    if(expanded[init]==0){
        minarc=-1;
        int tempcost;
        for(i=0;i<succ_list[init].size();i++){
            tempcost=succ_list[init][i].succ.size();
            for(j=0;j<succ_list[init][i].succ.size();j++){
                tempcost+=cost[succ_list[init][i].succ[j]];
            }
            if(mincost>tempcost){
                mincost=tempcost;
            }
        }
        expanded[init]=1;
        return mincost;
    }
    int tempcost,k;
    for(i=0;i<succ_list[init].size();i++){
        tempcost=succ_list[init][i].succ.size();
        for(j=0;j<succ_list[init][i].succ.size();j++){
            tempcost+=get_min_cost(succ_list[init][i].succ[j]);
        }
        mincost=min(mincost,tempcost);
    }
    return mincost;
}
int ao_algorithm(int init){
    int result;
    while(solved!=1){
        result=get_min_cost(init);
    }
    result=get_min_cost(init);
    return result;
}
int main()
{
    solved=0;
    int n,i,j,m,k,c,num;
    scanf("%d",&n);
    for(i=0;i<n;i++){
        expanded.push_back(0);
        scanf("%d",&c);
        cost.push_back(c);
    }
    scanf("%d",&m);
    while(m--){
        scanf("%d",&num);
        soln.insert(num);
    }
    succ_list=vector< vector<edge> >(n+2);
    edge e;
    for(i=0;i<n;i++){
        scanf("%d",&m);
        for(j=0;j<m;j++){
            scanf("%d",&k);
            e.succ.clear();
            while(k--){
                scanf("%d",&num);
                e.succ.push_back(num);
            }
            succ_list[i].push_back(e);
        }
    }
    printf("Solution: %d\n",ao_algorithm(0));
    return 0;
}
