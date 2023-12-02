#include <SFML/Graphics.hpp>
#include <sstream>
#include "view.h"
#include "map.h"

class Entity 
{
public:
    float dx, dy, x, y, speed, moveTimer;
    int width, height, health;
    bool life, isMove, onGround;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::String name;

    Entity(sf::Image& image, sf::String Name, float X, float Y, int W, int H)
    {
        x = X; y = Y; width = W; height = H; name = Name; moveTimer = 0;
        speed = 0; health = 100; dx = 0; dy = 0;
        life = true; onGround = false; isMove = false;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    }

    sf::FloatRect getRect()
    {
        return sf::FloatRect(x, y, width, height);
    }
};


class Player: public Entity
{
    public:
        enum {left, right, up, down, jump, stay} state;
        int score;

    Player(sf::Image &image, sf::String Name, float X, float Y, int W, int H): Entity(image, Name, X, Y, W, H)
    {
        score = 0; state = stay;
        if (name == "Virtual Guy") sprite.setTextureRect(sf::IntRect(0, 0, width, height));

    }

    void control() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            state = left; speed = 0.1;
            /*currentFrame += 0.005 * time;
            if (currentFrame > 11) currentFrame -= 11;
            player.sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, -32, 32));
            getPlayersCoordinateForView(player.getPlayerCoordinateX(), player.getPlayerCoordinateY());*/
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            state = right; speed = 0.1; 
            /*currentFrame += 0.005 * time;
            if (currentFrame > 11) currentFrame -= 11;
            player.sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 32));
            getPlayersCoordinateForView(player.getPlayerCoordinateX(), player.getPlayerCoordinateY());*/
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (onGround))
        {
            state = jump; onGround = false; dy = -0.35; 
            /*currentFrame += 0.005 * time;
            if (currentFrame > 11) currentFrame -= 11;
            player.sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 32));
            getPlayersCoordinateForView(player.getPlayerCoordinateX(), player.getPlayerCoordinateY());*/
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            state = down; speed = 0.1;
            /*currentFrame += 0.005 * time;
            if (currentFrame > 11) currentFrame -= 11;
            player.sprite.setTextureRect(sf::IntRect(32 * int(currentFrame), 0, 32, 32));
            getPlayersCoordinateForView(player.getPlayerCoordinateX(), player.getPlayerCoordinateY());*/
        }
    }

    void update(float time) 
    {
        control();
        switch (state) 
        {
            case right: dx = speed; break;
            case left: dx = -speed; break;
            case up: break;
            case down: dx = 0; break;
            case jump: break;
            case stay: break;
        }
        x += dx * time; collision(dx, 0);
        y += dy * time; collision(0, dy);

        sprite.setPosition(x, y);
        
        if (life) setPlayersCoordinateForView(x, y);

        if (health <= 0) 
        {
            life = false; 
            speed = 0;
        }

        if (!isMove) speed = 0;

        dy += 0.0008 * time;
    }

    void collision(float Dx, float Dy) 
    {
        for (int i = y / 32; i < (y + height) / 32 ; i++) 
        {
            for (int j = x / 32; j < (x + width) / 32; j++) 
            {
                if (TileMap[i][j] == '1' || TileMap[i][j] == '0')
                {
                    if (Dy > 0) { y = (32 * i) - height; dy = 0; onGround = true; }
                    if (Dy < 0) { y = (32 * i) + 32; dy = 0; }
                    if (Dx > 0) x = (32 * j) - width;
                    if (Dx < 0) x = (32 * j) + 32;
                }

                if (TileMap[i][j] == 't') 
                {
                    if (health < 35) {
                        health = 0;
                    }
                    else health -= 35;
                    TileMap[i][j] = ' ';
                }

                if (TileMap[i][j] == 'a') 
                {
                    if (health + 20 >= 100) health = 100;
                    else health += 20;
                    TileMap[i][j] = 'c';
                }
            }
        }
    }

    float getPlayerCoordinateX() { return x; }
    float getPlayerCoordinateY() { return y; }

};

class Enemy: public Entity
{
public:
    Enemy(sf::Image& image, sf::String Name, float X, float Y, int W, int H): Entity(image, Name, X, Y, W, H)
    {
        sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        dx = 0.05;
    }

    void collision(float Dx, float Dy)
    {
        for (int i = y / 32; i < (y + height) / 32; i++)
        {
            for (int j = x / 32; j < (x + width) / 32; j++)
            {
                if (TileMap[i][j] == '0' || TileMap[i][j] == '1')
                {
                    if (Dy > 0) { y = i * 32 - height; }
                    if (Dy < 0) { y = i * 32 + 32; }
                    if (Dx > 0) { x = j * 32 - width; dx = -0.08; sprite.setScale(-1, 1); }
                    if (Dx < 0) { x = j * 32 + 32; dx = 0.08; sprite.setScale(1, 1); }
                }
            }
        }
    }

