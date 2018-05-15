#if defined(ELEMENT_SLIM_HORIZONTAL_LIST_ENABLED) && defined(ELEMENT_SCROLL_STRING_ENABLED)

#include "slim_horizontal_list.h"

#ifdef __cplusplus
extern "C" {
#endif

static MResult draw ( MElement* b, MakiseGUI *gui );

static char *name = "SlimHorizontalList";

void mCreateSlimHorizontalList (	MSlimHorizontalList*						b,
									MContainer*									c,
									MPosition									pos,
									const MakiseStyle_SMSlimHorizontalList*		style	) {
	MElement *e = &b->e;

	m_element_create(	e, name, b,
						 1, MFocusPrior_NotFocusble,
						 pos, &draw,
						 0,	 0,
						 NULL,	 NULL,
						 0, 0	);

	/// Изначально строки нет.
	b->stringBase	=	NULL;
	b->stringNow	=	NULL;

	b->style		=	style;

	b->maxItem		=	0;
	b->currenItem	=	0;

	makise_g_cont_add( c, e );

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
	MAKISE_DEBUG_OUTPUT( "SlimHorizontalList %d created\n", e->id );
#endif
}

void mSlimHorizontalListScrollString (	MSlimHorizontalList*	b	) {
	if ( b->stringNow == NULL )
		return;

	uint32_t	len;
	len = makise_d_string_width( b->stringNow, MDTextAll, b->style->fontString );

	/// Если строка не вмещается в экран.
	if ( len > b->e.position.width ) {
		b->stringNow++;							/// Сдвигаемся на 1 символ.
	} else {
		b->stringNow	=	b->stringBase;		/// Строка с начала.
	}
}

void mSlimHorizontalListSetStringCurrentItem (	MSlimHorizontalList*	b, const char* stringItem	) {
	b->stringBase	=	stringItem;
	b->stringNow	=	stringItem;
}

void mSlimHorizontalSetItemCount (	MSlimHorizontalList*	b, uint32_t itemCount	) {
	b->maxItem	=	itemCount - 1;
}

void mSlimHorizontalListLeft (	MSlimHorizontalList*	b, const char* stringItem	) {
	if ( b->currenItem > 0 ) {
		b->currenItem--;
		b->stringBase	=	stringItem;
		b->stringNow	=	stringItem;
	}
}

void mSlimHorizontalListRight (	MSlimHorizontalList*	b, const char* stringItem	) {
	if ( b->currenItem != b->maxItem ) {
		b->currenItem++;
		b->stringBase	=	stringItem;
		b->stringNow	=	stringItem;
	}
}

static const uint8_t B_arrowLeftEmpty_data[] = {
	0xc8, 0x9a, 0xa9, 0x8c, 0x00
};

const MakiseBitmap B_arrowLeftEmpty = {
	.width		=	4,
	.height		=	8,
	.data		=	B_arrowLeftEmpty_data
};

static const uint8_t B_arrowLeftFull_data[] = {
	0xc8, 0xfe, 0xef, 0x8c, 0x00
};

const MakiseBitmap B_arrowLeftFull = {
	.width		=	4,
	.height		=	8,
	.data		=	B_arrowLeftFull_data
};

static const uint8_t B_arrowRightFull_data[] = {
	0x31, 0xf7, 0x7f, 0x13, 0x00
};

const MakiseBitmap B_arrowRightFull = {
	.width		=	4,
	.height		=	8,
	.data		=	B_arrowRightFull_data
};

static const uint8_t B_arrowRightEmpty_data[] = {
	0x31, 0x95, 0x59, 0x13, 0x00
};

const MakiseBitmap B_arrowRightEmpty = {
	.width		=	4,
	.height		=	8,
	.data		=	B_arrowRightEmpty_data
};


static MResult draw ( MElement* b, MakiseGUI *gui ) {
	MSlimHorizontalList *p = ( MSlimHorizontalList* )b->data;

	/// Рамка на весь объект.
	_m_e_helper_draw_box_param( gui->buffer, &b->position, p->style->borderColor, p->style->bgColor, 0 );

	MPosition			pos		=		b->position;

	/// Вычисляем габариты для прямоугольника, окружающего левую стрелку.
	pos.width					=		8;

	/// Рамка под левое поле.
	_m_e_helper_draw_box_param( gui->buffer, &pos, p->style->borderColor, p->style->bgColor, 0 );

	/// Рисуем левую стрелку.
	if ( p->currenItem != 0 ) {
		makise_d_bitmap( gui->buffer, pos.real_x + 2, pos.real_y + 2, &B_arrowLeftFull, p->style->bgArrowColor );			/// У нас не самый крайний элемент слева.
	} else {
		makise_d_bitmap( gui->buffer, pos.real_x + 2, pos.real_y + 2, &B_arrowLeftEmpty, p->style->bgArrowColor );			/// 0-й элемент (самый левый).
	}

	/// Вычисляем габариты для рамки строки.
	pos.real_x					+=		8 - 1;			/// 1 стобец + 1 пропуск + 4 изображение + 1 пропуск + 1 столбей. - 1 из-за того, что мы хотим наложить границы.
	pos.width					=		b->position.width - 16 + 2;		/// На каждый квадрат по 8 - 2(совмещение границ).

	/// Рамка под строку.
	_m_e_helper_draw_box_param( gui->buffer, &pos, p->style->borderColor, p->style->bgColor, 0 );

	/// Печатаем строку.
	if ( p->stringNow != NULL ) {
		makise_d_string(	gui->buffer,
							p->stringNow,
							MDTextAll,								/// Размер будет ограничен внутри автоматически.
							pos.real_x + 2,
							pos.real_y + 2 +
								( ( pos.height - 4 ) -				/// От высоты отнимаем толщину линии сверху и снизу + 1 пиксель между.
								p->style->fontString->height ) / 2,
							MDTextPlacement_LeftUp,
							p->style->fontString,
							p->style->stringColor	);
	}

	/// Вычисляем габариты для прямоугольника, окружающего правую стрелку.
	pos.real_x		+=	pos.width - 1;
	pos.width		=	8;

	/// Рамка под правое поле.
	_m_e_helper_draw_box_param( gui->buffer, &pos, p->style->borderColor, p->style->bgColor, 0 );

	/// Рисуем левую стрелку.
	if ( p->currenItem !=  p->maxItem ) {
		makise_d_bitmap( gui->buffer, pos.real_x + 2, pos.real_y + 2, &B_arrowRightFull, p->style->bgArrowColor );			/// У нас не самый крайний элемент слева.
	} else {
		makise_d_bitmap( gui->buffer, pos.real_x + 2, pos.real_y + 2, &B_arrowRightEmpty, p->style->bgArrowColor );			/// 0-й элемент (самый левый).
	}

	return M_OK;
}



#ifdef __cplusplus
}
#endif

#endif
