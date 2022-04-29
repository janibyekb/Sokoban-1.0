# Sokoban-1.0
Built Sokoban Game with C Programming Language and its library SDL-1.0.

Developer’s manual:
In this documentation, I intend to write about detailed developer’s manual of my program – Sokoban game. In other words, output of program, data structures, algorithms and some important list of functions will be explained.
The main goal of this program is to provide a customer with user-friendly and advanced or new features of Sokoban Game. Therefore, C programming language and its library- SDL 1.2 are used to program this game in terms of such reasons. Moreover, this program composed of three major parts, such as Game Board, Controls and Hall of Fame. Thus, solutions and algorithms used follows in this order.
Firstly, the Game board is the map arrangement of the game. To illustrate, program reads the room definition of the game from the text file and draws it on the screen. In this stage, data structure plays huge role and thus 2D dynamic memory is used to store the particular position of each items. In addition, enumerated data type is highly used in order to identify each items of room description. And this stored data in 2D array will be the main key of game process and controls.
In this process:
So, called:
-	Pieces** identify_map(FILE *map);
-	void set_position(Pieces **room, FILE *src);
two major functions are utilized to analyse and read the room 
description and store the position of each items to 2D dynamic
memory. Afterthat, in order to show or draw the map on the 
console screen, 2 small functions play huge roles.
-	void draw_pieces(SDL_Surface *source_img, SDL_Surface *dest_img, Pieces what, int x, int y);
-	void draw_room(Pieces **room, SDL_Surface *source_img, SDL_Surface *dest_img);
However, before the drawing of the map or even after, in terms of screen management, so called:
-	void display(Pieces **room, SDL_Surface *pieces, SDL_Surface *screen, char *filename);
function is utilized and will be utilized many times to manage and display every event on program to the screen for entire running time of program.

![image](https://user-images.githubusercontent.com/101016194/165882613-3b6c2792-bd29-4f82-a906-ea144d7a2803.png)                                 ![image](https://user-images.githubusercontent.com/101016194/165882624-62093a1b-0992-4207-8611-a68ff3cc32b2.png)

Secondly, the most significant section of this program is Controls. In this stage, keyboard handling controls will mostly be used to change the position of the player in order to accomplish the game tasks, and control the program itself. The user can change the position of player to the directions of up, down, left, right by pressing AWSD keys or up, down, left, right changing keyboard keys. And move_up(), move.left(), move_right(), move_left() functions are used inside of SDL event handling function. For example,         
switch(event.type){ 
case SDL_KEYDOWN:
 if(event.key.keysym.sym==SDLK_RIGHT || event.key.keysym.sym==SDLK_d){
                          	  move_right(room);
                    	        break;
                   	     }
In addition, in regards of controlling the characteristic of the game, such as newgame, restart, next_map and quit, several key functions are also used: 
Restarting the game:  restart(room, room1, &x,&y,&move);
Next_map: changing to next map  create_map is mainly used;
New_game – it restarts the map again but asks the username of the customer or player again. In this term, get_playername(username,font, screen) function is used to display new screen and get input of the user.

![image](https://user-images.githubusercontent.com/101016194/165882649-3c3c69f5-c7a6-4413-bddc-52e202856e60.png)

Finally, it brings us to the Hall of Fame stage. the board of Hall of fame is situated on the right side of the game board and it shows the names of the player and their stats by estimating how many steps it took him or her to complete the map. In this stage, function called
-	check_win(room, pieces, font, screen); constantly checks the game position data after every move of player, whether there is no target places left or not as a game rule.
And if player successfully completes the particular map after this checking, the name of the player and the needed number of moves to accomplish the map to him/her are stored in text file instead of database by storing_data(filename, username, move) function;
To conclude, this program is designed to be user-friendly and accommodate the exact needs of customers. And it is noteworthy fact that the functions used in this program can advanced to make user interface better. 

