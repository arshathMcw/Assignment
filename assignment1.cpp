#include <iostream>
#include <c7x.h>
#include <c7x_scalable.h>

using namespace std;
using namespace c7x;

int main(){
    uint64_t n = 4096,k = 8,row = k,col = n/k, arr[n],res[n];
    for(uint64_t idx = 0;idx < n;idx++){
        arr[idx] = idx+1;
    }
    const int vec_len = element_count_of<ulong_vec>::value;
    __SE_TEMPLATE_v1 seTemplate = __gen_SE_TEMPLATE_v1();
    seTemplate.ELETYPE   = se_eletype<ulong_vec>::value;
    seTemplate.VECLEN    = se_veclen<ulong_vec>::value;
    seTemplate.TRANSPOSE    = __SE_TRANSPOSE_64BIT;
    seTemplate.DIMFMT = __SE_DIMFMT_3D;
    seTemplate.ICNT0 = col;
    seTemplate.ICNT1 = vec_len;                 
    seTemplate.DIM1 = col;
    seTemplate.ICNT2 = (row/vec_len);  
    seTemplate.DIM2 = (col * vec_len);
    __SA_TEMPLATE_v1 saTemplate = __gen_SA_TEMPLATE_v1();
    saTemplate.DIMFMT = __SA_DIMFMT_1D;
    saTemplate.VECLEN    = sa_veclen<ulong_vec>::value;
    saTemplate.ICNT0 = n;
    __SA0_OPEN(saTemplate);
    __SE0_OPEN((void *)&arr[0], seTemplate);
    for(int32_t r = 0; r < (row/vec_len); r++) {
        for(int32_t c = 0; c < col; c++) {
            ulong_vec vIn = strm_eng<0, ulong_vec>::get_adv();
            __vpred pred = strm_agen<0, ulong_vec>::get_vpred();
            ulong_vec * addr = strm_agen<0, ulong_vec>::get_adv(&res[0]);
            __vstore_pred(pred, addr, vIn);
        }
    }
    __SA0_CLOSE();
    __SE0_CLOSE();
    for(uint64_t idx = 0;idx < n;idx++){
        cout<<res[idx]<<" ";
    }
    cout<<endl;
}