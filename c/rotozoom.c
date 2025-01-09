#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <caca.h>

#define TEXTURE_SIZE 256
#define TABLE_SIZE 65536

/* 24:8 Fixed point stuff */
#define PRECISION 8

#define FMUL(a, b) (((a)*(b))>>PRECISION)
#define TOFIX(d)   ((int)( (d)*(double)(1<<PRECISION) ))
#define TOINT(a)   (a>>PRECISION);
#define XSIZ 256
#define YSIZ 256

#include "texture.h"

int main() {
    caca_canvas_t *cv;
    caca_display_t *dp;
    caca_event_t ev;
    dp = caca_create_display(NULL);
    if (!dp) return 1;
    cv = caca_get_canvas(dp);
    int width = caca_get_canvas_width(cv);
    int height = caca_get_canvas_height(cv);
    static caca_dither_t *dither;
    static uint32_t screen[XSIZ * YSIZ];
    static int cos_tab[TABLE_SIZE], sin_tab[TABLE_SIZE];
    static int y_tab[TEXTURE_SIZE];

    uint32_t *p;
    static int alphaF, tF;
    int scaleF;

    /* register is quite a bad idea on CISC, but not on RISC */
    register unsigned int x, y;
    register unsigned int xxF, yyF, uF, vF, uF_, vF_;
    register unsigned int vu, vv;

    /* Prepare */
    for(x = 0; x < TABLE_SIZE; x++)
      {
	cos_tab[x] = TOFIX(cos(x * (360.0f / (float)TABLE_SIZE)));
	sin_tab[x] = TOFIX(sin(x * (360.0f / (float)TABLE_SIZE)));
      }
    for(x = 0; x < TEXTURE_SIZE; x++)
      y_tab[x] = x * TEXTURE_SIZE; /* start of lines offsets */

    /* Init */
    dither = caca_create_dither(32, XSIZ, YSIZ, XSIZ * 4,
				0x00FF0000,
				0x0000FF00,
				0x000000FF,
				0x00000000);

    /* Update */
        while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS | CACA_EVENT_QUIT, &ev, 0)) {

	  alphaF += 4;
	  tF     += 3;
	  scaleF = FMUL(sin_tab[tF & 0xFFFF], TOFIX(3)) + (TOFIX(4));
	  xxF    = FMUL(cos_tab[(alphaF) & 0xFFFF], scaleF);
	  yyF    = FMUL(sin_tab[(alphaF) & 0xFFFF], scaleF);
	  uF  = vF  = 0;
	  uF_ = vF_ = 0;
	  p = screen;

	  for(y = YSIZ; y--;)
	    {
	      for(x = XSIZ; x--;)
		{
		  uF += xxF;
		  vF += yyF;

		  vu = TOINT(uF);
		  vv = TOINT(vF);
		  vu &= 0xFF;       /* ARM doesn't like    */
		  vv &= 0xFF;       /* chars as local vars */

		  *p++ = texture256x256[vu + y_tab[vv]];
		}

	      uF = uF_ -= yyF;
	      vF = vF_ += xxF;
	    }
	  /* Render */
	  caca_dither_bitmap(cv, 0, 0,
			     caca_get_canvas_width(cv),
			     caca_get_canvas_height(cv),
			     dither, screen);
	  caca_refresh_display(dp);
	}

	caca_free_dither(dither);
	caca_free_canvas(cv);
	caca_free_display(dp);
	return 0;
}
