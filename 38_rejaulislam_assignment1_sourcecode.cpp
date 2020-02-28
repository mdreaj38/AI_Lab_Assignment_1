#include <bits/stdc++.h>
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
int R = 0;
int puzzle[20][20];
int dir_r[6] = {0, -1, 0, 1};
int dir_c[6] = {1, 0, -1, 0};

map<string, int> mp;
map<pair<string, int>, int> mp2;

vector<double> bfs, ucs, dls, ids, gbfs, a_star;

int online;
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
              return x.g_cost > y.g_cost;
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

void input() {  // input
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
}
void make_visit(int p[][20]) {
       string cur = "";
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cur += p[i][j] + '0';
              }
       }
       mp[cur] = 1;
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
void Breadth_First_Search(int p[][20], int R, int online) {
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
                     bfs.pb(cur.level);
                     bfs.pb(tot_node);
                     bfs.pb(((float)(clock() - time_req) / CLOCKS_PER_SEC));
                     if (online) {
                            printf("\nBFS: Level  === %d\n", cur.level);
                            cout << "Total Nodes : " << tot_node << endl;
                            cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                            //print_par(cur.state, cur.path, R);
                            cout << "------------------------------" << endl;
                     }
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

void UCS(int p[][20], int R, int online) {
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
                     ucs.pb(cur.level);
                     ucs.pb(tot_node);
                     ucs.pb(((float)(clock() - time_req) / CLOCKS_PER_SEC));
                     if (online) {
                            printf("\nUCS: Level  === %d\n", cur.level);
                            cout << "Total Nodes : " << tot_node << endl;
                            cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                            print_par(cur.state, cur.path, R);
                            cout << "------------------------------" << endl;
                     }
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

void dls_initiate(int p[][20], int R, int online) {
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
              cout << "------------------------------" << endl;

       } else {
              dls.pb(dls_ans.level);
              dls.pb(tot_node);
              dls.pb(((float)(clock() - time_req) / CLOCKS_PER_SEC));
              if (online) {
                     cout << "\nDLS: Level === " << dls_ans.level << endl;
                     cout << "Total Nodes : " << tot_node << endl;

                     cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                     print_par(dls_ans.state, dls_ans.path, R);
                     cout << "------------------------------" << endl;
              }
       }
}

void IDS(int p[][20], int R, int online) {
       cout << "\n\n\n";
       clock_t time_req = clock();
       DLS_ANS = 0;
       int f = 0;
       if (online) cout << "------------------------------" << endl;

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
              cout << "------------------------------" << endl;

       }

       else {
              ids.pb(dls_ans.level);
              ids.pb(tot_node);
              ids.pb(((float)(clock() - time_req) / CLOCKS_PER_SEC));
              if (online) {
                     cout << "\nIDLS: Level === " << dls_ans.level << endl;
                     cout << "Total Nodes : " << tot_node << endl;

                     cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                     print_par(dls_ans.state, dls_ans.path, R);
                     cout << "------------------------------" << endl;
              }
       }
}

