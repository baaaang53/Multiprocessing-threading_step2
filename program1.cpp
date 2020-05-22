// program 1. convolution and ReLU
//
//  Created by 방승연 on 2020/04/17.
//  Copyright © 2020 방승연. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

void Convolution(int** res, int*** data, int*** filt, int result_row, int result_col, int filt_row, int filt_col);
int small_calculate(int** filt,int** data, int filt_row, int filt_col, int row_index, int col_index);
void ReLU(int*** res, int fil_num, int d_row, int d_col);


int main(int argc, const char * argv[]) {
    int i, j, k;
    struct timeval startTime, endTime; // 시간 측정
    double diffTime;
    
    // start timer
    gettimeofday(&startTime,NULL);
    
//    filter
    int filt_num, filt_row, filt_col;
    std::cin >> filt_num;
    std::cin >> filt_row;
    std::cin >> filt_col;
    
    //공간 생성
    int ****filter = new int***[filt_num];
    for (i =0 ; i< filt_num; i++) {
        filter[i] = new int**[3];
        for (j = 0; j < 3; j++) {
            filter[i][j] = new int*[filt_row];
            for (k = 0; k < filt_row; k++) {
                filter[i][j][k] = new int[filt_col];
            }
        }
    }

    //get filter
    for (i = 0; i < filt_num; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < filt_row; k ++) {
                for (int l = 0 ;l < filt_col; l++) {
                    std::cin >> filter[i][j][k][l];
                }
            }
        }
    }
    
//  input data
    int data_row, data_col;
    std::cin >> data_row;
    std::cin >> data_col;
    
    //공간 생성
    int ***data = new int**[3]; // +2 is for padding
    for (i = 0; i < 3; i++) {
        data[i] = new int*[data_row+2];
        for (j =0; j < data_row+2; j++) {
            data[i][j] = new int[data_col+2];
        }
    }
    
    //get input
    for (i = 0; i < 3; i++) {
        for (j = 0; j < data_row+2; j++) {
            for (k = 0; k < data_col+2 ; k++) {
                if (j == 0 || k == 0 || j == data_row+1 || k == data_col+1) { // padding
                    data[i][j][k] = 0;
                }
                else {
                    std::cin >> data[i][j][k];
                }
            }
        }
    }
    
    
//  result
    //공간 생성
    int result_row = data_row + 2 - (filt_row - 1);
    int result_col = data_col + 2 - (filt_col - 1);
    int ***result = new int**[filt_num];
    for (i = 0; i<filt_num; i++) {
        result[i] = new int*[result_row];
        for (j = 0; j < result_row ; j++) {
            result[i][j] = new int[result_col];
        }
    }
    
    //계산
    for (i = 0; i < filt_num; i++) { //각 필터마다 convolution 계산을 해 준다.
        Convolution(result[i], data, filter[i], result_row, result_col, filt_row, filt_col);
    }
    
    //ReLU 연산
    ReLU(result, filt_num, result_row, result_col);
    
    //출력
    for (k = 0; k < filt_num; k++) {
        for (i = 0; i < result_row; i++) {
            for (j= 0; j<result_col; j++) {
                std::cout << result[k][i][j] << " ";
            }
            std::cout <<"\n";
        }
        std::cout << "\n";
    }
    
    //공간 해제
    //filter
    for (i = 0; i < filt_num; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < filt_row; k++) {
                delete [] filter[i][j][k];
            }
            delete [] filter[i][j];
        }
        delete [] filter[i];
    }
    
    //data
    for (i = 0; i < 3; i++) {
        for (j = 0; j < data_row + 2; j ++) {
            delete [] data[i][j];
        }
        delete [] data[i];
    }
    
    //result
    for (i = 0; i < filt_num; i++) {
        for (j = 0; j < result_row; j++) {
            delete [] result[i][j];
        }
        delete [] result[i];
    }
    // stop timer
    gettimeofday(&endTime, NULL);
    diffTime = (endTime.tv_sec - startTime.tv_sec) + ((endTime.tv_usec-startTime.tv_usec)/1000000.0);
    std::cout << diffTime * 1000<< "\n";
    
    return 0;
}

//Convolution 함수
void Convolution(int** res, int*** data, int*** filt, int result_row, int result_col, int filt_row, int filt_col){
    for (int ch = 0; ch < 3; ch++) { // 각 채널에 대하여
        for(int i = 0; i < result_row ;i++) {  //데이터를 훑으면서
             for (int j=0; j< result_col; j++) {
                 res[i][j] += small_calculate(filt[ch], data[ch], filt_row, filt_col, i,j);
             }
         }
    }
}

//필터가 씌워진 부분의 합성곱 연산을 하는 함수
int small_calculate(int** filt,int** data, int filt_row, int filt_col, int row_index, int col_index) {
    int resul = 0;
    for (int i = 0; i < filt_row; i++) {
        for (int j = 0; j < filt_col; j++) {
            resul += filt[i][j] * data[row_index+i][col_index+j];
        }
    }
    return resul;
}

//ReLU함수. 음수값을 모두 양수로 바꾸어 줌.
void ReLU(int*** res, int fil_num, int d_row, int d_col) {
    for (int i = 0; i < fil_num ; i++) {
        for (int j=0; j<d_row; j++) {
            for (int k=0; k<d_col; k++) {
                res[i][j][k] = max(0, res[i][j][k]);
            }
        }
    }
}

