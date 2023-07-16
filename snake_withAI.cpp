#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "headers/AI.hpp"
#include <iostream>
        

class game{
private:
    // window
    const unsigned int cell_size {7};
    const unsigned int column {32};
    const unsigned int row {32};
    const unsigned int resize {3};
    sf::RenderWindow window;
    sf::Font font;
    sf::Text pause_text;
    sf::Text score_text;
    sf::Text epoch_text;
    sf::Text step_text;
    sf::Text gen_text;
    sf::Text gfun_text;
    sf::Text ogfun_text;

    //game setting
    short pause_flag {1}; //for pause the game
    const unsigned int FPS {60};
    float speed {10};
    long goal{1000},epoch_max{500};
    static long score, highest_score;
    static long epoch;
    static long gen;
    const int defualt_value{10}; 
    double g_fun{}, old_g_fun{};
    int tail_len{defualt_value}, snake_step{}, step_max{60};
    sf::Vector2f snake_head{16,16}; 
    std::vector<sf::Vector2f> snake_tail;
    sf::Vector2i fruit_loc{rand() % 32, rand() % 32};
    sf::Vector2i direction {1,0};
    std::vector<char> moves;

    sf::Time TimeSinceLastMove {sf::Time::Zero};
    sf::Time TimePerFrame {sf::seconds(1.f/ speed)};
    sf::Clock clock ;
    std::vector<std::vector<short>> matrix {row, std::vector<short> (column, -1)};
    sf::RectangleShape rect{sf::Vector2f(cell_size * resize -1 ,cell_size * resize -1 )};

    //AI setting
    int input_n{28}, output_n{4}, h_1{22}, h_2{17}, control_sign{};
    std::vector<int> nodes_v {input_n, h_1, h_2, output_n};
    std::vector<double> ai_control_v; 
    std::vector <std::vector <std::vector <double>>> nodes_result_v;
    std::vector <std::vector <double>> ai_input;

    float node_size{8};
    ai::AI ai{input_n, output_n};

    
    sf::CircleShape node{node_size};
    sf::Vertex node_line[2] =
    {
        sf::Vector2f(0,0),
        sf::Vector2f(100,100)      
    };
    

public:
    void close_event();
    void move_event();
    void reset_event();
    void pause_sign();
    void add_move();

    //draw game    
    void background();
    void snake();
    void draw();

    //the main loop would be in the run function
    void run();

    //AI functions
    void draw_neural();
    void make_input();
    void ai_control();
    void ai_update();
};

long game::score = 0;
long game::epoch = 0;
long game::gen = 1;
long game::highest_score = 0;

//ai change weights
void game::ai_update(){
/*
    double x = static_cast<double>((score - highest_score) * 2);
    std::vector<std::vector<double>> cost{{x,x,x,x}};
    ai.back_propagation(cost, ai_input);
    ai.update_w();
*/  
    if (g_fun > old_g_fun){
        old_g_fun = g_fun;
        ai.save();
    }

    if(epoch == epoch_max){
        epoch = 0; 
        gen++;
    }        
    else
        epoch++;
    
    if(gen == 1)
        ai.new_ai();
    else
        ai.crossover();
        
}        

