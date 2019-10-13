#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfa.h"

struct _Nfa {
	State** states;
	int nStates;
};

struct _State {
	char* name;
	StateType type;
	Transition** transitions;
	int nTrans;
};

struct _Transition {
	char symbol;
	State** dest;
	int nDest;
};


Nfa * newNFA(State** states, int nStates) {
	if(states == NULL) {
		return NULL;
	}

	Nfa *new = (Nfa *)malloc(sizeof(Nfa));
	if (new == NULL){
		perror ("Error! memory not allocated.\n");
		return NULL;
	}

	new->nStates = nStates;

	new->states = (State**) malloc(nStates*sizeof(State*));
	for(int i = 0; i < nStates; i++) {
		new->states[i] = states[i];
	}
	return new;
}

void addTransition(State* origin, char symbol, State* dest) {
	if(origin == NULL || dest == NULL) {
		return;
	}

	Transition *new = NULL;

	for (int i=0; i<origin->nTrans; i++) {
		if (origin->transitions[i]->symbol == symbol) {
			new = origin->transitions[i];
		}
	}

	if (new == NULL) {
		new = (Transition *)malloc(sizeof(Transition));
		if (new == NULL){
			perror ("Error! memory not allocated.\n");
			return;
		}
		new->symbol = symbol;
		new->dest = (State**) malloc(sizeof(State));
		new->dest[0] = dest;
		new->nDest = 1;
		origin->nTrans++;
		origin->transitions = (Transition **)realloc(origin->transitions, origin->nTrans*sizeof(Transition*));
	  	origin->transitions[origin->nTrans-1] = new;
	} else {
		new->nDest++;
		new->dest = (State**) realloc(new->dest, new->nDest*sizeof(State*));
		new->dest[new->nDest-1] = dest;
	}
}

State * newState(char* name, StateType type) {
	if(name == NULL) {
		return NULL;
	}

	State *new = (State *)malloc(sizeof(State));
	if (new == NULL){
		perror ("Error! memory not allocated.\n");
		return NULL;
	}

	new->name = (char *) malloc((strlen(name)+1) * sizeof(char));
  	if (new->name == NULL){
   		free(new);
		perror ("Error! memory not allocated.\n");
		return NULL;
	}

	strcpy(new->name, name);

  	new->transitions = (Transition **) malloc(sizeof(Transition*));
	if (new->name == NULL){
		free(new->name);
    	free(new);
		perror ("Error! memory not allocated.\n");
		return NULL;
	}

	new->type = type;
  	new->nTrans = 0;

  	return new;
}

void printNFA(Nfa* nfa) {
	if(nfa == NULL) {
		return;
	}

	for (int i = 0; i<nfa->nStates; i++) {
		printState(nfa->states[i]);
	}
}

void printState(State *state) {
	if(state == NULL) {
		return;
	}
	printf("Transitions from %s (type %d):\n", state->name, state->type);
	
	for(int i=0; i<state->nTrans; i++) {
		printTransition(state->transitions[i]);
	}
}

void printStates(State **states, int nStates) {
	if(states == NULL) {
		return;
	}

	for (int i=0; i<nStates; i++) {
		printState(states[i]);
	}
}

void printTransition(Transition *trans) {
	if(trans == NULL) {
		return;
	}
	for (int i=0; i<trans->nDest; i++) {
		printf("%c -> %s\n", trans->symbol, trans->dest[i]->name);
	}
}


int runNFA(Nfa* nfa, char* str) {
	if(nfa == NULL || str == NULL) {
		return 0;
	}

  	State** currents = NULL;
	State** news = NULL;
	int nCurrent = 0;
	int nNew = 0;
	int k = 0;
	int x = 0;
	int i = 0;

	currents = (State**) malloc(sizeof(State*)*nfa->nStates);
	news = (State**) malloc(sizeof(State*)*nfa->nStates);

	for(i = 0; i<nfa->nStates; i++) {
		if (nfa->states[i]->type == START) {
			currents[nCurrent] = nfa->states[i];
			nCurrent++;
		}
	}

	for(i = 0; i<strlen(str); i++) {
		nNew = 0;
		for (x = 0; x<nCurrent; x++) {
			for(k = 0; k<currents[x]->nTrans; k++) {
				if (currents[x]->transitions[k]->symbol == str[i]) {
					nNew += addStates(news, nNew, currents[x]->transitions[k]->dest, currents[x]->transitions[k]->nDest);
				}
			}
		}
		currents = news;
		nCurrent = nNew;
	}

	printStates(currents, nCurrent);
	for (x = 0; x<nCurrent; x++) {
		if(currents[x]->type == FINAL) {
			return 1;
		}
	}
	return 0;
}

int addStates(State** new, int sizeNew,State** old, int nStates) {
	int x = 0;
	int found = 0;
	int added = 0;

	for (int j=0; j<nStates; j++) {
		found = 0;
		// Check if the state is in the array
		for (x = 0; x < sizeNew+added; x++) {
			if (new[x] == old[j]) {
				found = 1;
			}
		}
		if (found == 0) {
			new[sizeNew+added] = old[j];
			added++;
		}
	}
	return added;
}

void nfaFree(Nfa* nfa) {
	if(nfa == NULL) {
		return;
	}
	for (int i=0; i<nfa->nStates; i++) {
		stateFree(nfa->states[i]);
	}
	free(nfa->states);
	free(nfa);
}

void stateFree(State* state) {
	if(state == NULL) {
		return;
	}
	free(state->name);
	for (int i=0; i<state->nTrans; i++) {
		transFree(state->transitions[i]);
	}
	free(state->transitions);
	free(state);
}

void transFree(Transition* t) {
	free(t->dest);
	free(t);
}


int main(void) {
	State **states = NULL;
	Nfa* nfa = NULL;

	states = (State**) malloc(3 * sizeof(State*));

	states[0] = newState("q0", START);
	states[1] = newState("q1", OTHER);
	states[2] = newState("q2", FINAL);

	addTransition(states[0], '0', states[0]);
  	addTransition(states[0], '1', states[0]);
	addTransition(states[0], '0', states[1]);

  	addTransition(states[1], '1', states[2]);

  	nfa = newNFA(states, 3);


  	printNFA(nfa);

  	if (runNFA(nfa, "00110")) {
    	printf("OK\n");
  	} else {
    	printf("FAILED\n");
  	}

	nfaFree(nfa);
	free(states);
}
