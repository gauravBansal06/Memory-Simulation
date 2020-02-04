                                     //Most Frequently Used Page Replacement Policy
//Compile:- g++ mfu.cpp -o mfu
//Run:- ./mfu
//Input File name :- "input.txt" (see reference input file)  
#include <bits/stdc++.h>
using namespace std;

int P,N,overall_hit; //memory size, no. of processes, overall hits 
vector<int> hit,ref_count,nvp;     //hit= no. of hits per process, ref_count=no. of pages referenced per process, nvp=no. of virtual pages per process
vector<vector<int>> pagetable;    //page table per process 
vector<pair<int,int>> page_ref,memory;  //pages to be referenced

//Initialization 
void init()
{
    hit.clear(); 
    hit.assign(N,0);
 
    overall_hit = 0;
    memory.clear();

    pagetable.clear();
    vector<int> pr;    
    for(int i=0; i < N; i++){
        pr.assign(nvp[i],-1);
        pagetable.push_back(pr);
        pr.clear();
    }    
}

//printing page table
void print(int process)
{
    int num = nvp[process],i;
    for(i=0; i < num; i++)
        cout << "-------";     
    cout << "-" << endl;
    for(i=0; i < num; i++){
        cout << "|  " << pagetable[process][i] << "  ";
    }
    cout << "|" << endl;
    for(i=0; i < num; i++)
        cout << "-------";     
    cout << "-" << endl;
    for(i=0; i < num; i++)
        cout << "   " << i << "   ";     
    cout << endl;    
}

//global_page_replacement 
void global_page_replacement()
{
    init();  //initializing
    vector<int> mfu_check(P,0); //to get most frequently used page
    int curr_proc,curr_vp,rep_proc,rep_vp,mfu_page;
    for(int i=0; i < page_ref.size(); i++){
        curr_proc = page_ref[i].first;
        curr_vp = page_ref[i].second;
        if(pagetable[curr_proc][curr_vp] != -1){  // if hit
            hit[curr_proc]++;   //process hit
            overall_hit++;  //overall hits
            mfu_check[pagetable[curr_proc][curr_vp]]++;  //updating mfu value of index in memory
        }
        else{ //if miss
            cout << "Page fault for process = " << curr_proc << " and virtual page = " << curr_vp << ". Showing page table: " << endl;
            cout << "--------------------------------------------------------------------" << endl;
            cout << "Before Fault:" << endl;
            print(curr_proc);  //printing page table
            cout << endl;
            if(memory.size() < P){ //if memory is not full
                pagetable[curr_proc][curr_vp] = memory.size();
                mfu_check[memory.size()]++;
                memory.push_back({curr_proc,curr_vp});
            }
            else{
                mfu_page = max_element(mfu_check.begin(),mfu_check.end()) - mfu_check.begin();
                rep_proc = memory[mfu_page].first;
                rep_vp = memory[mfu_page].second;
                pagetable[curr_proc][curr_vp] = mfu_page;  //replacing page in memory
                pagetable[rep_proc][rep_vp] = -1;  //removing page from memory
                memory[mfu_page].first = curr_proc;
                memory[mfu_page].second = curr_vp;
                mfu_check[mfu_page] = 1;
            }
            cout << "After Fault:" << endl;
            print(curr_proc);  //printing page table
            for(int o = 0; o < 2; o++){
                for(int n=2; n < 70; n++)
                    cout << "-";
                cout << endl;
            }   
            cout << endl;  
        }
    }
    //printing overall hit ratio and hit ratio per process
    for(int j=0; j < N; j++)
        cout << "Hit ratio for Process " << j << " = " << ((float)hit[j])/ref_count[j] << endl;     
    cout << endl;
    cout << "Overall Hit Ratio = " << (float)overall_hit/page_ref.size() << endl;
    cout << "**********************************************" << endl;                            
}

//local_page_replacement
void local_page_replacement()
{
    init();  //initializing
    vector<int> mfu_check(P,0); //to get most frequently used page
    int curr_proc,curr_vp,rep_proc,rep_vp,mfu_page,flag;
    for(int i=0; i < page_ref.size(); i++){
        curr_proc = page_ref[i].first;
        curr_vp = page_ref[i].second;
        if(pagetable[curr_proc][curr_vp] != -1){  // if hit
            hit[curr_proc]++;   //process hit
            overall_hit++;  //overall hits
            mfu_check[pagetable[curr_proc][curr_vp]]++;  //updating mfu value of index in memory
        }
        else{ //if miss
            cout << "Page fault for process = " << curr_proc << " and virtual page = " << curr_vp << ". Showing page table: " << endl;
            cout << "--------------------------------------------------------------------" << endl;
            cout << "Before Fault:" << endl;
            print(curr_proc);  //printing page table
            cout << endl;
            if(memory.size() < P){ //if memory is not full
                pagetable[curr_proc][curr_vp] = memory.size();
                mfu_check[memory.size()]++;
                memory.push_back({curr_proc,curr_vp});
            }
            else{
                mfu_page=0;
                flag=0;
                for(int k=0; k < P; k++){
                    if(memory[k].first == curr_proc){
                        if(flag == 0){
                            mfu_page = k;
                            flag = 1;
                        }
                        else{
                            if(mfu_check[k] > mfu_check[mfu_page])
                                mfu_page = k;
                        }
                    }
                    else{
                        if(flag == 0){
                            if(mfu_check[k] > mfu_check[mfu_page])
                                mfu_page = k;
                        }
                    }          
                }
                rep_proc = memory[mfu_page].first;
                rep_vp = memory[mfu_page].second;
                pagetable[curr_proc][curr_vp] = mfu_page;  //replacing page in memory
                pagetable[rep_proc][rep_vp] = -1;  //removing page from memory
                memory[mfu_page].first = curr_proc;
                memory[mfu_page].second = curr_vp;
                mfu_check[mfu_page] = 1;
            }
            cout << "After Fault:" << endl;
            print(curr_proc);  //printing page table
            for(int o = 0; o < 2; o++){
                for(int n=2; n < 70; n++)
                    cout << "-";
                cout << endl;
            }   
            cout << endl;  
        }
    }
    //printing overall hit ratio and hit ratio per process
    for(int j=0; j < N; j++)
        cout << "Hit ratio for Process " << j << " = " << ((float)hit[j])/ref_count[j] << endl;     
    cout << endl;
    cout << "Overall Hit Ratio = " << (float)overall_hit/page_ref.size() << endl;
    cout << "**********************************************" << endl; 
}
    
int main()
{
    int n;
    pair<int,int> page;
    
    ifstream infile("input.txt");   // input file name is "input.txt"
    if(!infile){
      cout << "Unable to open input file." << endl;
      exit(0);
    }
    
    infile >> P; //memory size
    infile >> N; //no. of processes    
    for(int i=0; i < N; i++){
        infile >> n; 
        nvp.push_back(n);
        ref_count.push_back(0);
    }
    
    infile >> page.first;
    infile >> page.second;    
    while((page.first != -1) && (page.second != -1)){
        page_ref.push_back(page);
        ref_count[page.first]++;
        infile >> page.first;
        infile >> page.second;        
    }
    infile.close();
    
    cout << "Results for Global Page Replacement Policy : " << endl << endl; 
    global_page_replacement();    
    cout << endl;
    
    cout << "Results for Local Page Replacement Policy : " << endl << endl; 
    local_page_replacement();    
    cout << endl;
    
    return 0;
}
