#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <unordered_set>
// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

struct unknowns{
    int row;
    int colm;
};

vector<unknowns> unknowns_list;
vector<vector<string> > patient_list;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)       //n - size of value set
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


 // The whole network represted as a list of nodes
class network{


	
public:

    list <Graph_Node> Pres_Graph;


	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }

    int search_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int index=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return index;
            index++;
        }
    
            cout<<"node not found\n";
        return -1;
    }


	

};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }	
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

vector<int> get_roots(network Alarm){

    vector<int> roots;
    
    list<Graph_Node>::iterator listIt;
    int count=0;
    for(listIt=Alarm.Pres_Graph.begin();listIt!=Alarm.Pres_Graph.end();listIt++)
    {
        if(listIt->get_Parents().size()==0) roots.push_back(count);
          count ++;
    }

    return roots;
}

int get_index_val(vector<string> v, string s){
    for(int i=0 ;i<v.size(); i++){
        // cout<<v[i]<<endl;
        if(s.compare(v[i])==0)
            return i;
    }
    return -1;
}

bool has_space(string str){
    for (auto x : str) 
   { 
       if (x == ' ') 
        return true;
   }
   return false;
}

int check_prob_val(vector<string> values, vector<int> index){
    for(int i=0; i<index.size(); i++){
        if(has_space(values[index[i]]))
            return i;
    }
    return -1;
    //returns the index of parent in this node's list who prev had a "?"
}

int find_row(vector<int> entry, vector<Graph_Node> parents){
    int row = 0;
    int temp = 0;

    for(int i=0; i<entry.size(); i++){
        temp = entry[i];
        for(int j=i+1; j<entry.size(); j++){
            temp = temp*(parents[j].get_nvalues());
        }
        row = row + temp;
    }
    return row;
}

void get_var_val(string s, vector<string> &var, vector<float> &val){
    string temp = "";
    float value=0;
    int start=0;
    for(int i=0; i<s.size(); i++){
        if(s[i]==' '){
            temp = s.substr(start,i);
            start = i+3;
            i = start;
            while(s[i]!=' ')
                i++;
            value = stof(s.substr(start,i));
            var.push_back(temp);
            val.push_back(value);
        }
        i++;
        start = i;
    }
    return;
}


void find_cpt(network &Alarm, int ind, vector<vector<string> > patient_list){
    list<Graph_Node>::iterator it = Alarm.get_nth_node(ind);
    Graph_Node gn = (*it);
 //   cout<<"shreya "<<gn.get_name()<<" ind "<<ind<<endl;
    vector<string> parent_str = gn.get_Parents();
    vector<Graph_Node> parents;
    vector<int> parent_index;

    int m = gn.get_nvalues();
    int n = 1;

    for(int i=0; i<parent_str.size(); i++){
        list<Graph_Node>::iterator temp = Alarm.search_node(parent_str[i]);
        parents.push_back(*temp);
        parent_index.push_back(Alarm.search_index((*temp).get_name()));
        n = n*((*temp).get_nvalues());
    }
 //   cout<<"m "<<m<<" n "<<n<<endl;
    float cpt_values[n][m+1];
    for(int i=0; i<n; i++){
        for(int j=0; j<m+1; j++)
            cpt_values[i][j] = 0.0;
    }
    vector<int> entry;
    int x=0; 
    int y=0;

    //main loop
    for(int i=0; i<patient_list.size(); i++){
        bool flag = false;
        entry.clear();
        cout<<"here \n";
        //ignore this patient entry
            // cout<<i<<" damn "<< patient_list[i][ind] <<endl;

        if(patient_list[i][ind].compare("\"?\"")==0){
            unknowns u;
            u.row =i; u.colm = ind;
            unknowns_list.push_back(u);
            continue;
        }
        int prob_val = check_prob_val(patient_list[i], parent_index);
        //////////////////////////////////// case when one of the parent had an unknown value
        if(prob_val>=0){
            cout<<"here1 \n";
            for(int j=0; j<parent_index.size(); j++){
                if(j==prob_val)
                    break;
                vector<string> values_list = parents[j].get_values();
                int var_numb = get_index_val(values_list,patient_list[i][parent_index[j]]);
                if(var_numb<0){
                    break;
                } 
                entry.push_back(var_numb);
            }
            string s = patient_list[i][parent_index[prob_val]];
            vector<string> values_list = parents[prob_val].get_values(); 
            vector<string> var;
            vector<float> val;
            get_var_val(s, var, val);            
            for(int i=0; i<var.size(); i++){
                int var_numb = get_index_val(values_list,var[i]);
                if(var_numb<0){
                    break;
                } 
                std::vector<int>::iterator it = entry.begin();
                entry.insert(it+prob_val,var_numb);
                x = find_row(entry, parents);
                y = get_index_val(gn.get_values(),patient_list[i][ind]);
                cpt_values[x][y]+=val[i];
                cpt_values[x][m]+=val[i];
                entry.erase(it+prob_val);    
            }   
        }
        ////////////////////////////////////////// normal case where all variable values are known
        else{
            cout<<"here2 \n";
            for(int j=0; j<parent_index.size(); j++){
                vector<string> values_list = parents[j].get_values();
                int var_numb = get_index_val(values_list,patient_list[i][parent_index[j]]);
                if(var_numb<0){
                    break;
                } 
                entry.push_back(var_numb);
            }
            x = find_row(entry, parents);
            y = get_index_val(gn.get_values(),patient_list[i][ind]);
            // cout<<gn.get_values()[0]<<endl;
            // cout<<patient_list[i][ind]<<" "<<y <<endl;
            cpt_values[x][y]++;
           cout<<cpt_values[x][y]<<endl;
            cpt_values[x][m]++;
        }
        
    }

    vector<float> cpt_list;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            float d=0;
            if(cpt_values[i][m]>0)
                d = cpt_values[i][j]/cpt_values[i][m];
           cout<<ind<<"entry no.: "<<i<<" d "<<d<<endl;
            if(d==1){
                d = d-0.0002;    
            }
            if(d==0){
                d = d+0.0002;
            }
            cpt_list.push_back(d);
        }
    }

    (*Alarm.get_nth_node(ind)).set_CPT(cpt_list);
    cout<<"set done \n";
}


