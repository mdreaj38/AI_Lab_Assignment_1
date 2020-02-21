#include <bits/stdc++.h>
#include "input.h"

using namespace std;

#define pb push_back
#define ff first
#define ss second
#define read freopen("IN", "r", stdin)
#define write freopen("OUT", "w", stdout)
#define CLR(a) memset(a, 0, sizeof a)
#define dbg(a) cout << "Line " << __LINE__ << ": " << #a << " => " << a << endl

typedef long long ll;
int c = 0;

int dir_r[6] = {0, -1, 0, 1};
int dir_c[6] = {1, 0, -1, 0};

map<string, int> mp;
map<pair<string, int>, int> mp2;
int tot_node;
struct Node {
       int state[20][20];
       int action;  //0 = R,1 = U, 2 = L, 3 = D
       int level;
       int _r, _c;  // co-ordinate of empty space
       int g_cost, h_cost;
       string path = "";

} G;

struct UCS_compare {
       bool operator()(const Node& x, const Node& y) const {
              return y.g_cost < x.g_cost;
       }
};
struct GBFS_compare {
       bool operator()(const Node& x, const Node& y) const {
              return y.h_cost < x.h_cost;
       }
};

struct AStar_compare {
       bool operator()(const Node& x, const Node& y) const {
              return y.h_cost + y.g_cost < x.h_cost + x.g_cost;
       }
};

// void input() {  // input
//        cin >> R;
//        cout<<R<<endl;
//        //puzzle[i][j] = 0 means blank space
//        for (int i = 0; i < R; ++i) {
//               for (int j = 0; j < R; ++j) {
//                      cin >> puzzle[i][j];
//               }
//        }
// }
void make_visit(int p[][20]) {
       string cur = "";
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cur += p[i][j] + '0';
              }
       }
       mp[cur] = 1;
}
void make_not_visit(int p[][20]) {
       string cur = "";
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cur += p[i][j] + '0';
              }
       }
       mp[cur] = 0;
}
bool is_visit(int p[][20]) {
       string cur = "";
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cur += p[i][j] + '0';
              }
       }
       return mp[cur];
}
bool is_goal_state(int p[][20], int R) {
       string s = "", t = "";
       for (int i = 1; i < R * R; ++i) t += (i + '0');
       t += '0';
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j)
                     s += p[i][j] + '0';
       }
       return s == t;
}
bool is_valid(int x, int y) {
       return x >= 0 and x < R and y >= 0 and y < R;
}
int rev_act(int act) {
       if (act == 0) return 2;
       if (act == 2) return 0;
       if (act == 1) return 3;
       if (act == 3) return 1;
}

int misplaced_tiles(int p[][20]) {
       string s = "", t = "";
       for (int i = 1; i < R * R; ++i) t += (i + '0');
       t += '0';
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j)
                     s += p[i][j] + '0';
       }
       int count = 0;
       for (int i = 0; i < s.length(); ++i) {
              if (s[i] != t[i]) c++;
       }
       return c;
}

