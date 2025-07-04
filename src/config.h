#ifndef CONFIG_H
#define CONFIG_H

#define VIRTUAL_W 1200
#define VIRTUAL_H 700

#define SIZE 9

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;
extern ALLEGRO_BITMAP *virtual_screen;

#define FPS 60

#endif