void replace_unknowns(network Alarm, int ind, Graph_Node gn, vector<vector<string> > &patient_list, vector<unknowns> unknowns_list){
    vector<float> cpt_list = gn.get_CPT();

    vector<string> parent_str = gn.get_Parents();
    vector<Graph_Node> parents;
    vector<int> parent_index;

    int m = gn.get_nvalues();
    int n = 1;

    for(int i=0; i<parent_str.size(); i++){
        list<Graph_Node>::iterator temp = Alarm.search_node(parent_str[i]);
        parents.push_back(*temp);
        parent_index.push_back(Alarm.search_index((*temp).get_name()));
        n = n*((*temp).get_nvalues());
    }

    vector<int> entry;

    int start = 0;
    while(true){
        if(unknowns_list[start].colm==ind)
            break;
        else start++;
    }

    while(unknowns_list[start].colm==ind){
        int row = unknowns_list[start].row;
        for(int j=0; j<parent_index.size(); j++){
            vector<string> values_list = parents[j].get_values();
            int var_numb = get_index_val(values_list,patient_list[row][parent_index[j]]);
            if(var_numb<0){
                break;
            } 
            entry.push_back(var_numb);
        }
        int x = find_row(entry, parents);
        x = x*m;
        string input_s="";
        for(int k=0; k<m; k++){
            input_s = input_s + gn.get_values()[k] + " = " + to_string(cpt_list[x+k]) + " ";
        }
        patient_list[row][ind] = input_s;
        start++;
    }
    return;
}

vector<vector<string> > read_file(string filename){
    fstream file;
    string word = "";
    string line = "";
    file.open(filename.c_str()); 
    vector<vector<string> > patient_list;
    // extracting words from the file 
    while (getline(file,line)) 
    { 
        // displaying content
        vector<string> p;
        istringstream iss(line);

        while(iss>>word){
            p.push_back(word);
            // cout<<remove_quotes(word)<<endl;
        }
        
        patient_list.push_back(p);
    } 
    return patient_list;
}


void traverseIndex(network& Alarm, int index, vector<vector<string>> patient_list){

    find_cpt(Alarm,index,patient_list);

    // DOUBT: Which graph node? IS this right?
    Graph_Node gn = *(Alarm.get_nth_node(index));
    replace_unknowns(Alarm, index, gn, patient_list, unknowns_list);
    // WORK: Write a function to write the CPT values back in the file


}

void update_unknowns(network Alarm, int ind, Graph_Node gn, vector<vector<string> > &patient_list, vector<unknowns> unknowns_list){

    vector<string> parent_str = gn.get_Parents();
    vector<Graph_Node> parents;
    vector<int> parent_index;

    int m = gn.get_nvalues();
    int n = 1;

    for(int i=0; i<parent_str.size(); i++){
        list<Graph_Node>::iterator temp = Alarm.search_node(parent_str[i]);
        parents.push_back(*temp);
        parent_index.push_back(Alarm.search_index((*temp).get_name()));
        n = n*((*temp).get_nvalues());
    }

    // WORK: Your code seems too complicated to understand lol. That encoding to get the corresponding variable CPT values. I got a way of doing this. I will tell you when we meet tomorrow

}