void Greedy_Best_First_Search(int p[][20], int R, int online) {
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
              //if (is_visit(cur.state)) continue;
              make_visit(cur.state);
              if (is_goal_state(cur.state, R)) {
                     gbfs.pb(cur.level);
                     gbfs.pb(tot_node);
                     gbfs.pb(((float)(clock() - time_req) / CLOCKS_PER_SEC));
                     if (online) {
                            printf("GBFS: Level  === %d\n", cur.level);
                            cout << "Total Nodes : " << tot_node << endl;
                            cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                            print_par(cur.state, cur.path, R);
                            cout << "------------------------------" << endl;
                     }
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

void A_Star(int p[][20], int R, int online) {
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
                     a_star.pb(cur.level);
                     a_star.pb(tot_node);
                     a_star.pb(((float)(clock() - time_req) / CLOCKS_PER_SEC));
                     if (online) {
                            printf("\nA*: Level  === %d\n", cur.level);
                            cout << "Total Nodes : " << tot_node << endl;
                            cout << "Time: " << (float)(clock() - time_req) / CLOCKS_PER_SEC << endl;
                            print_par(cur.state, cur.path, R);
                            cout << "------------------------------" << endl;
                     }
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
void bfs_write() {
       freopen("bfs.txt", "w", stdout);
       for (int i = 0; i < 3; ++i) {
              if (i == 0)
                     cout << "Cost: ";
              else if (i == 1)
                     cout << "Total nodes: ";
              else
                     cout << "Time(s): ";
              for (int j = i; j < bfs.size(); j += 3) {
                     if (bfs[j] < 1)
                            cout << fixed << setprecision(8) << bfs[j] << "  ";
                     else
                            cout << (int)bfs[j] << "  ";
              }
              cout << endl;
       }
       fclose(stdout);
}
void ucs_write() {
       freopen("ucs.txt", "w", stdout);
       for (int i = 0; i < 3; ++i) {
              if (i == 0)
                     cout << "Cost: ";
              else if (i == 1)
                     cout << "Total nodes: ";
              else
                     cout << "Time(s): ";
              for (int j = i; j < ucs.size(); j += 3) {
                     if (ucs[j] < 1)
                            cout << fixed << setprecision(8) << ucs[j] << "  ";
                     else
                            cout << (int)ucs[j] << "  ";
              }
              cout << endl;
       }
       fclose(stdout);
}
void dls_write() {
       freopen("Dls.txt", "w", stdout);
       for (int i = 0; i < 3; ++i) {
              if (i == 0)
                     cout << "Cost: ";
              else if (i == 1)
                     cout << "Total nodes: ";
              else
                     cout << "Time(s): ";
              for (int j = i; j < dls.size(); j += 3) {
                     if (dls[j] < 1)
                            cout << fixed << setprecision(8) << dls[j] << "  ";
                     else
                            cout << (int)dls[j] << "  ";
              }
              cout << endl;
       }
       fclose(stdout);
}
void ids_write() {
       freopen("IDS.txt", "w", stdout);
       for (int i = 0; i < 3; ++i) {
              if (i == 0)
                     cout << "Cost: ";
              else if (i == 1)
                     cout << "Total nodes: ";
              else
                     cout << "Time(s): ";
              for (int j = i; j < ids.size(); j += 3) {
                     if (ids[j] < 1)
                            cout << fixed << setprecision(8) << ids[j] << "  ";
                     else
                            cout << (int)ids[j] << "  ";
              }
              cout << endl;
       }
       fclose(stdout);
}
void gbfs_write() {
       freopen("GBFS.txt", "w", stdout);
       for (int i = 0; i < 3; ++i) {
              if (i == 0)
                     cout << "Cost: ";
              else if (i == 1)
                     cout << "Total nodes: ";
              else
                     cout << "Time(s): ";
              for (int j = i; j < gbfs.size(); j += 3) {
                     if (gbfs[j] < 1)
                            cout << fixed << setprecision(8) << gbfs[j] << "  ";
                     else
                            cout << (int)gbfs[j] << "  ";
              }
              cout << endl;
       }
       fclose(stdout);
}
void AStar_write() {
       freopen("A_star.txt", "w", stdout);
       for (int i = 0; i < 3; ++i) {
              if (i == 0)
                     cout << "Cost: ";
              else if (i == 1)
                     cout << "Total nodes: ";
              else
                     cout << "Time(s): ";
              for (int j = i; j < a_star.size(); j += 3) {
                     if (a_star[j] < 1)
                            cout << fixed << setprecision(8) << a_star[j] << "  ";
                     else
                            cout << (int)a_star[j] << "  ";
              }
              cout << endl;
       }
       fclose(stdout);
}

void Online() {
       while (1) {
              input();

              while (1) {
                     cout << "Press \n 1 to BFS\n 2 to UCS\n 3 to DLS\n 4 to IDS\n 5 to GBFS\n 6 to A*\n 7 to simulate all algorithm \n";
                     int x;
                     cout << "\n\n No of Operation: ";
                     cin >> x;
                     if (x == 1 or x == 7) Breadth_First_Search(puzzle, R, 1);
                     if (x == 2 or x == 7) UCS(puzzle, R, 1);
                     if (x == 3 or x == 7) dls_initiate(puzzle, R, 1);
                     if (x == 4 or x == 7) IDS(puzzle, R, 1);
                     if (x == 5 or x == 7) Greedy_Best_First_Search(puzzle, R, 1);
                     if (x == 6 or x == 7) A_Star(puzzle, R, 1);
                     cout << endl;
                     cout << "\n 1 to continue searching\n 2 to continue from the beginning\n 3 to exit \n\n";
                     cin >> x;
                     if (x == 2) break;
                     if (x == 3) exit(-1);
              }
       }
}

void Offline() {
       freopen("in.txt", "r", stdin);
       for (int p = 0; p < 20; ++p) {
              cin >> R;
              for (int i = 0; i < R; ++i) {
                     for (int j = 0; j < R; ++j) {
                            cin >> puzzle[i][j];
                     }
              }
              Breadth_First_Search(puzzle, R, 0);
              UCS(puzzle, R, 0);
              Greedy_Best_First_Search(puzzle, R, 0);
              dls_initiate(puzzle, R, 0);
              IDS(puzzle, R, 0);
              A_Star(puzzle, R, 0);
              cout << "Please wait....\n";
       }
       fclose(stdin);
       bfs_write();
       ucs_write();
       dls_write();
       ids_write();
       gbfs_write();
       AStar_write();
}
void init() {
       int ch;
       cout << "1. Online\n2. Offline(Enter 20 input state in in.txt file before running this mode)\n";
       /*
       N
       NxN matrix
       */
       cin >> ch;
       if (ch == 1) {
              Online();
       } else {
              Offline();
       }
}
int main() {
       init();
}
