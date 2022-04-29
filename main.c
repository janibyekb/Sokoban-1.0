#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>


int height, width, X_POSITION, Y_POSITION;
enum {SIZE = 52, FRAME=26};

/* calculates the window coordinate for a square which is portioned parts of the room */
float sq2winpos(float coord) {
    return SIZE * coord + FRAME;
}

typedef enum Pieces{
    Empty=-1,
    Wall, Box, Player, Target, Done, Player_1, /* Player_1 for checking that if Player is on the position of Target*/

} Pieces;

/*identify map, and creating dynamic memory corresponding to map's height and width*/
Pieces** identify_map(FILE *map){
    rewind(map);
    int x=0, y=0;char p;
    while(!feof(map)){
        fscanf(map,"%c", &p);
        y++;
        if(p=='\n'){
            x++;
            y=0;
        }
    }
    height = x+1;
    width = y-1;
    Pieces **array;
    array = malloc(height*sizeof(Pieces));
    if(array == NULL){
		fprintf(stderr, "out of memory\n");

    }

    for(int i=0; i< height; i++){
        array[i]=malloc(width*sizeof(Pieces));
    }

    return array;

}


void set_position(Pieces **room, FILE *src){
    int x=0, y=0;
    char c;
    rewind(src);
    while(!(feof(src))){
        fscanf(src,"%c", &c);
        switch(c){
            case '-':
                room[x][y] = Empty; break;
            case '#':
                room[x][y] = Wall; break;
            case '@':
                room[x][y] = Player; break;
            case 'x':
                room[x][y] = Target; break;
            case 'b':
                room[x][y] = Box; break;
            case '0':
                room[x][y] = Done; break;
            default:
                room[x][y] = Empty;break;
        }
        y++;
        if(c=='\n'){
            x++;
            y=0;
        }
    }

}

void draw_pieces(SDL_Surface *source_img, SDL_Surface *dest_img, Pieces what, int x, int y) {
    /* copy part from source image, at these coordinates, of this size */
    SDL_Rect src = { (what % 6) * 62 + 10, (what / 6) * 60 + 10, SIZE, SIZE };

    /* to destination at these coordinates */
    SDL_Rect dest = { (x+1)*SIZE + FRAME, (y+0.5)*SIZE + FRAME*2,1, 1 };

    if (what == Empty)
        return;
    /* copy or draw portion of image on the screen */
    SDL_BlitSurface(source_img, &src, dest_img, &dest);
}

void draw_room(Pieces **room, SDL_Surface *source_img, SDL_Surface *dest_img) {
    int x, y;

    /* fill the whole area */
    boxColor(dest_img, 0, 0, dest_img->w - 1, dest_img->h - 1, 0xCCAD99FF);
    //rectangleColor(dest_img, sq2winpos(0), sq2winpos(0), sq2winpos(width), sq2winpos(height), 0xE6D1C3FF);
    /* draw the squares */
    for (x = 0; x < height; x++)
        for (y = 0; y < width; y++){
            draw_pieces(source_img, dest_img, room[x][y], y, x);
            if(room[x][y]==Player || room[x][y]==Player_1){
                X_POSITION = x;
                Y_POSITION = y;
            }
        }
}

