#include "input.h"
#include <bits/stdc++.h>
using namespace std;
int R = 0;
int puzzle[20][20];
void input() {  // input
       cin >> R;
       //puzzle[i][j] = 0 means blank space
       for (int i = 0; i < R; ++i) {
              for (int j = 0; j < R; ++j) {
                     cin >> puzzle[i][j];
              }
       }
}