#include <gb/gb.h>
#include <stdio.h>
#include "alpha.c"
#include "sprites.c"
#include "blank_screen.c"
#include "title.c"
#include "floor.c"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void init_title();
void init_game();
void check_input();
void apply_physics();
void draw_sprites();
void draw_background();
void update_switches();

// Player X,Y position
UINT8 player_pos[2];
UINT8 player_x_relative;
// Player X,Y velocity
// NOTE: negative Y values move upwards
INT8 player_velocity[2];
// Player X,Y acceleration
// NOTE: negative Y values accelerate upwards
INT8 player_accel[2];
// Jumps remaining
INT8 jumps_left;
// Downward acceleration due to gravity
INT8 gravity;
// Downward velocity cannot exceed this value
INT8 terminal_downward_velocity;
// Position of the floor
UINT8 floor_pos;
UINT8 base_floor_pos;
UINT8 scroll_speed;

void main() {
    init_title();
    init_game();
    
    while(1) {
        check_input();
        apply_physics();
        draw_sprites();
        draw_background();
        update_switches();
        wait_vbl_done();
    }
}

void init_title() {
    DISPLAY_ON;			        // Turn on the display
    printf(" \n\n\n\n       RUNNER\n\n\n\n\n    PRESS  START\n\n\n\n   (C) 2017 YNS88");
    //set_bkg_data(0, 47, alpha); // Load 47 tiles into background memory
    // Use the 'title_screen' array to write background tiles starting at 0,6 (tile positions)
    // and write for 10 tiles in width and 2 tiles in height
    // Use the 'blank_screen' array to write background tiles starting at 0,0 (tile positions)
    //  and for 20 tiles in width and 18 tiles in height
    //set_bkg_tiles(0,0,20,18,blank_screen);
    //set_bkg_tiles(0,6,10,2,title_screen);
    //SHOW_BKG;

    // Load the the 'sprites' tiles into sprite memory
    set_sprite_data(0, 1, sprites);

    NR52_REG = 0x8F;	        // Turn on the sound
    NR51_REG = 0x11;	        // Enable the sound channels
    NR50_REG = 0x77;	        // Increase the volume to its max

    waitpad(J_START);
}

void init_game() {
    set_bkg_tiles(0,0,20,18,blank_screen);
    SHOW_BKG;
    // Set the first movable sprite (0) to be the first tile in the sprite memory (0)
    set_sprite_tile(0,0);
    // Initial world state
    gravity = 2;
    terminal_downward_velocity = 10;
    base_floor_pos = 104;
    floor_pos = base_floor_pos;
    scroll_speed = 2;
    // Initial player state
    player_pos[0] = 32;
    player_pos[1] = floor_pos;
    player_velocity[0] = 0;
    player_velocity[1] = 0;
    player_accel[0] = 0;
    player_accel[1] = 0;
    jumps_left = 1;
}

void update_switches() {
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;
}

void check_input() {
    if (joypad() & J_START) {
        // TODO pause
    }
    if (jumps_left > 0) {
        if (joypad() & J_B) {
            player_accel[1] = -8;
            jumps_left--;
        }
    }

    // update player's position relative to the map
    player_x_relative += scroll_speed;
    if ((player_x_relative >= 80) && (player_x_relative <= 96)) {
        floor_pos = 255;
    } else {
        floor_pos = base_floor_pos;
    }

    // update velocity
    player_velocity[0] += player_accel[0];
    player_velocity[1] = MIN(terminal_downward_velocity, player_velocity[1] + player_accel[1]);
    // drag from gravity
    if (player_pos[1] < floor_pos) {
        player_velocity[1] += gravity;
    }

    // update position
    player_pos[0] = player_pos[0] + player_velocity[0];
    player_pos[1] = MIN(floor_pos, player_pos[1] + player_velocity[1]);

    // decay jumping acceleration
    player_accel[1] = MIN(0, player_accel[1] + 2);

    // recover jumps when on the ground
    if (player_pos[1] == floor_pos) {
        if (jumps_left == 0){
            jumps_left = 1;
        }
        if (player_velocity[1] > 0) {
            player_velocity[1] = 0;
        }
    }

    // Move the player sprite
    move_sprite(0, player_pos[0], player_pos[1]);
}

void apply_physics() {
    
}

void draw_sprites() {
    
}

void draw_background() {
    set_bkg_tiles(0,12,32,6,floor_tiles);
    scroll_bkg(scroll_speed, 0);
}
