#include <iostream>
#include "input.h"
#include <cmath>
#include <time.h>
auto p = readFile("/Users/parham/Desktop/cpp/nima-homa/nima-homa/j3013_4.sm");
int JN=32;
int availableR1=10;
int deadline=63;
int optimal=205;

bool is_successor(int i ,int j){
    for (int k = 0; k < p.jobs[i].amountSuccessors; ++k){
        if ( p.jobs[i].successors[k]==j){
            return true;
        }
    }return false;
}
int get_duration(int i){
    return p.jobs[i].modes[1].duration;
}
int get_r1(int i){
    return p.jobs[i].modes[1].usedResources.r[0];
}

int** create2DArray(int height, int width)
    {
      int** array2D = 0;
      array2D = new int*[height];
    
      for (int h = 0; h < height; h++)
      {
            array2D[h] = new int[width];
    
            for (int w = 0; w < width; w++)
            {
                  array2D[h][w] = 0;
            }
      }
    
      return array2D;
    }
int* create1DArray(int size)
{
    int* array1D = 0;
    array1D = new int[size];
    return array1D;
}
void init_dist(int** dist,int dueDate){
    
    for (int i = 0; i < JN; i++)
        for (int j = 0; j < JN; j++){
            if (i==j){
                dist[i][j]=0;
            }else if (is_successor(i+1, j+1)){
                dist[i][j] = get_duration(i+1);
            }
            else{
                dist[i][j]=std::numeric_limits<int>::min();
            }
        }
    dist[JN-1][0]=-dueDate;

}
//no need for the return value because if there is any change in dist it would affect ram and the changes will be there
void FW_algorithm(int** dist){
    int  i, j, k;
    for (k = 0; k < JN; k++) {
        for (i = 0; i < JN; i++) {
            for (j = 0; j < JN; j++) {
                if (dist[i][j] < (dist[i][k] + dist[k][j])
                && (dist[k][j] != std::numeric_limits<int>::min()
                && dist[i][k] != std::numeric_limits<int>::min()))
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
    
}
void print_2D_array(int height,int width,int** my2DArray){
    for (int h = 0; h < height; h++)
          {
                for (int w = 0; w < width; w++)
                {
                      printf("%i,", my2DArray[h][w]);
                }
                printf("\n");
          }
}
int * calculate_priorities(){
    int* priorities =new int[JN];
    for (int i =0 ;i<JN;i++){
        priorities[i]=get_duration(i+1)*get_r1(i+1);
    }
    return priorities;
}
bool isDone(bool * c){
    for (int i=0;i<JN;i++){
        if (c[i]== false){
            return false;
        }
    }
    return true;
}
void setSchedule(int ind,int time , bool * c,int * schedule){
    schedule[ind]=time;
    c[ind]=true;
}
bool isInPeriod(int start,int * sc,int ind){
    if (sc[ind]+get_duration(ind+1)>start && sc[ind]<=start){
        return true;
    }
    return false;
}
int calculateResourceUsed(bool * c,int * sc,int start){
    int result=0;
    for (int i =0 ;i<JN;i++){
        if (c[i]){
            if(isInPeriod(start,sc,i)){
                result=result+get_r1(i+1);
            }
        }
    }
    return result;
}
int get_surface(bool * c,int * sc,int start,int end,int availableR){
    int result=0;
    for (int i=start;i<end;i++){
        if (calculateResourceUsed( c, sc, i)>availableR){
            result=result+calculateResourceUsed( c, sc, i)-availableR;
            
        }
    }
    return result;
}

int * algorithm_main(int * es,int * ls,int * priorities,int availableR1,int** dist){
    
    bool * c;
    int * sc;
    
    c=new bool[JN];
    sc=create1DArray(JN);
    for(int i =0 ;i<JN;i++){
        c[i]=false;
        sc[i]=-1;
    }
    c[0]=true;
    sc[0]=0;
    for (int i =0 ;i<JN;i++){
        if (ls[i]==es[i]){
            
            setSchedule(i, ls[i], c, sc);
        }
    }
    
   
    
    while (isDone(c)==false){
        
        
        
        int max=-1;
        int indMax=-1;
        for (int i =0 ;i<JN;i++){
            if (c[i]==false){
                if (max<priorities[i]){
                    
                    indMax=i;
                    max=priorities[i];
                }
            }
        }
        
        int tempSurface=-1;
        int minStart=-1;
        
        
        for (int s=es[indMax];s<=ls[indMax];s++){
            int surface=get_surface(c, sc, s, s+get_duration(indMax+1), availableR1-get_r1(indMax+1));
            if(tempSurface<=surface){
                tempSurface=surface;
                minStart=s;
            }
        }
        
        setSchedule(indMax, minStart, c, sc);
        for (int i=0;i<JN;i++){
            if(c[i]==false){
                if (minStart+dist[indMax][i]>es[i]){
                    es[i]=minStart+dist[indMax][i];
                }
                
                if (minStart-dist[i][indMax]<ls[i]){
                    ls[i]=minStart-dist[i][indMax];
                }
            }
        }
        
        for (int i =0 ;i<JN;i++){
            if (ls[i]==es[i]){
                setSchedule(i, ls[i], c, sc);
            }
        }
    }
    
    
 
    /*
    std::cout<<std::endl<<"schedule:"<<std::endl;
    for (int i =0 ;i<32;i++){
        std::cout<<sc[i]<<"-";
    }
     
    std::cout<<std::endl;
     */
    return sc;
}
int algorithm_main_surface(int * es,int * ls,int * priorities,int availableR1,int** dist){
    int * es_copy=create1DArray(JN);
    int * ls_copy=create1DArray(JN);
    for (int i=0;i<JN;i++){
        es_copy[i]=es[i];
        ls_copy[i]=ls[i];
    }
    int * schedule=algorithm_main(es_copy,ls_copy,priorities,availableR1,dist);
    bool c[JN];
    for(int i=0;i<JN;i++){
        c[i]=true;
    }
    return get_surface(c, schedule, 0, deadline, availableR1);
}


int * get_next_answer (int * current_answer){
    int* result=create1DArray(JN);
    unsigned ind1=(unsigned)clock()*random()%(JN);
    
    
    for (int i=0;i<JN;i++){
        result[i]=current_answer[i];
    }
    unsigned ind2=(unsigned)clock()*random()%(JN);

    result[ind1]=current_answer[ind2];
    result[ind2]=current_answer[ind1];
    result[0]=0;
    return result;
}

int main(int argc, const char * argv[]) {
    
    
    
    //allocate space in ram
    int ** dist=create2DArray(JN, JN);
    
    //initialize the matris to start floyd warshal algorithm
    init_dist(dist,deadline);
    //run the algorithm
    FW_algorithm(dist);
    //extracet the es from the given matrix
    int es[JN];
    for (int i=0;i<JN;i++)
        es[i]=dist[0][i];
    
    //initialize the matrix with the es*1.25
    init_dist(dist,deadline);
    FW_algorithm(dist);
    int ls[JN];
    for (int i=0;i<JN;i++)
        ls[i]=deadline-dist[i][JN-1];
    ls[0]=0;
    //calculate the priorities using the formula : du*re
    int* priorities=calculate_priorities();
    
    //print the results
    std::cout<<std::endl<<"es:"<<std::endl;
    for (int i=0;i<JN;i++){
        std::cout<<es[i]<<"-";
    }
    std::cout<<std::endl<<"ls:"<<std::endl;
    for (int i =0 ;i<JN;i++){
        std::cout<<ls[i]<<"-";
    }
    std::cout<<std::endl<<"priority:"<<std::endl;
    for (int i =0 ;i<JN;i++){
        std::cout<<priorities[i]<<"-";
    }
    
    int score =algorithm_main_surface(es,ls,priorities,availableR1,dist);
    
    
    std::cout<<std::endl<<"init surface:"<<score<<"    init gap percentage:"<<(score-optimal)*100/optimal<<std::endl;
    std::cout<<"improvement starting ..."<<std::endl<<"..."<<std::endl;
    
    
    
    int curr_eval=score;
    int * curr=priorities;
    
    int best_eval=score;
    int * best=priorities;
    
    int iteration=0;
    float temp=1000;
    while(iteration<100){
        int * candidate = get_next_answer(curr);
        int candidate_eval =algorithm_main_surface(es,ls,candidate,availableR1,dist);
        
        
        //std::cout<<candidate_eval<<std::endl;
        if (candidate_eval <best_eval ){
            best=candidate;
            best_eval =candidate_eval;
            std::cout<<"iteration:"<<iteration<<" "<<"Score:"<<best_eval<<std::endl;
            std::cout<<"gap percentage:"<<(candidate_eval-optimal)*100/optimal<<std::endl;
            
            
            
        }
        int diff = (candidate_eval - curr_eval);
        float t = temp / iteration + 1;
        double metropolis = exp(-(diff / t));
        
        
        float my_random=((float)rand()/RAND_MAX) ;
        //std::cout<<metropolis<<std::endl;
        if (diff < 0 || my_random< metropolis){
            curr= candidate;
            curr_eval=candidate_eval;
        }
        iteration++;
    }
    int * sc=algorithm_main(es,ls,best,availableR1,dist);
    std::cout<<"best schedule :"<<std::endl;
    for (int i=0;i<JN;i++){
        std::cout<<sc[i]<<"_";
    }
    std::cout<<std::endl;
    return 0;
}
