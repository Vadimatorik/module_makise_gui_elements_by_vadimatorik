#pragma once

#ifdef ELEMENT_PLAYER_STATUS_BAR_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
	const MakiseFont*			const font_ercent;
	const uint32_t				font_font_ercent_col;
	const uint32_t				bg_color;
	const uint32_t				border_color;
} MakiseStyle_SMPlayerStatusBar;

typedef struct {
	uint32_t ( *get_state_play )			( void );
	uint32_t ( *get_percent_battery )		( void );
} MPlayerStatusBar_CallbackFunc;

typedef struct {
	MElement						e;
	const MPlayerStatusBar_CallbackFunc*	f;
	const MakiseStyle_SMPlayerStatusBar*	s;
} MPlayerStatusBar;

void m_create_player_status_bar( MPlayerStatusBar*				b,
								 MContainer*					c,
								 MPosition						pos,
								 const MakiseStyle_SMPlayerStatusBar* s,
								 const MPlayerStatusBar_CallbackFunc* f );


#ifdef __cplusplus
}
#endif

#endif
