#include <vector>
#include "matrix.hpp"
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iostream>

namespace ai{
class AI{
private:

    int input_n{}, output_n{}, ntimes{}, parant_size{10}, mut_r{5};
    std::vector <std::vector <std::vector <double>>> hidden_v, output_v, grad_w, grad_a, grad_z, nodes_result_v, nodes_input_v, w_store, bias_v;
    std::vector <std::vector <std::vector <std::vector <double>>>> bk_hidden_v, bk_output_v; 
    std::vector <std::vector <double>> target_v, test_input_v;
    const double default_value{0.5};
    double learn_rate{1.0}, n{};
    std::vector<std::vector<double>> test_output_one, cost, test_input_one, target_v_one;

public:

    AI(int x, int y){input_n = x, output_n = y;}
    void setting(std::vector <std::vector <double>>, std::vector <std::vector <double>>, int);
    void setting_without();

    void change_learn_rate(double);
    void add_hidden(int);
    void clean_hidden();
    void clean_result();

    double d_active(double);
    std::vector<std::vector<double>> activation(std::vector<std::vector<double>>, char);
    double active(double);

    void update_w();
    void back_propagation(std::vector<std::vector<double>>, std::vector<std::vector<double>>);
    std::vector<std::vector<double>> calculate(std::vector<std::vector<double>>);
    void fit();

    void test_h();
    void mutate(std::vector <std::vector <std::vector <double>>> &w, int m_rate);
    void save();
    void crossover();
    void new_ai();

