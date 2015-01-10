/*                             c net.c
                            desknet-0.1
                   Copyright (C) 2014 by John Slayton

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

Author's address as of 11.3.14 is 260 Sundance Ct, Azusa, CA 91792. */

#include <errno.h>
#include <fcntl.h>
//#include <linux/input.h>
//#include <linux/ioctl.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
//#include <sys/io.h>
//#include <sys/ioctl.h>
//#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>
#include <shlobj.h>

 /*                      INTEGER VARIABLES                            */

 /* global integers used for index counting.  */
int a = 0;
int b = 0;
int c = 0;

int boardnumber = 0;

 /* This flag triggers load a different class when set to 1.  */
int changeclass = 0;

 /* This is the number of a class whose records will be downloaded. */
int class = 1;

 /* This is also the number of a class to be loaded. */
int classnumber = 0;

 /* class_size holds the current class size in binary.  */
int class_size = 0;

int digit = 0;
 
 /* e is used for indexing each of 40 students and associated data */
int e = 0;

int goaround = 1;

 /* used to convert binary to bcd */
int hundredsregister = 0;    

 /* k is used to index strings returning from fetch_letter */
int k = 0;

 /* linelength stores number of characters in a printed line (hard copy). */
int lengthflag = 0;

int letter = 0;
int letra[41];

 /* ltr is an ASCII character returned from get_event() */
int ltr = 0;

int m = 0;

int messagenumber = 0;

 /* net_size holds the number of keyboards in the hardware system. */
int net_size = 40;

 /* used to convert binary to bcd */
int onesregister = 0;

 /* Used to select answer file */
int option = 0;

 /* 1 means go back to menu through any intervening routines. */
int panic = 0;

 /* pntr is an index showing offset from beginning of student text. */
int pntr = 0;

 /* repeat is a flag that shows whether menu item M is being repeatedly 
    invoked with no other items between invocations.  The first 
    invocation  of M sets this to 1; all other menu items reset it to 0.
    Only the first invocation of M requires a password */ 
int repeat = 0;

 /* used to convert binary to bcd */
int tensregister = 0;

 /* win_score contains the score needed to win math competitions.  It 
    can  be changed by the teacher when the program is running. */
int win_score = 10;

                            /* INTEGER ARRAYS */

 /* These are used to calculate math addition problems. */
int addend1[41];
int addend2[41];

 /* This array records how many points a student receives for a right 
    answer in an addition contest. */
int addpoints[41]; 

 /* advance is a flag that shows whether a new problem is to be 
    generated.  1 means generate it. */
int advance[41];

 /* This array contains answers for each of the current student 
    problems in math competitions. */
int answer[41];

 /* This flag keeps a keyboard from being read while a message is displayed. */
int blockinput[41];

 /* These are used to calculate math division problems. */
int dividend[41];
int divisor[41];

 /* These are used to calculate math multiplication problems. */
int factor1[41];
int factor2[41];

 /* This is used to calculate math subtraction problems. */
int minuend[41];

 /* This array holds offsets for each student from the beginning of the 
    current Wscreen line to the current cursor position. */
signed int cursor[41];

 /* This holds the current position (numbered left to right) of the 
    current digit being entered by a student as an answer to a math 
    contest problem. */
int digit_nmbr[41];

 /* This array records how many points a student receives for a right 
    answer in a division contest. */
int divpoints[41]; 

 /* These are file descriptors. */
int fz[41];

 /* 1 means student has entered a wrong answer in math competition. */
int goofed[41];

 /* Here's a hash table to decode lower case keypresses. */
int lc[255];

 /* These flag last keystroke or each keyboard as make (1) or break (0). */
int make_or_break[41];

 /* This array records how many points a student receives for a right 
    answer in a multiplication contest. */
int mulpoints[41]; 

 /* Here are the pages each student is working on in Check Your Math. */
int page[41];

 /* This is used to input multi-digit numbers from student keyboards. */
int placeflag[41];

 /* This is an offset to print chars in Check Your Math. */
int place[41];

int problem[41];

 /* These flags control what is happening on each student's line in 
    typing tutorial routines.  A 1 invokes print_line, which prints the 
    next line of practice characters.  A 2 indicates that a message has 
    just been printed, which will remain until the student pushes 
    another key.  When another key is in fact pushed, this flag becomes
    zero, enabling further student input. */
int renew[41];

 /* This holds accumulated points in math competitions.  score also 
    holds the number of student errors in the typing tutorial. */
int score[41];

int step[41];

int stop[41];

 /* This array holds student answers for math competitions. */
int stuans[41];

 /* This array records how many points a student receives for a right 
    answer in a subtraction contest. */
int subpoints[41]; 

int subtrahend[41];

 /* These arrays each hold a table of digits from which the math 
    competition routines draw randomly to make the problems.  t1 is
    the first term of a multiplication problem.  t2 is the second.
    Other types of problems use these tables similarly. */
int t1[10];
int t2[10];

 /* These are offsets from the beginning of each student's 
    composition to the beginning of his currently displayed line. */
int text_pos[41];

 /* This array holds the number of the current typing practice line for 
    each student. */
int tline[41]; 

 /* This array holds the message number for each line, used in checkwork */
int tt[41];

 /* Here's a hash table to decode upper case keypresses. */
int uc[255];

 /* These flags show whether last keypress was make-shift.  A 1 means 
     interpret next make as upper case. */
int upper_case[41];

 /* This flag shows that program is waiting for student to enter an answer
    to a math problem. */
int waiting[41];

                           /* CHARACTER ARRAYS */
char ttf_path[16] = "C:\\Windows\\Fonts";
						   
char answ[41][39];

char blankline[41] = "                     ";

 /* These variables hold the event number for each keyboard. */
char BUF[41][21] = {"/dev/input/event0","/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0", "/dev/input/event0" };

 /* Here's a buffer that holds file names for i/o operations. */
char Buf[50];

 /* These are the buffers for student compositions, limited for now to 
     5000 characters. */
char comp[41][5000];

 /* Here's an empty buffer used to clear other buffers by strcpy. */
char dum[41] = "";
char dumm[10] = "";

 /* This is a buffer used to assemble strings as typed in on teacher's 
    keyboard. */
char dummy[41] = "";

char factor[12];

 /* Here's a buffer containing ASCII representation of points per right 
    answer given in math contests.  It's used for i/o operations. */ 
char hndicap[41];

 /* Answers to text exercises */
char key[40000];

 /* A register used store the last line sent to screen, then erase it
    by reprinting it in background color. */
char last_line[41][21];

char displayline[41][21];

 /* Contains students' proposed answers for Check Your Math. */
char guess[41][21];

 /*  Used for i/o operations.  Holds # of keyboards in system. */
char net_buffer[2];

 /* This 4 digit password for the teacher keeps kids from messing 
    with important files.  It may be changed by hand using an editor 
    with the 'pass' file, or within the program using the hidden
    routine X. */
char password[5] = "1040";

char pass[5];
  /* This is a buffer containing a student composition being formatted 
     for hard copy printing. */
char proof[5000] = "";

 /* This is a long term array containing names of all current students.
    (10 character limit) */
char roster[41][10];

 /* This buffer is used to assemble strings being typed in on teacher's 
    keyboard. */
char shelf[41]="";

 /* This is a buffer used to hold the contents of shelf, less the 
    last character.  It is used to backspace input from teacher's 
    keyboard. */
char shortshelf[41];

 /* This is a temporary buffer holding one student's name.  (10 
    character limit) */
char student_name[10];

 /* This holds name of class or teacher displayed on main menu. */
char teacher[41];

char studint[15];

 /*  This array holds a teacher-created hash table used to create 
     problems for math competitions.  These are the second terms of 
     multiplication problems.  */
char terma[10] = "";
char terma_buffer[10] = "";

 /*  This array holds a teacher-created hash table used to create 
     problems for math competitions.  These are the second terms of 
     multiplication problems.  */
char termb[10] = "";
char termb_buffer[10] = "";

 /* These typing practice lines may be printed by print_line() or message(); */
char ttext[201][21]=
   {  
     "                    ", // 0
     "ff jj ff jj fj fj fj", // 1
     "jjf jjj ffj fff fjfj", // 2
     "jfj ffj fjf jjf jffj", // 3
     "dd kk dd kk dk dk kd", // 4
     "kdk ddk kdd dkk dkdk", // 5
     "jk fd jk fd kj df kj", // 6
     "dj jd dj dj kf df jf", // 7
     "jdd kkf dkd ffd jfkd", // 8
     "aa dd aa kk dada aka", // 9
     "dda jad kaj faj kdfa", // 10
     "sa sa sd sd js sj ks", // 11
     "das kas sad kad adss", // 12
     "ll aa kk ss jj dd ff", // 13
     "al sl dl fl la ls ld", // 14
     "lf lj lk all lad ask", // 15
     "fad lass fall salads", // 16
     "gg hh gg hh gg hh gh", // 17
     "gk gl ha hs dg dh hf", // 18
     "hj hk hd hs hf ha hl", // 19
     "hg gf gd gs ga kg lg", // 20
     "gas hag gal sag lags", // 21
     "jag ash dash lash gh", // 22
     "sash shag shall hall", // 23
     "gash hash flask flag", // 24
     "ed de ed de de ek ke", // 25
     "fed led head less he", // 26
     "ki ik ki ki ji ij ij", // 27
     "if is as hi he seeds", // 28 
     "did hid lid led skid", // 29
     "kiss hiss dish fades", // 30
     "less shell shed sled", // 31
     "dill dell sell fills", // 32 
     "sill jell kill skill", // 33
     "ill gill slag slides", // 34
     "hide lied like glide", // 35
     "ail sails fail jails", // 36
     "sale dale gale hails", // 37
     "dial file lead feeds", // 38
     "life fife leak seeks", // 39
     "rf rf ju ju frr juuj", // 40
     "frfe juji rffd uj jk", // 41 
     "free jug dusk disk a", // 42
     "dress jig dig rug jr", // 43
     "fur far fire sire sr", // 44
     "rid red risk dusk he", // 45
     "her she husk shirk a", // 46
     "fluff huff risk slur", // 47
     "guff gruff use fuses", // 48
     "sue lush rush rulers", // 49
     "ol lo lo oll lol klo", // 50
     "loose goose  rooster", // 51
     "so do go lo ho of or", // 52
     "hog log jog dog frog", // 53 
     "for ajar hod door jo", // 54
     "moor lore floor roar", // 55
     "hood hoof goods food", // 56
     "oil soil foil road i", // 57
     "old fold hold rolled", // 58
     "gold sold jokes soak", // 59
     "sole foal goal holes", // 60
     "horses foe goes does", // 61
     "tt rr ff gg tr rt fr", // 62
     "tf fg gr rt rg rf tg", // 63
     "it at to tut tot ate", // 64
     "tar tag fat rat toes", // 65
     "jot dot got hot lots", // 66
     "just dust gusto this", // 67
     "that there their the", // 68
     "eats seat feet great", // 69
     "heat greet feat hoot", // 70
     "tear star stall tall", // 71
     "tell till toll troll", // 72
     "toad roads load bold", // 73
     "jails pails railroad", // 74
     "trials trails tigers", // 75
     "litter latter ladder", // 76
     "loiter toil suit tad", // 77
     "yt fj ty jf jf yt fy", // 78
     "tg hy hu uy yu hu ty", // 79
     "yo you hurry hey you", // 80
     "soy joy key say days", // 81
     "lay hay ray ye delay", // 82
     "relay shay clay flay", // 83
     "shaggy froggy  risky", // 84
     "foggy gray frail sky", // 85
     "fry  dry sly lye try", // 86
     "ws sw ow wo ed ws ew", // 87
     "we ow wow wed dew aw", // 88
     "few hew was saw wits", // 89
     "jaw law yaw raw ways", // 90
     "wag wad walks washes", // 91
     "water week weak weed", // 92
     "wishes woke whiskers", // 93
     "away awoke with waft", // 94
     "pq qp p; ;p aq qa qp", // 95
     "up ape pie put quips", // 96
     "pot pet pat pit date", // 97
     "peak speak freak pry", // 98
     "quiet hire shire spy", // 99
     "spry sideswipes swig", // 100
     "quest post lost wars", // 101
     "nj jhh hn nh jhn nhj", // 102
     "no an in on tone fan", // 103
     "sing rings ding thin", // 104
     "hen then fingers not", // 105
     "notes  nothing funny", // 106
     "vff fv gv vg rv vrtf", // 107
     "dvd videos vanish va", // 108
     "dove stoves love eve", // 109
     "vine every never van", // 110
     "mj jm mn nm mh hm jn", // 111
     "km mh nm jm mn gm mg", // 112
     "kvf fnm mvn jmh jnhg", // 113
     "man name mane him he", // 114
     "map swap mad may ram", // 115
     "mashes smash stashes", // 116
     "bv vb fb bf gb bg gv", // 117
     "vm nb bn mv jb fb gb", // 118
     "hb bj java jabs bees", // 119
     "bad bid bud bed both", // 120
     "better worse best ab", // 121
     "bear web bright brag", // 122
     "cd dc sc cs cf cg gc", // 123
     "nc cn cb bc hc ch cj", // 124
     "can con cent sack ca", // 125
     "car cart carton core", // 126
     "scram brick sick sic", // 127
     "za az sz zs lz ze za", // 128
     "zq wz az dz zo zp cz", // 129
     "miz zip zap doze zoo", // 130
     "dozen zipper  zinger", // 131
     "xs zx xz dx xd mx xn", // 132
     "ex axe ox exude flex", // 133
     "fox box pox lox flax", // 134
     "Mm mM nM Mn Mr Mrs M", // 135
     "Hh Ii Jj Kk Ll Mm Nn", // 136
     "Oo Pp Uu Yy jJj MmMn", // 137
     "Aa Bb Cc Dd Ee Ff Gg", // 138
     "Qq Ww Rr Tt Zz Xx Cc", // 139
     "Vv Bb Dr Ca NY PhD G", // 140
     ".l. x. .x z. .z j. .", // 141
     "Mr. Mrs. Dr. Go now.", // 142
     "Sr. Srta. Go slowly.", // 143
     "My dog has no fleas.", // 144
     "abcdefghijklmnopqrst", // 145
     "uvwxyz Play all day.", // 146
     "He sings for dinner.", // 147
     "The dog likes bones.", // 148
     "Her car is very old.", // 149
     "I travel to Ontario.", // 150
     ",k k, ,m m, ,. ., l,", // 151
     "alpha,a beta, gamma,", // 152
     "dog, cat, rat, duck.", // 153
     "Sadly, he will lose.", // 154
     "// ;; // ;; /; ;/ ./", // 155
     "?/ /? .? ,.? ?., ?/?", // 156
     "Am I having fun yet?", // 157
     "How does a bird fly?", // 158
     "When will this end??", // 159
     "We are now finished.", // 160
     "                    ", // 161
     "                    ", // 162
     "                    ", // 163
     "                    ", // 164
     "                    ", // 165
     "                    ", // 166
     "                    ", // 167
     "                    ", // 168
     "                    ", // 169
     "                    ", // 170
     "                    ", // 171
     "                    ", // 172
     "                    ", // 173
     "                    ", // 174
     "                    ", // 175
     "                    ", // 176
     "                    ", // 177
     "                    ", // 178
     "                    ", // 179
     "    /   =           ", // 180
     "    X   =           ", // 181
     "    -   =           ", // 182
     "    +   =           ", // 183
     "                    ", // 184
     "                    ", // 185
     "                    ", // 186
     " What's the answer? ", // 187
     " BUMMER...          ", // 188
     "                    ", // 189
     "TRY IT AGAIN!       ", // 190
     "KEEP GOING!         ", // 191
     "SPEED IT UP...      ", // 192
     " LOOKS GOOD ...     ", // 193
     " What's the page?   ", // 194
     " What's the problem?"  // 195
      };

 /* This array holds ASCII representation of the number of each 
    student's practice typing line.  Used in i/o operations.
    (Corresponding binary representations are held in tline[]. */
char type_line[41] = "11111111111111111111111111111111111111111";

int winscore[3] = {0,0,0};

 /* This holds the value of win_score in ASCII form.  It is used in 
     i/o routines. */
char ww[2]; 

char z[0];
char z1[0];
char z2[0];
char z3[0];

                       /* ODDBALL VARIABLES */

 /* A file descripter used in i/o operations. */
FILE * (FP);
 
 /* Used in keyboarding to measure typing speed, and in math facts 
    competitions to clock timeout when wrong answer is entered. */
time_t *t= NULL;

 /* Used to time keyboarding and Check Your Math. */
unsigned long clk[41];

                          /* FUNCTIONS */

 /* Get keypress value */
int alpha();

 /* This saves current practice lines to the hard drive when returning 
    from typing tutorial routine. */
int bail();

 /* This is the main routine for typing tutorial.  Polls student 
    keyboards. */
int board();

 /* Add, delete students from class. */

 /* Erase all previously displayed lines. */
int clearall();

 /* Erase all student compositions. */
int clear_pro();

 /* Present "Push Enter to Continue" page. */
int detente();

 /* Verify desire to exit. */
int double_check();

 /* Empty event que. */
int dump();

 /* This routine creates a class identifier at top of main menu.  It may 
    be the teacher's name.  The routine also allows the teacher to 
    change the password.  It is invoked by hidden menu item X. */
int name_class();

 /* Check student typing for speed and accuracy. */
int eval_type();

int fetch_letter();
int fetch_name();
int fetch_win_score();

 /* Save student compositions to the hard drive. */
int file_save();

 /* Pull relevant answer from text answer file */
int find_answer();

int get_char();

 /* Recover last USB network configuration. */
int get_config();

 /* Input a binary digit into variable nr.  Return ASCII value of nr, 
    or 10. */
int get_digit();

 /* Argument is e (student #).  Returns ltr. */
int get_event(int);

 /* Download student compositions on program boot. */
int get_file();

 /* Download the class roster. */
int get_names();

int get_ten();

 /* Set the number of points a particular student will get for a right 
    answer in one of the math contests. */
int handicap();

int handicapenterpage();
int handicapoppage();

 /* The following routines create and display pseudo-random problems 
    for math facts contests. */
int make_add_prob();
int make_div_prob();
int make_mul_prob();
int make_sub_prob();

int subtract();
int multiply();

 /* This presents the basic menu. */
int menu();

 /* This prints a numbered message to the screen. */
int message(int e, int messagenumber);

 /* This opens a file for each keyboard, assigns control using ioc. */
int opener();

 /* This does keyboard polling for Check Your Math. */
int check();

 /* Print a practice line for the typing tutorial. */
int print_line();

 /* Print (hard copy) current composition for a selected student. */
int print_pro();

 /* Initiate word processor. */
int process();

 /* Return random digit which will be used as a pointer into a hash 
    table used to create problems for math competitions. */
int random_num();

int roster_page();

 /* Input and evaluate a password. */
int security();

int select_board_page();

 /* Pick an answer key. */
int select_book();

int select_line_page();

 /* Set the score that wins math facts contests. */
int set_win_score();

int splash_page();

 /* Create a space at any point in student composition by moving all 
    following text one space to the right.  Do not replace original 
    character at division point; result is to duplicate it one space to 
    the right. */
int wedge(int);

int win_page();

 /* Here are the screens */

SDL_Window* choices = NULL;
SDL_Surface* options = NULL;

SDL_Window* slate = NULL;
SDL_Surface* chalk = NULL;
SDL_Surface* ostraca = NULL;  
SDL_Surface* lin1 = NULL;
SDL_Surface* lettr = NULL;
SDL_Surface* hogwash = NULL;
SDL_Surface* correction = NULL;
SDL_Surface* scratchpad = NULL;
SDL_Surface* sym = NULL;
SDL_Surface* sym1 = NULL;
SDL_Surface* sym2 = NULL;
SDL_Surface* scratch5 = NULL;
SDL_Surface* scratch7 = NULL;

SDL_Surface* changeslate = NULL;
SDL_Surface* classslate = NULL;
SDL_Surface* winslate = NULL;

SDL_Color black = {0,0,0};
SDL_Color green = {7,167,8};
SDL_Color red = {255,0,0};
SDL_Color yellow = {248,250,26};
SDL_Color white = {255,255,255};

SDL_Color hue[41];

struct SDL_Rect tile[41][50];
struct SDL_Rect tilem[41][50];
struct SDL_Rect line[41];

struct SDL_Rect slots[41][50];

