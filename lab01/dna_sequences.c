#include <stdio.h>
#include <stdlib.h>

char map[256] = { 0 };

void init_map(){
    map['T'] = 3;
    map['A'] = 0;
    map['G'] = 2;
    map['C'] = 1;
}

int give_str_sortedness(int len, char * str){
    int n_chars[4] = { 0 };
    int total_sortedness = 0;

    for(int i_str = len-1; i_str >= 0; --i_str){
        // 0,1,2,3 corresponding to A,C,G,T
        char this_value = map[ str[i_str] ];

        // If it is T, for example, sums the number of As, Gs and Cs that came after.
        for(int i_smaller = this_value - 1; i_smaller >= 0; --i_smaller){
            total_sortedness += n_chars[i_smaller];
        }
        // And then this goes to the sum of 'those that came after'.
        n_chars[this_value] ++;
    }

    return total_sortedness;
}

int main2(){
    char * str = "ACGTTTTTTA";
    int str_len = 10;

    init_map();

    printf("Sortedness is : %d\n", give_str_sortedness(10, str));
}

int main(){
    int n_datasets;
    int sortedness[100];

    char sequences[100][100];

    init_map();

    scanf("%d", &n_datasets);

    for(int i_set = 0; i_set < n_datasets; ++i_set){
        int n_seq, sz_seq;

        scanf("%d%d", &sz_seq, &n_seq);

        for(int i = 0; i < n_seq; ++i){
            scanf("%s",sequences[i]);
            sortedness[i] = give_str_sortedness(sz_seq,sequences[i]);
        }

        // Print all the 0s, then all the 1s
        for(int i_iter = 0; i_iter < n_seq; ++i_iter){
            int i_smallest = 0;
            for(int i_list = 0; i_list < n_seq; ++i_list){
                if(
                        sortedness[i_list] != -1 &&
                        (sortedness[i_list] < sortedness[i_smallest] || sortedness[i_smallest] == -1)

                        ){
                    i_smallest = i_list;
                }
            }
            printf("%s\n", sequences[i_smallest]);
            sortedness[i_smallest] = -1;
        }
        printf("\n");
    }

    return 0;
}