void move_right(Pieces **room){
    int x=X_POSITION;
    int y=Y_POSITION, n=0;
    if(room[x][y]==Player_1)/* checking that if Player is on the position of Target*/
        n=1;

    switch(room[x][y+1]){
        case Empty:
            room[x][y]=Empty;
            room[x][y+1]=Player;break;
        case Wall:
            n=0;
            break;
        case Box:
            if(room[x][y+2]!=Wall && room[x][y+2]!=Box && room[x][y+2]!=Done){
                room[x][y]=Empty;
                room[x][y+1]=Player;
                if(room[x][y+2]==Target)
                    room[x][y+2]=Done;
                else
                    room[x][y+2]=Box;
                break;
            }
            else{
                n=0;
                break;
            }
        case Target:
            room[x][y]=Empty;
            room[x][y+1]=Player_1;
            break;
        case Done:
            if(room[x][y+2]!=Wall && room[x][y+2]!=Box && room[x][y+2]!=Done){
                room[x][y]=Empty;
                room[x][y+1]=Player_1;
                if(room[x][y+2]==Target)
                    room[x][y+2]=Done;
                else
                    room[x][y+2]=Box;
                break;
            }
            else{
                n=0;
                break;
            }
        default:
            break;
    }
    if(n==1)
        room[x][y]=Target; /*setting the position of Target when Player moved from the position of Target */
}
void move_left(Pieces **room){
    int x=X_POSITION;
    int y=Y_POSITION, n=0;
    if(room[x][y]==Player_1)
        n=1;
    switch(room[x][y-1]){
        case Empty:
            room[x][y]=Empty;
            room[x][y-1]=Player;break;
        case Wall:
            n=0;
            break;
        case Box:
            if(room[x][y-2]!=Wall && room[x][y-2]!=Box && room[x][y-2]!=Done){
                room[x][y]=Empty;
                room[x][y-1]=Player;
                if(room[x][y-2]==Target)
                    room[x][y-2]=Done;
                else
                    room[x][y-2]=Box;
                break;
            }
            else{
                n=0;
                break;
            }
        case Target:
            room[x][y]=Empty;
            room[x][y-1]=Player_1;
            break;
        case Done:
            if(room[x][y-2]!=Wall && room[x][y-2]!=Box && room[x][y-2]!=Done){
                room[x][y]=Empty;
                room[x][y-1]=Player_1;
                if(room[x][y-2]==Target)
                    room[x][y-2]=Done;
                else
                    room[x][y-2]=Box;
                break;
            }
            else{
                n=0;
                break;
            }
        default:
            break;
    }
    if(n==1)
        room[x][y]=Target;
}
void move_down(Pieces **room){
    int x=X_POSITION;
    int y=Y_POSITION,n=0;
    if(room[x][y]==Player_1)
        n=1;

    switch(room[x+1][y]){
        case Empty:
            room[x][y]=Empty;
            room[x+1][y]=Player;break;
        case Wall:
            n=0; break;
        case Box:
            if(room[x+2][y]!=Wall && room[x+2][y]!=Box && room[x+2][y]!=Done){
                room[x][y]=Empty;
                room[x+1][y]=Player;
                if(room[x+2][y]==Target)
                    room[x+2][y]=Done;
                else
                    room[x+2][y]=Box;
                break;
            }
            else{
                n=0;
                break;
            }
        case Target:
            room[x][y]=Empty;
            room[x+1][y]=Player_1;
            break;
        case Done:
            if(room[x+2][y]!=Wall && room[x+2][y]!=Box && room[x+2][y]!=Done){
                room[x][y]=Empty;
                room[x+1][y]=Player_1;
                if(room[x+2][y]==Target)
                    room[x+2][y]=Done;
                else
                    room[x+2][y]=Box;
                break;
            }
            else{
                n=0;
                break;
            }
        default:
            break;
    }
    if(n==1)
        room[x][y]=Target;
}
void move_up(Pieces **room){
    int x=X_POSITION;
    int y=Y_POSITION, n=0;
    if(room[x][y]==Player_1)
        n=1;

    switch(room[x-1][y]){
        case Empty:
            room[x][y]=Empty;
            room[x-1][y]=Player;break;
        case Wall:
            n=0;break;
        case Box:
            if(room[x-2][y]!=Wall && room[x-2][y]!=Box && room[x-2][y]!=Done){
                room[x][y]=Empty;
                room[x-1][y]=Player;
                if(room[x-2][y]==Target)
                    room[x-2][y]=Done;
                else
                    room[x-2][y]=Box;
                break;
            }
            else{
                n=0;
                break;
            }
        case Target:
            room[x][y]=Empty;
            room[x-1][y]=Player_1;
            break;
        case Done:
            if(room[x-2][y]!=Wall && room[x-2][y]!=Box && room[x-2][y]!=Done){
                room[x][y]=Empty;
                room[x-1][y]=Player_1;
                if(room[x-2][y]==Target)
                    room[x-2][y]=Done;
                else
                    room[x-2][y]=Box;
                break;
            }
            else{
                n=0;
                break;
            }
        default:
            break;
    }
    if(n==1)
        room[x][y]=Target;

}
int check_win(Pieces **room, SDL_Surface *pieces, TTF_Font *font,SDL_Surface *screen){
    /*check the the game if all the boxes are located on the target position*/
    SDL_Surface *textimg;
    SDL_Color white = {0, 255, 0};
    SDL_Rect where_to={sq2winpos(height/3),sq2winpos(width/2),400,80};
    int i, j, p=0;
    for(i=0;i<height; i++){
        for(j=0; j<width; j++){
            if(room[i][j]==Target)
                p++;
            if(room[i][j]==Player_1)
                p++;
        }
    }
    if(p==0){
        textimg = TTF_RenderUTF8_Solid(font, "YOU WON!!!", white);
        /* copy textimg to screen */
        SDL_BlitSurface(textimg, NULL, screen, &where_to);
        SDL_FreeSurface(textimg);
        SDL_Flip(screen);
        return 1;
    }
    return 0;
}