int
add()
{
  char a;
  int b = 0;
  TTF_Font  *fntVeraMono33 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 33 );
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 40);

                    /* OPEN EMPTY WHITE WINDOW */

  slate = SDL_CreateWindow("add",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

                 /* PUT net_size PROBLEMS ON THE SCREEN */

  for (e = 0; e < net_size; e++)
    {
      stop[e] = 0;
      goofed[e] = 0;
      for (b = 0; b < 20; b++) last_line[e][b] = ' ';
      renew[e] = 1;
      score[e]=0;
      make_add_prob(e);
    
      last_line[e][1] = (char)(addend1[e])+48;
      last_line[e][3] = '+';
      last_line[e][5] = (char)(addend2[e])+48;
      last_line[e][7] = '=';
      for (c = 0; c < 20; c++)
        {
          if(net_size==40)ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], hue[e]);
          else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], hue[e]);
          SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][c]);
        }
      
      placeflag[e] = 1;
    }

  SDL_UpdateWindowSurface(slate);

                         /* START KEYBOARD POLLING */
     
  while (1)
    {
      if (get_char() == 10) /* CHECK TEACHER'S KEYBOARD FOR RETURN TO MENU */
         {
           last_line[e][15] = ' ';
           last_line[e][16] = ' ';
           last_line[e][1] = ' ';
           last_line[e][3] = ' ';
           last_line[e][5] = ' ';
           goaround = 0;
           SDL_DestroyWindow(slate);
           return 0;
         }
      
      for (e = 0; e < net_size; e++)
        { 
          if (goofed[e] == 1)
            {
              if (time(t) - clk[e] > 3) /* END OR CONTINUE TIMEOUT PENALTY. */
                {
                  goofed[e] = 0;
                  if(net_size==40) lin1 = TTF_RenderText_Solid(fntVeraMono33, "X", white);
                  else lin1 = TTF_RenderText_Solid(fntVeraMono40, "X", white);
                  SDL_BlitSurface(lin1, NULL, chalk, &tilem[e][10]);
             
                  SDL_BlitSurface(lin1, NULL, chalk, &tilem[e][11]);
            
                  SDL_UpdateWindowSurface(slate);
                }
            
              else
                {              
                  get_event(e);
                  continue;
                } 
            }
          
                            /* GET STUDENT INPUT */
            
          if (goofed[e] == 0) 
            {   
              a =  get_event(e);        
              if (a == 0) continue;            
              if (a == 10) goto evaluate;
              if (stop[e] == 1) continue;
           
              if (placeflag[e] == 1)
                 {
                   if (((int)a < 48) || ((int)a > 57))  continue;
                   stuans[e] = (int) a - 48 ;
                   z[0] = (int) a;  
                   last_line[e][10] = (int) a;
                   placeflag[e] = 2;
                                
                   if(net_size==40) lettr = TTF_RenderText_Solid(fntVeraMono33, &z[0], hue[e]);
                   else lettr = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
                   SDL_BlitSurface(lettr, NULL, chalk, &tilem[e][10]);
                   SDL_UpdateWindowSurface(slate);
                   
                   continue;
                 }
                         
              if (placeflag[e] == 2)
                 {
                   if (((int)a < 48) || ((int)a > 57)) continue;
                     
                   z[0] = a;  
                   last_line[e][11] = (int) a;
                   stuans[e] = 10 * stuans[e] + (a - 48);
                      
                   if(net_size==40) lettr = TTF_RenderText_Solid(fntVeraMono33, &z[0], hue[e]);
                   else lettr = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
                   SDL_BlitSurface(lettr, NULL, chalk, &tilem[e][11]);
                   SDL_UpdateWindowSurface(slate);

                   stop[e] = 1;                     
                   continue;
                 }
              continue;       
            }       
                     
          continue;            
          evaluate:    
          if (stuans[e] == answer[e])
            { 
               for (c = 0; c < 20; c++)
                  {
                    if(net_size==40) hogwash = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], white);
                    else hogwash = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], white);
                    SDL_BlitSurface(hogwash, NULL, chalk, &tilem[e][c]);
                  }
               SDL_UpdateWindowSurface(slate);

               advance[e] = 1;
               score[e] = score[e] + addpoints[e + 1];
               onesregister = score[e] % 10;
               tensregister = score[e] / 10;
               last_line[e][10] = 32;
               last_line[e][15] = (char) tensregister + 48;
               last_line[e][16] = (char) onesregister + 48;
               if(last_line[e][15] == 48){last_line[e][15] = 32;}

               if(score[e] < win_score)
                 {
                   stop[e] = 0;
                   placeflag[e] = 1;
                   make_add_prob(e);
                 
                   last_line[e][1] = (char)(addend1[e])+48;
                   last_line[e][3] = '+';
                   last_line[e][5] = (char)(addend2[e])+48;
                   last_line[e][7] = '=';
                   last_line[e][10] = ' ';
                   last_line[e][11] = ' ';

                   for (c = 0; c < 20; c++)
                     {
                       if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], hue[e]);
                       else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], hue[e]);
                       SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][c]);
                     }
                   
                   SDL_UpdateWindowSurface(slate);
                 
                 }

                              /* DECLARE A WINNER */

               else
                 {
                   last_line[e][15] = ' ';
                   last_line[e][16] = ' ';
                   SDL_DestroyWindow(slate);
                   win_page();
                   goaround = 1;
                   return 0;
                 } 
               
            } 
              
              else if(stuans[e] != answer[e])
                {
                  stop[e] = 0;
                  placeflag[e] = 1;
                  clk[e] = time(t);
                  goofed[e] = 1;

                  z[0] = last_line[e][10];              
                  if(net_size==40) correction = TTF_RenderText_Solid(fntVeraMono33, &z[0], white);
                  else correction = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
                  SDL_BlitSurface(correction, NULL, chalk, &tilem[e][10]);
                  
                  z[0] = last_line[e][11];              
                  if(net_size==40) correction = TTF_RenderText_Solid(fntVeraMono33, &z[0], white);
                  else correction = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
                  SDL_BlitSurface(correction, NULL, chalk, &tilem[e][11]);
                  SDL_UpdateWindowSurface(slate);
                 
                  last_line[e][10] = 'X';
                  last_line[e][11] = 'X';
                
                  if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, "X" , black);
                  else ostraca = TTF_RenderText_Solid(fntVeraMono40, "X" , black);
                  SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][10]);
                  SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][11]);
                  SDL_UpdateWindowSurface(slate);
                }
        }        
    } 
  SDL_DestroyWindow(slate);
  return 0;          
}

int
alpha()
{
  SDL_Event event;

  while(1)
    {
      SDL_WaitEvent(&event);    
      if (event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
            {
              case SDLK_0:
                letter = 0;  
                break;
                
              case SDLK_1:
                letter = 1;  
                break;
                
              case SDLK_2:
                letter = 2;  
                break;
                 
              case SDLK_3:
                letter = 3;  
                break;
                  
              case SDLK_4:
                letter = 4;  
                break;
                  
              case SDLK_5:
                letter = 5;  
                break;
                  
              case SDLK_6:
                letter = 6;  
                break;
                  
              case SDLK_7:
                letter =7;  
                break;
                   
              case SDLK_8:
                letter =8;  
                break;
                   
              case SDLK_9:
                letter =9;  
                break;
                  
              case SDLK_RETURN:
                return letter;
                break;
            }
        }
    } 
   return letter;
}

int
bail()
{
  for (e = 0; e < 40; e++)
    {
      type_line[e] = (char) (tline[e] + 48);
    }  
	
// TODO: Make this work in Windows.
/*   if (class == 1) FP = fopen("/usr/local/com/class1/settings/type_line", "w");
  if (class == 2) FP = fopen("/usr/local/com/class2/settings/type_line", "w");
  if (class == 3) FP = fopen("/usr/local/com/class3/settings/type_line", "w");
  if (class == 4) FP = fopen("/usr/local/com/class4/settings/type_line", "w");
  if (class == 5) FP = fopen("/usr/local/com/class5/settings/type_line", "w");
  if (class == 6) FP = fopen("/usr/local/com/class6/settings/type_line", "w");
  if (class == 7) FP = fopen("/usr/local/com/class7/settings/type_line", "w");
  if (class == 8) FP = fopen("/usr/local/com/class8/settings/type_line", "w");
  if (class == 9) FP = fopen("/usr/local/com/class9/settings/type_line", "w");

  fputs(type_line, FP);
  fclose(FP); */
  return 0;
}

int
board()
{
  char a;
  TTF_Font  *fntVeraMono30 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 30);
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 40);

  slate = SDL_CreateWindow("keyboard",0,0,1024,768,0);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);

  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

                         /* PRINT net_size PRACTICE LINES */

  for (e = 0; e < net_size; e++)
    {
      tt[e] = (int) tline[e]; 
      renew[e] = 1;
      score[e]=0;
      cursor[e] = 0;
      if(net_size == 40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[tt[e]], hue[e]);
      else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[tt[e]], hue[e]);
      SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);

    }
     
  SDL_UpdateWindowSurface(slate);

  while (1)
    {
      if (get_char() == 10)
        {
          SDL_DestroyWindow(slate);
          return 0;
        }
      for (e = 0; e < net_size; e++)
        {

                        /* PENALTY TIMEOUT */

           if ((blockinput[e] == 1) & (time(t) - clk[e] < 4))
             {
               a = get_event(e);
               continue;
             }
           
                           /* END PENALTY */

           if ((blockinput[e] == 1) & (time(t) - clk[e] > 3))
             {
               blockinput[e] = 0;
               tt[e] = (int) tline[e]; 
               renew[e] = 1;
               score[e]=0;
               cursor[e] = 0;

               SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));
               if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[tt[e]], hue[e]);
               else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[tt[e]], hue[e]);
               SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);

               SDL_UpdateWindowSurface(slate);

               cursor[e] = 0;
               renew[e] = 0;
               score[e] = 0;
             }
          
                            /* CHECK FOR INPUT */

            a =  get_event(e);        
          
           if (a == 0) continue;
           if (cursor[e] == 20)
             {
               if (a == 10)          
                 {
                   eval_type();
                   tline[e] = tt[e];
                   SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));
                   if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[m], hue[e]);
                   else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[m], hue[e]);
                   SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);
              
                   SDL_UpdateWindowSurface(slate);
                 }
               continue;
             }
            
                    /* START CLOCK AT BEGINNING OF LINE */

           if (cursor[e] == 0)
                 {
                   clk[e] = time(t);
                 }
                         
           z[0] = a;
              
           if (a ==  ttext[tt[e]][cursor[e]])
             {
               score[e]++;
               if(net_size==40) scratchpad = TTF_RenderText_Solid(fntVeraMono30, &z[0], yellow);
               else scratchpad = TTF_RenderText_Solid(fntVeraMono40, &z[0], yellow);
               SDL_BlitSurface(scratchpad, NULL, chalk, &tile[e][cursor[e]]);
             } 

           else
             { 
               if(net_size==40) scratchpad = TTF_RenderText_Solid(fntVeraMono30, "X", black);
               else scratchpad = TTF_RenderText_Solid(fntVeraMono40, "X", black);
               SDL_BlitSurface(scratchpad, NULL, chalk, &tile[e][cursor[e]]);
             }

           SDL_UpdateWindowSurface(slate);

           cursor[e]++;
        }
    } 
  SDL_DestroyWindow(slate);
  return 0;          
}

int
changepage()
{
  TTF_Font  *fntVeraMono45 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 45);

  SDL_Window*  changeslate = SDL_CreateWindow("bumper",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* display = SDL_GetWindowSurface(changeslate);
  
  SDL_Renderer *rendc = SDL_CreateRenderer(changeslate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rendc, 255,255,255,255);
  SDL_RenderClear(rendc); 
  SDL_RenderPresent(rendc);

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(rendc, 1024,768);

  struct SDL_Rect slots[5];
    
  slots[0].x = 0;
  slots[0].y = 295;
        
  slots[1].x = 0;
  slots[1].y = 395;
      
  slots[2].x = 35;
  slots[2].y = 395;
      
  slots[3].x = 0;
  slots[3].y = 495;
      
  slots[4].x = 0;
  slots[4].y = 595;
      
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono45, "  NUMBER OF KEYBOARD TO BE ASSIGNED?", black);
  SDL_BlitSurface(line0, NULL, display, &slots[0]);
  SDL_UpdateWindowSurface(changeslate);

  char liner[3] = "  ";
  liner[2] = 0;
     
  int x = get_digit();
  liner[0] = (char) (x + 48);
  boardnumber = x;

  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono45, liner, hue[e]);
  SDL_BlitSurface(line1, NULL, display, &slots[1]);
  SDL_UpdateWindowSurface(changeslate);
 
  int y = get_digit();
  if (y > 9)
    {
       SDL_DestroyWindow(changeslate);
       fetch_name();
       return 0;
    }
  liner[1] = (char)(y + 48);
  boardnumber = 10 * boardnumber + y;

  SDL_Surface  *line2 = TTF_RenderText_Solid(fntVeraMono45, liner, hue[e]);
  SDL_BlitSurface(line2, NULL, display, &slots[1]);
  SDL_UpdateWindowSurface(changeslate);

  alpha();

  SDL_DestroyWindow(changeslate);
  fetch_name(e);

  return boardnumber;        
}

int
check()
{
  int d = 0;
  TTF_Font  *fntVeraMono30 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 30 );
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 40);

  slate = SDL_CreateWindow("check",0,0,1024,768,0);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

                           /* OPEN FOR net_size STUDENTS */

  for (e = 0; e < net_size; e++)
    {
       step[e] = 0;
       if(net_size == 40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[194], hue[e]);
       else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[194], hue[e]);
       SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);
       cursor[e] = 0;
       page[e] = 0;
    }
  
  SDL_UpdateWindowSurface(slate);

                           /* START ASKING QUESTIONS */

   while (1)
     {    
       if (get_char() == 10) 
       {
         SDL_DestroyWindow(slate);
         return 0;
       }
       for (e = 0; e < net_size; e++)
         {   
           a = 0;
           a = (int)get_event(e);
           if (a == 0) continue;
           z[0] = (int) a ;
           
                               /* ASK FOR PAGE NUMBER */
           
           if ((step[e] == 0) && (a > 47) && (a < 58))
             {

                      /* Start over if answer is more than 3 digits long. */

               jumpstart:
               if (cursor[e] > 2)
                 {

                   cursor[e] = 0;
                   page[e] = 0;
                   
                   SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));

                   if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[194], hue[e]);
                   else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[194], hue[e]);
                   SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);

                   SDL_UpdateWindowSurface(slate);
                   continue;
                 }
                               /* RECEIVE PAGE NUMBER */

               page[e] = 10 * page[e] + (int) a - 48;  

               if (cursor[e] == 0)
                 {
                   z1[0] = (int) a;
                   SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));

                   if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, &z[0], hue[e]);
                   else ostraca = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
                   SDL_BlitSurface(ostraca, NULL, chalk, &tile[e][cursor[e]]);
                 }

               else
                {
                  if (cursor[e] == 1)  z2[0] = (int) a;
                  if (cursor[e] == 2)  z3[0] = (int) a;

                  if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, &z[0], hue[e]);
                  else ostraca = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
                  SDL_BlitSurface(ostraca, NULL, chalk, &tile[e][cursor[e]]);
                } 

               SDL_UpdateWindowSurface(slate);
               cursor[e]++;  
             }                   

                               /* ASK FOR PROBLEM NUMBER */
             
           if ((step[e] == 0) && (a == 10))
             {
               step[e]++;
               cursor[e] = 0;
               SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));

               if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[195], hue[e]);
               else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[195], hue[e]);
               SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);

               SDL_UpdateWindowSurface(slate);

               continue; 
             }
           
                       /* RECEIVE PROBLEM NUMBER */		

           if ((step[e] == 1) && (a < 58) && (a > 47))
             {
               z[0] = (int) a;
               if (cursor[e] == 1) z2[0] = (int) a;
               if (cursor[e] == 2) z3[0] = (int) a;

               if (cursor[e] == 0)
                 {
                   z1[0] = (int) a;
                   SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));
                 }

               if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, &z[0], hue[e]);
               else ostraca = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
               SDL_BlitSurface(ostraca, NULL, chalk, &tile[e][cursor[e]]);
               
               SDL_UpdateWindowSurface(slate);

               problem[e] = 10 * problem[e] + (int) a - 48;  
               cursor[e]++;
             }

                       /* ASK FOR STUDENT ANSWER */
             
           if ((step[e] == 1) && (a == 10))
             {
               if (problem[e] == 0)
                 {
                   step[e]--;
                   cursor[e] = 4;
                   goto jumpstart;
                 }

               else
                {
                  step[e]++;
                  SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));
                
                  if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[187], hue[e]);
                  else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[187], hue[e]);
                  SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);
                  
                  SDL_UpdateWindowSurface(slate);
                
                  cursor[e] = 0;
                  continue;
                }
             }
                  
                         /* RECEIVE STUDENT ANSWER */
           
           if (step[e] == 2) 
             {
               if ((a != 10) && (a !=13))
                 {
                   if (cursor[e] == 0)
                     {
                       z1[e] = (int) a;
                       SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));

                     }
                   if (cursor[e] == 1) z2[0] = (int) a;
                   if (cursor[e] == 2) z3[0] = (int) a;              
                     
                   if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, &z[0], hue[e]);
                   else ostraca = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
                   SDL_BlitSurface(ostraca, NULL, chalk, &tile[e][cursor[e]]);
                  
                   SDL_UpdateWindowSurface(slate);
                 
                   guess[e][cursor[e]] = z[0];  
                       cursor[e]++;  
                 }
             }
                   
                           /* ANSWER RIGHT OR WRONG? */
                 
           if ((a == 10) && (step[e] == 2))
             {
                 find_answer();
                 SDL_FillRect(chalk, &line[e], SDL_MapRGB(chalk->format,255,255,255));

                 if (strcmp(answ[e],guess[e]) == 0)
                   {
                     if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[193], hue[e]);
                     else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[193], hue[e]);
                     SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);
                     cursor[e] = 0;
                     step[e] = 0;
                   }

                 else
                   {
                     if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono30, ttext[188], hue[e]);
                     else ostraca = TTF_RenderText_Solid(fntVeraMono40, ttext[188], hue[e]);
                     SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);
                     step[e] = 0;
                   }
                     problem[e] = 0;
                     SDL_UpdateWindowSurface(slate);
                     for (d = 0; d < 20; d ++)  guess[e][d] = 0;
             }
         }
     }
  SDL_DestroyWindow(slate);
  return 0;
}

int
clear_pro()
{
  for (e = 0; e < 40; e++)
    {
       for (a = 0; a < 5000; a++)
         {
           comp[e][a] = 0;
         }
    }
  file_save();
  return 0;
}

int
detente()
{
  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );
    
  slate = SDL_CreateWindow("operation",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);
   
  struct SDL_Rect slots[6];
    
  slots[0].x = 0;
  slots[0].y = 200;
    
  slots[1].x = 0;
  slots[1].y = 320;
    
  slots[2].x = 0;
  slots[2].y = 380;
        
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono50, " PRESS ENTER TO CONTINUE ", black);
  SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
             
  SDL_UpdateWindowSurface(slate); 
        
  SDL_Event event;
  while(1)
    {
      SDL_PollEvent(&event);    
      if (event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
           {
              case SDLK_RETURN:   
              SDL_DestroyWindow(slate);
              return 0;                  
           }
        }
    }
  SDL_DestroyWindow(slate);
  return 0;
}

int
divide()
{
  char a;
  TTF_Font  *fntVeraMono33 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 33 );
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 40);

                          /* OPEN EMPTY WHITE WINDOW */

  slate = SDL_CreateWindow("div",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

                      /* PUT net_size PROBLEMS ON THE SCREEN */
  
  for (e = 0; e < net_size; e++)
    {
      stop[e] = 0;
      goofed[e] = 0;
      for (b = 0; b < 20; b++) last_line[e][b] = ' ';
      last_line[e][15] = ' ';
      renew[e] = 1;
      score[e]=0;
      make_div_prob(e);
    
      onesregister =  dividend[e] % 10;
      tensregister = dividend[e] / 10;
      last_line[e][1] = (char) tensregister + 48;
      last_line[e][2] = (char) onesregister + 48;
      last_line[e][4] = '/';
      last_line[e][6] = (char) divisor[e] + 48;
      last_line[e][8] = '=';
      if (last_line[e][1] == 48){last_line[e][1] = 32;}
      
      for (c = 0; c < 20; c++)
        {
          if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], hue[e]);
          else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], hue[e]);
          SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][c]);
        }
    }  
    
  SDL_UpdateWindowSurface(slate);
                
                         /* START KEYBOARD POLLING. */
             
  while (1)
    {
      if (get_char() == 10)  /* CHECK TEACHER'S KEYBOARD FOR RETURN TO MENU. */
        {
          goaround = 0;
          SDL_DestroyWindow(slate);
          return 0;
        }
      for (e = 0; e < net_size; e++)
        {
          if (goofed[e] == 1)    /* END OR CONTINUE ERROR TIMEOUT PENALTY. */ 
            {
              if (time(t) - clk[e] > 3)
                {
                  goofed[e] = 0;
                  z[0] = last_line[e][11];
                  if (net_size==40)lin1 = TTF_RenderText_Solid(fntVeraMono33, &z[0], white);
                  else lin1 = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
                  SDL_BlitSurface(lin1, NULL, chalk, &tilem[e][11]);
                  SDL_UpdateWindowSurface(slate);
                }    
              else
                {
                  get_event(e);
                  continue;
                } 
            }
            
          if (goofed[e] == 0)  /* GET STUDENT INPUT. */
            {
              a = get_event(e);        
              if (a == 0) continue;
              if (a == 10) goto evaluatediv;
              if (stop[e] == 1) continue;

               if (((int) a < 48) || ((int) a > 57)) continue;
               stuans[e] = (int) a - 48;
               z[0] = (int) a;
               last_line[e][11] = (int)a;
                

               if(net_size==40) lettr = TTF_RenderText_Solid(fntVeraMono33, &z[0], hue[e]);
               else lettr = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
               SDL_BlitSurface(lettr, NULL, chalk, &tilem[e][11]);
               SDL_UpdateWindowSurface(slate);
               stop[e] = 1;
               continue;
            
                                       /* EVALUATE ANSWERS. */
                         
              evaluatediv:

                             /* ANSWER IS GOOD. */

              if (stuans[e] == answer[e])
                 { 
                   for (c = 0; c < 20; c++)
                     {
                       if(net_size==40) hogwash = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], white);
                       else hogwash = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], white);
                       SDL_BlitSurface(hogwash, NULL, chalk, &tilem[e][c]);
                     }
                   SDL_UpdateWindowSurface(slate);

                   advance[e] = 1;
                   score[e] = score[e] + divpoints[e + 1];
                   onesregister = score[e] % 10;
                   tensregister = score[e] / 10;
                   last_line[e][15] = (char) onesregister + 48;
                   last_line[e][14] = (char) tensregister + 48;
                   last_line[e][11] = 32;
                   if(last_line[e][14] == 48){last_line[e][14] = 32;}

                   if(score[e] < win_score)
                     {
                       stop[e] = 0;
                       make_div_prob(e);
                      
                       onesregister =  dividend[e] % 10;
                       tensregister = dividend[e] / 10;
                       last_line[e][1] = (char) tensregister + 48;
                       last_line[e][2] = (char) onesregister + 48;
                       last_line[e][4] = '/';
                       last_line[e][6] = (char) divisor[e] + 48;
                       last_line[e][8] = '=';
                       last_line[e][11] = ' ';
                       if (last_line[e][1] == 48){last_line[e][1] = 32;}
                      
                       for (c = 0; c < 20; c++)
                         {
                           if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], hue[e]);
                           else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], hue[e]);
                           SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][c]);
                         }
                       
                       SDL_UpdateWindowSurface(slate);
                     }
                                           
                              /* DECLARE A WINNER. */
                      
                   else
                     {
                       SDL_DestroyWindow(slate);
                       win_page();
                       goaround = 1;
                       return 0;
                     } 
                 }   
               
                             /* ANSWER IS BAD. */

               else if(last_line[e][11] != answer[e])
                 {
                   clk[e] = time(t);
                   goofed[e] = 1;
                   stop[e] = 0;

                   z[0] = last_line[e][11];                
                   if(net_size==40) correction = TTF_RenderText_Solid(fntVeraMono33, &z[0], white);
                   else correction = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
                   SDL_BlitSurface(correction, NULL, chalk, &tilem[e][11]);
                   SDL_UpdateWindowSurface(slate);

                   z[0] = 'X';
                   last_line[e][11] = 'X';
                   if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, &z[0], black);
                   else ostraca = TTF_RenderText_Solid(fntVeraMono40, &z[0], black);
                   SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][11]);
                   SDL_UpdateWindowSurface(slate);
                 }
            }
        }        
    } 
  SDL_DestroyWindow(slate);
  return 0;          
}

