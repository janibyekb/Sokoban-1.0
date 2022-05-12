# Sokoban-1.0
Built Sokoban Game with C Programming Language and its library SDL-1.0.

Developer’s manual:
In this documentation, I intend to write about detailed developer’s manual of my program – Sokoban game. In other words, output of program, data structures, algorithms and some important list of functions will be explained.
The main goal of this program is to provide a customer with user-friendly and advanced or new features of Sokoban Game. Therefore, C programming language and its library- SDL 1.2 are used to program this game in terms of such reasons. Moreover, this program composed of three major parts, such as Game Board, Controls and Hall of Fame. Thus, solutions and algorithms used follows in this order.
Firstly, the Game board is the map arrangement of the game. To illustrate, program reads the room definition of the game from the text file and draws it on the screen. In this stage, data structure plays huge role and thus 2D dynamic memory is used to store the particular position of each items. In addition, enumerated data type is highly used in order to identify each items of room description. And this stored data in 2D array will be the main key of game process and controls.
In this process:
So, called:
  - Pieces** identify_map(FILE *map); ![image](https://user-images.githubusercontent.com/101016194/167969315-da70a810-0dd9-4aa1-86e1-46c41709dc5e.png)
  - void set_position(Pieces **room, FILE *src);
  
two major functions are utilized to analyse and read the room
description and store the position of each items to 2D dynamic
memory. Afterthat, in order to show or draw the map on the
console screen, 2 small functions play huge roles.

  - void draw_pieces(SDL_Surface *source_img, SDL_Surface *dest_img, Pieces what, int x, int y);
  - void draw_room(Pieces **room, SDL_Surface *source_img, SDL_Surface *dest_img);
However, before the drawing of the map or even after, in terms of screen management, so called:
  - void display(Pieces **room, SDL_Surface *pieces, SDL_Surface *screen, char *filename);
function is utilized and will be utilized many times to manage and display every event on program to the screen for entire running time of program.
Secondly, the most significant section of this program is Controls. In this stage, keyboard handling controls will mostly be used to change the position of the player in order to accomplish the game tasks, and control the program itself. The user can change the position of player to the directions of up, down, left, right by pressing AWSD keys or up, down, left, right changing keyboard keys. And move_up(), move.left(), move_right(), move_left() functions are used inside of SDL event handling function. For example,
switch(event.type){
  case SDL_KEYDOWN:
    if(event.key.keysym.sym==SDLK_RIGHT || event.key.keysym.sym==SDLK_d){
        move_right(room);
          break;
      }
![image](https://user-images.githubusercontent.com/101016194/167969392-90ded46d-e1b1-4d3c-975e-fac8ecd21845.png)

In addition, in regards of controlling the characteristic of the game, such as newgame, restart, next_map and quit, several key functions are also used:
Restarting the game: restart(room, room1, &x,&y,&move);
Next_map: changing to next map → create_map is mainly used;
New_game – it restarts the map again but asks the username of the customer or player again. In this term, get_playername(username,font, screen) function is used to display new screen and get input of the user.