void traverse(network& Alarm, vector<int> roots, vector<vector<string>> patient_list){

    vector<int> newChild;
    //cout << "Endl" << endl;
    
    for (int i=0; i<roots.size() ; i++){
        traverseIndex(Alarm,roots[i],patient_list);
    //    cout << roots[i] << endl;

        vector<int> added = (*(Alarm.get_nth_node(roots[i]))).get_children();
        newChild.insert(newChild.end(),added.begin(), added.end());
    }

//    cout << newChild.size() << endl;


    std::unordered_set<int> set;
    std::copy(newChild.begin(),
            newChild.end(),
            std::inserter(set, set.end()));


    std::vector<int> newRoot;
    newRoot.assign(set.begin(), set.end());

//    for(int i=0; i<newRoot.size(); i++) cout << newRoot[i] << endl;

    
    if (newRoot.size() ==0) return;
    else{
        traverse(Alarm,newRoot,patient_list);
    }
    


}

void traverseIndex_EM(network& Alarm, int index, vector<vector<string>> patient_list){


    // DOUBT: Which graph node? IS this right?
    Graph_Node gn = *(Alarm.get_nth_node(index));
    update_unknowns(Alarm, index, gn, patient_list, unknowns_list);

}

void traverse_EM(network& Alarm, vector<int> roots, vector<vector<string>> patient_list){

    vector<int> newChild;

    for (int i=0; i<roots.size() ; i++){
        traverseIndex_EM(Alarm,roots[i],patient_list);
        vector<int> added = (*(Alarm.get_nth_node(roots[i]))).get_children();
        newChild.insert(newChild.end(),added.begin(), added.end());
    }

    if (newChild.size() ==0) return;
    else{
        traverse_EM(Alarm,newChild,patient_list);
    }


}

void traverseIndex_EM1(network& Alarm, int index, vector<vector<string>> patient_list){

  find_cpt(Alarm,index,patient_list);
    // WORK: Write a function to write the CPT values back in the file - they are gettin updated at each cpt calculation. Just need to convert that alarm network to file

}

void traverse_EM1(network& Alarm, vector<int> roots, vector<vector<string>> patient_list){

    vector<int> newChild;

    for (int i=0; i<roots.size() ; i++){
        traverseIndex_EM1(Alarm,roots[i],patient_list);
        vector<int> added = (*(Alarm.get_nth_node(roots[i]))).get_children();
        newChild.insert(newChild.end(),added.begin(), added.end());
    }

    if (newChild.size() ==0) return;
    else{
        traverse_EM1(Alarm,newChild,patient_list);
    }


}


bool goalTest (network Alarm, network Alarm_old){

    network vGraph1 = Alarm_old;    
 
    network vGraph2 = Alarm;    

    bool flag = true;

    int flagBreak = 0;
    for(int i=0; i<vGraph1.netSize(); i++){

        Graph_Node g1 = *(vGraph1.get_nth_node(i));
        Graph_Node g2 = *(vGraph2.get_nth_node(i));

        vector<float> CPT1 = g1.get_CPT();
        vector<float> CPT2 = g2.get_CPT();

        for(int j=0; j< CPT1.size(); j++){

            if (abs(CPT2[i] - CPT1[i]) > 0.0001){

                flagBreak = 1;
                flag = false;
                break;
            }

        }

        if (flagBreak == 1) {
            break;
        }

    }

    return flag;
}


int main()
{
      clock_t startt,endt;
      startt=clock();

    string filename = "records.dat";
    patient_list = read_file(filename);
    //cout<<patient_list.size()<<"  "<<patient_list[0].size()<<endl;
    
    network Alarm;
    Alarm=read_network();
    
    vector<int> roots;

    roots = get_roots(Alarm);
    vector<vector<string> > patient_list_copy = patient_list;

// The first traversal, storing the CPT valuess : Assumed to be working fine!
    traverse(Alarm,roots,patient_list_copy);

// DOUBT: I don't know if Alarm_old is a pointer or a scalar - scalar
   
   /* network Alarm_old = Alarm;

    int count = 0;
    while((!goalTest(Alarm,Alarm_old)) || (count == 0)){

        Alarm_old = Alarm;
        count++;

        endt = clock();
        if ((((endt - startt)/1000)/1000) >= ((2 * 60) -1)){
        
            break;
        } 

        traverse_EM(Alarm,roots,patient_list);
        traverse_EM1(Alarm,roots,patient_list);

    }

    */
    return 0;
}


// "low = 0.4 medium = 0.3 high = 0.3"