//create ai inputs
void game::make_input(){
    double diff_x{abs(fruit_loc.x - snake_head.x)}, diff_y{abs(snake_head.y - fruit_loc.y)};
    double diff_z{ sqrt(pow(diff_x, 2) + pow(diff_y, 2))};
//    double diff_x{1},diff_y{1},diff_z{1};

    std::vector<std::vector<double>> in_v(3, std::vector<double>(8, 99));//0:fruit 1:body 2:wall 
    double x{1},y{1},z{1};
    double d_u{},d_d{},d_l{},d_r{};
   
    //friut
    in_v[0][0] = (snake_head.x == fruit_loc.x && snake_head.y > fruit_loc.y)? diff_y: 0;
    in_v[0][1] = (snake_head.x == fruit_loc.x && snake_head.y < fruit_loc.y)? diff_y: 0;
    in_v[0][2] = (snake_head.y == fruit_loc.y && snake_head.x > fruit_loc.x)? diff_x: 0;
    in_v[0][3] = (snake_head.y == fruit_loc.y && snake_head.x < fruit_loc.x)? diff_x: 0;
    in_v[0][4] = (snake_head.x > fruit_loc.x && snake_head.y > fruit_loc.y)? diff_z: 0;
    in_v[0][5] = (snake_head.x < fruit_loc.x && snake_head.y > fruit_loc.y)? diff_z: 0;
    in_v[0][6] = (snake_head.x > fruit_loc.x && snake_head.y < fruit_loc.y)? diff_z: 0;
    in_v[0][7] = (snake_head.x < fruit_loc.x && snake_head.y < fruit_loc.y)? diff_z: 0;
 
    //body
    for(int i = snake_tail.size()-1; i >= 0; i--){
        x = abs(snake_head.x - snake_tail.at(i).x);
        y = abs(snake_head.y - snake_tail.at(i).y);
        z = sqrt(pow(x,2) + pow(y,2));

        in_v[1][0] = (snake_head.x == snake_tail.at(i).x && snake_head.y > snake_tail.at(i).y)?y:in_v[1][0];
        in_v[1][1] = (snake_head.x == snake_tail.at(i).x && snake_head.y < snake_tail.at(i).y)?y:in_v[1][1];
        in_v[1][2] = (snake_head.y == snake_tail.at(i).y && snake_head.x > snake_tail.at(i).x)?x:in_v[1][2];
        in_v[1][3] = (snake_head.y == snake_tail.at(i).y && snake_head.x < snake_tail.at(i).x)?x:in_v[1][3];
        in_v[1][4] = (snake_head.x > snake_tail.at(i).x && snake_head.y > snake_tail.at(i).y)?z:in_v[1][4];
        in_v[1][5] = (snake_head.x < snake_tail.at(i).x && snake_head.y > snake_tail.at(i).y)?z:in_v[1][5];
        in_v[1][6] = (snake_head.x > snake_tail.at(i).x && snake_head.y < snake_tail.at(i).y)?z:in_v[1][6];
        in_v[1][7] = (snake_head.x < snake_tail.at(i).x && snake_head.y < snake_tail.at(i).y)?z:in_v[1][7];
    }        

    //wall
    in_v[2][0] = snake_head.y;      //u
    in_v[2][1] = 31 - snake_head.y; //d
    in_v[2][2] = snake_head.x;      //l
    in_v[2][3] = 31 - snake_head.x; //r
    in_v[2][4] = (in_v[2][0]>0 && in_v[2][2]>0) ? sqrt(pow(in_v[2][0],2) + pow(in_v[2][2], 2)): 0;
    in_v[2][5] = (in_v[2][0]>0 && in_v[2][3]>0) ? sqrt(pow(in_v[2][0],2) + pow(in_v[2][3], 2)): 0;
    in_v[2][6] = (in_v[2][1]>0 && in_v[2][2]>0) ? sqrt(pow(in_v[2][1],2) + pow(in_v[2][2], 2)): 0;
    in_v[2][7] = (in_v[2][1]>0 && in_v[2][3]>0) ? sqrt(pow(in_v[2][1],2) + pow(in_v[2][3], 2)): 0;

    //direction
    if (direction.x > 0 && direction.y ==0) d_r = 1;
    else if (direction.x < 0 && direction.y ==0) d_l = 1;
    else if (direction.x == 0 && direction.y > 0) d_u = 1;
    else d_d = 1;

    //replace zero
    for(int i = 0; i < in_v.size(); i++){
        for(int j = 0; j < in_v.at(i).size(); j++)
            if(in_v[i][j] == 99) in_v[i][j] = 0;       
    }

    //fruit{u d l r ul ur dl dr} body{u d l r ul ur dl dr} wall{u d l r ul ur dl dr} direction{u d l r}
    ai_input = std::vector<std::vector<double>>{
               {in_v[0][0],in_v[0][1],in_v[0][2],in_v[0][3],in_v[0][4],in_v[0][5],in_v[0][6],in_v[0][7],
                in_v[1][0],in_v[1][1],in_v[1][2],in_v[1][3],in_v[1][4],in_v[1][5],in_v[1][6],in_v[1][7],
                in_v[2][0],in_v[2][1],in_v[2][2],in_v[2][3],in_v[2][4],in_v[2][5],in_v[2][6],in_v[2][7],
                d_u,d_d,d_l,d_r}};
   }

