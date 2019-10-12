#ifndef AF_H
#define AF_H

#include <stdio.h>
#include <stdlib.h>

#define MAXSTACK 100

typedef struct _Nfa Nfa;

typedef struct _State State;

typedef struct _Transition Transition;

typedef enum {
    START, FINAL, OTHER
} StateType;


Nfa * newDFA(State** states, int nStates);

void addTransition(State* origin, char symbol, State* dest);

State * newState(char* name, StateType type);

void printDFA(Nfa* nfa);

void printState(State *state);

void printTransition(Transition *trans) ;

int runDFA(Nfa* nfa, char* str);

int addStates(State** new, int sizeNew,State** old, int nStates);

void nfaFree(Nfa* nfa);

void stateFree(State* state);

void transFree(Transition* t);


#endif