int
double_check()
{
  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );
    
  slate = SDL_CreateWindow("operation",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);
   
  struct SDL_Rect slots[6];
    
  slots[0].x = 0;
  slots[0].y = 200;
    
  slots[1].x = 0;
  slots[1].y = 320;
    
  slots[2].x = 0;
  slots[2].y = 380;
        
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono50, " ARE YOU SURE YOU WANT TO QUIT? ", black);
  SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
             
  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono50, "            1 - QUIT NOW", black);
  SDL_BlitSurface(line1, NULL, chalk, &slots[1]);
             
  SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono50, "            2 - DON'T QUIT", black);
  SDL_BlitSurface(line2, NULL, chalk, &slots[2]);
             
  SDL_UpdateWindowSurface(slate); 
        
  SDL_Event event;
  option = 0;     
  while(1)
    {
      SDL_PollEvent(&event);    
      if (event.type == SDL_KEYDOWN)
        {
          switch(event.key.keysym.sym)
           {
             case SDLK_1:
               SDL_DestroyWindow(slate);
               security();
               if (panic == 1)
                 {
                   panic = 0;
                   option = 2;
                   SDL_DestroyWindow(slate);
                   return 0;
                 }
               exit(0);

             case SDLK_2:
               option = 2;  
               SDL_DestroyWindow(slate);
               return 0;                  
           }
        }
    }
  SDL_DestroyWindow(slate);
  return 0;
}

int
dump()
{
 int zz = 0;
  for (e = 0;  e < net_size; e++)
    {
      for(zz = 0; zz < 200; zz++)
       {
         a = (int) get_event(e);
       }
    }
  return 0;
}

int
eval_type()
{  
   renew[e] = 2;
   clk[e] = time(t) - clk[e];
   if (score[e] < 18) m = 190;
   if (score[e] > 17)
     {
       if (clk[e] > 15) m = 192;
       else
         {
           tt[e]++;
           m = 191;
         }
     }
   blockinput[e] = 1;
   clk[e] = time(t);
   return 0;    
}    

int
fetch_letter()
{
  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50);

  slate = SDL_CreateWindow("classname",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

               /* DIVIDE SLATE INTO TILES. */ 

   struct SDL_Rect tile[1000];
   int b = 0;
   for (c = 0;  c < 17; c++)
     {
       for (b = 0; b < 45;  b++)
          {
            tile[b+c*45].x = (b * 30);
            tile[b+c*45].y = c * 44;
          }
     }
   
   SDL_Surface *line = TTF_RenderText_Solid(fntVeraMono50, "WHO TEACHES THIS CLASS? ", red);
   SDL_BlitSurface(line, NULL, chalk, &tile[230]);
   SDL_UpdateWindowSurface(slate);

   for (k = 0; k < 20; k++)
     {
       teacher[k] = 0;
     }
   k = 0;
   int a = 325;
   char letter[2];
   SDL_Event event;
         
   while(1)
     {
       SDL_WaitEvent(&event);   
         {
            if (event.type == SDL_KEYDOWN)
              {
                if (1 == 1)                 
                  {
                    switch(event.key.keysym.sym)
                      {
                        case SDLK_0:
                          letter[0] =0;
                          break;
                         
                        case SDLK_1:
                          letter[0] ='1';
                          break;
                          
                        case SDLK_2:
                          letter[0] ='2';
                          break;
                          
                        case SDLK_3:
                          letter[0] ='3';
                          break;
                               
                        case SDLK_4:
                          letter[0] ='4';
                          break;
                      
                        case SDLK_5:
                          letter[0] ='5';
                          break;
                      
                        case SDLK_6:
                          letter[0] ='6';
                          break;
                                  
                        case SDLK_7:
                          letter[0] ='7';
                          break;
                                   
                        case SDLK_8:
                          letter[0] ='8';
                          break;
                            
                        case SDLK_9:
                          letter[0] ='9';
                          break;
                             
                        case SDLK_a:
                          letter[0] ='A';
                          break;
                              
                        case SDLK_b:
                          letter[0] ='B';
                          break;
                             
                        case SDLK_c:
                          letter[0] ='C';
                          break;
                               
                        case SDLK_d:
                          letter[0] ='D';
                          break;
                            
                        case SDLK_e:
                          letter[0] ='E';
                          break;
                               
                        case SDLK_f:
                          letter[0] ='F';
                          break;
                                
                        case SDLK_g:
                          letter[0] ='G';
                          break;
                               
                        case SDLK_h:
                          letter[0] ='H';
                          break;
                           
                        case SDLK_i:
                          letter[0] ='I';
                          break;
                             
                        case SDLK_j:
                          letter[0] ='J';
                          break;
                           
                        case SDLK_k:
                          letter[0] ='K';
                          break;
                              
                        case SDLK_l:
                          letter[0] ='L';
                          break;
                            
                        case SDLK_m:
                          letter[0] ='M';
                          break;
                             
                        case SDLK_n:
                          letter[0] ='N';
                          break;
                               
                        case SDLK_o:
                          letter[0] ='O';
                          break;
                                   
                        case SDLK_p:
                          letter[0] ='P';
                          break;
                                 
                        case SDLK_q:
                          letter[0] ='Q';
                          break;
                             
                        case SDLK_r:
                          letter[0] ='R';
                          break;
                              
                        case SDLK_s:
                          letter[0] ='S';
                          break;
                          
                        case SDLK_t:
                          letter[0] ='T';
                          break;
                         
                        case SDLK_u:
                          letter[0] ='U';
                          break;
                          
                        case SDLK_v:
                          letter[0] ='V';
                          break;
                          
                        case SDLK_w:
                          letter[0] ='W';
                          break;
                           
                        case SDLK_x:
                          letter[0] ='X';
                          break;
                               
                        case SDLK_y:
                          letter[0] ='Y';
                          break;
                        
                        case SDLK_z:
                          letter[0] ='Z';
                          break;
                              
                        case SDLK_SPACE:
                          letter[0] =' ';
                          break;
                                         
                        case SDLK_COMMA:
                          letter[0] ='<';
                          break;
                            
                        case SDLK_PERIOD:
                          letter[0] ='>';
                          break;
                           
                        case SDLK_SEMICOLON:
                          letter[0] =':';
                          break;
                           
                        case SDLK_SLASH:
                          letter[0] ='?';
                          break;
                                           
                        case SDLK_MINUS:
                          letter[0] ='_';
                          break;
                          
                        case SDLK_LEFTPAREN:
                          letter[0] ='(';
                          break;
                            
                        case SDLK_RIGHTPAREN:
                          letter[0] =')';
                          break;
                        
                        case SDLK_QUOTEDBL:
                          letter[0] ='"';
                          break;
                             
                        case SDLK_DOLLAR:
                          letter[0] ='$';
                          break;
                            
                        case SDLK_AMPERSAND:
                          letter[0] ='&';
                          break;
                         
                        case SDLK_EXCLAIM:
                          letter[0] ='!';
                          break;
                          
                        case SDLK_PLUS:
                          letter[0] ='+';
                          break;
                          
                        case SDLK_HASH:
                          letter[0] ='#';
                          break;
                          
                        case SDLK_QUESTION:
                          letter[0] ='?';
                          break;
                         
                        case SDLK_COLON:
                          letter[0] =':';
                          break;
                         
                        case SDLK_EQUALS:
                          letter[0] ='=';
                          break;
                             
                        case SDLK_GREATER:
                          letter[0] ='>';
                          break;
                        
                        case SDLK_LEFTBRACKET:
                          letter[0] ='[';
                          break;
                            
                        case SDLK_RIGHTBRACKET:
                          letter[0] =']';
                          break;
                        
                        case SDLK_LESS:
                          letter[0] ='<';
                          break;
                             
                        case SDLK_RETURN:   

// TODO: Make this work in Windows.
/*                           if (class == 1) FP = fopen("/usr/local/com/class1/names/teacher", "w");
                          if (class == 2) FP = fopen("/usr/local/com/class2/names/teacher", "w");
                          if (class == 3) FP = fopen("/usr/local/com/class3/names/teacher", "w");
                          if (class == 4) FP = fopen("/usr/local/com/class4/names/teacher", "w");
                          if (class == 5) FP = fopen("/usr/local/com/class5/names/teacher", "w");
                          if (class == 6) FP = fopen("/usr/local/com/class6/names/teacher", "w");
                          if (class == 7) FP = fopen("/usr/local/com/class7/names/teacher", "w");
                          if (class == 8) FP = fopen("/usr/local/com/class8/names/teacher", "w");
                          if (class == 9) FP = fopen("/usr/local/com/class9/names/teacher", "w");

                          fputs(teacher, FP);
                          fclose(FP); */
                          SDL_DestroyWindow(slate);
                          return 0;
                          break;

                        default:
                          letter[0] = ' ';
                          break;                     
                      }
                   
                    teacher[k] = letter[0];
                    letter[1] = 0; 
                    k++;
                    if (k > 20)
                      {
                        break;
                      }
                    SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono50, letter, black);
                    SDL_BlitSurface(line1, NULL, chalk, &tile[a]);
                    if (a < 675) {a++;}
                    SDL_UpdateWindowSurface(slate);
                 }
             }
         }
     }
  alpha();

  return 0;
}

int
fetch_name()
{  
  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );

                         /* OPEN EMPTY WHITE WINDOW. */

  slate = SDL_CreateWindow("add",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);

  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

                        /* CREATE RECTANGLE ARRAY. */

   struct SDL_Rect tile[1000];
   int b = 0;
   for (c = 0;  c < 17; c++)
     {
       for (b = 0; b < 45;  b++)
          {
            tile[b+c*45].x = (b * 30);
            tile[b+c*45].y = c * 44;
          }
     }
   
                         /* PUT MESSAGE ON SCREEN. */
   
   SDL_Surface *ask = TTF_RenderText_Solid(fntVeraMono50, "GIMME A NAME! ", red);
   SDL_BlitSurface(ask, NULL, chalk, &tile[235]);
   SDL_UpdateWindowSurface(slate);
   
   int a = 325;
   char letter;
   k = 0;

   for(k = 0; k < 10; k++)
     {
       while(1)
         {
           letter = get_char();         
           if (letter != 0) break;
         }
       if (letter == 10) break; 
       studint[k] = letter;
       studint[k+1] = 0;
       
       SDL_Surface *scratchpad = TTF_RenderText_Solid(fntVeraMono50, &studint[k], red);
       SDL_BlitSurface(scratchpad, NULL, chalk, &tile[a]);
       if (a < 675) {a++;}
       SDL_UpdateWindowSurface(slate);
     }

   int g = 0;
   for( g = 0; g < 10; g++)
     {
       roster[boardnumber][g] = studint[g];
     }
  return letter;
}

int
fetch_win_score()
{
   int b = 0;
   int digit = 0;
   TTF_Font  *fntVeraMono45 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 45);
      
                       /* CREATE SLATE ON WHICH TO WRITE */

  slate = SDL_CreateWindow("winscore",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

          
                       /* DIVIDE WHITEBOARD INTO TILES */

  struct SDL_Rect tile[1000];
  for (c = 0;  c < 17; c++)
    {
      for (b = 0; b < 45;  b++)
         {
           tile[b+c*45].x = (b * 30);
           tile[b+c*45].y = c * 44;
         }
    }
  
                               /* ASK FOR SCORE */
  
  SDL_Surface *line = TTF_RenderText_Solid(fntVeraMono45, "WHAT SCORE (between 10 and 99) WINS?", black);
  SDL_BlitSurface(line, NULL, chalk, &tile[316]);
  SDL_UpdateWindowSurface(slate);
   
  k = 0;         
  char proxy[2] = {0,0};
  proxy[0] = (char) get_digit() + 48;
  
  line = TTF_RenderText_Solid(fntVeraMono45, proxy , red);
  SDL_BlitSurface(line, NULL, chalk, &tile[430]);
  SDL_UpdateWindowSurface(slate);
  
  win_score = (int) proxy[0] - 48;
  
  proxy[1] = (char) get_digit() + 48;
  
  line = TTF_RenderText_Solid(fntVeraMono45, proxy , red);
  SDL_BlitSurface(line, NULL, chalk, &tile[430]);
  SDL_UpdateWindowSurface(slate);

  win_score = 10 * win_score + (int) proxy[1] - 48;

  alpha();

  ww[0] = (char)(win_score);

// TODO: Make this work in Windows.
/*   if (class == 1) FP = fopen("/usr/local/com/class1/settings/win_score", "w");
  if (class == 2) FP = fopen("/usr/local/com/class2/settings/win_score", "w");
  if (class == 3) FP = fopen("/usr/local/com/class3/settings/win_score", "w");
  if (class == 4) FP = fopen("/usr/local/com/class4/settings/win_score", "w");
  if (class == 5) FP = fopen("/usr/local/com/class5/settings/win_score", "w");
  if (class == 6) FP = fopen("/usr/local/com/class6/settings/win_score", "w");
  if (class == 7) FP = fopen("/usr/local/com/class7/settings/win_score", "w");
  if (class == 8) FP = fopen("/usr/local/com/class8/settings/win_score", "w");
  if (class == 9) FP = fopen("/usr/local/com/class9/settings/win_score", "w");

  fputs(ww, FP);
  fclose(FP); */
  SDL_DestroyWindow(slate);
  return digit;
}

int
file_save()
{
   for (e = 0;  e < 26; e++)
      {
        strcpy (Buf, "");

// TODO: Make this work in Windows.
/*         if (class == 1) strcpy (Buf, "/usr/local/com/class1/files/file");
        if (class == 2) strcpy (Buf, "/usr/local/com/class2/files/file");
        if (class == 3) strcpy (Buf, "/usr/local/com/class3/files/file");
        if (class == 4) strcpy (Buf, "/usr/local/com/class4/files/file");
        if (class == 5) strcpy (Buf, "/usr/local/com/class5/files/file");
        if (class == 6) strcpy (Buf, "/usr/local/com/class6/files/file");
        if (class == 7) strcpy (Buf, "/usr/local/com/class7/files/file");
        if (class == 8) strcpy (Buf, "/usr/local/com/class8/files/file");
        if (class == 9) strcpy (Buf, "/usr/local/com/class9/files/file");

        Buf[32] = e + 97;
        Buf[33] = 0;
         
        FP = fopen (Buf, "w");
        fputs (comp[e], FP);
        fclose (FP);
        
        strcpy (Buf, "");
        strcpy (Buf, "/root/Documents/");
        strcat (Buf, roster[e + 1]);
      
        if (roster[e + 1][0] != 0)
          {   
            FP = fopen (Buf, "w");
            fputs (comp[e], FP);
            fclose (FP);
          } */
      }

   for (e = 26;  e < 41; e++)
      {
        strcpy (Buf, "");

// TODO: Make this work in Windows.
/*         if (class == 1) strcpy (Buf, "/usr/local/com/class1/files/file");
        if (class == 2) strcpy (Buf, "/usr/local/com/class2/files/file");
        if (class == 3) strcpy (Buf, "/usr/local/com/class3/files/file");
        if (class == 4) strcpy (Buf, "/usr/local/com/class4/files/file");
        if (class == 5) strcpy (Buf, "/usr/local/com/class5/files/file");
        if (class == 6) strcpy (Buf, "/usr/local/com/class6/files/file");
        if (class == 7) strcpy (Buf, "/usr/local/com/class7/files/file");
        if (class == 8) strcpy (Buf, "/usr/local/com/class8/files/file");
        if (class == 9) strcpy (Buf, "/usr/local/com/class9/files/file");

        Buf[32] = e + 23;
        Buf[33] = 0;
         
        FP = fopen (Buf, "w");
        fputs (comp[e], FP);
        fclose (FP);
        
        strcpy (Buf, "");
        strcpy (Buf, "/root/Documents/");
        strcat (Buf, roster[e + 1]);
      
        if (roster[e + 1][0] != 0)
          {   
            FP = fopen (Buf, "w");
            fputs (comp[e], FP);
            fclose (FP);
          } */
      }
  return 0;
}

int
find_answer()
{
  b = 0;
  c = 0;
  for (a = 0; a < 40000; a++)
    {
      if (key[a] == 126)
        {
           b++;
           c = 0;
        }
      if (key[a] == 124) c++;
      if ((b == page[e]) && (c == problem[e]))
        {
          a++;
          break;
        }
    }

                           /* CLEAR ANSWER. */

  strncpy(answ[e], &dum[0], 40);
  for (c = 0; c < 39; c++)
    {
       if ((key[a] != 124) && (key[a] != 0) && (key[a] != 125))
         {
           answ[e][c] = key[a];
           a++;
         }
       else break;
    }
  return 0;
}

int
get_char()
{
   SDL_Event event;           
   SDL_PollEvent(&event);    
   if (event.type == SDL_KEYDOWN)
      {
        switch(event.key.keysym.sym)
           {
              case SDLK_0:
                letra[e] = '0';  
                break;
                
              case SDLK_1:
                letra[e] = '1';  
                break;
                
              case SDLK_2:
                letra[e] = '2';  
                break;
                 
              case SDLK_3:
                letra[e] = '3';  
                break;
                  
              case SDLK_4:
                letra[e] = '4';  
                break;
                  
                  case SDLK_5:
                    letra[e] = '5';  
                    break;
                  
                  case SDLK_6:
                    letra[e] = '6';  
                    break;
                  
                  case SDLK_7:
                    letra[e] = '7';  
                    break;
                   
                  case SDLK_8:
                    letra[e] = '8';  
                    break;
                   
                  case SDLK_9:
                    letra[e] = '9';  
                    break;
                  
                  case SDLK_a:
                    letra[e] ='A';
                    break;
                           
                  case SDLK_b:
                    letra[e] ='B';
                    break;
                          
                  case SDLK_c:
                    letra[e] ='C';
                    break;
                            
                  case SDLK_d:
                    letra[e] ='D';
                    break;
                         
                  case SDLK_e:
                    letra[e] ='E';
                    break;
                          
                  case SDLK_f:
                    letra[e] ='F';
                    break;
                             
                  case SDLK_g:
                    letra[e] ='G';
                    break;
                            
                  case SDLK_h:
                    letra[e] ='H';
                    break;
                           
                  case SDLK_i:
                    letra[e] ='I';
                    break;
                          
                  case SDLK_j:
                    letra[e] ='J';
                    break;
                           
                  case SDLK_k:
                    letra[e] ='K';
                    break;
                           
                  case SDLK_l:
                    letra[e] ='L';
                    break;
                            
                  case SDLK_m:
                    letra[e] ='M';
                    break;
                          
                  case SDLK_n:
                    letra[e] ='N';
                    break;
                            
                  case SDLK_o:
                    letra[e] ='O';
                    break;
                                
                  case SDLK_p:
                    letra[e] ='P';
                    break;
                              
                  case SDLK_q:
                    letra[e] ='Q';
                    break;
                          
                  case SDLK_r:
                    letra[e] ='R';
                    break;
                           
                  case SDLK_s:
                    letra[e] ='S';
                    break;
                          
                  case SDLK_t:
                    letra[e] ='T';
                    break;
                         
                  case SDLK_u:
                    letra[e] ='U';
                    break;
                          
                  case SDLK_v:
                    letra[e] ='V';
                    break;
                        
                  case SDLK_w:
                    letra[e] ='W';
                    break;
                        
                  case SDLK_x:
                    letra[e] ='X';
                    break;
                            
                  case SDLK_y:
                    letra[e] ='Y';
                    break;
                        
                  case SDLK_z:
                    letra[e] ='Z';
                    break;

                  case SDLK_SPACE:
                    letra[e] =' ';
                    break;
                                         
                  case SDLK_COMMA:
                    letra[e] ='<';
                    break;
                            
                  case SDLK_PERIOD:
                    letra[e] ='>';
                    break;
                           
                  case SDLK_SEMICOLON:
                    letra[e] =':';
                    break;
                           
                  case SDLK_SLASH:
                    letra[e] ='?';
                    break;
                                        
                  case SDLK_MINUS:
                    letra[e] ='_';
                    break;
                          
                  case SDLK_LEFTPAREN:
                    letra[e] ='(';
                    break;
                         
                  case SDLK_RIGHTPAREN:
                    letra[e] =')';
                    break;
                        
                  case SDLK_QUOTEDBL:
                    letra[e] ='"';
                    break;
                             
                  case SDLK_DOLLAR:
                    letra[e] ='$';
                    break;
                            
                  case SDLK_AMPERSAND:
                    letra[e] ='&';
                    break;
                         
                  case SDLK_EXCLAIM:
                    letra[e] ='!';
                    break;
                          
                  case SDLK_PLUS:
                    letra[e] ='+';
                    break;
                          
                  case SDLK_HASH:
                    letra[e] ='#';
                    break;
                          
                  case SDLK_QUESTION:
                    letra[e] ='?';
                    break;
                         
                  case SDLK_COLON:
                    letra[e] =':';
                    break;
                         
                  case SDLK_EQUALS:
                    letra[e] ='=';
                    break;
                             
                  case SDLK_GREATER:
                    letra[e] ='>';
                    break;
                        
                  case SDLK_LEFTBRACKET:
                    letra[e] ='[';
                    break;
                            
                  case SDLK_RIGHTBRACKET:
                    letra[e] =']';
                    break;
                        
                  case SDLK_LESS:
                    letra[e] ='<';
                    break;
                            
                  case SDLK_RETURN:
                    letra[e] = (char) 10;
                    return letra[e];
                    break;
                 
                  default:
                    return 0;
                }
            return letra[e];
         }
   return 0;
}

