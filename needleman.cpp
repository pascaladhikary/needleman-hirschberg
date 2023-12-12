#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <stack>
#include <chrono>
#include <iomanip>

#define MAX_N 1001 // limitation of this implementation is contig memory 

using namespace std;

int n, m;
int match_score, mismatch_score, gap_score;
string A, B;
int dp[MAX_N][MAX_N];

inline int needleman_wunsch()
{
    for (int i=0; i <= n; i++)
         dp[i][0] = dp[0][i] = -i * gap_score;
    for (int i=1; i<=n; i++) {
        for (int j=1; j<=m; j++) {
            int S = (A[i-1] == B[j-1]) ? match_score : -mismatch_score;
            dp[i][j] = max(dp[i-1][j-1] + S, max(dp[i-1][j] - gap_score, dp[i][j-1] - gap_score));
        }
    }
    return dp[n][m];
}

inline pair<string, string> get_optimal_alignment() {
    string retA, retB;
    stack<char> SA, SB;
    int ii = n, jj = m;
    while (ii != 0 || jj != 0) {
        if (ii == 0) {
            SA.push('-');
            SB.push(B[jj-1]);
            jj--;
        } else if (jj == 0) {
            SA.push(A[ii-1]);
            SB.push('-');
            ii--;
        }
        else {
            int S = (A[ii-1] == B[jj-1]) ? match_score : -mismatch_score;
            if (dp[ii][jj] == dp[ii-1][jj-1] + S) {
                SA.push(A[ii-1]);
                SB.push(B[jj-1]);
                ii--; jj--;
            } else if (dp[ii-1][jj] > dp[ii][jj-1]) {
                SA.push(A[ii-1]);
                SB.push('-');
                ii--;
            } else {
                SA.push('-');
                SB.push(B[jj-1]);
                jj--;
            }
        }
    }
    while (!SA.empty()) {
        retA += SA.top();
        retB += SB.top();
        SA.pop();
        SB.pop();
    }
    return make_pair(retA, retB);
}

int main() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    match_score = 2, mismatch_score = 1, gap_score = 1;
    freopen("input.txt", "r", stdin);
    A = "";
    B = "";
    std::cin >> A >> B;
    n = A.size(), m = B.size();

    auto t1 = high_resolution_clock::now();
    needleman_wunsch();
    pair<string, string> align = get_optimal_alignment();
    auto t2 = high_resolution_clock::now();
    
    cout << align.first << "\n" << align.second << "\n";

    duration<double, std::milli> ms_double = t2 - t1;
    std::setprecision(4); std::cout << ms_double.count() << "ms\n";
    return 0;
}