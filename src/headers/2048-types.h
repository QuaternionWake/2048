#ifndef TWFE_TYPES_H
#define TWFE_TYPES_H

typedef enum input qw_input;
typedef enum vaguePos qw_vaguePos;
typedef struct pos qw_pos;
typedef struct displayElement qw_displayElement;

enum input{INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT, INPUT_EXIT, INPUT_ENTER};

enum vaguePos{TLC,    TL,   TML,  TM,   TMR,   TR,    TRC,

              LT,     ITL,  ITML, ITM, ITMR,  ITR,     RT,
              LMT,    ILMT, MTL,  MT,   MTR, IRMT,    RMT,
              LM,     ILM,  ML,   M,     MR,  IRM,     RM,
              LMB,    ILMB, MBL,  MB,   MBR, IRMB,    RMB,
              LB,     IBL,  IBML, IBM, IBMR,  IBR,     RB,

              BLC,    BL,   BML,  BM,   BMR,   BR,    BRC};

struct pos{
    short x;
    short y;
};

struct displayElement{
    qw_vaguePos vaguePos;
    qw_displayElement *relativeTo;
    qw_pos relativePos;
    qw_pos absolutePos;
    qw_pos size;
    char render;
    char **contents;
};

#define clear() printf("\033[H\033[J")
#define max(a, b) (((a)>(b)) ? (a) : (b))
#define min(a, b) (((a)<(b)) ? (a) : (b))

#endif
