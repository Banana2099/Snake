#include <vector>
#include <iostream>
#include <stdlib.h>

//Multiplication
template <typename T>
std::vector<std::vector<T>> matrix_c(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B ){

    std::vector<std::vector<T>> ans( A.size() , std::vector<T> (B[0].size(), 0));

    //Error 
    try
    {
        if( A[0].size() != B.size())
        {        
           std::cout << "A.Shape(" << A.size()<<","<<A[0].size() <<"), B.Shape("<<B.size()<<","<<B[0].size()<<")" << std::endl;
           throw std::invalid_argument("Error: Shape Cannot be matched");
        }
    }
    catch(std::invalid_argument const& error)
    {
        std::cout << error.what() <<std::endl;
        exit(1);
    }

    //mult
    for(int k = 0; k < ans.size(); k++){
        for(int i = 0; i < ans.at(0).size(); i++){ 
            for(int j = 0; j < B.size(); j++){
                ans[k][i] += A[k][j] * B[j][i];
            }
        }
    }   
    return ans;
}
//Transpose
template <typename T>
std::vector<std::vector<T>> matrix_T(std::vector<std::vector<T>> A){

    std::vector<std::vector<T>> ans( A[0].size() , std::vector<T> (A.size(), 0));

    for(int i = 0; i < ans.size(); i++)
    {
        for(int j = 0; j < ans.at(0).size(); j++)
        {
            ans[i][j] = A[j][i];

        }        
    }        
    return ans;
}

//Subtraction
template <typename T>
std::vector<std::vector<T>> matrix_s(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B ){

    std::vector<std::vector<T>> ans( A.size() , std::vector<T> (B[0].size(), 0));

    //Error 
    try
    {
        if( A[0].size() != B[0].size() || A.size() != B.size())
        {        
           std::cout << "A.Shape(" << A.size()<<","<<A[0].size() <<"), B.Shape("<<B.size()<<","<<B[0].size()<<")" << std::endl;
           throw std::invalid_argument("Error: Shape Cannot be matched");
        }
    }
    catch(std::invalid_argument const& error)
    {
        std::cout << error.what() <<std::endl;
        exit(1);
    }

    for(int i = 0; i < ans.size(); i++)
    {
        for(int j = 0; j < ans.at(0).size(); j++)
                ans[i][j] = A[i][j] - B[i][j];
    }        
    return ans;
}

//Adding
template <typename T>

std::vector<std::vector<T>> matrix_a(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B ){

    std::vector<std::vector<T>> ans( A.size() , std::vector<T> (B[0].size(), 0));

    //Error 
    try
    {
        if( A[0].size() != B[0].size() || A.size() != B.size())
        {        
           std::cout << "A.Shape(" << A.size()<<","<<A[0].size() <<"), B.Shape("<<B.size()<<","<<B[0].size()<<")" << std::endl;
           throw std::invalid_argument("Error: Shape Cannot be matched");
        }
    }
    catch(std::invalid_argument const& error)
    {
        std::cout << error.what() <<std::endl;
        exit(1);
    }

    for(int i = 0; i < ans.size(); i++)
    {
        for(int j = 0; j < ans.at(0).size(); j++)
            ans[i][j] = A[i][j] + B[i][j];
    }        
    return ans;
}

//multiplication single
template <typename T>
std::vector<std::vector<T>> matrix_m(std::vector<std::vector<T>> A, T number){
    for(int i = 0; i < A.size(); i++)
    {
        for(int j = 0; j < A.at(0).size(); j++)
            A[i][j] *= number;
    }        
    return A;
}

//create rand matrix
std::vector<std::vector<double>> matrix_rand(int row, int col){

    std::vector<std::vector<double>> A;
    std::vector<double> temp;    
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j ++){
            double x = (double) (rand() % (100-(-100) + 1)+(-100))/100;   
            temp.push_back(x);
        }        
        A.push_back(temp);
        temp.clear();
    }        
    return A;
}

//rerange matric
template <typename T>
std::vector<std::vector<T>> matrix_re(std::vector<std::vector<T>> A){
 
    //Error 
    try
    {
        if( A.size() > 1){        
           throw std::invalid_argument("Error: lenght is too long");
        }
    }
    catch(std::invalid_argument const& error)
    {
        std::cout << error.what() <<std::endl;
        exit(1);
    }

    std::vector<std::vector<T>> new_A(A.at(0).size(), std::vector<T> (A.at(0).size(), 0));

    for(int i = 0; i < A.at(0).size(); i++){
        for(int j = 0; j < A.at(0).size(); j++){
            if(i == j)
                new_A[i][j] = A.at(0).at(j);        
        }
    }        
    return new_A;
}

// matrix multiplication sp
template <typename T>
std::vector<std::vector<T>> matrix_m_sp(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B){
 
    //Error 
    try
    {
        if( A.at(0).size() != B.at(0).size()){        
           throw std::invalid_argument("Error: lenght not aligh");
        }
    }
    catch(std::invalid_argument const& error)
    {
        std::cout << error.what() <<std::endl;
        exit(1);
    }

    for(int col = 0; col < A.at(0).size(); col++){
        for(int row = 0; row < A.size(); row++)
            A[row][col] *= B.at(0).at(col);
    }
    return A;
}

//element wise
template <typename T>
std::vector<std::vector<T>> matrix_wise(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B){
 
    //Error 
    try
    {
        if( (A.at(0).size() != B.at(0).size()) || (A.size() != B.size()) ){        
           throw std::invalid_argument("Error: lenght not aligh");
        }
    }
    catch(std::invalid_argument const& error)
    {
        std::cout << error.what() <<std::endl;
        exit(1);
    }

    for(int col = 0; col < A.at(0).size(); col++){
        for(int row = 0; row < A.size(); row++)
            A[row][col] *= B.at(row).at(col);
    }
    return A;

}
