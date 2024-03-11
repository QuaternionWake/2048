#ifndef TWFE_TYPES_H
#define TWFE_TYPES_H

typedef enum input qw_input;
typedef enum vaguePos qw_vaguePos;
typedef struct vec2 qw_vec2;
typedef struct displayElement qw_displayElement;

enum input {NO_INPUT, INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT, INPUT_EXIT, INPUT_CONFIRM, INPUT_SWITCH};

enum vaguePos {TLC,    TL,   TML,  TM,   TMR,   TR,    TRC,

               LT,     ITL,  ITML, ITM, ITMR,  ITR,     RT,
               LMT,    ILMT, MTL,  MT,   MTR, IRMT,    RMT,
               LM,     ILM,  ML,   M,     MR,  IRM,     RM,
               LMB,    ILMB, MBL,  MB,   MBR, IRMB,    RMB,
               LB,     IBL,  IBML, IBM, IBMR,  IBR,     RB,

               BLC,    BL,   BML,  BM,   BMR,   BR,    BRC};

struct vec2 {
    short x;
    short y;
};

struct displayElement {
    qw_vaguePos vaguePos;
    qw_displayElement *relativeTo;
    qw_vec2 relativePos;
    qw_vec2 absolutePos;
    qw_vec2 size;
    char render;
    char **contents;
};

#endif