    void update(float time) 
    {
        collision(dx, 0); x += dx * time; 

        if (name == "Mask Dude")
        {
            sprite.setPosition(x, y);
            if (health <= 0) { life = false; }
        }

        dy += 0.008 * time;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Game");
    view.reset(sf::FloatRect(0, 0, 640, 480));

    sf::Clock clock;
    float currentFrame{ 0 }, * curr = &currentFrame, currentFrameApple{ 0 }, currentFrameCollected{ 0 };

    sf::Image mapImage, trapImage, appleImage, collectedImage, brownBackImage, virtualGuyImage, enemyImage;
    trapImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Traps/Spike Head/Idle.png");
    mapImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Terrain/Terrain (16x16).png");
    appleImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Items/Fruits/Apple.png");
    collectedImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Items/Fruits/Collected.png");
    brownBackImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Background/Brown.png");
    virtualGuyImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Main Characters/Virtual Guy/Run (32x32).png");
    enemyImage.loadFromFile("C:/Users/ilyas/source/repos/Game/Main Characters/Mask Dude/Run (32x32).png");

    mapImage.createMaskFromColor(sf::Color(33, 31, 48));

    sf::Texture map, trap, apple, collected, brown;
    map.loadFromImage(mapImage);
    trap.loadFromImage(trapImage);
    apple.loadFromImage(appleImage);
    collected.loadFromImage(collectedImage);
    brown.loadFromImage(brownBackImage);

    sf::Sprite spriteMap, spriteTrap, spriteApple, spriteCollected;
    spriteTrap.setTexture(trap);
    spriteApple.setTexture(apple);
    spriteCollected.setTexture(collected);

    /*Tile trap("Traps/Spike Head/Idle.png"), map("Terrain/Terrain(16x16).png"), apple("Items/Fruits/Apple.png");
    Tile death("Main Characters/Disappearing(96x96).png"), collected("Items/Fruits/Collected.png");*/

    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::Text text("", font, 30);
    text.setStyle(sf::Text::Bold);

    Player p(virtualGuyImage, "Virtual Guy", 250, 250, 32, 32);
    Enemy enemy1(enemyImage, "Mask Dude", 400, 250, 32, 32);
    
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 500;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        p.update(time);
        enemy1.update(time);
        window.setView(view);
        window.clear(sf::Color(185, 110, 84));

        for (int i = 0; i < HEIGHT_MAP; i++)
        {
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ' || TileMap[i][j] == 'a' || TileMap[i][j] == 't' || TileMap[i][j] == 'c')
                {
                    spriteMap.setTexture(brown);
                    spriteMap.setTextureRect(sf::IntRect(0, 0, 32, 32));
                    spriteMap.setScale(1.f, 1.f);
                    spriteMap.setPosition(j * 32, i * 32);
                    window.draw(spriteMap);

                    if (TileMap[i][j] == 'a')
                    {
                        spriteApple.setTextureRect(sf::IntRect(0, 0, 32, 32));
                        spriteApple.setPosition(j * 32, i * 32);
                        currentFrameApple += 0.0029 * time;
                        if (currentFrameApple > 17) currentFrameApple -= 16;
                        spriteApple.setTextureRect(sf::IntRect(32 * int(currentFrameApple), 0, -32, 32));
                        window.draw(spriteApple);
                    }
                    else if (TileMap[i][j] == 't')
                    {
                        spriteTrap.setTextureRect(sf::IntRect(0, 0, 52, 52));
                        spriteTrap.setPosition(j * 32, i * 32);
                        spriteTrap.setOrigin(10.f, 25.f);
                        spriteTrap.setScale(0.85f, 0.85f);
                        window.draw(spriteTrap);
                    }
                    else if (TileMap[i][j] == 'c')
                    {
                        spriteMap.setTexture(collected);
                        spriteMap.setTextureRect(sf::IntRect(0, 0, 32, 32));
                        spriteMap.setPosition(j * 32, i * 32);
                        currentFrameCollected += 0.01 * time;
                        spriteMap.setTextureRect(sf::IntRect(32 * int(currentFrameCollected), 0, -32, 32));
                        window.draw(spriteMap);
                        if (int(currentFrameCollected) == 6) { TileMap[i][j] = ' '; currentFrameCollected = 0; }
                    }
                }
                if (TileMap[i][j] == '0' || TileMap[i][j] == '1' || TileMap[i][j] == 's')
                {
                    spriteMap.setTexture(map);
                    TileMap[i][j] == '0' ? spriteMap.setTextureRect(sf::IntRect(96, 0, 47, 47)) : TileMap[i][j] == '1' ? spriteMap.setTextureRect(sf::IntRect(320, 64, 32, 32)) : spriteMap.setTextureRect(sf::IntRect(47, 128, 32, 32));
                    TileMap[i][j] == '0' ? spriteMap.setScale(0.68f, 0.68f) : spriteMap.setScale(1.f, 1.f);
                    spriteMap.setPosition(j * 32, i * 32);
                    window.draw(spriteMap);
                }
            }
        }

        std::ostringstream playerHealth;
        playerHealth << p.health;
        text.setString("Health:" + playerHealth.str());
        text.setPosition(view.getCenter().x - 100, view.getCenter().y + 200);

        window.draw(enemy1.sprite);
        window.draw(p.sprite);
        window.draw(text);
        window.display();
    }

    return 0;
}