//network control
void game::ai_control(){

    double max_c{};  
    make_input();
    ai_control_v = ai.calculate(ai_input).at(0);
    nodes_result_v = ai.get_result();
    nodes_result_v.insert(nodes_result_v.begin(), matrix_T(ai_input));

    max_c = *std::max_element(ai_control_v.begin(), ai_control_v.end()); 

    if(ai_control_v.at(0) == max_c)
        moves.insert(moves.begin(),'L'), control_sign = 0; 
    else if(ai_control_v.at(1) == max_c)
        moves.insert(moves.begin(),'R'), control_sign = 1;
    else if(ai_control_v.at(2) == max_c)
        moves.insert(moves.begin(),'U'), control_sign = 2;
    else if(ai_control_v.at(3) == max_c)
        moves.insert(moves.begin(),'D'), control_sign = 3;

}



void game::draw_neural(){
    int p{0},row_p{0}, row_p_b{0};
    for(auto node_s: nodes_v){
        row_p = nodes_v.at(0) - node_s;   
        //nodes
        for(int i = 0; i < node_s; i++){
            node.setPosition(sf::Vector2f(column * cell_size * resize + 40 + node_size * p * 24, 3 * i * node_size + 10+ row_p * node_size*1.5));
            
            //make lights
            if(p < nodes_v.size()-1){
                if(abs(nodes_result_v.at(p).at(i).at(0)) >= 1)
                    node.setFillColor(sf::Color::Red);
                else
                    node.setFillColor(sf::Color(0,0,15));
            }    
            else{
                if(i == control_sign)
                    node.setFillColor(sf::Color::Green);
                else
                    node.setFillColor(sf::Color(0,0,15));
            }        

            window.draw(node); 
        }
        p++;

        //draw lines
        if(p < nodes_v.size()){
            int line_p = p-1; 
            row_p_b = nodes_v.at(0) - nodes_v.at(p-1);
            for(int i = 0; i < nodes_v.at(p-1); i++){
                for(int j = 0; j < nodes_v.at(p); j++){

                    node_line[0] = sf::Vector2f(column * cell_size * resize + 40 +node_size*2 + node_size * 24 * (p-1), 3 * i * node_size + 10 +node_size+ node_size * row_p_b* 1.5);
                    node_line[1] = sf::Vector2f(column * cell_size * resize + 40 + node_size * p * 24, 3 * j * node_size + 90 + ( (row_p < 10)? pow(row_p, 2)*1.8 : pow(row_p,2)*1.8));
                    
                    node_line[0].color = sf::Color::Red,node_line[1].color = sf::Color::Blue;

                    window.draw(node_line, 2 , sf::Lines);
                    
                }
            }           
        }
    }
}

// game over setting 
void game::reset_event(){
      
    direction.x = 1;
    direction.y = 0;
    snake_head.x = 16;
    snake_head.y = 16;
    tail_len = defualt_value;
    snake_step = 0;
       
    ai_update();

    if(score > highest_score)
        highest_score = score;
        
    snake_tail.clear();
    moves.clear();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            matrix[i][j] = -1;
        }        
    }
    score = 0;
}        