int
get_config()
{
	
// TODO: Make this work in Windows.
/*   FP = fopen("/usr/local/etc/events/bufa", "r");
  fgets(BUF[0], 22, FP);
  fclose(FP);
 
  FP = fopen("/usr/local/etc/events/bufb", "r");
  fgets(BUF[1], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufc", "r");
  fgets(BUF[2], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufd", "r");
  fgets(BUF[3], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufe", "r");
  fgets(BUF[4], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/buff", "r");
  fgets(BUF[5], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufg", "r");
  fgets(BUF[6], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufh", "r");
  fgets(BUF[7], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufi", "r");
  fgets(BUF[8], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufj", "r");
  fgets(BUF[9], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufk", "r");
  fgets(BUF[10], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufl", "r");
  fgets(BUF[11], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufm", "r");
  fgets(BUF[12], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufn", "r");
  fgets(BUF[13], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufo", "r");
  fgets(BUF[14], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufp", "r");
  fgets(BUF[15], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufq", "r");
  fgets(BUF[16], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufr", "r");
  fgets(BUF[17], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufs", "r");
  fgets(BUF[18], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/buft", "r");
  fgets(BUF[19], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufu", "r");
  fgets(BUF[20], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufv", "r");
  fgets(BUF[21], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufw", "r");
  fgets(BUF[22], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufx", "r");
  fgets(BUF[23], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufy", "r");
  fgets(BUF[24], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufz", "r");
  fgets(BUF[25], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufA", "r");
  fgets(BUF[26], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufB", "r");
  fgets(BUF[27], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufC", "r");
  fgets(BUF[28], 22, FP);
  fclose(FP);
   
  FP = fopen("/usr/local/etc/events/bufD", "r");
  fgets(BUF[29], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufE", "r");
  fgets(BUF[30], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufF", "r");
  fgets(BUF[31], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufG", "r");
  fgets(BUF[32], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufH", "r");
  fgets(BUF[33], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufI", "r");
  fgets(BUF[34], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufJ", "r");
  fgets(BUF[35], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufK", "r");
  fgets(BUF[36], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufL", "r");
  fgets(BUF[37], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufM", "r");
  fgets(BUF[38], 22, FP);
  fclose(FP);

  FP = fopen("/usr/local/etc/events/bufN", "r");
  fgets(BUF[39], 22, FP);
  fclose(FP); */
   
  return 0;
} 

int
get_digit()
{
   SDL_Event event;           
   while(1)
      {
        SDL_WaitEvent(&event);    
        if (event.type == SDL_KEYDOWN)
          {
            switch(event.key.keysym.sym)
               {
                  case SDLK_0:
                  digit = 0;  
                  break;
                
                  case SDLK_1:
                  digit = 1;  
                  break;
                
                  case SDLK_2:
                  digit = 2;  
                  break;
                 
                  case SDLK_3:
                  digit = 3;  
                  break;
                  
                  case SDLK_4:
                  digit = 4;  
                  break;
                  
                  case SDLK_5:
                  digit = 5;  
                  break;
                  
                  case SDLK_6:
                  digit = 6;  
                  break;
                  
                  case SDLK_7:
                  digit = 7;  
                  break;
                   
                  case SDLK_8:
                  digit = 8;  
                  break;
                   
                  case SDLK_9:
                  digit = 9;  
                  break;
                  
                  case SDLK_RETURN:
                  digit = 10;
                  return digit;
                  break;
                 
                  default:
                  digit = 10;
                  return digit;
               }
            return digit;
         }
    } 
}

int
get_event(int e)
{
  return 0;
  
  // TODO: Make it work in Windows.
  ltr = 0;
  int value = 0;
  int code = 0;
 
                    /* INITIALIZE THE FILE DESCRIPTOR SET. */
  
//  fd_set set;
//  FD_ZERO (&set);
//  FD_SET (fz[e], &set);
   
                   /* INITIALIZE THE TIMEOUT DATA STRUCTURE. */
  
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  
      /* SELECT RETURNS 0 IF TIMEOUT, 1 IF INPUT AVAILABLE, -1 IF ERROR. */
  
/*   if ((select (FD_SETSIZE, &set, NULL, NULL, &timeout)) < 1)
    {
      return 0;
    } */
  
                     /* SET HOW MANY BYTES WILL BE READ. */  
  size_t rb;

                     /*  GET ONLY ONE EVENT AT A TIME. */ 
/*   struct input_event ev[1];
  
  rb = read(fz[e], ev, sizeof(struct input_event));
 
  if (rb < (int) sizeof(struct input_event))
    {
      perror("evtest: short read");
      exit(0); 
      return 0;
    }
    
  if (EV_KEY == ev[0].type)
    {
       value = ev[0].value;
       code = ev[0].code;
          
       if (upper_case[e] == 0)
         {
           ltr = lc[code];
         }
       else
         { 
           ltr = uc[code];
         } 
       if( (code == 42) || (code == 54) )
         {
           if (value > 0)
             { 
               upper_case[e] = 1;
               ltr =  0;
             }
           else
             {
               upper_case[e] = 0;
             }
         } 
       if(value == 0)
         {
           ltr = 0; 
           make_or_break[0] = 0; 
         }
    }
   return ltr; */
}

int
get_file()
{ 
  int a;
  int incoming = 0;
  int char_nmbr;
  
                     /* ADDRESS THE CURRENT CLASS. */
  
// TODO: Make this work in Windows.
/* /*   if (class == 1) strcpy (Buf, "/usr/local/com/class1/files/file");
  if (class == 2) strcpy (Buf, "/usr/local/com/class2/files/file");
  if (class == 3) strcpy (Buf, "/usr/local/com/class3/files/file");
  if (class == 4) strcpy (Buf, "/usr/local/com/class4/files/file");
  if (class == 5) strcpy (Buf, "/usr/local/com/class5/files/file");
  if (class == 6) strcpy (Buf, "/usr/local/com/class6/files/file");
  if (class == 7) strcpy (Buf, "/usr/local/com/class7/files/file");
  if (class == 8) strcpy (Buf, "/usr/local/com/class8/files/file");
  if (class == 9) strcpy (Buf, "/usr/local/com/class9/files/file");

                /* OPEN INDIVIDUAL STUDENT FILES, FIRST 26. */

 /*  for (a = 0;  a < 26; a++)
    {
      Buf[32] = a + 97; 
      Buf[33] = 0;
      FP = fopen(Buf,"a+");
      for(char_nmbr = 0; char_nmbr < 5000; char_nmbr++)
        {
          incoming = fgetc(FP);
          if(incoming == -1) break;
          comp[a][char_nmbr] = (char) incoming;
        }
      fclose(FP);
    } */

                /* OPEN INDIVIDUAL STUDENT FILES, LAST 14. */
  
 /*  for (a = 26;  a < net_size; a++)
    {
      Buf[32] = a + 38; 
      Buf[33] = 0;
      FP = fopen(Buf,"a+");
      for(char_nmbr = 0; char_nmbr < 5000; char_nmbr++)
        {
          incoming = fgetc(FP);
          if(incoming == -1) break;
          comp[a][char_nmbr] = (char) incoming;
        }
      fclose(FP);
    } */
  return 0;
}

  /* THIS ROUTINE READS THE STUDENT NAMES FROM THE HARD DRIVE AND PLACES
     THEM IN THE ARRAY roster[][]. */
int
get_names()
{
  int a;
 
                   /*  ADDRESS THE CURRENT CLASS. */ 

// TODO: Make this work in Windows.
/*   if (class == 1) strcpy (Buf, "/usr/local/com/class1/names/name");
  if (class == 2) strcpy (Buf, "/usr/local/com/class2/names/name");
  if (class == 3) strcpy (Buf, "/usr/local/com/class3/names/name");
  if (class == 4) strcpy (Buf, "/usr/local/com/class4/names/name");
  if (class == 5) strcpy (Buf, "/usr/local/com/class5/names/name");
  if (class == 6) strcpy (Buf, "/usr/local/com/class6/names/name");
  if (class == 7) strcpy (Buf, "/usr/local/com/class7/names/name");
  if (class == 8) strcpy (Buf, "/usr/local/com/class8/names/name");
  if (class == 9) strcpy (Buf, "/usr/local/com/class9/names/name"); */

           /*  GET INDIVIDUAL STUDENT NAMES, FIRST 26. */

/*   for (a = 1;  a < 27; a++)
    {
      Buf[33] = 0;
      Buf[32] = a + 96; 
      FP = fopen(Buf, "a+");
      strcpy (roster[a], dumm);
      fgets (roster[a], 10, FP);
      fclose(FP);
    } */

             /* GET INDIVIDUAL STUDENT NAMES, LAST 14. */
  
/*   for (a = 27;  a < 41; a++)
    {
      Buf[32] = a + 38; 
      Buf[33] = 0;
      FP = fopen(Buf,"a+");
      strcpy (roster[a], dumm);
      fgets (roster[a], 10, FP);
      fclose(FP);
    } */
  return 0;
}

int
handicap()
{
  int a = 0;
  int operation = 0;
  e = select_board_page();
  a = handicapenterpage();
  operation = handicapoppage();
  
  if (operation == 1) addpoints[e + 1] = a ;
  if (operation == 2) subpoints[e + 1] = a;
  if (operation == 3) mulpoints[e + 1] = a;
  if (operation == 4) divpoints[e + 1] = a;

  for (e = 1;  e < 41;  e++){ hndicap[e] = (char)(addpoints[e] + 48);}

// TODO: Make this work in Windows.
/*   if (class == 1) FP = fopen("/usr/local/com/class1/settings/addpoints", "w");
  if (class == 2) FP = fopen("/usr/local/com/class2/settings/addpoints", "w");
  if (class == 3) FP = fopen("/usr/local/com/class3/settings/addpoints", "w");
  if (class == 4) FP = fopen("/usr/local/com/class4/settings/addpoints", "w");
  if (class == 5) FP = fopen("/usr/local/com/class5/settings/addpoints", "w");
  if (class == 6) FP = fopen("/usr/local/com/class6/settings/addpoints", "w");
  if (class == 7) FP = fopen("/usr/local/com/class7/settings/addpoints", "w");
  if (class == 8) FP = fopen("/usr/local/com/class8/settings/addpoints", "w");
  if (class == 9) FP = fopen("/usr/local/com/class9/settings/addpoints", "w");

  fputs(hndicap, FP);
  fclose(FP); */

  for (e = 1;  e < 41;  e++) hndicap[e] = (char)(subpoints[e] + 48);

/*   if (class == 1) FP = fopen("/usr/local/com/class1/settings/subpoints", "w");
  if (class == 2) FP = fopen("/usr/local/com/class2/settings/subpoints", "w");
  if (class == 3) FP = fopen("/usr/local/com/class3/settings/subpoints", "w");
  if (class == 4) FP = fopen("/usr/local/com/class4/settings/subpoints", "w");
  if (class == 5) FP = fopen("/usr/local/com/class5/settings/subpoints", "w");
  if (class == 6) FP = fopen("/usr/local/com/class6/settings/subpoints", "w");
  if (class == 7) FP = fopen("/usr/local/com/class7/settings/subpoints", "w");
  if (class == 8) FP = fopen("/usr/local/com/class8/settings/subpoints", "w");
  if (class == 9) FP = fopen("/usr/local/com/class9/settings/subpoints", "w");

  fputs(hndicap, FP);
  fclose(FP); */
  
  for (e = 1;  e < 41;  e++) hndicap[e] = (char)(mulpoints[e] + 48);

/*   if (class == 1) FP = fopen("/usr/local/com/class1/settings/mulpoints", "w");
  if (class == 2) FP = fopen("/usr/local/com/class2/settings/mulpoints", "w");
  if (class == 3) FP = fopen("/usr/local/com/class3/settings/mulpoints", "w");
  if (class == 4) FP = fopen("/usr/local/com/class4/settings/mulpoints", "w");
  if (class == 5) FP = fopen("/usr/local/com/class5/settings/mulpoints", "w");
  if (class == 6) FP = fopen("/usr/local/com/class6/settings/mulpoints", "w");
  if (class == 7) FP = fopen("/usr/local/com/class7/settings/mulpoints", "w");
  if (class == 8) FP = fopen("/usr/local/com/class8/settings/mulpoints", "w");
  if (class == 9) FP = fopen("/usr/local/com/class9/settings/mulpoints", "w");

  fputs(hndicap, FP);
  fclose(FP); */
  
  for (e = 1;  e < 41;  e++) hndicap[e] = (char)(divpoints[e] + 48);

/*   if (class == 1) FP = fopen("/usr/local/com/class1/settings/divpoints", "w");
  if (class == 2) FP = fopen("/usr/local/com/class2/settings/divpoints", "w");
  if (class == 3) FP = fopen("/usr/local/com/class3/settings/divpoints", "w");
  if (class == 4) FP = fopen("/usr/local/com/class4/settings/divpoints", "w");
  if (class == 5) FP = fopen("/usr/local/com/class5/settings/divpoints", "w");
  if (class == 6) FP = fopen("/usr/local/com/class6/settings/divpoints", "w");
  if (class == 7) FP = fopen("/usr/local/com/class7/settings/divpoints", "w");
  if (class == 8) FP = fopen("/usr/local/com/class8/settings/divpoints", "w");
  if (class == 9) FP = fopen("/usr/local/com/class9/settings/divpoints", "w");

  fputs(hndicap, FP);
  fclose(FP); */
  return 0;
}

int
handicapenterpage()
{
  slate = SDL_CreateWindow("point",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);

  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );
  struct SDL_Rect slots[3];
    
     slots[0].x = 0;
     slots[0].y = 295;
    
     slots[1].x = 0;
     slots[1].y = 395;
    
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono50, "          ENTER HANDICAP", black);
  SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
  SDL_UpdateWindowSurface(slate);

  int letter = 0;
  while(1)
    {
      letter = get_char();
      if((letter > 48) && (letter < 58)) break;
    }    
                   
  char proxy[2];
  proxy[0] =  letter;
  letter = letter - 48;
  proxy[1] = 0;
  
  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono50, proxy, black);
  SDL_BlitSurface(line1, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate);           

  SDL_DestroyWindow(slate);
  return (letter);
}

int
handicapoppage()
{
  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );
  
  slate = SDL_CreateWindow("operation",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);
  
  struct SDL_Rect slots[6];
    
     slots[0].x = 0;
     slots[0].y = 200;
    
     slots[1].x = 0;
     slots[1].y = 320;
    
     slots[2].x = 0;
     slots[2].y = 380;
    
     slots[3].x = 0;
     slots[3].y = 440;
     
     slots[4].x = 0;
     slots[4].y = 500;
        
     slots[5].x = 0;
     slots[5].y = 560;
        
     SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono50, "      ARE WE TALKING:", black);
     SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
             
     SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono50, "            1 - ADDITION?", black);
     SDL_BlitSurface(line1, NULL, chalk, &slots[1]);
             
     SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono50, "            2 - SUBTRACTION?", black);
     SDL_BlitSurface(line2, NULL, chalk, &slots[2]);
             
     SDL_Surface *line3 = TTF_RenderText_Solid(fntVeraMono50, "            3 - MULTIPLICATION?", black);
     SDL_BlitSurface(line3, NULL, chalk, &slots[3]);
            
     SDL_Surface *line4 = TTF_RenderText_Solid(fntVeraMono50, "            4 - DIVISION?", black);
     SDL_BlitSurface(line4, NULL, chalk, &slots[4]);
           
     SDL_UpdateWindowSurface(slate); 
        
     int option = 0;
       
     SDL_Event event;
     
     while(1)
       {
         SDL_PollEvent(&event);    
         if (event.type == SDL_KEYDOWN)
           {
             switch(event.key.keysym.sym)
                 {
                   case SDLK_1:
                   option = 1;  
   SDL_DestroyWindow(slate);
                   return option;
                   break;
                  
                   case SDLK_2:
                   option = 2;  
   SDL_DestroyWindow(slate);
                   return option;
                   break;
                  
                   case SDLK_3:
                   option = 3;
   SDL_DestroyWindow(slate);
                   return option;  
                   break;
                  
                   case SDLK_4:
                   option = 4;
   SDL_DestroyWindow(slate);
                   return option;  
                   break;
                   
                   case SDLK_ESCAPE:
                   double_check();
                   break;
                  
                   case SDLK_RETURN:
   SDL_DestroyWindow(slate);
                   return option;
                   break;
                 }
           }
       }
   SDL_DestroyWindow(slate);
   return option;
}

int
get_ten()
{
   TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50);

  slate = SDL_CreateWindow("factor",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);

  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

  struct SDL_Rect tile[1000];
  k = 0;
  int b = 0;
  for (c = 0;  c < 17; c++)
    {
      for (b = 0; b < 45;  b++)
        {
          tile[b+c*45].x = (b * 30);
          tile[b+c*45].y = c * 44;
        }
    }
    
   SDL_Surface *line = TTF_RenderText_Solid(fntVeraMono50, "ENTER 9 DIGITS (No zeros)", red);
    
   SDL_BlitSurface(line, NULL, chalk, &tile[230]);
   SDL_UpdateWindowSurface(slate);
        
   int a = 325;
   int letter = 0;
   SDL_Event event;
         
   while(1)
     {
       SDL_WaitEvent(&event);   
       if (event.type == SDL_KEYDOWN)
         {
           
           switch(event.key.keysym.sym)
             {
                case SDLK_1:
                  letter =1;
                  break;
                   
                case SDLK_2:
                  letter =2;
                  break;
                   
                case SDLK_3:
                  letter = 3;
                  break;
                   
                case SDLK_4:
                  letter =4;
                  break;
                   
                case SDLK_5:
                  letter =5;
                  break;
                   
                case SDLK_6:
                  letter =6;
                  break;
                   
                case SDLK_7:
                  letter =7;
                  break;
                   
                case SDLK_8:
                  letter =8;
                  break;
                   
                case SDLK_9:
                  letter =9;
                  break;

                default:
                  letter = 0;
             }
           if (letter > 0)
             {
               factor[k] = (char) letter+48;
               factor[k+1] = 0;
               if (k > 8)
                  {
                     break;
                  }
               SDL_Surface *line = TTF_RenderText_Solid(fntVeraMono50, &factor[k], red);
               SDL_BlitSurface(line, NULL, chalk, &tile[a]);
               if (a < 675) {a++;}
               SDL_UpdateWindowSurface(slate);
               k++;
             }
           if (k > 8) break;
         }
     }
  SDL_DestroyWindow(slate);
  return 0;
}