/*Screen management: displaying the things on the screen*/
void display(Pieces **room, SDL_Surface *pieces, SDL_Surface *screen, char *filename){
    TTF_Font *font;
    FILE *stats;
    SDL_Surface *textimg,*arrow,*text_label, *text_name, *text_move, *text_label1;

    draw_room(room, pieces, screen);

    SDL_Color white = {255, 255, 255};
    SDL_Rect where_to={sq2winpos(0),sq2winpos(0),400,80};
    SDL_Rect where_to1={sq2winpos(width+1.5),sq2winpos(height)+FRAME/2,400,80};


    stats = fopen(filename, "r");
    if(stats==NULL)
        return;

    float n=2;
    char name[100]; int move;
    char move_char[100];

    TTF_Init();
    font = TTF_OpenFont("fonts/liberationserif-regular.ttf", 24);
    if (!font) {
        fprintf(stderr, "The font could not be opened! %s\n", TTF_GetError());
        exit(1);
    }
    textimg = TTF_RenderUTF8_Solid(font, "New Game[N]   Restart[R]   Next Map[M]   Quit[Q]", white);
    /* copy textimg to screen */
    SDL_BlitSurface(textimg, NULL, screen, &where_to);
    /* copy textimg to screen */
    arrow = IMG_Load("assets/arrow.png");
    SDL_BlitSurface(arrow, NULL, screen, &where_to1);

    SDL_Rect dest0={sq2winpos(width+1.5), sq2winpos(0.7),400,80};
    SDL_Rect dest1={sq2winpos(width+1.5), sq2winpos(1.3),400,80};

    text_label = TTF_RenderUTF8_Solid(font, "Stats on this map:", white);
    SDL_BlitSurface(text_label, NULL, screen, &dest0);

    text_label1 = TTF_RenderUTF8_Solid(font, "Name         Moves", white);
    SDL_BlitSurface(text_label1, NULL, screen, &dest1);

    while(fscanf(stats, "%d, %s", &move, name)!=EOF){
        text_name = TTF_RenderUTF8_Solid(font, name, white);
        sprintf(move_char,"%d", move);
        text_move = TTF_RenderUTF8_Solid(font, move_char, white);
        SDL_Rect dest_name={sq2winpos(width+1.5), sq2winpos(n),400,80}, dest_move={sq2winpos(width+4), sq2winpos(n),400,80};
        SDL_BlitSurface(text_name, NULL, screen, &dest_name);
        SDL_BlitSurface(text_move, NULL, screen, &dest_move);
        n=n+0.5;
    }


    SDL_Flip(screen);
    fclose(stats);
}

void get_playername(char *dest,TTF_Font *font, SDL_Surface *screen) {


    int x=70,y=120,w=400,h=40;
    SDL_Rect source = { 0, 0, w, h}, dest_rect = { x, y, w, h }, dest1 = {x, 80, w, h};
    SDL_Color textcol = {255, 255, 255}, bkgrnd = { 0, 0, 0 };
    SDL_Surface *textimg, *label;
    SDL_Event event;

    int len = 0;
    dest[len] = 0x0000;   /* terminating 0 */
    SDL_EnableUNICODE(1);
    bool enter = false;
    bool quit = false;




    while (!quit && !enter) {
        /* draw text */
        SDL_FreeSurface(screen);
        boxColor(screen, 0, 0, screen->w - 1, screen->h - 1, 0xCCAD99FF);

        boxRGBA(screen, x, y, x + w - 1, y + h - 1, bkgrnd.r, bkgrnd.g, bkgrnd.b, 255);
        label = TTF_RenderUTF8_Solid(font, "Player Name:", textcol);
        SDL_BlitSurface(label, NULL, screen, &dest1);

        textimg = TTF_RenderUTF8_Solid(font, dest, textcol);
        SDL_BlitSurface(textimg, &source, screen, &dest_rect);

        SDL_FreeSurface(textimg);

        SDL_Flip(screen);
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.unicode) {
                    case 0x0000:
                        /* no such character (like shift key) */
                        break;
                    case '\r':
                    case '\n':
                        /* enter: end of entry */
                        enter = true;
                        break;
                    case ' ':
                        break;
                    case '\b':
                        /* backspace: delete backwards */
                        if (len > 0)
                            dest[--len] = 0x0000;
                        break;
                    default:
                        if(len<14){
                        /* character: put into the array and terminate */
                        dest[len++] = event.key.keysym.unicode;
                        dest[len] = 0x0000;
                        }
                        break;
                    }
                break;
            case SDL_QUIT:
                /* put it back into the event queue, as
                 * nothing can we do with it. */
                SDL_PushEvent(&event);
                quit = true;
                break;
        }
    }
}

void restart(Pieces **room, Pieces **room1, int *x,int *y,int *move){

    for(int i=0; i<height; i++){
        for(int k=0;k<width; k++){
            room[i][k]=room1[i][k];
            if(room[i][k]==Player || room[i][k]==Player_1){
                *x=i; *y=k; //adjucting the position of player to current x, y position variables
            }
        }
    }
    *move=0;
}