void game::snake(){
    //tail recover
    snake_tail.insert(snake_tail.begin(), snake_head);
    
    if(snake_tail.size() >= tail_len)
    {   
        sf::Vector2f mov {snake_tail[snake_tail.size() - 1]};
        snake_tail.pop_back();
        matrix[mov.x][mov.y] *= -1;
    }   

    //head movement  
    snake_head.x += 1 * direction.x;
    snake_head.y += 1 * direction.y;
    /*
    if(snake_head.x >= column)
        snake_head.x = 0;
    else if(snake_head.x<0)
        snake_head.x = column -1;   

    if(snake_head.y >= row)
        snake_head.y = 0;
    else if(snake_head.y < 0)
        snake_head.y = row-1;
    */

    //ai adjustment
    if(snake_head.x >= column || snake_head.x < 0 || snake_head.y >= row || snake_head.y < 0)
        reset_event();

    //fruit 
    matrix[fruit_loc.x][fruit_loc.y] = 2;
    if(fruit_loc.x == snake_head.x && fruit_loc.y == snake_head.y)
    {
        matrix[fruit_loc.x][fruit_loc.y] = -1;
        fruit_loc.x = rand() % 32;
        fruit_loc.y = rand() % 32;
        //in case fruit location and body are the same
        while(matrix[fruit_loc.x][fruit_loc.y]==1) 
        {
            fruit_loc.x = rand() % 32;
            fruit_loc.y = rand() % 32;
        }        
        tail_len += 1;
        score += 1;
        snake_step = 0;
    }
    //game over
    if(matrix[snake_head.x][snake_head.y] == 1)
        reset_event();
    else if(snake_step >= step_max)
        reset_event();
    matrix[snake_head.x][snake_head.y] *= -1 ;    
}

//add move
void game::add_move(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moves.insert(moves.begin(),'L'); 
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moves.insert(moves.begin(),'R');
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        moves.insert(moves.begin(),'U');
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moves.insert(moves.begin(),'D');
}

void game::move_event(){
    char mm {'N'};
    while(!moves.empty())
    {   
        mm = moves[moves.size()-1];    
        moves.pop_back();
        if(direction.x != 1 && direction != sf::Vector2i(-1,0)&& mm == 'L') 
        {
            direction = sf::Vector2i(-1,0); 
            moves.clear();
            snake_step++;           
        }
        if(direction.x != -1 && direction != sf::Vector2i(1,0)&& mm == 'R')
        {
            direction = sf::Vector2i(1,0); 
            moves.clear();
            snake_step++;
        }
        if(direction.y != 1 && direction != sf::Vector2i(0,-1)&& mm == 'U')
        {  
            direction = sf::Vector2i(0,-1); 
            moves.clear();
            snake_step++;
        }
        if(direction.y != -1 && direction != sf::Vector2i(0,1) && mm == 'D') 
        {  
            direction = sf::Vector2i(0,1); 
            moves.clear();
            snake_step++;
        }
     }
    g_fun = snake_step + (pow(2, score) + pow(score, 2.1)*500) - (pow(score,1.2)*pow((0.25*snake_step), 1.3));
   
}

void game::background(){
    for(unsigned int i = 0; i< row; i++)    
    {
        for(unsigned j = 0; j< column; j++)
        {
            rect.setPosition(cell_size * resize * i,cell_size * resize * j);      
            if(matrix[i][j] < 0)
                rect.setFillColor(sf::Color(0,0,20));
            else if(matrix[i][j] == 2)
                rect.setFillColor(sf::Color(200,0,10));
            else
                rect.setFillColor(sf::Color::White);

            window.draw(rect);
        }                       
    }        
}        

