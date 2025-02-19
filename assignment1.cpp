#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>
using namespace std;
using namespace c7x;
int main(){
    uint64_t  n,k ;
    cout<<"Enter The Number of Elements : ";
    cin>>n;
    cout<<"Enter the Group Size : ";
    cin>>k;
    uint64_t row = k,col = n/k, arr[n],res[n];
    //  Assign input values
    for(uint64_t idx = 0;idx < n;idx++){
        arr[idx] = idx+1;
    }

    // Configure both Streaming Engine and Streaming Address Generator
    const int vec_len = element_count_of<ulong_vec>::value;
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<ulong_vec>::value;
    seTemplate.VECLEN    = se_veclen<ulong_vec>::value;
    seTemplate.TRANSPOSE    = __SE_TRANSPOSE_64BIT;
    seTemplate.DIMFMT = __SE_DIMFMT_2D;
    seTemplate.ICNT0 = col;
    seTemplate.ICNT1 = row;                 
    seTemplate.DIM1 = col;
    __SA_TEMPLATE_v1 saTemplate = __gen_SA_TEMPLATE_v1();
    saTemplate.DIMFMT = __SA_DIMFMT_1D;
    saTemplate.VECLEN    = sa_veclen<ulong_vec>::value;
    saTemplate.ICNT0 = n;
    __SA0_OPEN(saTemplate);
    __SE0_OPEN((void *)&arr[0], seTemplate);
    // Main Operation
    for(int32_t r = 0; r < (row * col /vec_len); r++) {
        ulong_vec vIn = strm_eng<0, ulong_vec>::get_adv();
        __vpred pred = strm_agen<0, ulong_vec>::get_vpred();
        ulong_vec * addr = strm_agen<0, ulong_vec>::get_adv(&res[0]);
        __vstore_pred(pred, addr, vIn);
    }
    __SA0_CLOSE();
    __SE0_CLOSE();
    for(uint64_t idx = 0;idx < n;idx++){
        cout<<res[idx]<<" ";
    }
    cout<<endl;
}