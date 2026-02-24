#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_display.h>
#include <bn_random.h>
#include <bn_vector.h>
#include <bn_log.h>

#include "bn_sprite_items_dot.h"

// Set max/min x position to be the edges of the display
static constexpr int HALF_SCREEN_WIDTH = bn::display::width() / 2;
static constexpr int HALF_SCREEN_HEIGHT = bn::display::height() / 2;
static constexpr bn::fixed MIN_X = -HALF_SCREEN_WIDTH;
static constexpr bn::fixed MAX_X = HALF_SCREEN_WIDTH;
static constexpr bn:: fixed MIN_Y = -HALF_SCREEN_HEIGHT;
static constexpr bn::fixed MAX_Y = HALF_SCREEN_HEIGHT;
bn::random random;


// Starting speed of a bouncer
//static constexpr bn::fixed BASE_SPEED = random.get_int(-3, 3);

// Maximum number of bouncers on screen at once
static constexpr int MAX_BOUNCERS = 20;

class Bouncer {
public:
    bn::sprite_ptr sprite = bn::sprite_items::dot.create_sprite();
    bn::fixed x_speed;
    bn::fixed y_speed; 

    void update() {
        
            
            bn::fixed y = sprite.y();

            bn::fixed x = sprite.x();

            // Update x position by adding speed
            x += x_speed;
            y += y_speed;

            // If we've gone off the screen on the right (for x)
            if(x > MAX_X) {
                // Snap back to screen and reverse direction
                x = MAX_X;
                x_speed *=-1;
            }
            // If we've gone off the screen on the left (for x)
            if(x < MIN_X) {
                // Snap back to screen and reverse direction
                x = MIN_X;
                x_speed *= -1;
            }
            // if we've gone off the screen on the bottom (for y)
            if(y> MAX_Y) {
                // Snap back to screen and reverse direction
                y = MAX_Y;
                y_speed *= -1;
            }
            // if we've gone off the screen on the top (for y)
            if(y < MIN_Y) {
                // Snap back to screen and reverse direction
                y = MIN_Y;
                y_speed *= -1;
            }

            sprite.set_x(x);
            sprite.set_y(y);
        
    }

};


            
bn::fixed average_x(bn::vector<Bouncer, MAX_BOUNCERS>& bouncers) {
    bn::fixed x_sum = 0;
    for(Bouncer bouncer : bouncers) {
        x_sum += bouncer.sprite.x();
    }
    if(bouncers.size() > 0) {
        x_sum /= bouncers.size();
    }
    return x_sum;

    
};

void add_bouncer(bn::vector<Bouncer, MAX_BOUNCERS>& bouncers) {
    // Only add if we're below the maximum
    if(bouncers.size() < bouncers.max_size()) {
        bouncers.push_back(Bouncer());

        // Set the new bouncer's speed to a random value between -3 and 3
        bouncers.back().x_speed = random.get_int(-3, 3);
        bouncers.back().y_speed = random.get_int(-3, 3);
        //prevent the speed from being 0
        if(bouncers.back().x_speed ==0) {
            bouncers.back().x_speed = 1;
            
        }
        if(bouncers.back().y_speed ==0) {
            bouncers.back().y_speed = 1;
            
        }
        
        
    }
};

int main() {
    bn::core::init();
    bn::backdrop::set_color(bn::color(5, 0, 31));

    bn::vector<Bouncer, MAX_BOUNCERS> bouncers = {};

    // Sprites and x speeds of bouncers
    // Items with the same index correspond to each other
    //bn::vector<bn::sprite_ptr, MAX_BOUNCERS> sprites = {};
    //bn::vector<bn::fixed, MAX_BOUNCERS> x_speeds = {};

    

    

    while(true) {
        // if A is pressed add a new bouncer
        if(bn::keypad::a_pressed()) {
            add_bouncer(bouncers);

        }

        // if B is pressed print the average to the console
        if(bn::keypad::b_pressed()) {
            
            // Add all x positions together
           

           BN_LOG("Average x: ", average_x(bouncers));
        }

        // for each bouncer
        for (Bouncer& bouncer : bouncers) {
            // Update the bouncer's position
            bouncer.update();
        }
        

        bn::core::update();
    }
}