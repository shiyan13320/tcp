#ifndef SEQSTACK_H_
#define SEQSTACK_H_
#include <stdio.h>
#define MAXSIZE 20

typedef struct {
    long long data[MAXSIZE];
    int top;
}SeqStack;

/* init */
SeqStack *Init_SeqStack();

/* empty */
int Empty_SeqStack(SeqStack *s);

/* push */
int Push_SeqStack(SeqStack *s, long long x);

/* pop */
long long Pop_SeqStack(SeqStack *s);

/* top */
long long Top_SeqStack(SeqStack *s);

#endif
