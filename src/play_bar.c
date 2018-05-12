#include "play_bar.h"

#include "text_func.h"

#ifdef ELEMENT_PLAY_BAR_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

static MResult draw ( MElement* b, MakiseGUI *gui );

static char name[] = "Play bar";
static char init_char_array[9] = "00:00:00";

void m_create_play_bar ( MContainer*                    c,
                         MPlayBar*                      b,
                         MPosition                      pos,
                         uint8_t                        margin_play_bar,
                         MakiseStyle_PlayBar*           style ) {
    b->style        = style;
    b->margin       = margin_play_bar;
    b->upper_bound_time_sec = 0;
    b->current_time_sec     = 0;
    b->duty                 = 0;
    b->time_width           = makise_d_string_width(init_char_array,9,b->style->font);
    b->pb_width             = pos.width - ( b->time_width * 2 ) - 2;
    // 00:00:00.
    memcpy( b->current_time_string, init_char_array, 9 );
    memcpy( b->upper_bound_time_string, init_char_array, 9 );

    MElement *e             = &b->el;
    m_element_create        ( e, name, b, 1, 1, pos, &draw, NULL, NULL, NULL, NULL, 0, NULL );
    makise_g_cont_add       ( c, e );

    MAKISE_DEBUG_OUTPUT( "Play_bar %d created.\n", e->id );
}

static MResult draw ( MElement* b, MakiseGUI *gui ) {
    MPlayBar *e =           ((MPlayBar*)b->data);
    makise_d_string         ( gui->buffer,
                              e->current_time_string, 9,
                              b->position.real_x, b->position.real_y,
                              MDTextPlacement_LeftUp,
                              e->style->font, e->style->time_color );

    makise_d_rect_filled    ( gui->buffer,
                              b->position.real_x + e->time_width + 1,
                              b->position.real_y,
                              e->pb_width,
                              b->position.height,
                              e->style->border_color,
                              e->style->bg_color );

    makise_d_rect_filled    ( gui->buffer,
                              b->position.real_x + e->time_width + 1 + e->margin + 1,
                              b->position.real_y + e->margin + 1,
                              ( e->pb_width - e->margin * 2 - 2 ) * e->duty / 100,
                              b->position.height - e->margin * 2 - 2,
                              e->style->duty_color,
                              e->style->duty_color );

    makise_d_string         ( gui->buffer,
                              e->upper_bound_time_string, 9,
                              b->position.real_x + e->time_width + 1 + e->pb_width + 2,
                              b->position.real_y,
                              MDTextPlacement_LeftUp,
                              e->style->font, e->style->time_color);
    return 0;
}

void m_play_bar_inc_sec ( MPlayBar* b ) {
    if ( b->current_time_sec > b->upper_bound_time_sec ) return;
    b->current_time_sec++;
    b->duty = (uint8_t)((float)100 / (float)b->upper_bound_time_sec * (float)b->current_time_sec);
    convert_time_sec_to_char( b->current_time_sec, b->current_time_string );
}

void m_play_bar_set_new_track ( MPlayBar* b, uint32_t track_time_sec ) {
    b->upper_bound_time_sec = track_time_sec;
    b->current_time_sec = 0;
    b->duty = 0;
    memcpy( b->current_time_string, init_char_array, 9 );
    convert_time_sec_to_char( b->upper_bound_time_sec, b->upper_bound_time_string );
}

#ifdef __cplusplus
}
#endif

#endif
