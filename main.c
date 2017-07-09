#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define WINDOW_TITLE "NaveShooter"
//#define WINDOW_WIDTH 800
//#define WINDOW_HEIGHT 600

typedef enum {
    WINDOW_WIDTH = 600,
    WINDOW_HEIGHT= 600,
    MS = 10,
    LEN_NAVE = 20,
    VX_NAVE = 5,
    VY_NAVE = 5,
    PROJECTILE_LEN = 20,
    VX_P = 0,
    VY_P = 10
} constants;

typedef struct Nave Nave;
typedef struct Projectile Projectile;
typedef enum Direction {UP, DOWN, LEFT, RIGHT} Direction;

struct Nave
{
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int vx, vy; //axis velocities
    Projectile *projectiles;
};


struct Projectile
{
    int x1, y1;
    int x2, y2;
    int vx, vy;
    Projectile *next;
};

void moveNave(Nave *nave, Direction dir);
void moveProjectile(Projectile *p, Direction dir);
void moveAllProjectiles(Nave *nave);
void fireNave(Nave *nave);
void paintNave(Nave *nave, SDL_Renderer *renderer);
void paintProjectiles(Nave *nave, SDL_Renderer *renderer);
void deleteProjectiles(Projectile *p);

int main(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        SDL_Quit();
        return -1;
    } else {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "OK", "SDL intialized", NULL);
    }

    int gameOver = 0;
    SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, (1366 - WINDOW_WIDTH) / 2, (768 - WINDOW_HEIGHT) / 2, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    int typeEvent;

    Nave nave = {
        WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
        WINDOW_WIDTH / 2 - LEN_NAVE, WINDOW_HEIGHT / 2 + LEN_NAVE,
        WINDOW_WIDTH / 2 + LEN_NAVE, WINDOW_HEIGHT / 2 + LEN_NAVE,
        VX_NAVE, VY_NAVE,
        NULL
    };
    const unsigned char *keys = SDL_GetKeyboardState(NULL);

    while(!gameOver)
    {
        //Events

        if(SDL_PollEvent(&event))
        {
            typeEvent = event.type;
            if(typeEvent == SDL_QUIT)
            {
                gameOver = 1;
            }
            else if(typeEvent == SDL_MOUSEBUTTONDOWN)
            {

                int x, y;
                if(SDL_GetMouseState(&x, &y))
                {
                    switch (event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            printf("Left button\n");
                            fireNave(&nave);
                            break;
                        case SDL_BUTTON_RIGHT:
                            printf("Right button\n");
                            break;
                        case SDL_BUTTON_MIDDLE:
                            printf("Mouse wheel was clicked\n");
                            break;
                        default:
                            break;
                    }
                    printf("x: %d, y: %d\n", x, y);
                }
            }
            else if(typeEvent == SDL_KEYDOWN)
            {
                if(keys[SDL_SCANCODE_ESCAPE])
                {
                    //printf("Escape was pressed");
                    gameOver = 1;
                }
                else if(keys[SDL_SCANCODE_Q])
                {
                    //printf("Q was pressed");
                }
                else if(keys[SDL_SCANCODE_LEFT])
                {
                    moveNave(&nave, LEFT);
                }
                else if(keys[SDL_SCANCODE_RIGHT])
                {
                    moveNave(&nave, RIGHT);
                }
                else if(keys[SDL_SCANCODE_UP])
                {
                    moveNave(&nave, UP);
                }
                else if(keys[SDL_SCANCODE_DOWN])
                {
                    moveNave(&nave, DOWN);
                }
                else if(keys[SDL_SCANCODE_SPACE])
                {
                    fireNave(&nave);
                }
            }
        }
        deleteProjectiles(nave.projectiles);
        //Screen

        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,0,255,0,0);
        moveAllProjectiles(&nave);

        paintNave(&nave, renderer);
        SDL_SetRenderDrawColor(renderer,255,255,0,0);
        paintProjectiles(&nave, renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(MS);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void moveNave(Nave *nave, Direction dir)
{
    switch(dir)
    {
        case UP:
            nave->y1 -= VY_NAVE;
            nave->y2 -= VY_NAVE;
            nave->y3 -= VY_NAVE;
            break;
        case DOWN:
            nave->y1 += VY_NAVE;
            nave->y2 += VY_NAVE;
            nave->y3 += VY_NAVE;
            break;
        case LEFT:
            nave->x1 -= VX_NAVE;
            nave->x2 -= VX_NAVE;
            nave->x3 -= VX_NAVE;
            break;
        case RIGHT:
            nave->x1 += VX_NAVE;
            nave->x2 += VX_NAVE;
            nave->x3 += VX_NAVE;
            break;
        default:
            break;
    }
}

void moveProjectile(Projectile *p, Direction dir)
{
    switch (dir) {
        case UP:
            p->y1 -= VY_P;
            p->y2 -= VY_P;
            break;
        case DOWN:
            p->y1 += VY_P;
            p->y2 += VY_P;
            break;
        case LEFT:
            p->x1 -= VX_P;
            p->x2 -= VX_P;
            break;
        case RIGHT:
            p->x1 += VX_P;
            p->x2 += VX_P;
            break;
    default:
        break;
    }
}

void moveAllProjectiles(Nave *nave)
{
    Projectile *it = nave->projectiles;
    while (it != NULL) {
        moveProjectile(it, UP);
        it = it->next;
    }
}

void fireNave(Nave *nave)
{
    if(nave == NULL) return;
    if(nave->projectiles == NULL)
    {
        printf("First shot");
        nave->projectiles = (Projectile*)SDL_malloc(sizeof(Projectile));
        nave->projectiles->x1 = nave->x1;
        nave->projectiles->y1 = nave->y1;
        nave->projectiles->x2 = nave->x1;
        nave->projectiles->y2 = nave->y1 - PROJECTILE_LEN;
        nave->projectiles->vx = VX_P;
        nave->projectiles->vy = VY_P;
        nave->projectiles->next = NULL;
    }
    else
    {
        printf("nave->projectiles != NULL");
        Projectile *it = nave->projectiles; //The head of the linked list
        while(it->next != NULL)
        {
            it = it->next;
        }
        it->next = (Projectile*) SDL_malloc(sizeof(Projectile));
        it = it->next;
        it->x1 = nave->x1;
        it->y1 = nave->y1;
        it->x2 = nave->x1;
        it->y2 = nave->y1 - PROJECTILE_LEN;
        it->vx = VX_P;
        it->vy = VY_P;
        it->next = NULL;
    }
}

void paintNave(Nave *nave, SDL_Renderer *renderer)
{
    /*SDL_Point points[4] =
    {
        {nave->x1, nave->y1},
        {nave->x2, nave->y2},
        {nave->x3, nave->y3},
        {nave->x1, nave->y1}
    };
    SDL_RenderDrawLines(renderer, points, 4);
*/
    filledTrigonRGBA(renderer, nave->x1, nave->y1, nave->x2, nave->y2, nave->x3, nave->y3,
                     0, 0, 255, 255);
}

void paintProjectiles(Nave *nave, SDL_Renderer *renderer)
{
    printf("----------------\n");
    Projectile *it = nave->projectiles;
    int count = 0;
    while (it != NULL) {
         SDL_RenderDrawLine(renderer, it->x1, it->y1, it->x2, it->y2);
        it = it->next;
        printf("Projectile: %d\n", ++count);
    }
}

void deleteProjectiles(Projectile *p)
{
    Projectile *it = p;
    Projectile *auxP;
    if(it == NULL) return;
    while(it->next != NULL)
    {
        if(it->next->y1 < 0)
        {
            auxP = it->next->next;
            SDL_free(it->next);
            it->next = auxP;
        }
        else
        {
            it = it->next;
        }
    }
}
