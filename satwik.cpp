#include <bits/stdc++.h>
using namespace std;

struct node{
    
    node* parent;
    vector<node*>children;
    int num_visits;
    int state[3][3];
    int reward;
};
node* head=NULL;

bool vertical(int A[3][3],int a){
    //int i;
    int sum=0;
    for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
    	{if(A[j][i]==a)
    	sum++;
		}
		}
		if(sum==3)
		return true;
		else
		sum=0;
	}
	return false;
    
}
bool horizontal(int A[3][3],int a){
    //int i;
    int sum=0;
    for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
    	{if(A[i][j]==a)
    	sum++;
		}
		}
		if(sum==3)
		return true;
		else
		sum=0;
	}
	return false;
    
}
bool cross(int A[3][3],int a)
{int sum=0;

for(int i=0;i<3;i++)
{if(A[i][i]==a)
    	sum++;
		}
if(sum==3)
return true;
sum=0;
if(A[0][2]==a && A[1][1]==a && A[2][0]==a)
return true;
return false;
}
int num_minus_one(int A[3][3])
{int count=0;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		if(A[i][j]==-1)
		count++;
	}
	return count;
}
//decides winning or losing
bool termination(int A[3][3],int a)
{
	if(horizontal(A,a)||vertical(A,a)||cross(A,a))
	return true;
	return false;
}
bool stop(int A[3][3],int a)
{
	if(termination(A,a) || num_minus_one(A)==0)
	return true;
	return false;
}
long int ucb(struct node*t)
{
	if(t->num_visits==0)
	return 65534;
	else
	return(t->reward/t->num_visits+(2*sqrt(log(t->parent->num_visits)/t->num_visits)));
	
}

int reward_calc(int A[3][3],int a)
{
	if(termination(A,a)==true)
	return 1;
	else if(num_minus_one(A)==0)
	return 0;
	else
	return -1;
}
//creates node with just copying states in parent
struct node* node_create(struct node* parent_node){
	struct node* temp=new node;
	temp->num_visits=0;
	temp->reward=0;
	temp->parent=parent_node;
	for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
    	temp->state[i][j]=parent_node->state[i][j];
		}	
	}
	return temp;
	
}
//simulates to give end reward
int rollout(int A[3][3],int player1,int player2,int winner){
	vector<int>minus_one;int m,count=0;
	int k=player1,B[3][3];
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			B[i][j]=A[i][j];
			if(A[i][j]==-1){
				minus_one.push_back(10*i+j);
			}
		}
	}
	// for(int i=0;i<3;i++){
	// 	for(int j=0;j<3;j++){
	// 		B[i][j]=A[i][j];
				
	// 		}
	// 	}
	
	while(stop(B,1)==false && stop(B,2)==false){
		count++;
		m=rand()%minus_one.size(); //doubt here
		//cout<<m<<endl;
		B[(minus_one[m])/10][(minus_one[m])%10]=k;
		
	// cout<<endl;
		if(count%2!=0)
		k=player2;
		if(count%2==0)
		k=player1;
		minus_one.erase(minus_one.begin()+m);	}
	return(reward_calc(B,winner));
	
}



int main(){
    node* temp;
    int n=0;//number of games
    while(n<3000/1)
    {
    	int player=1;
    	int other_player=2;
    	if(head==NULL)//creating root node
    	{
		temp=new node;
		temp->num_visits=0;
	temp->reward=0;
	temp->parent=NULL;// change made here because the parent of the root node is null
	for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
    	temp->state[i][j]=-1;
    	 // doubt
		}	
	}
	head=temp;
		}
	else{temp=head; // this else loop is executed after creating root node 
    int count=0;//for deciding the turn of the player
		cout<<"n value is "<<n<<endl;
	
	//this while loop helps in reaching leaf node

		while(temp->children.size()!=0 && stop(temp->state,1)==false && stop(temp->state,2)==false){
			cout<<"printing current size------------------"<<temp->children.size()<<endl;
			count++;
			
			vector<int>ucb_array,ta;
			//calculating best child here using ucb score
			for(int k=0;k<temp->children.size();k++){ 
				// cout<<"ucb score of this child is "<<ucb(temp->children[k])<<endl;
				ucb_array.push_back(ucb(temp->children[k]));
				cout<<"num of visits of this child are "<<temp->children[k]->num_visits<<endl;
			}
			ta=ucb_array;
			// cout<<"ta size is -----------------"<<ta.size()<<endl;
			sort(ucb_array.begin(),ucb_array.end());
			reverse(ucb_array.begin(),ucb_array.end());
			int x=ucb_array[0],d;//int d;
			vector<int> ucb_eql_child;
				for(int p=0;p<ta.size();p++)    
				{
					if(ta[p]==x)
				{
					ucb_eql_child.push_back(p);
					
						//d=p;
						}
					
				}
				d=rand()%ucb_eql_child.size();
				temp=temp->children[d];
			
			cout<<"children size "<<temp->children.size()<<endl;
			cout<<"num minus one in temp "<<num_minus_one(temp->state)<<endl;
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			cout<<temp->state[i][j]<<" ";
			cout<<endl;
		}
		if(stop(temp->state,1)==true || stop(temp->state,2)==true){//backprop after reaching the end
			int r=reward_calc(temp->state,1);
			while(temp->parent!=NULL)	
			{temp->reward+=r;// doubt in adding reward to parent
				temp->num_visits+=1;
				temp=temp->parent;
				r=r*(-1);
				
				
					}

					

		}
			
		}//while loop for using ucb ends here

		//this loop is for performing rollout on every child
		if(num_minus_one(temp->state)>0){
		
		if(count%2==0)
		{
		player=1;other_player=2;}
		
		else
		{
		player=2;other_player=1;}
		vector<int>minus_one_indexes;

		//saving positions of -1
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				if(temp->state[i][j]==-1)
				minus_one_indexes.push_back((10*i)+j);
			}
			
		}
	
	//loop for creating child and rollout
		for(int i=0;i<minus_one_indexes.size();i++)
		{	struct node* temp1=temp;
			cout<<"creating child"<<endl;

			temp=node_create(temp);

			temp->state[minus_one_indexes[i]/10][minus_one_indexes[i]%10]=player;//again and again we are assigning same player
			temp1->children.push_back(temp);
			temp1=temp;	
		
			int roll_out_reward=rollout(temp->state,other_player,player,1);
			cout<<"rollout reward of this child is "<<roll_out_reward<<endl;
			while(temp1->parent!=NULL)	
			{temp1->reward+=roll_out_reward;// doubt in adding reward to parent
				temp1->num_visits+=1;
				temp1=temp1->parent;
				roll_out_reward =roll_out_reward*(-1); 
				
				
					}
					
		temp=temp->parent;					
		}
			}
		
		
	}
		cout<<"n is ------------------------------------------"<<n<<endl;
		n++;
		
	}
	return(0);
	}
    
	
	
	
