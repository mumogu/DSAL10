#include <list>
#include <queue>
#include <bitset>
#include <vector>
#include <set>

using namespace std;

// Just for my personal lazyness
typedef bitset<64> grid;

struct gameGrid {
	grid g;
	vector<char> h;	
};

grid puzzles[10] = {
	0x12345678D9BCAE0Fll,
	0x12345678AF0B9DECll,
	0x51342A68907CDEBFll,
	0x26341A7805BC9DEFll,
	0x51249638DA7CEB0Fll,
	0x123469705AB8DEFCll,
	0x13745208A6EC9BDFll,
	0x1234A6B709F85DECll,
	0x3748150296BCDAEFll,
	0x023716845AFB9DECll	
};

grid correct = 0x123456789ABCDEF0ll;

// Returns the position of the empty cell. Numering is from LSB to MSB starting at 0
int findZero(grid b) {
	for(int i=0; i<16; i++) {
		if(b[i*4] == 0 && b[i*4+1] == 0 && b[i*4+2] == 0 && b[i*4+3] == 0)
			return i;		
	}	
	return 0; // Just to shut up the Compiler-Warning
}

grid swapNibble(grid g, int zero, int dest) {
	for(int i=0; i<4; i++) {
		g[zero*4+i] = g[dest*4+i];
		g[dest*4+i] = 0;
	}
	return g;
}

void printSolution(vector<char> h) {
	vector<char>::iterator it;
	for(it = h.begin(); it < h.end(); it++) {
		printf("%c", *it);
	}
}

void solve(grid start) {	
	// Initialize the queue and stuff...
	queue<gameGrid> q;
	vector<char> empty;
	struct gameGrid first = {start, empty};
	struct gameGrid current;
	q.push(first);
	
	// We need to kepp track, of which boards be have already explored
	set<unsigned long> seen;

	while(!q.empty()) {
		current = q.front();
		q.pop();
		
		if(current.g == correct)
			break;
		
		int gap = findZero(current.g);		
		
		if(gap >= 4) {
			// swap-down is possible
			grid candidate = swapNibble(current.g, gap, gap-4);

			if(seen.find(candidate.to_ulong()) == seen.end()) {
				vector<char> currentHistory = current.h;
				currentHistory.push_back('D');
				
				struct gameGrid temp = {candidate, currentHistory};
				q.push(temp);
				
				seen.insert(candidate.to_ulong());
			}
		}
		
		if(gap <= 11) {
			// swap-up is possible			
			grid candidate = swapNibble(current.g, gap, gap+4);
			
			if(seen.find(candidate.to_ulong()) == seen.end()) {
				vector<char> currentHistory = current.h;
				currentHistory.push_back('U');
				
				struct gameGrid temp = {candidate, currentHistory};
				q.push(temp);
				
				seen.insert(candidate.to_ulong());
			}
		}		
		if(gap%4 != 0 ) {
			// swap-right is possible			
			grid candidate = swapNibble(current.g, gap, gap-1);

			if(seen.find(candidate.to_ulong()) == seen.end()) {
				vector<char> currentHistory = current.h;
				currentHistory.push_back('R');
				
				struct gameGrid temp = {candidate, currentHistory};
				q.push(temp);
				
				seen.insert(candidate.to_ulong());
			}
		}		
		if((gap+1)%4 != 0) {
			// swap-left is possible	
			grid candidate = swapNibble(current.g, gap, gap+1);
			
			if(seen.find(candidate.to_ulong()) == seen.end()) {
				vector<char> currentHistory = current.h;
				currentHistory.push_back('L');
				
				struct gameGrid temp = {candidate, currentHistory};
				q.push(temp);				
				
				seen.insert(candidate.to_ulong());
			}
		}	
	}
	
	printf("Found solutuion: "); printSolution(current.h); printf("\n");
	printf("Length of solution is: %d\n", (int)current.h.size());
	printf("Size of seen is: %d", (int)seen.size());
}

int main (int argc, char * const argv[]) {
	for(int i=0; i<10; i++) {
		printf("Puzzle #%d:", i);
		solve(puzzles[i]);
		printf("\n\n");
	}
}