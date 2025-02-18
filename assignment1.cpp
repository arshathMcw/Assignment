#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x;

int main(){
    int n = 64;
    uint64_t arr[n];
    for(uint64_t idx = 0;idx < n;idx++){
        arr[idx] = idx+1;
        cout<<arr[idx]<<" ";
    }
    int row = 8,col = 8;
    cout<<endl;
    const int vec_len = element_count_of<long_vec>::value;
    cout<<vec_len<<endl;
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<long_vec>::value;
    seTemplate.VECLEN    = se_veclen<long_vec>::value;
    seTemplate.ICNT0 = row*col;
    uint64_t mat[row][col];
    __SE0_OPEN((void *)&arr[0], seTemplate);

    // long_vec vIn1 = strm_eng<0, long_vec>::get_adv();
    // vIn1.print();


    for(int r = 0;r < row;r++){
        uint64_t *matIdx=&mat[r][0];
        for(int c = 0;c < col;c+=vec_len){
            long_vec vIn1 = strm_eng<0, long_vec>::get_adv();
            // vIn1.print();
            *(long_vec *) (matIdx) = vIn1;
            matIdx += vec_len;
        }
    }
    for(int r = 0;r < row;r++){
        for(int c = 0;c < col;c++){
            cout<<mat[r][c]<<" ";
        }
        cout<<endl;
    }
    __SE0_CLOSE();
}