/*
 * mandelbrot_set.c
 *
 *  Created on: 6 Mar 2018
 *      Author: ahmed.zamouche@gmail.com
 */

#define FIXED_POINT 1

#if FIXED_POINT == 1
#include "q8_7.h"
#else
#define _Add(a, b) (a+b)
#define _Sub(a, b) (a-b)
#define _Mul(a, b) (a*b)
#define _Div(a, b) (a/b)
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#if FIXED_POINT == 1
typedef Q8_7_t Base_t;
#else
typedef float Base_t;
#endif

#if FIXED_POINT == 0
typedef struct Complex_s
{
  Base_t re;
  Base_t im;
}Complex_t;
#endif

typedef struct Point_s
{
  Base_t x;
  Base_t y;
  uint32_t color;
}Point_t;

const size_t MAX_ITERATION = 1000;
#if FIXED_POINT == 1
const Base_t RADIUS = (1 << Q8_7_SHIFT) << 1;
#else
const Base_t RADIUS = 2;
#endif

#define SCREEN_XSIZE  512
#define SCREEN_YSIZE  128

typedef Point_t* Screen_t;

Point_t s_screen[SCREEN_XSIZE * SCREEN_YSIZE] = {0};

static inline void complex_set(Complex_t *c, Base_t re, Base_t im)
{
  c->re = re;
  c->im = im;
}

static inline void complex_add(Complex_t *c, Complex_t *a, Complex_t *b)
{
  c->re = a->re + b->re;
  c->im = a->im + b->im;
}

static inline  void complex_mul(Complex_t *c, Complex_t *a, Complex_t *b)
{
  c->re = a->re * b->re - a->im * b->im;
  c->im = a->re * b->im + a->im * b->re;
}

static inline void screen_to_complex(Complex_t *c, Point_t *p)
{
#if FIXED_POINT == 1
  c->re = ((p->x - (SCREEN_XSIZE >> 1)) * (1 << Q8_7_SHIFT)) / (SCREEN_XSIZE >> 2);
  c->im = ((p->y - (SCREEN_YSIZE >> 1)) * (1 << Q8_7_SHIFT)) / (SCREEN_YSIZE >> 2);
#else
c->re = (p->x - (SCREEN_XSIZE / 2)) / (SCREEN_XSIZE/4);
c->im = (p->y - (SCREEN_YSIZE / 2)) / (SCREEN_YSIZE/4);
#endif
}

void screen_print(Point_t *screen)
{
  fprintf(stdout, " \
  <!DOCTYPE html> \
    <html> \
      <head> \
        <meta charset='UTF-8'> \
        <title>Mandelbrot set</title> \
      </head> \
      <body bgcolor='#000000'> \
        <pre>");
  for (size_t y = 0; y < SCREEN_YSIZE ; y++) {
    for (size_t x = 0; x < SCREEN_XSIZE; x++) {
      fprintf(stdout, "<span style='color:#%06X'>¤</span>", screen[SCREEN_XSIZE*y + x].color & 0x00FFFFFF);
    }
    fprintf(stdout, "<br />\n");
  }
  fprintf(stdout, "</pre></body></html>\n");
}

void point_color_cal(Point_t *p)
{

  Complex_t c, z_n;

  screen_to_complex(&c, p);

  complex_set(&z_n, 0, 0);

  size_t iteration = 0;

  Base_t rr = 0;

  while(iteration < MAX_ITERATION && (rr < _Mul(RADIUS , RADIUS))){

    Base_t re_re = _Mul(z_n.re , z_n.re);
    Base_t im_im = _Mul(z_n.im , z_n.im);
    Base_t _2_re_im = 2*_Mul(z_n.re , z_n.im);

    rr = _Add(re_re , im_im);

    z_n.re = _Add(_Sub(re_re , im_im) , c.re);
    z_n.im = _Add(_2_re_im , c.im);

    iteration++;
  }
  if(iteration == MAX_ITERATION){
    p->color = 0;
  }else{
#if FIXED_POINT == 1
    p->color = (iteration * rr) * 2;
#else
    p->color = (iteration * rr) * 128;
#endif
  }
}

int main(int argc, char **argv)
{
  for (size_t x = 0; x < SCREEN_XSIZE ; x++) {
    for (size_t y = 0; y < SCREEN_YSIZE; y++) {
      s_screen[y*SCREEN_XSIZE + x].x = x;
      s_screen[y*SCREEN_XSIZE + x].y = y;
      point_color_cal(&s_screen[y*SCREEN_XSIZE + x]);
    }
  }
  screen_print(s_screen);
  return 0;
}