void p_par(int p[][20], string path, int l, int _x, int _y) {
       if (l < 0) {
              return;
       }

       int temp[20][20];
       for (int i = 0; i < R; ++i)
              for (int j = 0; j < R; ++j)
                     temp[i][j] = p[i][j];

       int act = rev_act(path[l] - '0');
       int n_x = _x + dir_r[act], n_y = _y + dir_c[act];
       swap(temp[n_x][n_y], temp[_x][_y]);
       p_par(temp, path, l - 1, n_x, n_y);
       //0 = R,1 = U, 2 = L, 3 = D
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cout << "      " << temp[i][j];
              }
              cout << endl;
       }
       cout << "\n\n";
       cout << "Action: ";
       if (path[l] == '0')
              cout << "Right\n";
       else if (path[l] == '1')
              cout << "Up\n";
       else if (path[l] == '2')
              cout << "Left\n";
       else
              cout << "Down\n";
}
void print_par(int p[][20], string path, int R) {
       p_par(p, path, path.length() - 1, R - 1, R - 1);
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cout << "     " << p[i][j];
              }
              cout << endl;
       }
}
void Breadth_First_Search(int p[][20], int R) {
       clock_t time_req = clock();
       cout << "\n\n\n";
       mp.clear();
       int c = 0, f = 0;
       for (int i = 0; i < R; ++i)
              for (int j = 0; j < R; ++j) {
                     G.state[i][j] = p[i][j];
                     if (!p[i][j]) G._r = i, G._c = j;
              }
       queue<Node> q;
       tot_node = 1;
       q.push(G);
       while (!q.empty()) {
              Node cur = q.front();
              q.pop();
              if (is_visit(cur.state)) continue;
              make_visit(cur.state);

              if (is_goal_state(cur.state, R)) {
                     printf("\nBFS: Level  === %d\n", cur.level);
                     cout << "Total Nodes : " << tot_node << endl;
                     cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                     print_par(cur.state, cur.path, R);
                     cout << "------------------------------" << endl;

                     return;
              }
              //0 = R,1 = U, 2 = L, 3 = D
              for (int i = 0; i < 4; ++i) {
                     int n_r = cur._r + dir_r[i];
                     int n_c = cur._c + dir_c[i];
                     if (is_valid(n_r, n_c)) {
                            Node temp = cur;
                            swap(temp.state[n_r][n_c], temp.state[temp._r][temp._c]);
                            if (!is_visit(temp.state)) {
                                   temp.level = cur.level + 1;
                                   temp.action = i;
                                   temp._r = n_r;
                                   temp._c = n_c;
                                   temp.path += (i + '0');
                                   tot_node++;
                                   q.push(temp);
                            }
                     }
              }
       }
       cout << "BFS: No Solution\n";
       cout << "Total Nodes : " << tot_node << endl;
       cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
       cout << "------------------------------" << endl;

}
void UCS(int p[][20], int R) {
       cout << "\n\n\n";
       clock_t time_req = clock();
       mp.clear();
       int f = 0;
       for (int i = 0; i < R; ++i)
              for (int j = 0; j < R; ++j) {
                     G.state[i][j] = p[i][j];
                     if (!p[i][j]) G._r = i, G._c = j;
              }
       priority_queue<Node, vector<Node>, UCS_compare> q;
       q.push(G);
       tot_node = 1;
       while (!q.empty()) {
              Node cur = q.top();
              q.pop();
              if (is_visit(cur.state)) continue;
              make_visit(cur.state);
              if (is_goal_state(cur.state, R)) {
                     printf("\nUCS: Level  === %d\n", cur.level);
                     cout << "Total Nodes : " << tot_node << endl;
                     cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                     print_par(cur.state, cur.path, R);
                     cout << "------------------------------" << endl;


                     return;
              }
              //0 = R,1 = U, 2 = L, 3 = D
              for (int i = 0; i < 4; ++i) {
                     int n_r = cur._r + dir_r[i];
                     int n_c = cur._c + dir_c[i];
                     if (is_valid(n_r, n_c)) {
                            Node temp = cur;
                            swap(temp.state[n_r][n_c], temp.state[temp._r][temp._c]);
                            if (!is_visit(temp.state)) {
                                   tot_node++;
                                   temp.level = cur.level + 1;
                                   temp.action = i;
                                   temp._r = n_r;
                                   temp._c = n_c;
                                   temp.g_cost = (cur.g_cost) + 1;
                                   temp.path += (i + '0');
                                   q.push(temp);
                            }
                     }
              }
       }
       cout << "UCS: No Solution\n";
       cout << "Total Nodes : " << tot_node << endl;
       cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
       cout << "------------------------------" << endl;


       return;
}
int DLS_ANS;
struct Node dls_ans;
void make_visitDLS(int p[][20], int l) {
       string cur = "";
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cur += p[i][j] + '0';
              }
       }
       cur += '0';
       mp2[{cur, l}] = 1;
}
bool is_visitDLS(int p[][20], int l) {
       string cur = "";
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cur += p[i][j] + '0';
              }
       }
       cur += '0';
       return mp2[{cur, l}];
}
void DLS(Node cur, int limit) {
       if (DLS_ANS or limit < 0 or is_visitDLS(cur.state, cur.level)) return;
       if (is_goal_state(cur.state, R)) {
              DLS_ANS = 1;
              dls_ans = cur;
              return;
       }
       make_visitDLS(cur.state, cur.level);
       tot_node++;
       for (int i = 0; i < 4; ++i) {
              int n_r = cur._r + dir_r[i];
              int n_c = cur._c + dir_c[i];

              if (!is_valid(n_r, n_c)) continue;
              Node temp = cur;
              swap(temp.state[n_r][n_c], temp.state[temp._r][temp._c]);
              temp.level = cur.level + 1;
              temp.action = i;
              temp._r = n_r;
              temp._c = n_c;
              temp.level = cur.level + 1;
              temp.path += (i + '0');
              DLS(temp, limit - 1);
       }
       //make_not_visit(cur.state);
}

