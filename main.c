#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "game_common.h"

typedef struct player{
    float x, y ;  // coordonnées
    float wp, hp; // dimensions
    float dx, dy; // deplacement par pas de temps,
} PLAYER;

int main() {
    // declarations
    ALLEGRO_DISPLAY * game_display;
    ALLEGRO_TIMER * game_timer;
    ALLEGRO_EVENT_QUEUE * fifo;
    ALLEGRO_COLOR red;
    ALLEGRO_EVENT event;
    PLAYER p;
    bool fini = false;
    bool gauche = false;

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
    p.wp = 80;
    p.hp = 50;
    p.dx = 10;
    p.dy = 10;
    p.x = WDISPLAY /2.0 - p.wp/2.0;
    p.y = HDISPLAY /2.0 - p.hp/2.0;

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
                        gauche = true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        if((p.x+p.wp)>=WDISPLAY){
                            p.x = WDISPLAY - p.wp;
                        } else{
                            p.x += p.dx;
                        }
                        break;
                }
                break;
            case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_LEFT:
                        gauche =false;
                        break;
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                if (gauche == true) {
                    if (p.x <= 0) {
                        p.x = 0;
                    } else {
                        p.x = p.x - p.dx;
                    }
                }
                al_clear_to_color(BLACK);
                al_draw_filled_rectangle(p.x, p.y, p.x + p.wp, p.y + p.hp, red);
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
