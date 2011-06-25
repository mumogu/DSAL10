#include <queue>
#include <bitset>
#include <vector>
#include <set>
#include <stdint.h>

using namespace std;

struct gameGrid {
	uint64_t g;
	vector<char> h; // History of swaps done to reach g
};

uint64_t puzzles[10] = {
	0x12345678D9BCAE0Fll, 0x12345678AF0B9DECll, 
	0x51342A68907CDEBFll, 0x26341A7805BC9DEFll,
	0x51249638DA7CEB0Fll, 0x123469705AB8DEFCll,
	0x13745208A6EC9BDFll, 0x1234A6B709F85DECll,
	0x3748150296BCDAEFll, 0x023716845AFB9DECll	
};

uint64_t correct = 0x123456789ABCDEF0ll;

// Returns the position of the empty cell. Numering is from bottom right to top left starting at 0
int findZero(uint64_t g) {
	for(int i=0; i<16; i++) {
		if((g & ((uint64_t)0xF << i*4)) == 0)
			return i;
	}
	return 0;
}

// Who's the boss now?
uint64_t swapNibble(uint64_t g, int zero, int data) {
	return (((g & ((uint64_t)0xF << (4*data))) >> (4*data)) << 4*zero) | (g & ~((uint64_t)0xF << 4*data));
}

void printSolution(vector<char> h) {
	vector<char>::iterator it;
	for(it = h.begin(); it < h.end(); it++) {
		printf("%c", *it);
	}
}

void processCandidate(uint64_t* pCandidate, gameGrid* pCurrent, queue<gameGrid>* pQ, set<uint64_t>* pSeen, char direction) {
	if((*pSeen).find(*pCandidate) == (*pSeen).end()) {		
		vector<char> currentHistory = (*pCurrent).h;
		currentHistory.push_back(direction);
		
		gameGrid temp = {*pCandidate, currentHistory};
		(*pQ).push(temp);
		
		(*pSeen).insert(*pCandidate);		
	}
}

void solve(uint64_t problem) {	
	queue<gameGrid> q; // working queue for BFS

	vector<char> empty;
	q.push((gameGrid){problem, empty});

	// We need to kepp track, of which boards be have already explored
	set<uint64_t> seen; // Set is probably implemented as a binary tree

	gameGrid current;
	
	while(!q.empty()) {
		current = q.front();
		q.pop();
		
		if(current.g == correct)
			break;
		
		int gap = findZero(current.g);			
		
		if(gap >= 4) { // swap-down is possible
			uint64_t candidate = swapNibble(current.g, gap, gap-4);
			processCandidate(&candidate, &current, &q, &seen, 'D');			
		}		
		if(gap <= 11) { // swap-up is possible			
			uint64_t candidate = swapNibble(current.g, gap, gap+4);			
			processCandidate(&candidate, &current, &q, &seen, 'U');
		}		
		if(gap%4 != 0 ) { // swap-right is possible			
			uint64_t candidate = swapNibble(current.g, gap, gap-1);
			processCandidate(&candidate, &current, &q, &seen, 'R');
		}		
		if((gap+1)%4 != 0) { // swap-left is possible	
			uint64_t candidate = swapNibble(current.g, gap, gap+1);	
			processCandidate(&candidate, &current, &q, &seen, 'L');
		}	
	}
	
	printf("Found solutuion: "); printSolution(current.h); printf("\n");
	printf("Length of solution is: %d\n", (int)current.h.size());
	printf("Size of seen is: %d", (int)seen.size());
}

int main (int argc, char * const argv[]) {	
	for(int i=0; i<10; i++) {
		printf("Puzzle #%d:", i+1);
		solve(puzzles[i]);
		printf("\n\n");
	}
}
