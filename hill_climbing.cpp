#include<cstdio>
#include<map>
#include<algorithm>
#include<climits>
#include<vector>
#include<queue>
using namespace std;
class node{
    public:
    int id;
    int huer_cost;
        node(){
            id=INT_MAX;
            huer_cost=INT_MAX;
        }
        node(int i){
            id=i;
            huer_cost=INT_MAX;
        }
        node(int i,int huer){
            id=i;
            huer_cost=huer;
        }
        node(const node& n){
            id=n.id;
            huer_cost=n.huer_cost;
        }
        bool operator<(const node& n)const{
            return huer_cost>n.huer_cost;
        }
        int total_cost(){
            return huer_cost;
        }
};
map<int,int> fwd_map,inv_map,parent;
void init_states(){
    int arr[9],i,init,counter=0;
    for(i=0;i<9;i++)
        arr[i]=i;
    do
    {
        init=0;
        for(i=0;i<9;i++){
            init=10*init+arr[i];
        }
        counter++;
        fwd_map.insert(make_pair(init,counter));
        inv_map.insert(make_pair(counter,init));
    }while(next_permutation(arr,arr+9));
}
int compute_cost(int state){
    int cost=0,temp=state;
    int arr[9],i=0,j;
    if(state<100000000){
        arr[i]=0;
        i++;
    }
    j=8;
    while(j>=i){
        arr[j]=temp%10;
        j--;
        temp/=10;
    }
    for(j=1;j<=8;j++){
        for(i=0;i<9;i++){
            if(arr[i]==j)
                break;
        }
        temp=i-(j-1);
        if(temp<0)
            temp=-temp;
        cost+=temp;
    }
    for(i=0;i<9;i++){
        if(arr[i]==0)
            break;
    }
    temp=8-i;
    if(temp<0)
        temp=-temp;
    cost+=temp;
    return cost;
}
vector<int> next_states(int state){
    vector<int> result;
    int mat[3][3],i,j,temp=state,next_mat[3][3],next_val,zerox,zeroy;
    for(i=2;i>=0;i--){
        for(j=2;j>=0;j--){
            next_mat[i][j]=mat[i][j]=temp%10;
            temp/=10;
            if(mat[i][j]==0){
                zerox=i;
                zeroy=j;
            }
        }
    }
    if(zerox>0){
        temp=next_mat[zerox][zeroy];
        next_mat[zerox][zeroy]=next_mat[zerox-1][zeroy];
        next_mat[zerox-1][zeroy]=temp;
        next_val=0;
        for(i=0;i<3;i++){
            for(j=0;j<3;j++){
                next_val=10*next_val+next_mat[i][j];
            }
        }
        result.push_back(next_val);
        temp=next_mat[zerox][zeroy];
        next_mat[zerox][zeroy]=next_mat[zerox-1][zeroy];
        next_mat[zerox-1][zeroy]=temp;
    }
    if(zerox<2){
        temp=next_mat[zerox][zeroy];
        next_mat[zerox][zeroy]=next_mat[zerox+1][zeroy];
        next_mat[zerox+1][zeroy]=temp;
        next_val=0;
        for(i=0;i<3;i++){
            for(j=0;j<3;j++){
                next_val=10*next_val+next_mat[i][j];
            }
        }
        result.push_back(next_val);
        temp=next_mat[zerox][zeroy];
        next_mat[zerox][zeroy]=next_mat[zerox+1][zeroy];
        next_mat[zerox+1][zeroy]=temp;
    }
    if(zeroy>0){
        temp=next_mat[zerox][zeroy];
        next_mat[zerox][zeroy]=next_mat[zerox][zeroy-1];
        next_mat[zerox][zeroy-1]=temp;
        next_val=0;
        for(i=0;i<3;i++){
            for(j=0;j<3;j++){
                next_val=10*next_val+next_mat[i][j];
            }
        }
        result.push_back(next_val);
        temp=next_mat[zerox][zeroy];
        next_mat[zerox][zeroy]=next_mat[zerox][zeroy-1];
        next_mat[zerox][zeroy-1]=temp;
    }
    if(zeroy<2){
        temp=next_mat[zerox][zeroy];
        next_mat[zerox][zeroy]=next_mat[zerox][zeroy+1];
        next_mat[zerox][zeroy+1]=temp;
        next_val=0;
        for(i=0;i<3;i++){
            for(j=0;j<3;j++){
                next_val=10*next_val+next_mat[i][j];
            }
        }
        result.push_back(next_val);
        temp=next_mat[zerox][zeroy];
        next_mat[zerox][zeroy]=next_mat[zerox][zeroy+1];
        next_mat[zerox][zeroy+1]=temp;
    }
    return result;
}
int goal;
void hill_climbing(int init){
    priority_queue<node> open;
    open.push(node(fwd_map[init],compute_cost(init)));
    node curr,temp_successor,successor;
    int state,i,temp_id,j=0;
    vector<int> nxt_list;
    while(!open.empty()){
        curr=open.top();
        open.pop();
        state=inv_map[curr.id];
        j++;
        if(state==goal)
            break;
        nxt_list=next_states(state);
        successor=node();
        for(i=0;i<nxt_list.size();i++){
            temp_id=fwd_map[nxt_list[i]];
            temp_successor=node(temp_id,compute_cost(nxt_list[i]));
            if(temp_successor.total_cost()<successor.total_cost())
                successor=temp_successor;
        }
        if(successor.total_cost()<curr.total_cost())
        {
            parent[inv_map[successor.id]]=state;
            open.push(successor);
        }
    }
}
int main()
{
    printf("Enter board:\n");
    int arr[10],i,j,init=0,temp;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
        {
            scanf("%d",&arr[3*i+j]);
            init=10*init+arr[3*i+j];
        }
    init_states();
    temp=0;
    for(i=1;i<=8;i++){
        temp=10*temp+i;
    }
    temp=temp*10;
    goal=temp;
    hill_climbing(init);
    while(temp!=init&&parent.find(temp)!=parent.end()){
        printf("%d->",temp);
        temp=parent[temp];
    }
    printf("%d\n",init);
    return 0;
}