int
main(int ARGC, char *ARGV[])
{
  int c;
 
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();   

  /*  How many keyboards are in the network?  */
// TODO: Make this work in Windows.
/*   FP = fopen("/usr/local/etc/net_size", "r");
  fgets(net_buffer, 2, FP);
  fclose(FP); */
  net_size = (10 * ((int)net_buffer[0]-48));

  if(net_size == 20)
   {
     for (c = 0;  c < 5; c++)
        {
          for (b = 0; b < 20; b++)
             {
               tile[c][b].x = (b * 24) + 5;
               tile[c][b].y = (c * 60)+50;  // y runs from 0 to 224
               tile[c][b].h = 45;
               tile[c][b].w = 30;

               tilem[c][b].x = (b * 24) + 5;
               tilem[c][b].y = (c * 60)+50;  // y runs from 0 to 224
               tilem[c][b].h = 45;
               tilem[c][b].w = 30;
             }
          line[c].x = 5 ;
          line[c].y = (c * 60)+50;  // y runs from 0 to 224
          line[c].h = 45;
          line[c].w = 745;
        }
           
      for (c = 5;  c < 10; c++)
        {
          for (b = 0; b < 21; b++)
             {
               tile[c][b].x = (b * 24) + 5;  // x's run from 5 to 605
               tile[c][b].y = (c * 60)+110;  // y runs from 280 to 579
               tile[c][b].h = 45;
               tile[c][b].w = 30;

               tilem[c][b].x = (b * 24) + 5;  // x's run from 5 to 605
               tilem[c][b].y = (c * 60)+110;  // y runs from 280 to 579
               tilem[c][b].h = 45;
               tilem[c][b].w = 30;
             }
          line[c].x = 5;
          line[c].y = (c * 60) + 110;
          line[c].h = 45;
          line[c].w = 745;
        }
          
     for (c = 10;  c < 15; c++)
        {
          for (b = 0; b < 21; b++)
             {
               tile[c][b].x = (b * 24) + 540;  // x's run from 540 to 1015
               tile[c][b].y = ((c - 10) * 60) + 50;
               tile[c][b].h = 45;
               tile[c][b].w = 30;

               tilem[c][b].x = (b * 24) + 540;  // x's run from 540 to 1015
               tilem[c][b].y = ((c - 10) * 60) + 50;
               tilem[c][b].h = 45;
               tilem[c][b].w = 30;
             }
          line[c].x = 540;  // x runs from 540 to 1205
          line[c].y = ((c - 10) * 60) + 50;
          line[c].h = 45;
          line[c].w = 610;
        }
         
      for (c = 15;  c < 20; c++)
        {
          for (b = 0; b < 21; b++)
             {
               tile[c][b].x = (b * 24) + 540;
               tile[c][b].y = ((c - 10) * 60) + 110;
               tile[c][b].h = 45;
               tile[c][b].w = 30;

               tilem[c][b].x = (b * 24) + 540;
               tilem[c][b].y = ((c - 10) * 60) + 110;
               tilem[c][b].h = 45;
               tilem[c][b].w = 30;
             }
          line[c].x = 540;
          line[c].y = ((c - 10) * 60) + 110;
          line[c].h = 45;
          line[c].w = 610;
        }
         
      for(a = 0; a < 20;)
        {
          hue[a].r = 0;
          hue[a].g = 0;
          hue[a].b = 0;
          
          hue[a+1].r = 0;
          hue[a+1].g = 0;
          hue[a+1].b = 255;
          
          hue[a+2].r = 255;
          hue[a+2].g = 0;
          hue[a+2].b = 0;
          
          hue[a+3].r = 7;
          hue[a+3].g = 167;
          hue[a+3].b = 8;
          
          hue[a+4].r = 0;
          hue[a+4].g = 200;
          hue[a+4].b = 200;
          
          a = a + 5;
        }
   }
       
  if(net_size == 30)
    {
      for (c = 0;  c < 5; c++)
        {
          for (b = 0; b < 20; b++)
            {
              tile[c][b].x = (b * 24) + 5;
              tile[c][b].y = (c * 42);  // y runs from 0 to 224
              tile[c][b].h = 45;
              tile[c][b].w = 30;

              tilem[c][b].x = (b * 24) + 5;
              tilem[c][b].y = (c * 42);  // y runs from 0 to 224
              tilem[c][b].h = 45;
              tilem[c][b].w = 30;
            }
          line[c].x = 5 ;
          line[c].y = (c * 42);  // y runs from 0 to 224
          line[c].h = 45;
          line[c].w = 745;
        }
        
      for (c = 5;  c < 10; c++)
        {
          for (b = 0; b < 21; b++)
             {
               tile[c][b].x = (b * 24) + 5;  // x's run from 5 to 605
               tile[c][b].y = (c * 42)+46;  // y runs from 280 to 579
               tile[c][b].h = 45;
               tile[c][b].w = 30;

               tilem[c][b].x = (b * 24) + 5;  // x's run from 5 to 605
               tilem[c][b].y = (c * 42)+46;  // y runs from 280 to 579
               tilem[c][b].h = 45;
               tilem[c][b].w = 30;
             }
          line[c].x = 5;
          line[c].y = (c * 42) + 46;
          line[c].h = 45;
          line[c].w = 745;
        }
         
      for (c = 10;  c <15; c++)
        {
          for (b = 0; b < 21; b++)
            {
              tile[c][b].x = (b * 24) + 5;  // x's run from 5 to 605
              tile[c][b].y = (c * 42) + 92;  // y runs from 654 to 934
              tile[c][b].h = 45;
              tile[c][b].w = 30;

              tilem[c][b].x = (b * 24) + 5;  // x's run from 5 to 605
              tilem[c][b].y = (c * 42) + 92;  // y runs from 654 to 934
              tilem[c][b].h = 45;
              tilem[c][b].w = 30;
            }
          line[c].x = 5;
          line[c].y = (c * 42) + 92;
          line[c].h = 45;
          line[c].w = 745;
        }
          
      for (c = 15;  c < 20; c++)
        {
          for (b = 0; b < 21; b++)
            {
              tile[c][b].x = (b * 24) + 540; // x's run from 540
              tile[c][b].y = ((c - 15) * 42);
              tile[c][b].h = 45;
              tile[c][b].w = 30;

              tilem[c][b].x = (b * 24) + 540; // x's run from 540
              tilem[c][b].y = ((c - 15) * 42);
              tilem[c][b].h = 45;
              tilem[c][b].w = 30;
            }
          line[c].x = 540; // x runs from 540 to 1085
          line[c].y = ((c - 15) * 42);
          line[c].h = 45;
          line[c].w = 610;
        }
        
     for (c = 20;  c < 25; c++)
       {
         for (b = 0; b < 21; b++)
           {
             tile[c][b].x = (b * 24) + 540;  // x's run from 540 to 1015
             tile[c][b].y = ((c - 15) * 42) + 46;
             tile[c][b].h = 45;
             tile[c][b].w = 30;

             tilem[c][b].x = (b * 24) + 540;  // x's run from 540 to 1015
             tilem[c][b].y = ((c - 15) * 42) + 46;
             tilem[c][b].h = 45;
             tilem[c][b].w = 30;
           }
         line[c].x = 540;  // x runs from 540 to 1205
         line[c].y = ((c - 15) * 42) + 46;
         line[c].h = 45;
         line[c].w = 610;
       }
        
     for (c = 25;  c < 30; c++)
       {
         for (b = 0; b < 21; b++)
           {
             tile[c][b].x = (b * 24) + 540;
             tile[c][b].y = ((c - 15) * 42) + 92;
             tile[c][b].h = 45;
             tile[c][b].w = 30;

             tilem[c][b].x = (b * 24) + 540;
             tilem[c][b].y = ((c - 15) * 42) + 92;
             tilem[c][b].h = 45;
             tilem[c][b].w = 30;
           }
         line[c].x = 540;
         line[c].y = ((c - 15) * 42) + 92;
         line[c].h = 45;
         line[c].w = 610;
       }
         
     for(a = 0; a < 30;)
       {
         hue[a].r = 0;
         hue[a].g = 0;
         hue[a].b = 0;
         
         hue[a+1].r = 0;
         hue[a+1].g = 0;
         hue[a+1].b = 255;
         
         hue[a+2].r = 255;
         hue[a+2].g = 0;
         hue[a+2].b = 0;
         
         hue[a+3].r = 7;
         hue[a+3].g = 167;
         hue[a+3].b = 8;
         
         hue[a+4].r = 0;
         hue[a+4].g = 0;
         hue[a+4].b = 0;
         
         a = a + 5;
       }
    }

  if(net_size == 40)
    {
      for (c = 0;  c < 5; c++)
         {
           for (b = 0; b < 20; b++)
              {
                tile[c][b].x = (b * 18) + 5;
                tile[c][b].y = (c * 31);
                tile[c][b].h = 45;
                tile[c][b].w = 30;
                
                tilem[c][b].x = (b * 20) + 5;
                tilem[c][b].y = (c * 32);
                tilem[c][b].h = 45;
                tilem[c][b].w = 30;
              }
           line[c].x = 5 ;
           line[c].y = (c * 31);
           line[c].h = 45;
           line[c].w = 745;
         }
         
       for (c = 5;  c < 10; c++)
         {
           for (b = 0; b < 21; b++)
               {
                tile[c][b].x = (b * 18) + 5;
                tile[c][b].y = (c * 31)+35;
                tile[c][b].h = 45;
                tile[c][b].w = 30;
                 
                tilem[c][b].x = (b * 20) + 5;
                tilem[c][b].y = (c * 32)+35;
                tilem[c][b].h = 45;
                tilem[c][b].w = 30;
              }
           line[c].x = 5;
           line[c].y = (c * 31) + 35;
           line[c].h = 45;
           line[c].w = 745;
         }
           
       for (c = 10;  c <15; c++)
         {
           for (b = 0; b < 21; b++)
              {
                tile[c][b].x = (b * 18) + 5;
                tile[c][b].y = (c * 31) + 70;
                tile[c][b].h = 45;
                tile[c][b].w = 30;
                
                tilem[c][b].x = (b * 20) + 5;
                tilem[c][b].y = (c * 32) + 70;
                tilem[c][b].h = 45;
                tilem[c][b].w = 30;
              }
           line[c].x = 5;
           line[c].y = (c * 31) + 70;
           line[c].h = 45;
           line[c].w = 745;
         }
           
       for (c = 15;  c < 20; c++)
         {
           for (b = 0; b < 21; b++)
              {
                tile[c][b].x = (b * 18) + 5;
                tile[c][b].y = (c * 31) + 105;
                tile[c][b].h = 45;
                tile[c][b].w = 30;
                
                tilem[c][b].x = (b * 20) + 5;
                tilem[c][b].y = (c * 32) + 105;
                tilem[c][b].h = 45;
                tilem[c][b].w = 30;
              }
           line[c].x = 5;
           line[c].y = (c * 31 + 105);
           line[c].h = 45;
           line[c].w = 610;
         }
            
      for (c = 20;  c < 25; c++)
         {
           for (b = 0; b < 21; b++)
              {
                tile[c][b].x = (b * 18) + 540;
                tile[c][b].y = ((c - 20) * 31);
                tile[c][b].h = 45;
                tile[c][b].w = 30;
                
                tilem[c][b].x = (b * 20) + 540;
                tilem[c][b].y = ((c - 20) * 32);
                tilem[c][b].h = 45;
                tilem[c][b].w = 30;
              }
           line[c].x = 540;
           line[c].y = ((c - 20) * 31);
           line[c].h = 45;
           line[c].w = 610;
         }
             
       for (c = 25;  c < 30; c++)
         {
           for (b = 0; b < 21; b++)
              {
                tile[c][b].x = (b * 18) + 540;
                tile[c][b].y = ((c - 20) * 31) + 35;
                tile[c][b].h = 45;
                tile[c][b].w = 30;
                
                tilem[c][b].x = (b * 20) + 540;
                tilem[c][b].y = ((c - 20) * 32) + 35;
                tilem[c][b].h = 45;
                tilem[c][b].w = 30;
              }
           line[c].x = 540;
           line[c].y = ((c - 20) * 31) + 35;
           line[c].h = 45;
           line[c].w = 610;
         }
           
       for (c = 30;  c < 35; c++)
         {
           for (b = 0; b < 21; b++)
              {
                tile[c][b].x = (b * 18) + 540;
                tile[c][b].y = ((c - 20) * 31) + 70;
                tile[c][b].h = 45;
                tile[c][b].w = 30;
                
                tilem[c][b].x = (b * 20) + 540;
                tilem[c][b].y = ((c - 20) * 32) + 70;
                tilem[c][b].h = 45;
                tilem[c][b].w = 30;
              }
           line[c].x = 540;
           line[c].y = ((c - 20) * 31) + 70;
           line[c].h = 45;
           line[c].w = 610;
         }
           
       for (c = 35;  c < 41; c++)
         {
           for (b = 0; b < 21; b++)
              {
                tile[c][b].x = (b * 18) + 540;
                tile[c][b].y = ((c - 20) * 31) + 105;
                tile[c][b].h = 45;
                tile[c][b].w = 30;
                
                tilem[c][b].x = (b * 20) + 540;
                tilem[c][b].y = ((c - 20) * 32) + 105;
                tilem[c][b].h = 45;
                tilem[c][b].w = 30;
              }
           line[c].x = 540;
           line[c].y = ((c - 20) * 31) + 105;
           line[c].h = 45;
           line[c].w = 610;
         }
          
       for(a = 0; a < 40;)
         {
           hue[a].r = 0;
           hue[a].g = 0;
           hue[a].b = 0;
           
           hue[a+1].r = 0;
           hue[a+1].g = 0;
           hue[a+1].b = 255;
            
           hue[a+2].r = 255;
           hue[a+2].g = 0;
           hue[a+2].b = 0;
           
           hue[a+3].r = 7;
           hue[a+3].g = 167;
           hue[a+3].b = 8;
           
           hue[a+4].r = 0;
           hue[a+4].g = 0;
           hue[a+4].b = 0;
           
           a = a + 5;
         }
    }

  int op = 0;
  int a = 0;
  int b = 0; 
                  /*  LOWER CASE DECODING TABLE  */
   
  /*  a to z  */ 
  lc[30]=97; lc[48]=98; lc[46]=99; lc[32]=100; lc[18]=101; lc[33]=102;
  lc[34]=103; lc[35]=104; lc[23]=105; lc[36]=106; lc[37]=107; lc[38]=108;
  lc[50]=109; lc[49]=110; lc[24]=111; lc[25]=112; lc[16]=113; lc[19]=114;
  lc[31]=115; lc[20]=116; lc[22]=117; lc[47]=118; lc[17]=119; 
  lc[45]=120; lc[21]=121; lc[44]=122;
  
  /*  Miscellaneous  */
  lc[0]=0;
  lc[101]=10;lc[57]=32;lc[1]=27;lc[41]=96;lc[12]=45;/* cr,sp,esc,`,-  */
  lc[15]=9;lc[13]=61;lc[52]=46;lc[51]=44; /* tab = . ,  */
  lc[53]=47;lc[43]=92;lc[39]=59; /* \ / ;  */
  lc[40]=39;lc[28]=10;lc[26]=91;lc[27]=93;/*',tab,[,]  */
  lc[111]=127;lc[110]=129;lc[14]=0;/* delete,insert,backspace  */
  lc[105]=130;lc[106]=131;/* crsrleft,crsrright  */
  
  /*  0 to 9  */
  lc[11]=48;lc[2]=49;lc[3]=50;lc[4]=51;lc[5]=52;lc[6]=53;lc[7]=54;
  lc[8]=55;lc[9]=56;lc[10]=57;
  
                 /*  UPPER CASE DECODING TABLE  */
  
  /*  A to Z  */
  uc[30]=65;uc[48]=66;uc[46]=67;uc[32]=68;uc[18]=69;uc[33]=70;uc[34]=71;
  uc[35]=72;uc[23]=73;uc[36]=74;uc[37]=75;uc[38]=76;uc[50]=77;uc[49]=78;
  uc[24]=79;uc[25]=80;uc[16]=81;uc[19]=82;uc[31]=83;uc[20]=84;uc[22]=85;
  uc[47]=86;uc[17]=87;uc[45]=88;uc[21]=89;uc[44]=90;
  
  /*  Miscellaneous  */
  
  /*  cr  sp  exc  ~  */
  uc[101]=10; uc[57]=32; uc[1]=27; uc[41]=126; uc[12]=95;
 
  /*  +  <  >  */
  uc[13]=43; uc[52]=62; uc[51]=60;
  
  /*  |  :  ?  */
  uc[43]=124; uc[39]=58; uc[53]=63;
  
  /*  "  tab  backspace  */
  uc[40]=34; uc[15]=9; uc[14]=0;
  
  /*  {  }  delete  insert  */
  uc[26]=123; uc[27]=125; uc[111]=127; uc[110]=129;
  
  /*  csr_left  csr_right  */
  uc[105]=130; uc[106]=131;

  /*  ) to (  */
  uc[11]=41; uc[2]=33; uc[3]=64; uc[4]=35; uc[5]=36; uc[6]=37; uc[7]=94;
  uc[8]=38; uc[9]=42; uc[10]=40;
  

  while (1 == 1)
   {
      /*  These download the lists of numbers from which the program 
          randomly draws to create the math problems.  */

// TODO: Make this work in Windows.
/*       if (class == 1) FP = fopen("/usr/local/com/class1/settings/terma", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/settings/terma", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/settings/terma", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/settings/terma", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/settings/terma", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/settings/terma", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/settings/terma", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/settings/terma", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/settings/terma", "r");
      fgets(terma_buffer, 10, FP);
      fclose(FP); */
      strcpy(terma, terma_buffer);
      
/*       if (class == 1) FP = fopen("/usr/local/com/class1/settings/termb", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/settings/termb", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/settings/termb", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/settings/termb", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/settings/termb", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/settings/termb", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/settings/termb", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/settings/termb", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/settings/termb", "r");
      fgets(termb_buffer, 10, FP);
      fclose(FP); */
      strcpy(termb, termb_buffer);

      /*  Download name of the teacher for this class  */
/*       if (class == 1) FP = fopen("/usr/local/com/class1/names/teacher", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/names/teacher", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/names/teacher", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/names/teacher", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/names/teacher", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/names/teacher", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/names/teacher", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/names/teacher", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/names/teacher", "r");
      fgets(teacher, 15, FP);
      fclose(FP); */
      
      /*  Get the score needed to win math games  */
/*       if (class == 1) FP = fopen("/usr/local/com/class1/settings/win_score", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/settings/win_score", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/settings/win_score", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/settings/win_score", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/settings/win_score", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/settings/win_score", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/settings/win_score", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/settings/win_score", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/settings/win_score", "r");
      fgets(ww, 2, FP);
      fclose(FP); */
      
      if(strlen(ww) == 0)
        {
          win_score = 90;
        }
      else
        {
          win_score=(int)ww[0];
        }
      
      /*  Download a list showing what typing practice line each student is
          working on.  */
/*       if (class == 1) FP = fopen("/usr/local/com/class1/settings/type_line", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/settings/type_line", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/settings/type_line", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/settings/type_line", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/settings/type_line", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/settings/type_line", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/settings/type_line", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/settings/type_line", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/settings/type_line", "r");
      fgets(type_line, 41, FP);
      fclose(FP); */
         
      /*  Download system password  */
/*       FP = fopen("/usr/local/etc/pass", "r");
      fgets(pass, 5, FP);
      fclose(FP); */
        
      /*  These sections download the handicaps for each kind of math 
          operation  */

/*       if (class == 1) FP = fopen("/usr/local/com/class1/settings/addpoints", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/settings/addpoints", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/settings/addpoints", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/settings/addpoints", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/settings/addpoints", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/settings/addpoints", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/settings/addpoints", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/settings/addpoints", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/settings/addpoints", "r"); 
      fgets(hndicap, 42, FP);
      fclose(FP); */
      for (e = 0;  e < 41;  e++)
        {
          addpoints[e] = (int)(hndicap[e]) - 48;
        }
         
/*       if (class == 1) FP = fopen("/usr/local/com/class1/settings/subpoints", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/settings/subpoints", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/settings/subpoints", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/settings/subpoints", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/settings/subpoints", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/settings/subpoints", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/settings/subpoints", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/settings/subpoints", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/settings/subpoints", "r");
      fgets(hndicap, 42, FP);
      fclose(FP); */
      for (e = 0;  e < 41;  e++)
        {
          subpoints[e] = (int)(hndicap[e]) - 48;
        }
    
/*       if (class == 1) FP = fopen("/usr/local/com/class1/settings/mulpoints", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/settings/mulpoints", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/settings/mulpoints", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/settings/mulpoints", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/settings/mulpoints", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/settings/mulpoints", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/settings/mulpoints", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/settings/mulpoints", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/settings/mulpoints", "r");
      fgets(hndicap, 42, FP);
      fclose(FP); */
      for (e = 0;  e < 41;  e++)
        {
          mulpoints[e] = (int)(hndicap[e]) - 48;
        }
        
/*       if (class == 1) FP = fopen("/usr/local/com/class1/settings/divpoints", "r");
      if (class == 2) FP = fopen("/usr/local/com/class2/settings/divpoints", "r");
      if (class == 3) FP = fopen("/usr/local/com/class3/settings/divpoints", "r");
      if (class == 4) FP = fopen("/usr/local/com/class4/settings/divpoints", "r");
      if (class == 5) FP = fopen("/usr/local/com/class5/settings/divpoints", "r");
      if (class == 6) FP = fopen("/usr/local/com/class6/settings/divpoints", "r");
      if (class == 7) FP = fopen("/usr/local/com/class7/settings/divpoints", "r");
      if (class == 8) FP = fopen("/usr/local/com/class8/settings/divpoints", "r");
      if (class == 9) FP = fopen("/usr/local/com/class9/settings/divpoints", "r");
      fgets(hndicap, 42, FP);
      fclose(FP); */
      for (e = 0;  e < 41;  e++)
        {
          divpoints[e] = (int)(hndicap[e]) - 48;
        }
         
      /*  This changes the numbers which the program randomly draws to
          create math problems from ASCII to binary.  */
      for (a = 0; a < 9; a++)
        {
           t1[a] = (int)(terma[a]) - 48;
           t2[a] = (int)(termb[a]) - 48;
        }
        
      /*  Convert ASCII representation of typing line numbers to binary.  */
      for (e = 0; e < 41; e++)
        {
          tline[e] = ((int)(type_line[e]) - 48);
          
          /*  If bit 7 is set, calculate line number from negative value.  */
          if (tline[e] < 0)
            {
              tline[e] = 256 + tline[e];
            }
        }
        
      /*  Eliminate Division by Zero  */
      for (a = 0; a < 10; a++)
        {
          if (t1[a] == 0) t1[a] = 1;
          if (t2[a] == 0) t2[a] = 1;
        }
        
      /*  Clear Word Processor Files  */
      for (e = 0; e < 41; e++)
        {
          for (b = 0; b < 5001; b++) comp[e][b] = 0;
        }
      b = 0;
        
      /*  Set Initial Scores to 0  */
      for (e = 0; e < 42; e++)
        {
          score[e] = 0;
        }
         
      /*  Recover Files From Hard Drive  */


      get_file();

      get_names();
      get_config();
      
      if (op == 0)
        {
          opener();
          op = 1;
        }

      splash_page();
      while (1)
        {
          if (changeclass == 1)
            {
              changeclass = 0;
              class = classnumber;
              SDL_DestroyWindow(choices);
              break;
            }
          menu();
        }
   }
}

int
make_add_prob()
{ 
  int a = t1[random_num()];
  int b = t2[random_num()];
  advance[e] = 0;
  stuans[e] = 0;
  addend1[e] = t1[a];
  addend2[e] = t2[b];
  answer[e] = t1[a] + t2[b]; 
  return 0;
}

int
make_div_prob(e)
{
  int a = random_num();
  int b = random_num();
  advance[e] = 0;
  dividend[e] = t1[a] * t2[b];
  divisor[e] = t1[a];
  answer[e] = t2[b];
  return 0;
}

int
make_mul_prob()
{
  int a = random_num();
  int b = random_num();
  advance[e] = 0;
  answer[e] = t1[a] * t2[b];
  factor1[e] = t1[a];
  factor2[e] = t2[b];
  stuans[e] = 0;
  return 0;
}

