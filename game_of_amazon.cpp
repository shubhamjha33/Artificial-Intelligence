#include <cstdio>
#include <vector>
#include <climits>
#include <iostream>
#include <sys/time.h>
#include <map>
#include <queue>
#include <cstring>
#include <set>
using namespace std;
int player,memcheck;
typedef vector<int> vi;
inline double get_time() {
	timeval t;
	gettimeofday(&t,NULL);
	return t.tv_sec + t.tv_usec * 1e-6;
}
bool inBounds(int x,int y){
    if(x>=0&&x<10&&y>=0&&y<10)
        return true;
    return false;
}
class State{
	public:
		vector<vi> mat;
		vi moves;
		int score,currPlayer;
		State(){
			score=INT_MIN;
        }
		bool operator<(const State& st) const{
		    if(currPlayer==player)
                return score>st.score;
			else
                return score<st.score;
		}
		State(vector<vi> board,int cp,vi mov){
			mat=board;
			currPlayer=cp;
			moves=mov;
			compute_score();
		}
		State(const State& s){
			mat=s.mat;
			score=s.score;
			currPlayer=s.currPlayer;
			moves=s.moves;
		}
		void compute_score(){
			int i,j,v,p,pos=0,neg=0,x,y;
			int dir[][2]={{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}};
			int visited[10][10];
			score=0;
			vector<int> comp;
			queue<int> q;
			for(i=0;i<10;i++){
                for(j=0;j<10;j++){
                    if(mat[i][j]==-1)
                        visited[i][j]=-1;
                    else
                        visited[i][j]=-2;
                }
			}
			for(i=0;i<10;i++){
                for(j=0;j<10;j++){
                    if(mat[i][j]!=0)
                        continue;
                    if(visited[i][j]!=-1)
                        continue;
                    comp.push_back(1);
                    visited[i][j]=comp.size()-1;
                    v=10*i+j;
                    q.push(v);
                    while(!q.empty()){
                        x=q.front()/10;
                        y=q.front()%10;
                        q.pop();
                        for(p=0;p<8;p++){
                            if(inBounds(x+dir[p][0],y+dir[p][1])&&visited[x+dir[p][0]][y+dir[p][1]]==-1){
                                visited[x+dir[p][0]][y+dir[p][1]]=visited[i][j];
                                comp[visited[i][j]]++;
                                q.push(10*(x+dir[p][0])+y+dir[p][1]);
                            }
                        }
                    }
                }
			}
			set<int> comp_list;
			for(i=0;i<10;i++){
                for(j=0;j<10;j++){
                    if(mat[i][j]<=0)
                        continue;
                    comp_list.clear();
                    if(mat[i][j]>0){
                        for(p=0;p<8;p++){
                            x=i+dir[p][0];
                            y=j+dir[p][1];
                            if(inBounds(x,y)&&visited[x][y]>=0){
                                if(comp_list.find(visited[x][y])==comp_list.end()){
                                    comp_list.insert(visited[x][y]);
                                    if(mat[i][j]==player){
                                        pos+=comp[visited[x][y]];
                                    }
                                    else
                                        neg+=comp[visited[x][y]];
                                }
                            }
                        }
                    }
                }
			}
			if(neg==0)
                score=10000;
            else if(pos==0)
                score=-10000;
            else
                score=pos-neg;
		}
};
priority_queue<State> get_next_moves(State start){
	int i,j,l,m,p,q,x,y,a,b;
	int dir[][2]={{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}};
	State temp,curr;
	priority_queue<State> pq;
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(start.mat[i][j]==start.currPlayer){
				temp=start;
				temp.currPlayer=3-start.currPlayer;
				for(p=0;p<8;p++){
					l=i+dir[p][0];
					m=j+dir[p][1];
					while(inBounds(l,m)&&temp.mat[l][m]==0){
						temp.mat[l][m]=start.currPlayer;
						temp.mat[i][j]=0;
						for(q=0;q<8;q++){
							x=l+dir[q][0];
							y=m+dir[q][1];
							while(inBounds(x,y)&&temp.mat[x][y]==0){
								temp.mat[x][y]=-1;
								temp.compute_score();
								curr=temp;
								curr.moves[0]=i;
								curr.moves[1]=j;
								curr.moves[2]=l;
								curr.moves[3]=m;
								curr.moves[4]=x;
								curr.moves[5]=y;
								pq.push(curr);
								memcheck++;
								temp.mat[x][y]=0;
								x=x+dir[q][0];
								y=y+dir[q][1];
							}
						}
						temp.mat[l][m]=0;
						temp.mat[i][j]=start.currPlayer;
						l+=dir[p][0];
						m+=dir[p][1];
					}
				}
			}
			else
				continue;
		}
	}
	return pq;
}
double start_time;
class HashNode{
    public:
        char str[102];
        int player;
        HashNode(){
        }
        HashNode(const HashNode& hn){
            strcpy(str,hn.str);
            player=hn.player;
        }
        HashNode(State s){
            player=s.currPlayer;
            for(int i=0;i<10;i++){
                for(int j=0;j<10;j++){
                    if(s.mat[i][j]==-1){
                        str[10*i+j]='4';
                    }
                    else{
                        str[10*i+j]='0'+s.mat[i][j];
                    }
                }
            }
            str[100]='\0';
        }
        bool operator<(const HashNode& hn)const{
            return strcmp(str,hn.str);
        }
};
map<HashNode,priority_queue<State> > next_states;
void idfs(State s){
    if(get_time()-start_time>0.8){
        return;
    }
    HashNode hn(s),temp;
    if(next_states.find(hn)==next_states.end()){
        //printf("1:%d %d %d\n",s.currPlayer,next_states.size(),memcheck);
        next_states.insert(make_pair(hn,get_next_moves(s)));
    }
    else{
        //printf("2:%d %d %d\n",s.currPlayer,next_states.size(),memcheck);
        State curr=next_states[hn].top();
        next_states[hn].pop();
        idfs(curr);
        temp=HashNode(curr);
        if(next_states.find(temp)!=next_states.end())
            curr.score=next_states[temp].top().score;
        next_states[hn].push(curr);
    }
}
void perform_best_move(State start){
    start_time=get_time();
    /*if(start.score==10000||start.score==-10000)
        return;*/
    int prevmemcheck=-1;
    while(get_time()-start_time<=0.8){
        idfs(start);
        if(prevmemcheck!=memcheck){
            prevmemcheck=memcheck;
        }
        else
            break;
    }
    //printf("OK till here\n");
    HashNode s(start);
    //printf("OK till here %d\n",next_states[s].size());
    State best=next_states[s].top();
    //printf("OK till here\n");
    for(int i=0;i<6;i+=2){
        printf("%d %d\n",best.moves[i],best.moves[i+1]);
    }
}
int main()
{
    memcheck=0;
	int i,j,num;
	vector<vi> board(10);
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			scanf("%d",&num);
			board[i].push_back(num);
		}
	}
	scanf("%d",&player);
	vi moves;
	for(i=0;i<6;i++)
        moves.push_back(-1);
	State s(board,player,moves);
	perform_best_move(s);
    return 0;
}