void storing_data(char *filename, char *text, int move){ /*appending data of winner player in text file*/
    FILE *fp;

    fp = fopen(filename, "a");
    if(fp==NULL)
        return;

    fprintf(fp, "%d, %s\n", move, text);

    fclose(fp);

}

Pieces** create_map(FILE *map){
    Pieces **room;
    room = identify_map(map);
    set_position(room, map);

    return room;
}


int main(int argc, char *argv[]) {

    SDL_Event event;
    SDL_Surface *screen;
    SDL_Surface *pieces;
    FILE *map1,*map2,*map3;
    Pieces **room, **room1;
    TTF_Font *font;

    char username[100];
    char filename[100] = "maps/stats_map1.txt";
    int map_number=1;

    map1=fopen("maps/map1.txt","r");
    if(map1==NULL)
    {
        printf("cant open file!");
        return 1;
    }
    map2 = fopen("maps/map2.txt","r");
    map3 = fopen("maps/map3.txt","r");

    room = create_map(map1);
    room1 = create_map(map1);/*storing copy of the room description which will be needed to restart the game map*/



    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    screen = SDL_SetVideoMode(SIZE*(width+5)+FRAME*2,SIZE*(height+2)+FRAME*2, 0, SDL_ANYFORMAT);
    if (!screen) {
        fprintf(stderr, "The window couldnt be opened\n");
        exit(1);
    }

    SDL_WM_SetCaption("Sokoban 2.0", "Sokoban");
    pieces = IMG_Load("assets/items.png");
    if(!pieces){
        fprintf(stderr, "The file could not be opened!\n");
        exit(1);
    }
    TTF_Init();
    font = TTF_OpenFont("ARLRDBD.ttf", 32);
    if (!font) {
        fprintf(stderr, "The font could not be opened! %s\n", TTF_GetError());
        exit(1);
    }


    get_playername(username, font, screen);

    display(room, pieces, screen, filename);

    int move=0;


    while(event.type != SDL_QUIT){
            int x, y;
            x=X_POSITION; y=Y_POSITION;

            while(SDL_PollEvent(&event)){
                switch(event.type){
                    case SDL_KEYDOWN:
                        if(event.key.keysym.sym==SDLK_RIGHT || event.key.keysym.sym==SDLK_d){
                            move_right(room);
                            break;
                        }
                        if(event.key.keysym.sym==SDLK_LEFT || event.key.keysym.sym==SDLK_a){
                            move_left(room);
                            break;
                        }
                        if(event.key.keysym.sym==SDLK_DOWN || event.key.keysym.sym==SDLK_s){
                            move_down(room);
                            break;
                        }
                        if(event.key.keysym.sym==SDLK_UP || event.key.keysym.sym==SDLK_w){
                            move_up(room);
                            break;
                        }
                        /*restart the game*/
                        if(event.key.keysym.sym==SDLK_r){
                            restart(room, room1, &x,&y,&move);
                            break;
                        }
                        if(event.key.keysym.sym==SDLK_n){
                            get_playername(username,font, screen);
                            restart(room, room1,&x,&y,&move);
                        }
                        if(event.key.keysym.sym==SDLK_q){
                            event.type=SDL_QUIT;
                        }
                        if(event.key.keysym.sym==SDLK_m){
                            map_number++;
                            if(map_number==1){
                                room = create_map(map1);
                                room1 = create_map(map1);
                                strcpy(filename, "maps/stats_map1.txt");
                            }
                            if(map_number==2){
                                room = create_map(map2);
                                room1 = create_map(map2);
                                strcpy(filename, "maps/stats_map2.txt");
                            }
                            if(map_number==3){
                                room = create_map(map3);
                                room1 = create_map(map3);
                                strcpy(filename, "maps/stats_map3.txt");
                                map_number=map_number-3;
                            }
                            SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
                            screen = SDL_SetVideoMode(SIZE*(width+5)+FRAME*2,SIZE*(height+2)+FRAME*2, 0, SDL_ANYFORMAT);
                            if (!screen) {
                                fprintf(stderr, "The window couldnt be opened\n");
                                exit(1);
                            }
                        }
                    default:
                        break;
                }
                display(room, pieces, screen, filename);
                if(x!=X_POSITION || y!=Y_POSITION){
                    move++;
                }

            }
            if(check_win(room, pieces, font, screen)){

                storing_data(filename, username, move);
                restart(room,room1,&x,&y,&move);
            }

    }


    SDL_FreeSurface(pieces);
    fclose(map1);
    fclose(map2);
    fclose(map3);

    free(room);
    free(room1);
    SDL_Quit();

    return 0;
}
