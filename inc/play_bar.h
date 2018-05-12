#pragma once

#ifdef ELEMENT_PLAY_BAR_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
	uint32_t					bg_color;
	uint32_t					border_color;
	uint32_t					duty_color;
	uint32_t					time_color;
	const MakiseFont*			font;
} MakiseStyle_PlayBar;

// Progress bar.
typedef struct {
	MElement						el;
	uint8_t							margin;
	uint32_t						upper_bound_time_sec;
	uint32_t						current_time_sec;
	const MakiseStyle_PlayBar*		style;
	char							current_time_string[9];
	char							upper_bound_time_string[9];
	uint8_t							duty;
	uint32_t						time_width;
	uint32_t						pb_width;
} MPlayBar;

void m_create_play_bar (	MPlayBar*						b,
							MContainer*						c,
							MPosition						pos,
							uint8_t							margin_play_bar,
							const MakiseStyle_PlayBar*		style	);

void m_play_bar_inc_sec		 ( MPlayBar* b );
void m_play_bar_set_new_track	( MPlayBar* b, uint32_t track_time_sec );

#ifdef __cplusplus
}
#endif

#endif
