#include <iostream>

void quicksort(int array[],int start, int end);
int partition(int array[],int start, int end);

int main(){
    int array[] = {4,5,6,3,2,8,9,1,7,0};
    size_t lenght = sizeof(array)/sizeof(array[0])-1;

    for(int num : array){
        std::cout << num << " ";
    }
    std::cout << '\n';

    quicksort(array,0,lenght);

    for(int num : array){
        std::cout << num << " ";
    }
    std::cout << '\n';
    return 0;    
}

void quicksort(int array[],int start, int end){

    if(end <= start) return;

    int pivot = partition(array,start,end);
    quicksort(array,start,pivot-1);
    quicksort(array,pivot + 1,end);

    return;
}

int partition(int array[],int start, int end){
    int pivot = array[end];
    int i = start -1;
    
    for(int j = start; j<= end;j++){
        if(array[j] < pivot){
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    i++;
        int temp = array[i];
        array[i] = array[end];
        array[end] = temp;    
    return i;
};