    std::vector <std::vector <std::vector <double>>> get_result();
};

//
void AI::crossover(){
    int f{},m{},cho{};
    f = rand() % bk_hidden_v.size();
    m = rand() % bk_hidden_v.size();
    
    //in case same number
    while(f == m)
        m = rand() % bk_hidden_v.size();

    //create new weights
    for(int i = 0; i < hidden_v.size(); i++){
        for(int j = 0; j < hidden_v.at(i).size(); j++){
            for(int k = 0; k < hidden_v.at(i).at(j).size(); k++){
                cho = rand() % parant_size + 1;
                hidden_v[i][j][k] =(cho > parant_size/2)?bk_hidden_v.at(f)[i][j][k]:bk_hidden_v.at(m)[i][j][k];
            }    
        }        
    } 
    for(int i = 0; i < output_v.size(); i++){
        for(int j = 0; j < output_v.at(i).size(); j++){
            for(int k = 0; k < output_v.at(i).at(j).size(); k++){
                cho = rand() % parant_size + 1;
                output_v[i][j][k] =(cho > parant_size/2)?bk_output_v.at(f)[i][j][k]:bk_output_v.at(m)[i][j][k];
            } 
        }        
    } 
    mutate(hidden_v, mut_r);
    mutate(output_v, mut_r); 
}

//bk as parants 
void AI::save(){
    bk_hidden_v.push_back(hidden_v);
    bk_output_v.push_back(output_v);
    if(bk_hidden_v.size() > parant_size && bk_output_v.size() > parant_size){
        bk_hidden_v.erase(bk_hidden_v.begin());
        bk_output_v.erase(bk_output_v.begin());
    }
}

//change all weights
void AI::new_ai(){
    for(int i = 0; i < hidden_v.size(); i++){
        for(int j = 0; j < hidden_v.at(i).size(); j++){
            for(int k = 0; k < hidden_v.at(i).at(j).size(); k++){
                hidden_v[i][j][k] = (double) (rand() % (100-(-100) + 1)+(-100))/100; 
            }    
        }        
    } 
    for(int i = 0; i < output_v.size(); i++){
        for(int j = 0; j < output_v.at(i).size(); j++){
            for(int k = 0; k < output_v.at(i).at(j).size(); k++){
                output_v[i][j][k] =  (double) (rand() % (100-(-100) + 1)+(-100))/100; 
            } 
        }        
    }
}

//mulation function
void AI::mutate(std::vector <std::vector <std::vector <double>>> &w, int m_rate){
    for(int i = 0; i < w.size(); i++){
        for(int j = 0; j < w.at(i).size(); j++){
            for(int k = 0; k < w.at(i).at(j).size(); k++){
                int r{rand() % 100};     
                if(r < m_rate)    
                    w[i][j][k] += (double) (rand() % (100-(-100) + 1)+(-100))/100; 
                if(w[i][j][k] > 1) w[i][j][k] =1;
                if(w[i][j][k] < -1) w[i][j][k] =-1;
            }    
        }        
    } 
}


//change learning rate
void AI::change_learn_rate(double rate){
    learn_rate = rate;
}

//get node result
std::vector <std::vector <std::vector <double>>> AI::get_result(){
    return nodes_result_v;
}

//test output nodes
void AI::test_h(){
    for(int k = 0; k < hidden_v.size(); k++){
        std::cout<< "layer weights:" << k + 1<< std::endl;    
        for(int i =0; i < hidden_v[k].size();i++){                                             
            for(int j = 0; j < hidden_v[k][i].size(); j++){
                std::cout<< hidden_v[k][i][j] << " ";
            }
            std::cout << "\n";
        }
    } 
    std::cout<< "output_v"<< std::endl;    
    for(int i =0; i < output_v[0].size();i++){                                             
        for(int j = 0; j < output_v[0][i].size(); j++){
            std::cout<< output_v[0][i][j] << " ";
        }
        std::cout << "\n";
    }
}        

//basic setting
void AI::setting(std::vector <std::vector <double>> i,std::vector <std::vector <double>> goal, int n){
    ntimes = n;
    test_input_v = i;    
    target_v = goal;    
    output_v.clear();
    output_v.push_back(matrix_rand(hidden_v[hidden_v.size()-1][0].size(), output_n)); 
    bias_v.push_back(matrix_rand(1, output_n));
}

//setting without parameters
void AI::setting_without(){
    output_v.clear();
    output_v.push_back(matrix_rand(hidden_v[hidden_v.size()-1][0].size(), output_n)); 
    bias_v.push_back(matrix_rand(1, output_n));
}


//remove all hidden
void AI::clean_hidden(){
    hidden_v.clear();
}

//increase hidden
void AI::add_hidden(int nods){
    //create weights
    if(hidden_v.size() != 0)
        hidden_v.push_back(matrix_rand(hidden_v[hidden_v.size() - 1].at(0).size() , nods)); 
    else
        hidden_v.push_back(matrix_rand(input_n, nods));        

    //create bias
    bias_v.push_back(matrix_rand(1, nods));
}

// fuctions
double AI::active(double x){
    return 1/ (1 + exp(-x));
}

double AI::d_active(double x){        
    return active(x) * (1-active(x));
}
//activation function
std::vector<std::vector<double>> AI::activation(std::vector<std::vector<double>> v, char type){

    if(type=='i' || type=='d'){        
        for(int u = 0; u < v.size(); u++){ 
            for(int j = 0; j < v[u].size(); j++){
                v.at(u).at(j) = (type == 'i') ? active(v.at(u).at(j)): d_active(v.at(u).at(j));    
            }   
        }    
    }
    else
        std::cout<<"wrong activation paramater"<<std::endl;   
    return v; 
}        

//calculate prediction {only accept (1 x nodes) shape inputs}
std::vector<std::vector<double>> AI::calculate(std::vector<std::vector<double>> input_v){

    for(int i = 0; i < hidden_v.size(); i++){
        //z inputs    
        input_v = matrix_c(input_v, hidden_v[i]);  
        input_v = matrix_s(input_v, bias_v[i]);
        nodes_input_v.push_back(matrix_T(input_v));        

        //active function
        input_v = activation(input_v,'i'); 
        nodes_result_v.push_back(matrix_T(input_v));

    }
    //result
    input_v = matrix_c(input_v, output_v[0]);   
    input_v = matrix_s(input_v, bias_v[bias_v.size() - 1]);
    nodes_input_v.push_back(matrix_T(input_v));
    input_v = activation(input_v,'i'); 

 
    return input_v;
}

//calculate gradient of weight
void AI::back_propagation(std::vector<std::vector<double>> cost, std::vector<std::vector<double>> input_one){

    //last layer
    grad_z.insert(grad_z.begin(), activation(nodes_input_v.at(nodes_input_v.size() - 1), 'd'));     
    grad_a.insert(grad_a.begin(), matrix_wise(matrix_T(cost), grad_z.at(0)));
    grad_w.insert(grad_w.begin(), matrix_c(nodes_result_v[nodes_result_v.size()-1], matrix_T(grad_a.at(0))));
 

    //middle layers
    grad_z.insert(grad_z.begin(), activation(nodes_input_v.at(nodes_input_v.size() - 2), 'd'));
    grad_a.insert(grad_a.begin(), matrix_wise(matrix_c(output_v.at(0), grad_a.at(0)), grad_z.at(0)));

    for(int i = hidden_v.size() - 2 ; i >= 0; i-- ){
        grad_w.insert(grad_w.begin(), matrix_c(nodes_result_v[i], matrix_T(grad_a.at(0))));
        grad_z.insert(grad_z.begin(), activation(nodes_input_v.at(i), 'd'));
        grad_a.insert(grad_a.begin(), matrix_wise(matrix_c(hidden_v[i+1], grad_a.at(0)),grad_z.at(0)));

    }

    //first layer
    grad_w.insert(grad_w.begin(), matrix_c(matrix_T(input_one), matrix_T(grad_a.at(0))));

   //store weights
    for(int j = 0; j <= hidden_v.size(); j++ ){
        if(w_store.size() <= hidden_v.size())
            w_store.push_back(grad_w[j]);
        else
            w_store[j] = matrix_a(w_store[j], grad_w[j]);            
    }

    //acc n times
    n++;
 
    //clear
    grad_z.clear();
    grad_a.clear();
    grad_w.clear();
}

//clean result
void AI::clean_result(){
    nodes_result_v.clear();        
    nodes_input_v.clear();
}


// update weights 
void AI::update_w(){
    n = 1 / n; 

    //update
    for(int j = 0; j < w_store.size() - 1; j++ ){
        hidden_v[j] = matrix_s(hidden_v[j],matrix_m(matrix_m(w_store[j], n), learn_rate)); 
    }
    output_v[0] = matrix_s(output_v[0], matrix_m(matrix_m(w_store[w_store.size() - 1], n), learn_rate));

    //clear
    w_store.clear();
    n = 0;
}

//run multiple times 
void AI::fit(){ 

    for(int i = 0; i < ntimes; i++){
        for(int j = 0; j < test_input_v.size(); j++){    
            //split input    
            test_input_one.push_back(test_input_v.at(j));
            target_v_one.push_back(target_v.at(j));

            //run 
            test_output_one = calculate(test_input_one);     
            cost = matrix_m(matrix_s(test_output_one, target_v_one), 2.0);
            back_propagation(cost, test_input_one);

            //clear
            clean_result();
            test_input_one.clear();
            target_v_one.clear();
        }
        update_w();
    }
}        


};

