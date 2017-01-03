#include <gb/gb.h>
#include <stdio.h>
#include "alpha.c"
#include "blank_screen_hello.c"
#include "hello_title.c"

void init();
void checkInput();

void main() {
    init();
    waitpad(J_START);
    HIDE_BKG;
    printf("Press A or B:\n\n\n");
    
    while(1) {
        checkInput();
        // Game code here
    }
}

void init() {
    DISPLAY_ON;			        // Turn on the display
    set_bkg_data(0, 47, alpha); // Load 47 tiles into background memory
    // Use the 'title_screen' array to write background tiles starting at 0,6 (tile positions)
    // and write for 10 tiles in width and 2 tiles in height
    // Use the 'blank_screen' array to write background tiles starting at 0,0 (tile positions)
    //  and for 20 tiles in width and 18 tiles in height
    set_bkg_tiles(0,0,20,18,blank_screen);
    set_bkg_tiles(0,6,10,2,title_screen);
    SHOW_BKG;

    NR52_REG = 0x8F;	        // Turn on the sound
    NR51_REG = 0x11;	        // Enable the sound channels
    NR50_REG = 0x77;	        // Increase the volume to its max
}

void checkInput() {
    if (joypad() & J_B) {
        // The B button was pressed!
        printf("Hello Yunus!\n");
        waitpadup();
    } else if (joypad() & J_A) {
        printf("Hello Rayne!\n");
        waitpadup();
    }
}