int
make_sub_prob()
{ 
  int a = random_num();
  int b = random_num();
  advance[e] = 0;
  subtrahend[e] = t1[a] + t2[b];
  minuend[e] = t1[a];
  stuans[e] = 0;
  answer[e] = t2[b];
  return 0;
}

int
menu()
{
  int a = 0; 
  starthere:
  choices = SDL_CreateWindow("20w",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  options = SDL_GetWindowSurface(choices);
  SDL_Renderer *rendc = SDL_CreateRenderer(choices, -1,SDL_RENDERER_SOFTWARE);


  SDL_SetRenderDrawColor(rendc, 255,255,255,255);
  SDL_RenderClear(rendc);
  SDL_RenderPresent(rendc);
  SDL_UpdateWindowSurface(choices);
     
  SDL_Event event;
      
  TTF_Font  *fntVeraMono30 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 30 );
     
  struct SDL_Rect tile[25];
       
  tile[0].x = 0;  /* teacher */
  tile[0].y = 0;
  tile[0].h = 30;
    
  tile[1].x = 130; /* math */
  tile[1].y = 100;
  tile[1].h = 30;
    
  tile[2].x = 380; /* division */
  tile[2].y = 40;
  tile[2].h = 30;
      
  tile[3].x = 380;  /* multiplication */
  tile[3].y = 70;
  tile[3].h = 30;
   
  tile[4].x = 380;  /* subtraction */
  tile[4].y = 100;
  tile[4].h = 30;
    
  tile[5].x = 380;  /* addition */
  tile[5].y = 130;
  tile[5].h = 30;
    
  tile[6].x = 380;  /* check your math */
  tile[6].y = 160;
  tile[6].h = 30;
     
  tile[7].x = 0;  /* blank line */
  tile[7].y = 190;
  tile[7].h = 30;
    
  tile[8].x = 75; /* language */
  tile[8].y = 265;
  tile[8].h = 30;
    
  tile[9].x = 380;  /* keyboarding */
  tile[9].y = 250;
  tile[9].h = 30;
    
  tile[10].x = 380;  /* word processing */
  tile[10].y = 280;
  tile[10].h = 30;
    
  tile[11].x = 100;  /* blank line */
  tile[11].y = 280;
  tile[11].h = 30;
      
  tile[12].x = 50;  /* management */  
  tile[12].y = 480;
  tile[12].h = 30;
    
  tile[13].x = 380;  /* math handicaps */
  tile[13].y = 340;
  tile[13].h = 30;
    
  tile[14].x = 380;  /* win score */
  tile[14].y = 370;
  tile[14].h = 30;
    
  tile[15].x = 380;  /* first factor */
  tile[15].y = 400;
  tile[15].h = 30;
    
  tile[16].x = 380;  /* second factor */
  tile[16].y = 430;
  tile[16].h = 30;
    
  tile[17].x = 380;  /* one typing line */
  tile[17].y = 460;
  tile[17].h = 30;
    
  tile[18].x = 380;  /* all typing lines */
  tile[18].y = 490;
  tile[18].h = 30;
    
  tile[19].x = 380;  /* erase all writing */
  tile[19].y = 520;
  tile[19].h = 30;
    
  tile[20].x = 380;  /* print one */
  tile[20].y = 550;
  tile[20].h = 30;
     
  tile[21].x = 380;  /* print all */
  tile[21].y = 580;
  tile[21].h = 30;
     
  tile[22].x = 380;  /* read roster */
  tile[22].y = 610;
  tile[22].h = 30;
     
  tile[23].x = 380;  /* change roster */
  tile[23].y = 640;
  tile[23].h = 30;
     
  tile[24].x = 380;  /* change classname */
  tile[24].y = 670;
  tile[24].h = 30;
     
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono30, teacher, red);
  SDL_BlitSurface(line0, NULL, options, &tile[0]);
   
  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono30, "MATH", red);
  SDL_BlitSurface(line1, NULL, options, &tile[1]);
   
  SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono30, "A - Division", red);
  SDL_BlitSurface(line2, NULL, options, &tile[2]);
    
  SDL_Surface *line3 = TTF_RenderText_Solid(fntVeraMono30, "B - Multiplication", red);
  SDL_BlitSurface(line3, NULL, options, &tile[3]);
    
  SDL_Surface *line4 = TTF_RenderText_Solid(fntVeraMono30, "C - Subtraction", red);
  SDL_BlitSurface(line4, NULL, options, &tile[4]);
    
  SDL_Surface *line5 = TTF_RenderText_Solid(fntVeraMono30, "D - Addition", red);
  SDL_BlitSurface(line5, NULL, options, &tile[5]);
   
  SDL_Surface *line6 = TTF_RenderText_Solid(fntVeraMono30, "E - Check Your Math", red);
  SDL_BlitSurface(line6, NULL, options, &tile[6]);
    
  SDL_Surface *line7 = TTF_RenderText_Solid(fntVeraMono30, "", red);
  SDL_BlitSurface(line7, NULL, options, &tile[7]);
    
  SDL_Surface *line8 = TTF_RenderText_Solid(fntVeraMono30, "LANGUAGE", green);
  SDL_BlitSurface(line8, NULL, options, &tile[8]);
    
  SDL_Surface *line9 = TTF_RenderText_Solid(fntVeraMono30, "F - Keyboarding", green);
  SDL_BlitSurface(line9, NULL, options, &tile[9]);
    
  SDL_Surface *line10 = TTF_RenderText_Solid(fntVeraMono30, "G - Word Processing", green);
  SDL_BlitSurface(line10, NULL, options, &tile[10]);
    
  SDL_Surface *line11 = TTF_RenderText_Solid(fntVeraMono30, "", red);
  SDL_BlitSurface(line11, NULL, options, &tile[11]);
    
  SDL_Surface *line12 = TTF_RenderText_Solid(fntVeraMono30, "MANAGEMENT", black);
  SDL_BlitSurface(line12, NULL, options, &tile[12]);
    
  SDL_Surface *line13 = TTF_RenderText_Solid(fntVeraMono30, "H - Math Facts Handicaps", black);
  SDL_BlitSurface(line13, NULL, options, &tile[13]);
   
  SDL_Surface *line14 = TTF_RenderText_Solid(fntVeraMono30, "I - Set Winning Score", black);
  SDL_BlitSurface(line14, NULL, options, &tile[14]);
    
  SDL_Surface *line15 = TTF_RenderText_Solid(fntVeraMono30, "J - First Multiplication Factor", black);
  SDL_BlitSurface(line15, NULL, options, &tile[15]);
    
  SDL_Surface *line16 = TTF_RenderText_Solid(fntVeraMono30, "K - Second Multiplication Factor", black);
  SDL_BlitSurface(line16, NULL, options, &tile[16]);
  
  SDL_Surface *line17 = TTF_RenderText_Solid(fntVeraMono30, "L - Set One Student's Typing Line", black);
  SDL_BlitSurface(line17, NULL, options, &tile[17]);
    
  SDL_Surface *line18 = TTF_RenderText_Solid(fntVeraMono30, "M - Set Back All Typing Lines", black);
  SDL_BlitSurface(line18, NULL, options, &tile[18]);
    
  SDL_Surface *line19 = TTF_RenderText_Solid(fntVeraMono30, "N - Erase All Student Writing", black);
  SDL_BlitSurface(line19, NULL, options, &tile[19]);
    
  SDL_Surface *line20 = TTF_RenderText_Solid(fntVeraMono30, "O - Print One Student's Writing", black);
  SDL_BlitSurface(line20, NULL, options, &tile[20]);
   
  SDL_Surface *line21 = TTF_RenderText_Solid(fntVeraMono30, "P - Print All Students' Writing", black);
  SDL_BlitSurface(line21, NULL, options, &tile[21]);
   
  SDL_Surface *line22 = TTF_RenderText_Solid(fntVeraMono30, "Q - Read Roster", black);
  SDL_BlitSurface(line22, NULL, options, &tile[22]);
    
  SDL_Surface *line23 = TTF_RenderText_Solid(fntVeraMono30, "R - Change Roster", black);
  SDL_BlitSurface(line23, NULL, options, &tile[23]);
         
   SDL_Surface *line24 = TTF_RenderText_Solid(fntVeraMono30, "S - Change Classname", black);
  SDL_BlitSurface(line24, NULL, options, &tile[24]);
         
  SDL_UpdateWindowSurface(choices);
    
  while(1)
    {
      SDL_WaitEvent(&event);
      if (event.type == SDL_KEYDOWN)
        {
          switch (event.key.keysym.sym)
           {
             /*  DIVISION  */
             case SDLK_a:
                repeat = 0;
                goaround = 1;                   
                while(1)
                  {
                    if(goaround == 1)
                      {
                        SDL_DestroyWindow(choices);
                        dump();
                        divide();
                      }
                    else break;
                  }
                goto starthere;
                break;
               
             /*  MULTIPLICATION  */
             case SDLK_b:
                repeat = 0;
                goaround = 1;
                while(1)
                   {
                     if(goaround == 1)
                       {
                         SDL_DestroyWindow(choices);
                         dump();
                         multiply();
                       }
                     else break;
                   }
                goto starthere;
                break;         
                
             /*  SUBTRACTION  */
             case SDLK_c:
                repeat = 0;
                goaround = 1;
                while(1)
                   {
                     if(goaround == 1)
                       {
                         SDL_DestroyWindow(choices);
                         dump();
                         subtract();
                       }
                     else break;
                   }
                goto starthere;
                break;
          
             /*  ADDITION  */
             case SDLK_d:
                repeat = 0;
                goaround = 1;
                while(1)
                  {
                    if(goaround == 1)
                      {
                        SDL_DestroyWindow(choices);
                        dump();
                        add();
                      }
                    else break;
                  }
                 goto starthere;
                break;
                
             /*  CHECK YOUR MATH  */
             case SDLK_e:
                repeat = 0;
                SDL_DestroyWindow(choices);
                select_book();
                dump();
                check();
                goto starthere;
                break;
                
             /*  KEYBOARDING  */
             case SDLK_f:
                repeat = 0;
                SDL_DestroyWindow(choices);
                dump();
                board();
                bail();
                goto starthere;
                break;
                
             /*  WORD PROCESSOR  */
             case SDLK_g:
                repeat = 0;
                SDL_DestroyWindow(choices);
                dump();
                process();
                file_save();
                goto starthere;
                break;
                
             /*  SET HANDICAPS  */
             case SDLK_h:
                repeat = 0;
                SDL_DestroyWindow(choices);
                security();
                if (panic == 1)
                  {
                    panic = 0;
                    goto starthere;
                  }
                handicap();
                goto starthere;
                break;
                
             /*  SET WINNING SCORE  */
             case SDLK_i:
                repeat = 0;
                if (panic == 1)
                  {
                    panic = 0;
                    goto starthere;
                  }
                SDL_DestroyWindow(choices);
                fetch_win_score();
                goto starthere;
                break;
                   
             /*  SET FIRST FACTORS  */
             case SDLK_j:
                repeat = 0;
                SDL_DestroyWindow(choices);
                security();
                if (panic == 1)
                  {
                    panic = 0;
                    goto starthere;
                  }
                get_ten();
                for (b = 0; b < 9; b++)
                   { 
                     t1[b] = (int)factor[b];
                     a = t1[b];
                     terma[b] = (char)(a);  
                   } 
                
// TODO: Make this work in Windows.
/*                 if (class == 1) FP = fopen("/usr/local/com/class1/settings/terma", "w");
                if (class == 2) FP = fopen("/usr/local/com/class2/settings/terma", "w");
                if (class == 3) FP = fopen("/usr/local/com/class3/settings/terma", "w");
                if (class == 4) FP = fopen("/usr/local/com/class4/settings/terma", "w");
                if (class == 5) FP = fopen("/usr/local/com/class5/settings/terma", "w");
                if (class == 6) FP = fopen("/usr/local/com/class6/settings/terma", "w");
                if (class == 7) FP = fopen("/usr/local/com/class7/settings/terma", "w");
                if (class == 8) FP = fopen("/usr/local/com/class8/settings/terma", "w");
                if (class == 9) FP = fopen("/usr/local/com/class9/settings/terma", "w");
                 
                fputs(terma, FP);
                fclose(FP); */
                for (b = 0; b < 9; b++)
                   {
                     t1[b] = t1[b] - 48;
                   }
                SDL_DestroyWindow(choices);
                detente();
                goto starthere;
                break;
                  
             /*  SET SECOND FACTO  RS  */
             case SDLK_k:
                repeat = 0;
                SDL_DestroyWindow(choices);
                security();
                if (panic == 1)
                  {
                    panic = 0;
                    goto starthere;
                  }
                 get_ten();
                for (b = 0; b < 9; b++)
                  {
                    t2[b] = (int)factor[b];
                    a = t2[b];
                    termb[b] = (char)(a);
                  }  
/*                 if (class == 1) FP = fopen("/usr/local/com/class1/settings/termb", "w");
                if (class == 2) FP = fopen("/usr/local/com/class2/settings/termb", "w");
                if (class == 3) FP = fopen("/usr/local/com/class3/settings/termb", "w");
                if (class == 4) FP = fopen("/usr/local/com/class4/settings/termb", "w");
                if (class == 5) FP = fopen("/usr/local/com/class5/settings/termb", "w");
                if (class == 6) FP = fopen("/usr/local/com/class6/settings/termb", "w");
                if (class == 7) FP = fopen("/usr/local/com/class7/settings/termb", "w");
                if (class == 8) FP = fopen("/usr/local/com/class8/settings/termb", "w");
                if (class == 9) FP = fopen("/usr/local/com/class9/settings/termb", "w");
                
                fputs(termb, FP);
                fclose(FP); */
                for (b = 0; b < 9; b++)
                   {
                     t2[b] = t2[b] - 48;
                   }
                SDL_DestroyWindow(choices);
                detente();


                goto starthere;
                break;
                
             /*  CHANGE ONE STUDENT'S TYPING LINE  */
             case SDLK_l:
                repeat = 0;
                SDL_DestroyWindow(choices);
                select_line_page();
                bail();
                goto starthere;
                break;
                 
             /*  SET BACK ALL STUDENTS' TYPING LINES  */
             case SDLK_m:
                if (repeat == 0)
                  {
                    SDL_DestroyWindow(choices);
                    security();
                    if (panic == 1)
                      {
                        panic = 0;
                        goto starthere;
                      }
                  }
                repeat = 1;
                for (e=0;  e<net_size; e++)
                  {
                    if (tline[e] > 1)  tline[e]=tline[e] - 1;
                  }      
                SDL_DestroyWindow(choices);
                goto starthere;
                break;
                
             /*  ERASE STUDENT WRITING  */
             case SDLK_n:
                repeat = 0;
                SDL_DestroyWindow(choices);
                security();
                if (panic == 1)
                  {
                    panic = 0;
                    goto starthere;
                  }
                clear_pro();
                goto starthere;
                break;
                
             /*  PRINT ONE STUDENT'S WRITING  */
             case SDLK_o:
                lengthflag = 0;
                repeat = 0;
                SDL_DestroyWindow(choices);
                select_board_page();
                if (panic == 1)
                  {
                    panic = 0;
                    goto starthere;
                  }
                print_pro();
                goto starthere;
                break;
                 
             /*  PRINT ALL STUDENTS' WRITING  */
             case SDLK_p:
                lengthflag = 1;
                repeat = 0;
                SDL_DestroyWindow(choices);
                for (e = 0;  e < net_size;  e++)
                  {
                     b = 66;
                     print_pro();
                     if (panic == 1)
                       {
                         panic = 0;
                         goto starthere;
                       }
                  }
                goto starthere;
                break;
                
             /*  READ ROSTER  */
             case SDLK_q:
                repeat = 0;
                SDL_DestroyWindow(choices);
                roster_page();
                goto starthere;
                break; 
                
             /*  CHANGE ROSTER  */
             case SDLK_r:
                repeat = 0;
                SDL_DestroyWindow(choices);
                security();
                if (panic == 1)
                   {
                     panic = 0;
                     goto starthere;
                   }
                changepage();

             for (e = 1; e < 27; e++)
               {
                 strcpy (Buf, "");
/*                  if (class == 1) strcpy (Buf, "/usr/local/com/class1/names/name");
                 if (class == 2) strcpy (Buf, "/usr/local/com/class2/names/name");
                 if (class == 3) strcpy (Buf, "/usr/local/com/class3/names/name");
                 if (class == 4) strcpy (Buf, "/usr/local/com/class4/names/name");
                 if (class == 5) strcpy (Buf, "/usr/local/com/class5/names/name");
                 if (class == 6) strcpy (Buf, "/usr/local/com/class6/names/name");
                 if (class == 7) strcpy (Buf, "/usr/local/com/class7/names/name");
                 if (class == 8) strcpy (Buf, "/usr/local/com/class8/names/name");
                 if (class == 9) strcpy (Buf, "/usr/local/com/class9/names/name");

               /* SAVE FIRST 26 NAMES */
                 Buf[32] = e + 96;
                 Buf[33] = 0;
                  
/*                  FP = fopen (Buf, "w");
                 fputs (roster[e], FP);
                 fclose (FP); */
               }

               /* SAVE LAST 14 NAMES */
             for (e = 27; e < 41; e++)
               {
                 strcpy (Buf, "");
/*                  if (class == 1) strcpy (Buf, "/usr/local/com/class1/names/name");
                 if (class == 2) strcpy (Buf, "/usr/local/com/class2/names/name");
                 if (class == 3) strcpy (Buf, "/usr/local/com/class3/names/name");
                 if (class == 4) strcpy (Buf, "/usr/local/com/class4/names/name");
                 if (class == 5) strcpy (Buf, "/usr/local/com/class5/names/name");
                 if (class == 6) strcpy (Buf, "/usr/local/com/class6/names/name");
                 if (class == 7) strcpy (Buf, "/usr/local/com/class7/names/name");
                 if (class == 8) strcpy (Buf, "/usr/local/com/class8/names/name");
                 if (class == 9) strcpy (Buf, "/usr/local/com/class9/names/name");

                 Buf[32] = e + 38;
                 Buf[33] = 0;
                  
                 FP = fopen (Buf, "w");
                 fputs (roster[e], FP);
                 fclose (FP); */
               }

             goto starthere;
             break;
                
             /*  SET TEACHER'S NAME  */
             case SDLK_s:
                repeat = 0;
                SDL_DestroyWindow(choices);
                security();
                if (panic == 1)
                  {
                    panic = 0;
                    goto starthere;
                  }
                fetch_letter();
                SDL_DestroyWindow(choices);
                goto starthere;
                break;
                          
             case SDLK_1:
                classnumber = 1;
                changeclass = 1;
                return classnumber;
                break;
                
             case SDLK_2:
                classnumber = 2;
                changeclass = 1;
                return classnumber;
                break;
             
             case SDLK_3:
                classnumber = 3;
                changeclass = 1;
                return classnumber;
                break;
              
             case SDLK_4:
                classnumber = 4;
                changeclass = 1;
                return classnumber;
                break;
                
             case SDLK_5:
                classnumber = 5;
                changeclass = 1;
                return classnumber;
                break;
              
             case SDLK_6:
                classnumber = 6;
                changeclass = 1;
                return classnumber;
                break;
             
             case SDLK_7:
                classnumber = 7;
                changeclass = 1;
                return classnumber;
                break;
              
             case SDLK_8:
                classnumber = 8;
                changeclass = 1;
                return classnumber;
                break;
              
             case SDLK_9:
                classnumber = 9;
                changeclass = 1;
                return classnumber;
                break;
             
             case SDLK_ESCAPE:
                SDL_DestroyWindow(choices);
                double_check();
                goto starthere;
                break;

             default:
                ;
           }
        }
    }
  return 0;
}

int
message(e, tt)
{
   TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );

      {
         ostraca = TTF_RenderText_Solid(fntVeraMono50, ttext[tt], hue[e]);
         SDL_BlitSurface(ostraca, NULL, chalk, &line[e]);
      }

   return 0;
}