void dls_initiate(int p[][20], int R) {
       cout << "\n\n\n";

       clock_t time_req = clock();
       mp2.clear();
       int f = 0;
       for (int i = 0; i < R; ++i)
              for (int j = 0; j < R; ++j) {
                     G.state[i][j] = p[i][j];
                     if (!p[i][j]) G._r = i, G._c = j;
              }
       DLS_ANS = 0;
       tot_node = 0;
       DLS(G, 30);
       if (!DLS_ANS) {
              cout << "DLS: NO Solution\n";
              cout << "Total Nodes : " << tot_node << endl;
              cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;

       } else {
              cout << "\nDLS: Level === " << dls_ans.level << endl;
              cout << "Total Nodes : " << tot_node << endl;

              cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
              print_par(dls_ans.state, dls_ans.path, R);
       }
       cout << "------------------------------" << endl;

}
void IDS(int p[][20], int R) {
       cout << "\n\n\n";
       clock_t time_req = clock();
       DLS_ANS = 0;
       int f = 0;
       cout << "------------------------------" << endl;

       for (int i = 0; i < R; ++i)
              for (int j = 0; j < R; ++j) {
                     G.state[i][j] = p[i][j];
                     if (!p[i][j]) G._r = i, G._c = j;
              }

       G.level = 0;
       DLS_ANS = 0;
       tot_node = 0;
       for (int i = 1; DLS_ANS == 0; ++i) {
              mp2.clear();
              DLS(G, i);
       }
       if (!DLS_ANS) {
              cout << "IDL   S: No_Solution\n";
              cout << "Total Nodes : " << tot_node << endl;
              cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
       }

       else {
              cout << "\nIDLS: Level === " << dls_ans.level << endl;
              cout << "Total Nodes : " << tot_node << endl;

              cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
              print_par(dls_ans.state, dls_ans.path, R);
       }
       cout << "------------------------------" << endl;

}
void Greedy_Best_First_Search(int p[][20], int R) {
       cout << "\n\n\n";

       clock_t time_req = clock();
       mp.clear();
       int f = 0;
       for (int i = 0; i < R; ++i)
              for (int j = 0; j < R; ++j) {
                     G.state[i][j] = p[i][j];
                     if (!p[i][j]) G._r = i, G._c = j;
              }
       G.h_cost = misplaced_tiles(G.state);
       priority_queue<Node, vector<Node>, GBFS_compare> q;
       tot_node = 1;
       q.push(G);

       while (!q.empty()) {
              Node cur = q.top();
              q.pop();
              if (is_visit(cur.state)) continue;
              make_visit(cur.state);
              if (is_goal_state(cur.state, R)) {
                     printf("GBFS: Level  === %d\n", cur.level);
                     cout << "Total Nodes : " << tot_node << endl;
                     cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                     print_par(cur.state, cur.path, R);
                     cout << "------------------------------" << endl;


                     return;
              }
              //0 = R,1 = U, 2 = L, 3 = D
              for (int i = 0; i < 4; ++i) {
                     int n_r = cur._r + dir_r[i];
                     int n_c = cur._c + dir_c[i];
                     if (is_valid(n_r, n_c)) {
                            Node temp = cur;
                            swap(temp.state[n_r][n_c], temp.state[temp._r][temp._c]);
                            if (!is_visit(temp.state)) {
                                   tot_node++;
                                   temp.level = cur.level + 1;
                                   temp.action = i;
                                   temp._r = n_r;
                                   temp._c = n_c;
                                   temp.h_cost = misplaced_tiles(temp.state);
                                   temp.path += (i + '0');
                                   q.push(temp);
                            }
                     }
              }
       }
       cout << "GBFS: No_Solution\n";
       cout << "Total Nodes : " << tot_node << endl;
       cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
       cout << "------------------------------" << endl;

}
void A_Star(int p[][20], int R) {
       cout << "\n\n\n";

       clock_t time_req = clock();
       mp.clear();
       int f = 0;
       for (int i = 0; i < R; ++i)
              for (int j = 0; j < R; ++j) {
                     G.state[i][j] = p[i][j];
                     if (!p[i][j]) G._r = i, G._c = j;
              }
       G.h_cost = misplaced_tiles(G.state);
       priority_queue<Node, vector<Node>, AStar_compare> q;
       tot_node = 1;
       q.push(G);

       while (!q.empty()) {
              Node cur = q.top();
              q.pop();
              if (is_visit(cur.state)) continue;
              make_visit(cur.state);
              if (is_goal_state(cur.state, R)) {
                     printf("\nA*: Level  === %d\n", cur.level);
                     cout << "Total Nodes : " << tot_node << endl;
                     cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                     print_par(cur.state, cur.path, R);
                     cout << "------------------------------" << endl;


                     return;
              }
              //0 = R,1 = U, 2 = L, 3 = D
              for (int i = 0; i < 4; ++i) {
                     int n_r = cur._r + dir_r[i];
                     int n_c = cur._c + dir_c[i];
                     if (is_valid(n_r, n_c)) {
                            Node temp = cur;
                            swap(temp.state[n_r][n_c], temp.state[temp._r][temp._c]);
                            if (!is_visit(temp.state)) {
                                   tot_node++;
                                   temp.level = cur.level + 1;
                                   temp.action = i;
                                   temp._r = n_r;
                                   temp._c = n_c;
                                   temp.g_cost = cur.g_cost + 1;
                                   temp.h_cost = misplaced_tiles(temp.state);
                                   temp.path += (i + '0');
                                   q.push(temp);
                            }
                     }
              }
       }
       cout << "A*: No Solution\n ";
       cout << "Total Nodes : " << tot_node << endl;

       cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
       cout << "------------------------------" << endl;

}

