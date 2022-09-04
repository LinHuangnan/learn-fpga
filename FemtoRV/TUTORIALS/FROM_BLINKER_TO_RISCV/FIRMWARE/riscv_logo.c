/* 
 * FEMTORV32 - FEMTOSOC demo program:
 * Displaying a rotating RISCV logo
 */ 

#include <stdio.h>

#ifdef __linux__
#include <unistd.h>
#endif


/* The RISCV logo, with a tiny resolution
 * (remember, I only got 4Kb of RAM
 * on the IceStick !) 
 */
unsigned char logo[16][16] = {
   {7,7,7,7,7,7,5,3,3,3,3,3,3,3,3,7},
   {7,7,7,7,7,7,7,5,3,3,3,3,3,3,3,7},
   {1,1,1,1,2,7,7,7,3,3,3,3,3,3,3,7},
   {0,0,0,0,0,1,7,7,5,3,3,3,3,3,3,7},
   {0,0,0,0,0,0,7,7,6,3,3,3,3,3,6,7},
   {0,0,0,0,0,0,7,7,5,3,3,3,3,4,7,7},
   {0,0,0,0,0,2,7,7,4,3,3,3,3,7,7,7},
   {0,2,2,2,7,7,7,6,3,3,3,3,6,7,7,7},
   {0,7,7,7,7,7,6,3,3,3,3,5,7,7,2,7},
   {0,1,7,7,7,4,3,3,3,3,3,7,7,7,0,7},
   {0,0,2,7,7,6,3,3,3,3,6,7,7,1,0,7},
   {0,0,0,2,7,7,5,3,3,5,7,7,2,0,0,7},
   {0,0,0,0,7,7,7,5,4,7,7,2,0,0,0,7},
   {0,0,0,0,0,7,7,7,7,7,7,0,0,0,0,7},
   {0,0,0,0,0,1,7,7,7,7,1,0,0,0,0,7},
   {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7}
};


/* 
 * ANSI color codes:
 * https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
 */


#define ANSIRGB(R,G,B) "\033[48;2;" #R ";"  #G ";" #B "m  "

#define ANSICOL(C) "\033[" #C "m  "


/* 
 * The colormap.
 */

/*
 * This ones corresponds to the official RISC-V logo,
 * but uses more bandwidth (full RGB ANSI codes)
 */
/*
const char* cmap[8] = {
   ANSIRGB(040,051,116),
   ANSIRGB(123,128,155),
   ANSIRGB(170,172,188),
   ANSIRGB(249,177,021),
   ANSIRGB(249,190,101),
   ANSIRGB(249,199,130),
   ANSIRGB(252,216,176),
   ANSIRGB(250,251,248)
};
*/

/* more compact colormap */
const char* cmap[8] = {
   ANSICOL(44),
   ANSICOL(104),
   ANSICOL(47),
   ANSICOL(102),
   ANSICOL(103),
   ANSICOL(103),
   ANSICOL(103),
   ANSICOL(107)
};

/*
 * Generated by TOOLS/make_sintab.c
 */

int sintab[64] = {
   0,25,49,74,97,120,142,162,181,197,212,225,236,244,251,254,
   256,254,251,244,236,225,212,197,181,162,142,120,97,74,49,25,
   0,-25,-49,-74,-97,-120,-142,-162,-181,-197,-212,-225,-236,-244,
   -251,-254,-256,-254,-251,-244,-236,-225,-212,-197,-181,-162,
   -142,-120,-97,-74,-49,-25
};


#define GL_width  40
#define GL_height 40


void main() {

    int frame = 0;
    int last_col = -1;
    for(;;) {
        printf("\033[H"); // reset cursor position

        int scaling = (sintab[frame&63]+400) << 1;
        int Ux = scaling*sintab[frame & 63];         
        int Uy = scaling*sintab[(frame + 16) & 63];  
        int Vx = -Uy;                                
        int Vy =  Ux;                                

        int X0 = -(GL_width/2)*(Ux+Vx);
        int Y0 = -(GL_height/2)*(Uy+Vy);

	for(int y=0; y<GL_height; ++y) {
	    int X = X0;
	    int Y = Y0;
	    for(int x=0; x<GL_width; ++x) {
	        unsigned char col = logo[(Y >> 18)&15][(X >> 18)&15];
	        printf(col == last_col ? "  " : cmap[col]);
	        last_col = col;
	        X += Ux;
	        Y += Uy;
	    }
	    printf("\033[49m\n"); // reset color to black and newline
	    last_col = -1;
	    X0 += Vx;
	    Y0 += Vy;
	}
        ++frame;
#ifdef __linux__       
        usleep(20000);
#endif
//       if(frame > 20) break;
    }
}
