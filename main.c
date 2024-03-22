#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "game_common.h"


typedef struct player{
    float x, y ;  // coordonnées
    float wp, hp; // dimensions
    float dx, dy; // deplacement par pas de temps,
} PLAYER;

void initPlayer(PLAYER * p, float wp, float hp, float dx, float dy){
    p->dx = dx;
    p->dy = dy;
    p->wp = wp;
    p->hp = hp;
    p->x = WDISPLAY / 2.0 - wp / 2.0;
    p->y = HDISPLAY / 2.0 - hp / 2.0;
}

int main() {
    // declarations
    ALLEGRO_DISPLAY * game_display;
    ALLEGRO_TIMER * game_timer;
    ALLEGRO_EVENT_QUEUE * fifo;
    ALLEGRO_COLOR red;
    ALLEGRO_EVENT event;
    PLAYER p1, p2;
    bool fini = false;
    bool flags[NUMFLAGS] = {false};

    // initialisation
    // al_init_*
    if (!al_init()){
        printf("Erreur d'initialisation allegro");
        return 0;
    }
    assert(al_init_primitives_addon());
    // al_install_*
    assert(al_install_keyboard());
    // initialisation player
    initPlayer(&p1, 80, 50, 10,10);
    initPlayer(&p2, 100, 20, 5,5);

    // Creation
    // al_create_*
    game_display = al_create_display(WDISPLAY, HDISPLAY);
    game_timer = al_create_timer(1.0/24.0); //FPS (frame per second)
    fifo = al_create_event_queue();
    red = al_map_rgba(255, 0, 0, 150);

    // Sources
    al_register_event_source(fifo, al_get_display_event_source(game_display));
    al_register_event_source(fifo, al_get_timer_event_source(game_timer));
    al_register_event_source(fifo, al_get_keyboard_event_source());

    // boucle d'events
    al_start_timer(game_timer);
    while (!fini){
        al_wait_for_event(fifo, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                fini = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                        fini = true;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        flags[LEFT] = true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        flags[RIGHT] = true;
                        break;
                    case ALLEGRO_KEY_Q:
                        flags[Q]= true;
                        break;
                    case ALLEGRO_KEY_D:
                        flags[D]= true;
                        break;
                }
                break;
            case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_LEFT:
                        flags[LEFT] =false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        flags[RIGHT] = false;
                        break;
                    case ALLEGRO_KEY_Q:
                        flags[Q]= false;
                        break;
                    case ALLEGRO_KEY_D:
                        flags[D]= false;
                        break;
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                if (flags[LEFT] == true) {
                    if (p1.x <= 0) {
                        p1.x = 0;
                    } else {
                        p1.x = p1.x - p1.dx;
                    }
                }
                if(flags[RIGHT]){
                    if((p1.x + p1.wp) >= WDISPLAY){
                        p1.x = WDISPLAY - p1.wp;
                    } else{
                        p1.x += p1.dx;
                    }
                }
                if (flags[Q] == true) {
                    if (p2.x <= 0) {
                        p2.x = 0;
                    } else {
                        p2.x = p2.x - p2.dx;
                    }
                }
                if(flags[D]){
                    if((p2.x + p2.wp) >= WDISPLAY){
                        p2.x = WDISPLAY - p2.wp;
                    } else{
                        p2.x += p2.dx;
                    }
                }
                al_clear_to_color(BLACK);
                al_draw_filled_rectangle(p1.x, p1.y, p1.x + p1.wp, p1.y + p1.hp, red);
                al_draw_filled_rectangle(p2.x, p2.y, p2.x + p2.wp, p2.y + p2.hp, red);

                al_flip_display();
                break;
        }
    }


    // liberation
    al_destroy_display(game_display);
    al_destroy_timer(game_timer);
    al_destroy_event_queue(fifo);

    return 0;
}