void game::draw(){ 
    std::stringstream ss{}, ep{}, sp{}, ge{}, gf{}, ogf{};
    ss << "SCORE: " << score;
    score_text.setString(ss.str());

    ep << "Epoch: " << epoch;
    epoch_text.setString(ep.str());
    
    sp << "Step: " << snake_step;
    step_text.setString(sp.str());

    ge << "Gen: " << gen;
    gen_text.setString(ge.str());

    gf << "Finess: " << g_fun;
    gfun_text.setString(gf.str());

    ogf << "best_F: " << old_g_fun;
    ogfun_text.setString(ogf.str());

    if(TimeSinceLastMove.asSeconds() >= TimePerFrame.asSeconds())
    {        
        window.clear();

        ai_control();
        move_event();
        snake();

        background();
        draw_neural();

        window.draw(score_text);
        window.draw(epoch_text);
        window.draw(step_text);
        window.draw(gen_text);
        window.draw(gfun_text);
        window.draw(ogfun_text);

        window.display();
        TimeSinceLastMove = sf::Time::Zero;
    }
} 

void game::pause_sign(){
    pause_text.setFillColor(sf::Color::White);
    pause_text.setFont(font);
    pause_text.setPosition( cell_size * column * resize + 100, cell_size * row * resize / 2);
    pause_text.setString("PAUSE");

    window.clear();

    background();
    window.draw(score_text);
    window.draw(pause_text);
    window.draw(step_text);
    window.draw(gen_text);
    window.draw(gfun_text);
    window.draw(ogfun_text);

    window.display();
}

//for any event
void game::close_event(){
    sf::Event event;
    while (window.pollEvent(event))
    {    
        switch(event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();
                add_move(); 
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    speed = 100;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
                    speed = 10;

                break;
            default:
                break;    
        }
    }
}        

void game::run(){
    //other initialisation
    font.loadFromFile("font/Arial.ttf");
    window.create(sf::VideoMode( cell_size * column * resize + 700, cell_size * row * resize + 50), "Program");
    window.setFramerateLimit(FPS);

    score_text.setFillColor(sf::Color::White);
    score_text.setFont(font);
    score_text.setPosition(10, cell_size * column * resize);

    epoch_text.setFillColor(sf::Color::White);
    epoch_text.setFont(font);
    epoch_text.setPosition(cell_size * column * resize, cell_size * column * resize);

    step_text.setFillColor(sf::Color::White);
    step_text.setFont(font);
    step_text.setPosition(cell_size * column * resize * 1.4, cell_size * column * resize);

    gen_text.setFillColor(sf::Color::White);
    gen_text.setFont(font);
    gen_text.setPosition(cell_size * column * resize * 1.7, cell_size * column * resize);

    gfun_text.setFillColor(sf::Color::White);
    gfun_text.setFont(font);
    gfun_text.setCharacterSize(20);
    gfun_text.setPosition(cell_size * column * resize * 1.7, cell_size * (column - 3)* resize);
 
    ogfun_text.setFillColor(sf::Color::White);
    ogfun_text.setFont(font);
    ogfun_text.setCharacterSize(20);
    ogfun_text.setPosition(cell_size * column * resize * 1.7, cell_size * (column - 2) * resize);
 
    //snake initialisation
    matrix[snake_head.x][snake_head.y] = 1;

    //AI initialisation
    ai.add_hidden(h_1);
    ai.add_hidden(h_2);
    ai.setting_without();
    ai.change_learn_rate(0.1);
    node.setOutlineThickness(2);
    node.setOutlineColor(sf::Color(0, 100, 150));


    while(window.isOpen())
    {
        sf::Time df = clock.restart();
        TimeSinceLastMove += df;
        TimePerFrame = sf::seconds(1.f/speed);

        //pause
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        { 
            pause_sign();
            pause_flag *= -1;
            sf::sleep(sf::seconds(0.2));
            TimeSinceLastMove = sf::Time::Zero;
        }
        
        //restart
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {   
            if(pause_flag == -2) 
            {
                pause_flag = 1;    
                sf::sleep(sf::seconds(0.2));      
            }    
        }
        
        //game update
        if(pause_flag > 0)  
            draw();

        //close
        close_event();

    }
}



int main(void){

    game gg;
    gg.run();

    return 0;
} 




