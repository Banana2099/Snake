#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <stdlib.h>


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


    //game setting
    short pause_flag {1}; //for pause the game
    const unsigned int FPS {60};
    float speed {10};
    static long score;
    static int tail_len;
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
    


public:
    void draw();
    void close_event();
    void move_event();
    void reset_event();
    void pause_sign();
    void add_move();

    //draw game    
    void background();
    void snake();

    //the main loop would be in the run function
    void run();

    //AI functions

};

long game::score = 0;
int game::tail_len = 2;

void game::reset_event(){
    direction.x = 1;
    direction.y = 0;
    snake_head.x = 16;
    snake_head.y = 16;
    tail_len = 2;
    snake_tail.clear();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            matrix[i][j] = -1;
        }        
    }
    score = 0;
    pause_flag = -2;
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
    if(snake_head.x >= column)
        snake_head.x = 0;
    else if(snake_head.x<0)
        snake_head.x = column -1;   

    if(snake_head.y >= row)
        snake_head.y = 0;
    else if(snake_head.y < 0)
        snake_head.y = row-1;

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
        score += 10;
    }
    //game over
    if(matrix[snake_head.x][snake_head.y] == 1)
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
        if(direction.x != 1 && mm == 'L') 
        {
            direction = sf::Vector2i(-1,0); 
            moves.clear();
        }
        if(direction.x != -1 && mm == 'R')
        {
            direction = sf::Vector2i(1,0); 
            moves.clear();
        }
        if(direction.y != 1 && mm == 'U')
        {  
            direction = sf::Vector2i(0,-1); 
            moves.clear();
        }
        if(direction.y != -1 && mm == 'D') 
        {  
            direction = sf::Vector2i(0,1); 
            moves.clear();
        }
     }
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
    std::stringstream ss;
    ss << "SCORE: " << score;
    score_text.setString(ss.str());

    if(TimeSinceLastMove.asSeconds() >= TimePerFrame.asSeconds())
    {        
        window.clear();

        move_event();
        snake();
        background();
        window.draw(score_text);

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
                break;
            default:
                break;    
        }

    }
}        

void game::run(){
    //other initialisation
    font.loadFromFile("/Users/TSHung/Downloads/ProjectC/font/Arial.ttf");
    window.create(sf::VideoMode( cell_size * column * resize + 400, cell_size * row * resize + 50), "Program");
    window.setFramerateLimit(FPS);

    score_text.setFillColor(sf::Color::White);
    score_text.setFont(font);
    score_text.setPosition(10,cell_size * column * resize );
    
    //snake initialisation
    matrix[snake_head.x][snake_head.y] = 1;

     while(window.isOpen())
    {
        sf::Time df = clock.restart();
        TimeSinceLastMove += df;
        
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

        close_event();

    }
}



int main(void){

    game gg;
    gg.run();

    return 0;
} 