void init() {
       while (1) {
              double N;
              cout << "Enter N of N-puzzle: ";
              cin >> N;
              N++;
              R = (int)(sqrt(N));
              for (int i = 0; i < R; ++i) {
                     for (int j = 0; j < R; ++j) {
                            cin >> puzzle[i][j];
                     }
              }
              while (1) {
                     cout << "Press \n 1 to BFS\n 2 to UCS\n 3 to DLS\n 4 to IDS\n 5 to GBFS\n 6 to A*\n 7 to simulate all algorithm \n";
                     int x;
                     cout << "\n\n No of Operation: ";
                     cin >> x;
                     if (x == 1 or x == 7) Breadth_First_Search(puzzle, R);
                     if (x == 2 or x == 7) UCS(puzzle, R);
                     if (x == 3 or x == 7) dls_initiate(puzzle, R);
                     if (x == 4 or x == 7) IDS(puzzle, R);
                     if (x == 5 or x == 7) Greedy_Best_First_Search(puzzle, R);
                     if (x == 6 or x == 7) A_Star(puzzle, R);
                     cout << endl;
                     cout << "\n 1 to continue searching\n 2 to continue from the beginning\n 3 to exit \n\n";
                     cin >> x;
                     if (x == 2) break;
                     if (x == 3) exit(-1);
              }
       }
}

int main() {
       //  read;
       // write;
       //input();
       init();
}
//g++ AiSearch.cpp input.cpp && ./a.out