int
multiply()
{
  char a;
  TTF_Font  *fntVeraMono33 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 33 );
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 40 );

                      /* OPEN EMPTY WHITE WINDOW. */

  slate = SDL_CreateWindow("mul",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

                      /* PUT net_size PROBLEMS ON THE SCREEN. */   

  for (e = 0; e < net_size; e++)
    {
      stop[e] = 0;
      goofed[e] = 0;

      for (b = 0; b < 20; b++) last_line[e][b] = ' ';

      renew[e] = 1;
      score[e]=0;
      make_mul_prob(e);

      last_line[e][2] = (char) factor1[e]+48;
      last_line[e][4] = 'X';    
      last_line[e][6] = (char) factor2[e]+48;    
      last_line[e][8] = '=';    

      for (c = 0; c < 20; c++)
        {
           if(net_size==40)ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], hue[e]);
           else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], hue[e]);
           SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][c]);
        }
      placeflag[e] = 1;
    }

  SDL_UpdateWindowSurface(slate);

                           /* START KEYBOARD POLLING. */
  while (1)
    {
      if (get_char() == 10)  /* CHECK TEACHER'S KEYBOARD FOR RETURN TO MENU. */
        {
          last_line[e][16] = ' ';
          last_line[e][17] = ' ';
          goaround = 0;
          SDL_DestroyWindow(slate);
          return 0;
        }
      for (e = 0; e < net_size; e++)  
        { 
          if (goofed[e] == 1)  /* END OR CONTINUE ERROR TIMEOUT PENALTY. */
            {
              if (time(t) - clk[e] > 1)
                {
                  goofed[e] = 0;
                  last_line[e][11] = ' ';
                  last_line[e][12] = ' ';
                   
                  if(net_size==40) lin1 = TTF_RenderText_Solid(fntVeraMono33, "X", white);
                  else lin1 = TTF_RenderText_Solid(fntVeraMono40, "X", white);
                  SDL_BlitSurface(lin1, NULL, chalk, &tilem[e][11]);
                  SDL_BlitSurface(lin1, NULL, chalk, &tilem[e][12]);
                  SDL_UpdateWindowSurface(slate);
                }
                
              else
                {       
                  get_event(e);       
                  continue;
                } 
            }   
                
          if (goofed[e] == 0)  /* GET STUDENT INPUT. */
            {
              a = (int) get_event(e);        
              if (a == 0) continue;
              if (a == 10) goto evaluatemul;
              if (stop[e] == 1) continue;
                 
              if (placeflag[e] == 1)
                 {       
                   if (((int)a < 48) || ((int)a > 57))  continue;
                   stuans[e] = (int) a - 48 ;
                   z[0] = (int) a;  
                   last_line[e][11] = (int) a;
                   placeflag[e] = 2;
                  
                   if(net_size==40)lettr = TTF_RenderText_Solid(fntVeraMono33, &z[0], hue[e]);
                   else lettr = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
                   SDL_BlitSurface(lettr, NULL, chalk, &tilem[e][11]);
                   SDL_UpdateWindowSurface(slate);
                  
                   continue;
                 }
                  
              if (placeflag[e] == 2)
                 {          
                   if (((int)a < 48) || ((int)a > 57)) continue;
                   z[0] = a;  
                   last_line[e][12] = (int) a;
                   stuans[e] = 10 * stuans[e] + (a - 48);
                    
                   if(net_size==40) lettr = TTF_RenderText_Solid(fntVeraMono33, &z[0], hue[e]);             
                   else lettr = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);             
                   SDL_BlitSurface(lettr, NULL, chalk, &tilem[e][12]);
                   SDL_UpdateWindowSurface(slate);
                   
                   stop[e] = 1;
                   continue;
                 }       
               continue;
            }
          continue;
                 
          evaluatemul:
          if (stuans[e] == answer[e])
            { 
              for (c = 0; c < 20; c++)
                {
                  if(net_size==40)hogwash = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], white);
                  else hogwash = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], white);
                  SDL_BlitSurface(hogwash, NULL, chalk, &tilem[e][c]);
                }
              SDL_UpdateWindowSurface(slate);
                 
              advance[e] = 1;
              score[e] = score[e] + mulpoints[e + 1];
              onesregister = score[e] % 10;
              tensregister = score[e] / 10;
              last_line[e][11] = 32;
              last_line[e][12] = 32;
              last_line[e][16] = (char) tensregister + 48;
              last_line[e][17] = (char) onesregister + 48;
              if(last_line[e][16] == 48){last_line[e][16] = 32;}
                  
              if(score[e] < win_score)
                {
                  stop[e] = 0;
                  placeflag[e] = 1;
                  make_mul_prob(e);
                  
                  last_line[e][1] = ' ';
                  last_line[e][2] = (char) factor1[e]+48;
                  last_line[e][4] = 'X';    
                  last_line[e][6] = (char) factor2[e]+48;    
                  last_line[e][8] = '=';    
                   
                  for (c = 0; c < 20; c++)
                    {
                      if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], hue[e]);
                      else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], hue[e]);
                      SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][c]);
                    }
                  placeflag[e] = 1;
                  SDL_UpdateWindowSurface(slate);
                }
              
              else
                {
                  last_line[e][16] = ' ';
                  last_line[e][17] = ' ';
                  SDL_DestroyWindow(slate);
                  win_page();
                  goaround = 1;
                  return 0;
                 } 
            }
             
          else if(stuans[e] != answer[e])
            {
              stop[e] = 0;
              placeflag[e] = 1;
              clk[e] = time(t);
              goofed[e] = 1;
              
              z[0] = last_line[e][11];
              if(net_size==40) correction = TTF_RenderText_Solid(fntVeraMono33, &z[0], white);
              else correction = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
              SDL_BlitSurface(correction, NULL, chalk, &tilem[e][11]);
              SDL_UpdateWindowSurface(slate);
              
              z[0] = last_line[e][12];
              if(net_size==40)correction = TTF_RenderText_Solid(fntVeraMono33, &z[0], white);
              else correction = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
              SDL_BlitSurface(correction, NULL, chalk, &tilem[e][12]);
              SDL_UpdateWindowSurface(slate);
               
              z[0] = 'X';
              last_line[e][11] = 'X';
              last_line[e][12] = 'X';
              
              if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, "X" , black);
              else ostraca = TTF_RenderText_Solid(fntVeraMono40, "X" , black);
              SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][11]);
              SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][12]);
              SDL_UpdateWindowSurface(slate);
            }
        }        
    }
  SDL_DestroyWindow(slate);
  return 0;          
}

int
opener()
{
	//  TODO: Make this work in Windows.
/*   for(e = 0; e <net_size;  e++)
    {
      fz[e] = open(BUF[e], O_RDONLY);
      ioctl(fz[e], EVIOCGRAB);
    } */
  return 0;
}

int
print_pro()
{
  int a = 0;
  int ones = 0;
  int tens = 0;

  for (a = 0; a < 4000; a++)
    {
      proof[a] = 0;
    }

if (comp[e-1][0] == 0) return 0;  

  for (a = 0; a < 4000; a++)
    {
      proof[a] = comp[e-1][a];
    }
  
  struct SDL_Rect slots[3];
        
  slots[0].x = 0;
  slots[0].y = 295;
       
  slots[1].x = 45;
  slots[1].y = 395;
      
  slots[2].x = 80;
  slots[2].y = 395;
  
  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );
            
                      /* PUT LINE LENGTH INTO INTEGER b. */
      
  if (lengthflag == 0)
    {
      slate = SDL_CreateWindow("hardcopy",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
      SDL_Surface* chalk = SDL_GetWindowSurface(slate);
      SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
      SDL_SetRenderDrawColor(rends, 255,255,255,255);
      SDL_RenderClear(rends);
      SDL_RenderPresent(rends);
      SDL_UpdateWindowSurface(slate);
              
      SDL_Surface *line = TTF_RenderText_Solid(fntVeraMono50, " ENTER LINE LENGTH (Between 10 and 70)", black);
      SDL_BlitSurface(line, NULL, chalk, &slots[0]);
      SDL_UpdateWindowSurface(slate); 
         
      z[1] = 0;
      
      tens = get_digit();
      if((tens < 1) || (tens > 7))
        {
          SDL_DestroyWindow(slate);
          return 0;
        }
      z[0] = (char) tens + 48;
      line = TTF_RenderText_Solid(fntVeraMono50, &z[0], black);
      SDL_BlitSurface(line, NULL, chalk, &slots[1]);
      SDL_UpdateWindowSurface(slate); 
      tens = tens * 10;
      
      ones = get_digit();
      z[0] = (char) ones + 48;
      line = TTF_RenderText_Solid(fntVeraMono50, &z[0], black);
      SDL_BlitSurface(line, NULL, chalk, &slots[2]);
      SDL_UpdateWindowSurface(slate); 
      b = tens + ones;
    }

     /* SET POINTER TO BEGINNING OF STUDENT e'S FILE. */

  text_pos[e] = 0;

     /* IF FILE IS MORE THAN ONE LINE LONG, DO THIS SECTION UNTIL YOU PASS THE END OF
     THE FILE. proof[b] WOULD BE THE FIRST CHARACTER OF THE NEXT LINE, BUT IF IT'S ZERO,
     SKIP THIS SECTION. */      
  
  if (proof[b] != 0)  
    {
      while (proof[text_pos[e]] != 0)
        { 
             /* ADVANCE TO THE FARTHEST POSSIBLE BEGINNING OF NEXT LINE. */
          
          text_pos[e] = text_pos[e] + b;
            
             /* NOW BACK UP THROUGH THE CURRENT LINE, CHECKING EACH CHARACTER FOR
             NEWLINE. */             
          
          for (c = 1; c <= b; c++)
            { 
              if (proof[text_pos[e] - c] == 10)
                {
                    /* THE NEWLINES WILL NOT PRINT A CHARACTER.  THE NEXT
                    CHARACTER WILL ACTUALLY START A NEW LINE, SO WE
                    ADVANCE A SPACE TO GET PAST THE NEWLINE, AND
                    THEN ADD b AGAIN TO REACH THE MAXIMUM BEGINNING OF
                    THE LINE AFTER THAT. */
                 
                  text_pos[e] = text_pos[e] - c + 1 + b;
                  break;
                }
            }
           
            /* WE MAY HAVE PASSED THE END OF THE FILE, SO WE BACK UP
            THROUGH THE CURRENT LINE, LOOKING FOR 0 OR A SPACE. */    
          
          for (c = 1; c < b; c++)
            {
              if (proof[text_pos[e] - c] == 0) break;  /*  back to while, then
                                                           forward to     */
              if (proof[text_pos[e] - c] == 32)
                { 
                   /* THIS WAS THE LAST SPACE ON THE CURRENT LINE, SO
                   IT IS THE PLACE TO DO A LINE-FEED.  WE SUBSTITUTE
                   NEWLINE FOR THE SPACE.  THEN WE ADVANCE A SPACE
                   TO LAND ON THE FIRST CHARACTER OF THE NEXT LINE. */
                   
                  proof[text_pos[e] - c] = (char)(10);
                  text_pos[e] = text_pos[e] - c + 1;
                  break;  /*  back to while  */
                }
            }  
         }
    }
	                    
    /* GO BACK THROUGH THE WHOLE FILE UNTIL YOU HIT A ZERO.  INSERT
    CR AND TAB CONTROL CHARACTERS AFTER EACH NEWLINE. */
  
  for (c = 0; c < 5000; c++)
    {
      if (proof[c] == 0)
        {
          break;
        }
      if (proof[c] == 10)
        {
          wedge(c);
          proof[c + 1] = 9;
          wedge(c);
          proof[c + 1] = 13;
        }
    }
      
    /* ADD A TAB AT THE BEGINNING OF THE FIRST LINE, TOO. */

  c = 0;
  wedge(c);
  proof[0] = 9;
  
    /* AND SKIP DOWN SOME LINES FOR A TOP MARGIN. */

  wedge(c);
  proof[0] = 10;
  wedge(c);
  proof[0] = 10;
  wedge(c);
  proof[0] = 10;
  wedge(c);
  proof[0] = 10;
      
    /* SAVE FORMATTED FILE AS 'output'. */

// TODO: Make this work in Windows.
/*   FP =  fopen("/usr/local/com/class1/files/output", "w");
  for(b = 0; proof[b]!= 0; b++)
    {
      fputc(proof[b], FP);  
    }
  fclose(FP); */

    /* GO PRINT output, THEN RETURN TO MENU. */

//  system("lpr   /usr/local/com/class1/files/output");
  return 0;
}

int
process()
{
  int b = 0;
  TTF_Font  *fntVeraMono30 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 30);
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 40);

  slate = SDL_CreateWindow("processor",0,0,1024,768,0);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

            /*  text_pos[] IS THE OFFSET FROM THE BEGINNING OF STUDENT COMPOSITION TO
            THE FIRST LETTER OF DISPLAYED LINE (displayline[][]).  cursor[] IS THE 
            OFFSET FROM text_pos[] TO THE CURRENT CHARACTER.

            COPY OPENING LINES OF net_size STUDENTS TO THE SCREEN. */

  for (e = 0; e < net_size; e++)
    {
      cursor[e] = 0;
      for (a = 0; a < 20; a++)
       {
         displayline[e][a] = comp[e][text_pos[e] + a];
       }      
          if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, "_", hue[e]);
          else sym = TTF_RenderText_Solid(fntVeraMono40, "_", hue[e]);
          SDL_BlitSurface(sym, NULL, chalk, &tile[e][0]);

          for (b = 0; b < 20; b++)
            {
              if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, &displayline[e][b] , hue[e]);
              else sym = TTF_RenderText_Solid(fntVeraMono40, &displayline[e][b] , hue[e]);
              SDL_BlitSurface(sym, NULL, chalk, &tile[e][b]);
            } 
    }   

  SDL_UpdateWindowSurface(slate);

  while (1)
   {         
       /*  GO BACK TO MAIN MENU IF MASTER KEYBOARD RETURNS NEWLINE. */ 

     if (get_char() == 10) 
        {
          SDL_DestroyWindow(slate);
          return 0;
        }       

       /* START POLLING KEYBOARDS. */

     for (e = 0; e < net_size; e++)
       {
         a = get_event(e);
         if ((int)a == 0) continue;              
         z[0] = a;
           
           /* MAKE SPACE FOR INSERTING A CHARACTER, OR... */
          
         if ((int)a == 129)   
           {
             if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, displayline[e], white);
             else sym = TTF_RenderText_Solid(fntVeraMono40, displayline[e], white);
              SDL_BlitSurface(sym, NULL, chalk, &line[e]);

              for (b = 4999; b > text_pos[e] + cursor[e]; b--)
                {
                  comp[e][b] = comp[e][b - 1];
                }
              comp[e][b] = (char)(32);
   
              for(b = 0; b < 20; b++)
                {
                  displayline[e][b] = comp[e][text_pos[e] + b];
                }
              
              if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, displayline[e], hue[e]);
              else sym = TTF_RenderText_Solid(fntVeraMono40, displayline[e], hue[e]);
              SDL_BlitSurface(sym, NULL, chalk, &line[e]);
              SDL_UpdateWindowSurface(slate);
           
              continue;  
           }
           
           /* DELETE A CHARACTER AND CLOSE SPACE WHERE IT WAS, OR .... */
     
         if (a == 127)
            {
              if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, displayline[e], white);
              else sym = TTF_RenderText_Solid(fntVeraMono40, displayline[e], white);
              SDL_BlitSurface(sym, NULL, chalk, &line[e]);

              for (a = text_pos[e] + cursor[e]; a < 4999; a++)
                {
                  comp[e][a] = comp[e][a + 1];
                }
              for (a = 0; a < 20; a++)
                {
                  displayline[e][a] = comp[e][text_pos[e] + a];
                }

              if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, displayline[e], hue[e]);
              else sym = TTF_RenderText_Solid(fntVeraMono40, displayline[e], hue[e]);
              SDL_BlitSurface(sym, NULL, chalk, &line[e]);
              SDL_UpdateWindowSurface(slate);

              continue;  
            }
           
           /* DO NOTHING, OR... */

         if (a == 124) continue; /* IGNORE PIPE SYMBOL FROM KEYBOARD. MOVE ON TO NEXT 'e'.
                                    SCROLL TEXT TO RIGHT ON LEFT CURSOR IF cursor[e]
                                    IS ZERO, OR... */               
         
         if ((cursor[e] == 0) && (text_pos[e] > 0) && (a == 130))
           {
             if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, displayline[e], white);
             else sym = TTF_RenderText_Solid(fntVeraMono40, displayline[e], white);
             SDL_BlitSurface(sym, NULL, chalk, &line[e]);

             text_pos[e]--;
             for (b = 0; b < 20; b++)
                {
                  displayline[e][b] = comp[e][text_pos[e] + b];
                } 
          
             if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, displayline[e], hue[e]);
             else sym = TTF_RenderText_Solid(fntVeraMono40, displayline[e], hue[e]);
             SDL_BlitSurface(sym, NULL, chalk, &line[e]);
             
             if(text_pos[e] == 0)
               {
                 if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, "_", hue[e]);
                 else sym = TTF_RenderText_Solid(fntVeraMono40, "_", hue[e]);
                 SDL_BlitSurface(sym, NULL, chalk, tile[e]);
               }
             SDL_UpdateWindowSurface(slate);
             
             continue;
           }
           
            /* MOVE CURSOR TO THE RIGHT, OR ... */
         
         if (a == 131)
           {
               /* STOP IF END OF FILE. */
             if (comp[e][text_pos[e] + cursor[e]] == 0) continue;
                       
               /* SCROLL TEXT TO LEFT IF AT END OF DISPLAYED LINE. */
             if (cursor[e] == 19)
               {
                 if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, displayline[e], white);
                 else sym = TTF_RenderText_Solid(fntVeraMono40, displayline[e], white);
                 SDL_BlitSurface(sym, NULL, chalk, &line[e]);
                
                 text_pos[e]++;
                 for (b = 0; b < 20; b++) 
                    {
                      displayline[e][b] = comp[e][text_pos[e] + b];
                    }
                 
                 if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, displayline[e], hue[e]);
                 else sym = TTF_RenderText_Solid(fntVeraMono40, displayline[e], hue[e]);
                 SDL_BlitSurface(sym, NULL, chalk, &line[e]);
                 SDL_UpdateWindowSurface(slate);
               
                 continue;
               }
               
               /* MOVE CURSOR ONE SPACE TO RIGHT IF NOT AT END OF DISPLAYED LINE. */
             else
               {
                 if(net_size==40) sym1 = TTF_RenderText_Solid(fntVeraMono30, "_", white);
                 else sym1 = TTF_RenderText_Solid(fntVeraMono40, "_", white);
                 SDL_BlitSurface(sym1, NULL, chalk, &tile[e][cursor[e]]);
                 cursor[e]++;
                 
                 if(net_size==40) sym2 = TTF_RenderText_Solid(fntVeraMono30, "_", hue[e]);
                 else sym2 = TTF_RenderText_Solid(fntVeraMono40, "_", hue[e]);
                 SDL_BlitSurface(sym2, NULL, chalk, &tile[e][cursor[e]]);         
                 SDL_UpdateWindowSurface(slate);
           
                 continue;
               }
           } 
           
            /* MOVE CURSOR LEFT, OR... */
           
         if ((a == 130) && (cursor[e] > 0))
           {
             z[0] = displayline[e][cursor[e]];              

                 if(net_size==40) sym2 = TTF_RenderText_Solid(fntVeraMono30, "_", white);
                 else sym2 = TTF_RenderText_Solid(fntVeraMono40, "_", white);
                 SDL_BlitSurface(sym2, NULL, chalk, &tile[e][cursor[e]]);         

             cursor[e]--;
                 if(net_size==40) sym2 = TTF_RenderText_Solid(fntVeraMono30, "_", hue[e]);
                 else sym2 = TTF_RenderText_Solid(fntVeraMono40, "_", hue[e]);
                 SDL_BlitSurface(sym2, NULL, chalk, &tile[e][cursor[e]]);         
             SDL_UpdateWindowSurface(slate);             
             
             continue;
           }

           /* PRINT INPUT CHARACTER FROM CURSOR 0 TO CURSOR 18, OR... */

         if ((a != 130) && (cursor[e] < 19))
           {
             z[0] = displayline[e][cursor[e]];
             if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, &z[0], white);
             else sym = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
             SDL_BlitSurface(sym, NULL, chalk, &tile[e][cursor[e]]);

             if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, "_", white);
             else sym = TTF_RenderText_Solid(fntVeraMono40, "_", white);
             SDL_BlitSurface(sym, NULL, chalk, &tile[e][cursor[e]]);
             z[0] = a;
             displayline[e][cursor[e]] = z[0];
             comp[e][text_pos[e] + cursor[e]] = z[0];

             if(net_size==40) sym = TTF_RenderText_Solid(fntVeraMono30, &z[0], hue[e]);
             else sym = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
             SDL_BlitSurface(sym, NULL, chalk, &tile[e][cursor[e]]);

             cursor[e]++;

             if(net_size==40) sym1 = TTF_RenderText_Solid(fntVeraMono30, "_", hue[e]);
             else sym1 = TTF_RenderText_Solid(fntVeraMono40, "_", hue[e]);
             SDL_BlitSurface(sym1, NULL, chalk, &tile[e][cursor[e]]);

             SDL_UpdateWindowSurface(slate);
             
             continue;
           }

            /* SCROLL TEXT TO LEFT AND PRINT INPUT CHARACTER IF CURSOR IS 19. */

         if (cursor[e] == 19)
           {
             if(net_size==40) scratch5 = TTF_RenderText_Solid(fntVeraMono30, displayline[e], white);
             else scratch5 = TTF_RenderText_Solid(fntVeraMono40, displayline[e], white);
             SDL_BlitSurface(scratch5, NULL, chalk, &line[e]);
             SDL_UpdateWindowSurface(slate);

             z[0] = a;
             displayline[e][cursor[e]] = z[0];
             comp[e][text_pos[e] + cursor[e]] = z[0];

             text_pos[e]++;

             for (b = 0; b < 20; b++) 
               {
                 displayline[e][b] = comp[e][text_pos[e] + b];
               }
               
             if(net_size==40) scratch5 = TTF_RenderText_Solid(fntVeraMono30, displayline[e], hue[e]);
             else scratch5 = TTF_RenderText_Solid(fntVeraMono40, displayline[e], hue[e]);
             SDL_BlitSurface(scratch5, NULL, chalk, &line[e]);

             if(displayline[e][19] == 0)
               {
                 if(net_size==40) scratch7 = TTF_RenderText_Solid(fntVeraMono30, "_", hue[e]);
                 else scratch7 = TTF_RenderText_Solid(fntVeraMono40, "_", hue[e]);
                 SDL_BlitSurface(scratch7, NULL, chalk, &tile[e][cursor[e]]);
               }

             else
               {
                 if(net_size==40) scratch7 = TTF_RenderText_Solid(fntVeraMono30, &displayline[e][cursor[e]], hue[e]);
                 else scratch7 = TTF_RenderText_Solid(fntVeraMono40, &displayline[e][cursor[e]], hue[e]);
                 SDL_BlitSurface(scratch7, NULL, chalk, &tile[e][cursor[e]]);
               }
           }
         SDL_UpdateWindowSurface(slate);
       }   
   }
  SDL_DestroyWindow(slate);
  return 0;
}

int
random_num()
{
   a = 0;
  a = rand() / 230000000;
  return a;
}

