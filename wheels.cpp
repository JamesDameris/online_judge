/* James Dameris
 * Online Judge Problem 10067
 * Playing With Wheels
 * 1/24/22
 */

#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <cmath>

using namespace std;

int nthdigit(int n, int k) {
    while (n--) {
        k /= 10;
    }
    return k % 10;
}

bool bfs(int start, int end, std::unordered_map<int, int> & D) {
    std::queue<int> Q;
    int current = start;
    int sign[2] = {1,-1};
    int p;
    if (start == end) {
        return true;
    }
    Q.push(start);
    while (!Q.empty()) {
        int f = Q.front();
        Q.pop();

        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < 4; i++) {
                p = i;
                if ((nthdigit(p,f) == 9) && j == 0) {
                    current = f + (pow(10,i) * -9);
                }
                else if((nthdigit(p,f) == 0) && j == 1) {
                    current = f + (pow(10,i) * 9);
                }
                else {
                    current = f + (pow(10,i) * sign[j]);
                }
                if (D.at(current) == 0) {
                    D[current] = 1 + D[f];
                    Q.push(current);
                    // cout << "Current: " << current << endl;
                    if (current == end) {
                        return true;
                    }
                    
                }
            }
        }   
    }
    return false;
}

int main () {
    int N = 0;
    int numBan = 0;
    int start1000s = 0;
    int start100s = 0;
    int start10s = 0;
    int start1s = 0;
    int start = 0;
    int end1000s = 0;
    int end100s = 0;
    int end10s = 0;
    int end1s = 0;
    int end = 0;
    int skip = 0;
    cin >> N;
    for (int i = 0; i < N; i++) { // reading inputs
        cin >> start1000s >> start100s >> start10s >> start1s;
        start = start1000s * 1000 + start100s * 100 + start10s * 10 + start1s; // start number
        cin >> end1000s >> end100s >> end10s >> end1s;
        end = end1000s * 1000 + end100s * 100 + end10s * 10 + end1s; // stop number
        cin >> numBan;
        int b1000s[numBan];
        int b100s[numBan];
        int b10s[numBan];
        int b1s[numBan];
        int banned[numBan];
        for (int j = 0; j < numBan; j++) {
            cin >> b1000s[j] >> b100s[j] >> b10s[j] >> b1s[j];
            banned[j] = b1000s[j] * 1000 + b100s[j] * 100 + b10s[j] * 10 + b1s[j];
        }
        unordered_map<int, int> D; 
        for (int i = 0; i < 10000; i++) {
            D[i] = 0;
        }
        for (int i = 0; i < numBan; i++) {
            if (start == banned[i]) {
                cout << "-1" << endl;
                skip = 1;
                break;
            }
            D.at(banned[i]) = -1;
        }
        if (skip == 1) {
            skip = 0;
            continue;
        }
        bool ans = bfs(start,end,D);
        if (ans == true) {
            cout << D.at(end) << endl;
        }
        else {
            cout << -1 << endl;
        }
    }
    return 0;
}





