#include <stdio.h>
#include <stdlib.h>
#include "SeqStack.h"

/* init */
SeqStack *Init_SeqStack() {
    SeqStack *s;
    s = (SeqStack*)malloc(sizeof(SeqStack));
    if (!s) {
        return -1;
    }
    memset(s, 0, sizeof(SeqStack));
    s->top = -1;
    return s;
}

/* empty */
int Empty_SeqStack(SeqStack *s) {
    if (s->top == -1) {
        return 1;
    } else {
        return 0;
    }
}

/* push */
int Push_SeqStack(SeqStack *s, long long x) {
    if (s->top == MAXSIZE-1) {
        return 0;
    } else {
        s->top++;
        s->data[s->top] = x;
        return 1;
    }
}

/* pop */
long long Pop_SeqStack(SeqStack *s) {
    long long x;
    if (Empty_SeqStack(s)) {
        return 0;
    } else {
        x = s->data[s->top];
        s->data[s->top] = 0;
        s->top--;
        return x;
    }
}

/* top */
long long Top_SeqStack(SeqStack *s) {
    if (Empty_SeqStack(s)) {
        return 0;
    } else {
        return (s->data[s->top]);
    }
}
