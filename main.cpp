//
//  main.cpp
//  nima-homa
//
//  Created by parham bagherzadeh on 4/5/1401 AP.
//

#include <iostream>
#include "input.h"

auto p = readFile("/Users/parham/Desktop/cpp/nima-homa/nima-homa/j301_1.sm");

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
int** create2DArray(unsigned height, unsigned width)
    {
      int** array2D = 0;
      array2D = new int*[height];
    
      for (int h = 0; h < height; h++)
      {
            array2D[h] = new int[width];
    
            for (int w = 0; w < width; w++)
            {
                  
                  array2D[h][w] = w + width * h;
            }
      }
    
      return array2D;
    }
    
void FW_algorithm(int** dist){
    int  i, j, k;
    for (i = 0; i < 32; i++)
        for (j = 0; j < 32; j++){
            if (i==j){
                dist[i][j]=0;
            }else if (is_successor(i+1, j+1)){
                dist[i][j] = get_duration(i+1);
            }
            else{
                dist[i][j]=std::numeric_limits<int>::min();
            }
        }
    dist[31][0]=-p.horizon;

    for (k = 0; k < 32; k++) {
        for (i = 0; i < 32; i++) {
            for (j = 0; j < 32; j++) {
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
    int* priorities =new int[32];
    for (int i =0 ;i<32;i++){
        priorities[i]=get_duration(i+1)*get_r1(i+1);
    }
    return priorities;
}
int main(int argc, const char * argv[]) {
    int ** dist=create2DArray(32, 32);
    FW_algorithm(dist);
    int* priorities=calculate_priorities();
    
    print_2D_array(32,32,dist);
    for (int i=0;i<32;i++){
        std::cout<<p.horizon-dist[i][31]<<std::endl;
    }
    for (int i =0 ;i<32;i++){
        //std::cout<<priorities[i]<<std::endl;
    }
    return 0;
}