int
roster_page()
{
  TTF_Font  *fntVeraMono33 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 30);
  TTF_Font  *fntVeraMono20 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 20);

  slate = SDL_CreateWindow("add",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface *chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);
  
  for (c = 0;  c < 21; c++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[c][b].x = (b * 30) + 5;
            slots[c][b].y = (c * 32);  // y runs from 0 to 530
            slots[c][b].h = 45;
            slots[c][b].w = 30;
          }
     }

   for (c = 21;  c < 41; c++)
     {
       for (b = 0; b < 20; b++)
          {
            slots[c][b].x = (b * 30) + 520; // x's run from 520 to 1090
            slots[c][b].y = ((c - 20) * 32);
            slots[c][b].h = 45;
            slots[c][b].w = 30;
          }
     }

  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono20, "               TYPE  ADD  SUB  MUL DIV                      TYPE  ADD  SUB  MUL  DIV", hue[e]);
  SDL_BlitSurface(line0, NULL, chalk, slots[0]);
  
  char ttline[4];
  ttline[3] = 0; 
  
  char apoints[2];
  apoints[1] = 0;
  
  char spoints[2];
  spoints[1] = 0;
  
  char dpoints[2];
  dpoints[1] = 0;
   
  char mpoints[2];
  mpoints[1] = 0;
  for (e = 1; e < 21; e++)
    {  
      SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono33, roster[e],  hue[e]);
      SDL_BlitSurface(line1, NULL, chalk, slots[e]);

      ttline[0] = (char) ((tline[e-1] / 100) + 48);
      if(ttline[0] == '0') {ttline[0] = ' ';}
       
      ttline[1] = (char) ((tline[e-1] % 100) / 10) + 48;
      if((ttline[0] == ' ') & (ttline[1] == '0')) {ttline[1] = ' ';}
        
      ttline[2] = (char) (tline[e-1] % 10) + 48;
      
      apoints[0] =  (char) addpoints[e]+48;
      spoints[0] =  (char) subpoints[e]+48;
      mpoints[0] =  (char) mulpoints[e]+48;
      dpoints[0] =  (char) divpoints[e]+48;
      
      SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono33, &ttline[0],  hue[e]);
      SDL_BlitSurface(line2, NULL, chalk, &slots[e][5]);
      
      SDL_Surface *line3 = TTF_RenderText_Solid(fntVeraMono33, &apoints[0],  hue[e]);
      SDL_BlitSurface(line3, NULL, chalk, &slots[e][8]);

      SDL_Surface *line4 = TTF_RenderText_Solid(fntVeraMono33, &spoints[0],  hue[e]);
      SDL_BlitSurface(line4, NULL, chalk, &slots[e][10]);
      
      SDL_Surface *line5 = TTF_RenderText_Solid(fntVeraMono33, &mpoints[0],  hue[e]);
      SDL_BlitSurface(line5, NULL, chalk, &slots[e][12]);
    
      SDL_Surface *line6 = TTF_RenderText_Solid(fntVeraMono33, &dpoints[0],  hue[e]);
      SDL_BlitSurface(line6, NULL, chalk, &slots[e][14]);
    }

  for (e = 21; e < 41; e++)
    {  
      SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono33, roster[e],  hue[e]);
      SDL_BlitSurface(line1, NULL, chalk, slots[e]);

      ttline[0] = (char) ((tline[e-1] / 100) + 48);
      if(ttline[0] == '0') {ttline[0] = ' ';}
       
      ttline[1] = (char) ((tline[e-1] % 100) / 10) + 48;
      if((ttline[0] == ' ') & (ttline[1] == '0')) {ttline[1] = ' ';}
        
      ttline[2] = (char) (tline[e-1] % 10) + 48;
      
      apoints[0] =  (char) addpoints[e]+48;
      spoints[0] =  (char) subpoints[e-1]+48;
      mpoints[0] =  (char) mulpoints[e-1]+48;
      dpoints[0] =  (char) divpoints[e-1]+48;
      
      SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono33, &ttline[0],  hue[e]);
      SDL_BlitSurface(line2, NULL, chalk, &slots[e][6]);
      
      SDL_Surface *line3 = TTF_RenderText_Solid(fntVeraMono33, &apoints[0],  hue[e]);
      SDL_BlitSurface(line3, NULL, chalk, &slots[e][9]);

      SDL_Surface *line4 = TTF_RenderText_Solid(fntVeraMono33, &spoints[0],  hue[e]);
      SDL_BlitSurface(line4, NULL, chalk, &slots[e][11]);
      
      SDL_Surface *line5 = TTF_RenderText_Solid(fntVeraMono33, &mpoints[0],  hue[e]);
      SDL_BlitSurface(line5, NULL, chalk, &slots[e][13]);
    
      SDL_Surface *line6 = TTF_RenderText_Solid(fntVeraMono33, &dpoints[0],  hue[e]);
      SDL_BlitSurface(line6, NULL, chalk, &slots[e][15]);
   }

  SDL_UpdateWindowSurface(slate); 
  alpha();
  SDL_DestroyWindow(slate);
  return 0;
}

int
security()
{
  int a = 0;
  char s[5] = {0,0,0,0,0};
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 30);
  
  SDL_Window*  slate = SDL_CreateWindow("bumper",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
     
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends); 
  SDL_RenderPresent(rends);
     
  struct SDL_Rect slots[2];
    
  slots[0].x = 0;
  slots[0].y = 220;
  slots[0].h = 30;
      
  slots[1].x = 0;
  slots[1].y = 320;
  slots[1].h = 30;
    
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono40, "     ENTER YOUR PASSWORD", black);
  SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
  SDL_UpdateWindowSurface(slate);

  s[0] = (char) get_digit() + 48;
  if((s[0] < 48) || (s[0] > 57))
    {
      panic = 1;
      SDL_DestroyWindow(slate);
      return 0;
    }

  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono40, "*", black);
  SDL_BlitSurface(line1, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate);

  s[1] = (char) get_digit() + 48;
  if((s[1] < 48) || (s[1] > 57))
    {
      panic = 1;
      SDL_DestroyWindow(slate);
      return 0;
    }

  SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono40, "**", black);
  SDL_BlitSurface(line2, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate); 

  s[2] = (char) get_digit() + 48;
  if((s[2] < 48) || (s[2] > 57))
    {
      panic = 1;
      SDL_DestroyWindow(slate);
      return 0;
    }

  SDL_Surface *line3 = TTF_RenderText_Solid(fntVeraMono40, "***", black);
  SDL_BlitSurface(line3, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate); 

  s[3] = (char) get_digit() + 48;
  if((s[3] < 48) || (s[3] > 57))
    {
      panic = 1;
      SDL_DestroyWindow(slate);
      return 0;
    }

  SDL_Surface *line4 = TTF_RenderText_Solid(fntVeraMono40, "****", black);
  SDL_BlitSurface(line4, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate); 
  alpha();
     
  for (a = 0; a < 4; a++)
     if (s[a] != password[a])
        {
          SDL_DestroyWindow(slate);
          panic = 1;
          SDL_DestroyWindow(slate);
          return 0;
        }
  SDL_DestroyWindow(slate);
  return 0;
}

int
select_board_page()
{
  char proxy[3] = {0,0,0};
  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );

  slate = SDL_CreateWindow("selectboard",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);
     
  struct SDL_Rect slots[2];
      
  slots[0].x = 0;
  slots[0].y = 295;
      
  slots[1].x = 0;
  slots[1].y = 395;
        
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono50, "        WHICH KEYBOARD?", black);
  SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
  SDL_UpdateWindowSurface(slate);
     
  onesregister = get_digit();
  e = onesregister ;
  proxy[0]= onesregister + 48;
   
  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono50, proxy, black);
  SDL_BlitSurface(line1, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate);
       
  tensregister = get_digit();
  if(tensregister == 10)
    {
      SDL_DestroyWindow(slate);
      return e - 1;
    }
  e = 10 * e + tensregister;
  proxy[1]= tensregister + 48;
       
  SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono50, proxy, black);
  SDL_BlitSurface(line2, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate);

  alpha();
  SDL_DestroyWindow(slate);
  return e - 1;
   }

int
select_book()
{
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 40 );
    
  slate = SDL_CreateWindow("operation",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);
   
  struct SDL_Rect slots[6];
    
  slots[0].x = 0;
  slots[0].y = 200;
    
  slots[1].x = 0;
  slots[1].y = 320;
    
  slots[2].x = 0;
  slots[2].y = 380;
    
  slots[3].x = 0;
  slots[3].y = 440;
     
  slots[4].x = 0;
  slots[4].y = 500;
        
  slots[5].x = 0;
  slots[5].y = 560;
        
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono40, "      WHICH BOOK?", black);
  SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
             
  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono40, "         1 - Text 1", black);
  SDL_BlitSurface(line1, NULL, chalk, &slots[1]);
             
  SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono40, "         2 - Text 2", black);
  SDL_BlitSurface(line2, NULL, chalk, &slots[2]);
             
  SDL_Surface *line3 = TTF_RenderText_Solid(fntVeraMono40, "         3 - Text 3", black);
  SDL_BlitSurface(line3, NULL, chalk, &slots[3]);
            
  SDL_Surface *line4 = TTF_RenderText_Solid(fntVeraMono40, "         4 - Text 4", black);
  SDL_BlitSurface(line4, NULL, chalk, &slots[4]);
           
  SDL_UpdateWindowSurface(slate); 
        
  SDL_Event event;
     
  while(1)
    {
      SDL_PollEvent(&event);    
      if (event.type == SDL_KEYDOWN)
        {
// TODO: Make this work in Windows.
/*           switch(event.key.keysym.sym)
           {
             case SDLK_1:
             FP = fopen("/usr/local/etc/answerkeys/text1", "r");
             fgets(key, 40000, FP);
             fclose(FP);
             SDL_DestroyWindow(slate);
             return 0;
             break;
                  
             case SDLK_2:
             FP = fopen("/usr/local/etc/answerkeys/text2", "r");
             fgets(key, 40000, FP);
             fclose(FP);
             SDL_DestroyWindow(slate);
             return 0;
             break;
                  
             case SDLK_3:
             FP = fopen("/usr/local/etc/answerkeys/text3", "r");
             fgets(key, 40000, FP);
             fclose(FP);
             SDL_DestroyWindow(slate);
             return 0;
             break;
                  
             case SDLK_4:
             FP = fopen("/usr/local/etc/answerkeys/text4", "r");
             fgets(key, 40000, FP);
             fclose(FP);
             SDL_DestroyWindow(slate);
             return 0;
             break;
           } */
        }
    }
  SDL_DestroyWindow(slate);
  return 0;
}

int
select_line_page(e)
{
  char proxy[4] = {0,0,0,0};
  e = select_board_page(e);
  if(e > net_size) {return 0;}

  slate = SDL_CreateWindow("pickline",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

  TTF_Font  *fntVeraMono50 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 50 );
     
  struct SDL_Rect slots[2];
     
     slots[0].x = 0;
     slots[0].y = 295;
      
     slots[1].x = 0;
     slots[1].y = 395;
     
  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono50, "             WHICH LINE?", black);
  SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
  SDL_UpdateWindowSurface(slate);
     
  onesregister = get_digit();
  if((onesregister == 10)||(onesregister == 0))
    {
      SDL_DestroyWindow(slate);
      return 0;
    }
  tline[e] = onesregister ;
  proxy[0]= onesregister + 48;

  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono50, proxy, black);
  SDL_BlitSurface(line1, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate); 
     
  tensregister = get_digit();
  if(tensregister == 10)
    {
      SDL_DestroyWindow(slate);
      return 0;
    }
  tline[e] = 10 * tline[e] + tensregister ;
  proxy[1]= tensregister + 48;

  SDL_Surface *line2 = TTF_RenderText_Solid(fntVeraMono50, proxy, black);
  SDL_BlitSurface(line2, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate); 
      
  hundredsregister = get_digit();
  if(hundredsregister == 10)
    {
      SDL_DestroyWindow(slate);
      return 0;
    }
  tline[e] = 10 * tline[e] + hundredsregister ;
  if(tline[e] > 160)
    {
      tline[e] = 160;
      SDL_DestroyWindow(slate);
      return 0;
    }
  proxy[2]= hundredsregister + 48;

  SDL_Surface *line3 = TTF_RenderText_Solid(fntVeraMono50, proxy, black);
  SDL_BlitSurface(line3, NULL, chalk, &slots[1]);
  SDL_UpdateWindowSurface(slate); 
  alpha();
  SDL_DestroyWindow(slate);
  return e;
} 
     
int
splash_page()
{
  TTF_Font  *fntVeraMono90 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 90);
  TTF_Font  *fntVeraMono35 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 35);

  SDL_Window*  window = SDL_CreateWindow("bumper",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* display = SDL_GetWindowSurface(window);
  
  SDL_Renderer *rendw = SDL_CreateRenderer(window, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rendw, 255,255,255,255);
  SDL_RenderClear(rendw); 
  SDL_RenderPresent(rendw);

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(rendw, 1024,768);

  struct   SDL_Rect lines[6];
  lines[0].x = 160;
  lines[0].y = 200;
  lines[0].h = 50;
    
  lines[1].x = 305;
  lines[1].y = 400;
  lines[1].h = 30;
      
  lines[2].x = 230;
  lines[2].y = 450;
  lines[2].h = 30;
   
  lines[3].x = 260;
  lines[3].y = 500;
  lines[3].h = 30;
    
  lines[4].x = 170;
  lines[4].y = 550;
  lines[4].h = 30;
  
  lines[5].x = 230;
  lines[5].y = 600;
  lines[5].h = 30;
     
  SDL_Surface *liner1 = TTF_RenderText_Solid(fntVeraMono90, " CLASSNET-02 ", red);
  SDL_BlitSurface(liner1, NULL, display, &lines[0]);
     
  SDL_Surface *liner2 = TTF_RenderText_Solid(fntVeraMono35, "DISTRIBUTABLE UNDER", black);
  SDL_BlitSurface(liner2, NULL, display, &lines[1]);

  SDL_Surface *liner3 = TTF_RenderText_Solid(fntVeraMono35, "GNU GENERAL PUBLIC LICENSE", black);
  SDL_BlitSurface(liner3, NULL, display, &lines[2]);

  SDL_Surface *liner4 = TTF_RenderText_Solid(fntVeraMono35, "FREE SOFTWARE FOUNDATION", black);
  SDL_BlitSurface(liner4, NULL, display, &lines[3]);

  SDL_Surface *liner5 = TTF_RenderText_Solid(fntVeraMono35, "51 FRANKLIN STREET, FIFTH FLOOR", black);
  SDL_BlitSurface(liner5, NULL, display, &lines[4]);

  SDL_Surface *liner6 = TTF_RenderText_Solid(fntVeraMono35, "BOSTON, MA  02110-1301, USA", black);
  SDL_BlitSurface(liner6, NULL, display, &lines[5]);
      
  SDL_UpdateWindowSurface(window);
  SDL_Delay(1000);

  SDL_DestroyWindow(window);
  return 0;
}

int
subtract()
{
  char a;
  TTF_Font  *fntVeraMono33 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 33 );
  TTF_Font  *fntVeraMono40 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 40);

                          /* OPEN EMPTY WHITE WINDOW. */

  SDL_Window*  slate = SDL_CreateWindow("sub",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);
  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  SDL_UpdateWindowSurface(slate);

                          /* PUT net_size PROBLEMS ON THE SCREEN. */

  for (e = 0; e < net_size; e++)
    {
      for(b = 0; b < 20; b++) last_line[e][b] = ' ';
      stop[e] = 0;
      goofed[e] = 0;
      renew[e] = 1;
      score[e]=0;
      make_sub_prob(e);
     
      onesregister =  subtrahend[e] % 10;
      tensregister = subtrahend[e] / 10;
      last_line[e][2] =  onesregister + 48;
      last_line[e][1] = (char) tensregister + 48;
      last_line[e][4] = '-';
      last_line[e][6] = (char) minuend[e] + 48;
      last_line[e][8] = '=';
      last_line[e][11] = 32;
      if(last_line[e][1] == 48){last_line[e][1] = 32;}
     
      for (c = 0; c < 20; c++)
        {
          if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], hue[e]);
          else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], hue[e]);
          SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][c]);
        }
    }

  SDL_UpdateWindowSurface(slate);

                      /* START KEYBOARD POLLING. */

  while (1)
    {
      if (get_char() == 10)   /* CHECK TEACHER'S KEYBOARD FOR RETURN TO MENU. */
        {
          SDL_DestroyWindow(slate);
          goaround = 0;
          return 0;
        }
      for (e = 0; e < net_size; e++)
        {
          if (goofed[e] == 1)    /* END OR CONTINUE ERROR TIMEOUT PENALTY. */
            {
              if (time(t) - clk[e] > 3)
                {
                  goofed[e] = 0;
                  z[0] = last_line[e][11];
                  if(net_size==40) lin1 = TTF_RenderText_Solid(fntVeraMono33, &z[0], white);
                  else lin1 = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
                  SDL_BlitSurface(lin1, NULL, chalk, &tilem[e][11]);
                  SDL_UpdateWindowSurface(slate);
                }
              else
                {              
                  get_event(e);
                  continue;
                } 
            }
            
           if (goofed[e] == 0)   /* GET STUDENT INPUT. */
             {
               a = get_event(e);
               if (a == 0) continue;
               if (a == 10) goto evaluatesub;
               if (stop[e] == 1) continue;

               if (((int) a < 48) || ((int) a > 57)) continue;
               stuans[e] = (int) a - 48;
               z[0] = (int) a;
               last_line[e][11] = (int) a;
               
               if(net_size==40) lettr = TTF_RenderText_Solid(fntVeraMono33, &z[0], hue[e]);
               else lettr = TTF_RenderText_Solid(fntVeraMono40, &z[0], hue[e]);
               SDL_BlitSurface(lettr, NULL, chalk, &tilem[e][11]);
               SDL_UpdateWindowSurface(slate);

               stop[e] = 1;
               continue;
                                 /* EVALUATE ANSWERS. */	
               evaluatesub:
               if (stuans[e] == answer[e])
                 { 
                   for (c = 0; c < 20; c++)
                      {
                         if(net_size==40) hogwash = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], white);
                         else hogwash = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], white);
                         SDL_BlitSurface(hogwash, NULL, chalk, &tilem[e][c]);
                      }
                   
                   SDL_UpdateWindowSurface(slate);
                   
                   advance[e] = 1;
                   score[e] = score[e] + subpoints[e + 1];
                   onesregister = score[e] % 10;
                   tensregister = score[e] / 10;
                   last_line[e][15] = (char) onesregister + 48;
                   last_line[e][14] = (char) tensregister + 48;
                   last_line[e][11] = 32;
                   if(last_line[e][14] == 48){last_line[e][14] = 32;}

                                 /* ANSWER GOOD */

                   if(score[e] < win_score)
                      {
                        stop[e] = 0;
                        make_sub_prob(e);
                      
                        onesregister =  subtrahend[e] % 10;
                        tensregister = subtrahend[e] / 10;
                        last_line[e][2] =  onesregister + 48;
                        last_line[e][1] = (char) tensregister + 48;
                        last_line[e][4] = '-';
                        last_line[e][6] = (char) minuend[e] + 48;
                        last_line[e][8] = '=';
                        if(last_line[e][1] == 48){last_line[e][1] = 32;}
                      
                        for (c = 0; c < 20; c++)
                          {
                            if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][c], hue[e]);
                            else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][c], hue[e]);
                            SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][c]);
                          }
                          
                        SDL_UpdateWindowSurface(slate);
                      }

                   else
                      {
                        SDL_DestroyWindow(slate);
                        win_page();
                        goaround = 1;
                        return 0;
                      } 
                 }
             
                              /* ANSWER BAD */
                            
               else if(last_line[e][11] != answer[e])
                 {
                   clk[e] = time(t);
                   stop[e] = 0;
                   goofed[e] = 1;

                   z[0] = last_line[e][11];
                   if(net_size==40) correction = TTF_RenderText_Solid(fntVeraMono33, &z[0], white);
                   else correction = TTF_RenderText_Solid(fntVeraMono40, &z[0], white);
                   SDL_BlitSurface(correction, NULL, chalk, &tilem[e][11]);
                   SDL_UpdateWindowSurface(slate);

                   last_line[e][11] = 'X';
                   if(net_size==40) ostraca = TTF_RenderText_Solid(fntVeraMono33, &last_line[e][11], black);
                   else ostraca = TTF_RenderText_Solid(fntVeraMono40, &last_line[e][11], black);
                   SDL_BlitSurface(ostraca, NULL, chalk, &tilem[e][11]);
                   SDL_UpdateWindowSurface(slate);
                 }
             }
        }        
    } 
  SDL_DestroyWindow(slate);
  return 0;          
}

int
wedge(int c)
{
  int a = 0;
  for (a = 4999; a > c; a--) proof[a] = proof[a - 1];
  return 0;
}

int
win_page()
{
	
  TTF_Font  *fntVeraMono110 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 110);
  TTF_Font  *fntVeraMono45 = TTF_OpenFont( "C:/Windows/Fonts/VeraMono.ttf", 45);

  SDL_Window*  slate = SDL_CreateWindow("uwin",0,0,1024,768,SDL_WINDOW_FULLSCREEN_DESKTOP);
  SDL_Surface* chalk = SDL_GetWindowSurface(slate);

  SDL_Renderer *rends = SDL_CreateRenderer(slate, -1,SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(rends, 255,255,255,255);
  SDL_RenderClear(rends);
  SDL_RenderPresent(rends);
  
  struct SDL_Rect slots[2];
     
  slots[0].x = 0;
  slots[0].y = 225;

  slots[1].x = 100;
  slots[1].y = 400;      

  SDL_Surface *line0 = TTF_RenderText_Solid(fntVeraMono45, " THE WINNER IS:  ", black);
  SDL_BlitSurface(line0, NULL, chalk, &slots[0]);
     
  SDL_Surface *line1 = TTF_RenderText_Solid(fntVeraMono110, roster[e+1], red);
  SDL_BlitSurface(line1, NULL, chalk, &slots[1]);

  SDL_UpdateWindowSurface(slate);

  SDL_Delay(10000);
        
  score[e] = 0;
  advance[e] = 1;
  SDL_DestroyWindow(slate);
  return 0;
}
