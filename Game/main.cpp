#include <SFML/Graphics.hpp>
#include <sstream>
#include "map.h"
#include "view.h"

class PLayer {
private: 
    float x, y;
public:
    float width, height, dx, dy, speed;
    int direction, score{ 0 }, health{ 100 };
    bool life = true;
    sf::String File;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    
    PLayer(sf::String F, float X, float Y, float W, float H, int score) 
    {
        x = X; y = Y; dx = 0; dy = 0; speed = 0; direction = 0, score = 0, health = 100;
        life = true;
        File = F;
        width = W; height = H;
        image.loadFromFile("C:/Users/ilyas/source/repos/Game/Main Characters/" + File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, width, height));
    }

    void update(float time) 
    {
        switch (direction) 
        {
            case 0: dx = speed; dy = 0; break;
            case 1: dx = -speed; dy = 0; break;
            case 2: dx = 0; dy = speed; break;
            case 3: dx = 0; dy = -speed; break;
        }
        x += dx * time;
        y += dy * time;

        speed = 0;
        sprite.setPosition(x, y);
        collision();
    }

    void collision() {
        for (int i = y / 32; i < (y + height) / 32 ; i++) {
            for (int j = x / 32; j < (x + width) / 32; j++) {
                if (TileMap[i][j] == '0') 
                {
                    if (dy > 0) y = (32 * i) - height;
                    if (dy < 0) y = (32 * i) + 32;
                    if (dx > 0) x = (32 * j) - width;
                    if (dx < 0) x = (332 * j) + 32;
                }

                if (TileMap[i][j] == 's') 
                {
                    score++;
                    TileMap[i][j] = ' ';
                }

                if (TileMap[i][j] == 't') 
                {
                    health -= 20;
                    TileMap[i][j] = ' ';
                }

                if (TileMap[i][j] == 'a') 
                {

                }
            }
        }
    }

    float getPlayerCoordinateX() { return x; }
    float getPlayerCoordinateY() { return y; }

};


int main(){
    sf::RenderWindow window(sf::VideoMode(640, 480), "Game");

    view.reset(sf::FloatRect(0, 0, 640, 480));

    PLayer player("Virtual Guy/Run (32x32).png", 250, 250, 32, 32, 0);
    sf::Clock clock;
    float currentFrame{ 0 };

    sf::Image mapImage, trapImage, appleImage;
    trapImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Traps/Spike Head/Idle.png");
    mapImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Terrain/Terrain (16x16).png");
    appleImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Items/Fruits/Apple.png");

    sf::Texture map, trap, apple;
    map.loadFromImage(mapImage);
    trap.loadFromImage(trapImage);
    apple.loadFromImage(appleImage);

    sf::Sprite spriteMap, spriteTrap, spriteApple;
    spriteMap.setTexture(map);
    spriteTrap.setTexture(trap);
    spriteApple.setTexture(apple);

    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::Text text("", font, 30);
    text.setStyle(sf::Text::Bold);
    //text.setColor(sf::Color::Red);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 400;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
        {
            player.direction = 1; player.speed = 0.1;
            currentFrame += 0.005 * time;
            if (currentFrame > 11) currentFrame -= 11;
            player.sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, -32, 32));
            getPlayersCoordinateForView(player.getPlayerCoordinateX(), player.getPlayerCoordinateY());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        { 
            player.direction = 0; player.speed = 0.1;
            currentFrame += 0.005 * time;
            if (currentFrame > 11) currentFrame -= 11;
            player.sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 32)); 
            getPlayersCoordinateForView(player.getPlayerCoordinateX(), player.getPlayerCoordinateY());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
        { 
            player.direction = 3; player.speed = 0.1;
            currentFrame += 0.005 * time;
            if (currentFrame > 11) currentFrame -= 11;
            player.sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 32)); 
            getPlayersCoordinateForView(player.getPlayerCoordinateX(), player.getPlayerCoordinateY());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
        {
            player.direction = 2; player.speed = 0.1;
            currentFrame += 0.005 * time;
            if (currentFrame > 11) currentFrame -= 11;
            player.sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 32));
            getPlayersCoordinateForView(player.getPlayerCoordinateX(), player.getPlayerCoordinateY());
        }

        player.update(time);
        window.setView(view);
        window.clear(sf::Color(228, 100, 100));

        for (int i = 0; i < HEIGHT_MAP; i++) 
        {
            for (int j = 0; j < WIDTH_MAP; j++) 
            {
                if (TileMap[i][j] == ' ') spriteMap.setTextureRect(sf::IntRect(144, 0, 31, 31));
                if (TileMap[i][j] == 's') spriteMap.setTextureRect(sf::IntRect(47, 128, 31, 31));
                if (TileMap[i][j] == '0') spriteMap.setTextureRect(sf::IntRect(320, 64, 31, 31));

                spriteMap.setPosition(j * 32, i * 32);
                window.draw(spriteMap);
                if (TileMap[i][j] == 't') 
                {
                    spriteTrap.setTextureRect(sf::IntRect(0, 0, 52, 52));
                    spriteTrap.setPosition(j * 32, i * 32);
                    spriteTrap.setOrigin(10.f, 20.f);
                    window.draw(spriteTrap);
                }
                if (TileMap[i][j] == 'a') 
                {
                    spriteApple.setTextureRect(sf::IntRect(0, 0, 32, 32));
                    spriteApple.setPosition(j * 32, i * 32);
                    window.draw(spriteApple);
                            
                }
            }
        }
        std::ostringstream playerScore;
        playerScore << player.score;
        text.setString("Collected:" + playerScore.str());
        text.setPosition(view.getCenter().x - 120, view.getCenter().y + 170);
        window.draw(text);
        window.draw(player.sprite);
        window.display();
    }

    return 0;
}
