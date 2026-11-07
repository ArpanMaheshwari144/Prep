## PATTERN 5 — MATRIX

```
> ★ **YAHAN NAHI MILENGI** (grid dikhti hai, par technique alag hai):
> ```
> number of islands · rotting oranges · max area of island   ->  PATTERN 13 (GRAPHS)
>       grid hai, par kaam BFS/DFS ka hai — visit + mark + neighbors
> search 2D matrix                                           ->  PATTERN 7 (BINARY SEARCH)
>       naam me matrix hai, par rows sorted hain -> ek lambi sorted array maan lo
> ```


 BROAD IDEA: 2D grid pe index-navigation (boundary/direction careful; copy-pen pe grid banao).

── (STANDALONE — har ek apni index-navigation) ──

 KAB: 2D matrix pe index ka khel: spiral, transpose, set zeroes, row / col ginti
```

### ▸ SET MATRIX ZEROES

```
m x n matrix di hai. Jis cell me 0 hai, uski poori row aur poora column 0 kar do. Isi matrix me badlaav karna hai (in-place).
  e.g. [[1,1,1],[1,0,1],[1,1,1]] -> [[1,0,1],[0,0,0],[1,0,1]]
  kyun: 0 cell (1,1) pe -> row 1 aur col 1 poore 0, baaki same
```

```cpp
void setZeroes(vector<vector<int>> &matrix) {   // in-place, 2 marker array, O(m+n) space
    int row = matrix.size();
    int col = matrix[0].size();
    vector<int> zeroRow(row, -1);           // -1 = unmarked
    vector<int> zeroCol(col, -1);
    for (int i = 0; i < row; i++)           // ★ pass-1: sirf MARK karo
        for (int j = 0; j < col; j++)
            if (matrix[i][j] == 0) {        // 2 pass: ek hi pass me 0 kiya to naye 0 bhi mark ho jaate
                zeroRow[i] = 0;
                zeroCol[j] = 0;
            }
    for (int i = 0; i < row; i++)           // pass-2: marked ho to 0
        for (int j = 0; j < col; j++)
            if (zeroRow[i] == 0 || zeroCol[j] == 0)   // || : row ya col koi ek bhi marked -> 0
                matrix[i][j] = 0;
}
```

### ▸ SPIRAL MATRIX (LC-54)  = 4 boundary andar simat-te (clockwise)

```
m x n matrix di hai. Uske saare numbers spiral kram me (bahar se andar ki taraf, ghadi ki sui ki disha me) ek list me lautao.
  e.g. [[1,2,3],[4,5,6],[7,8,9]] -> [1,2,3,6,9,8,7,4,5]
  kyun: upar 1,2,3 -> right col 6,9 -> neeche ulta 8,7 -> left col upar 4 -> beech 5
```

```
TASVEER:

GUARD — jo loop jis row/col pe chal raha, guard USI ka:
   3rd loop  BOTTOM row chhaap raha  ->  if (top <= bottom)
             matlab bottom abhi bhi top se NEECHE/barabar hai -> bottom row bachi hai -> chhaapo
   4th loop  LEFT col chhaap raha    ->  if (left <= right)
             matlab left abhi bhi right se PEHLE/barabar hai -> left col bachi hai -> chhaapo
   1st/2nd loop (top row, right col) pe guard NAHI -> while(top<=bottom && left<=right) bacha leta

GUARD — kyun?
   sawaal: "jo row / col ab chhaapne ja raha, wo ABHI BACHI hai?"
   ek hi row ya ek hi col bachi ho to ulti disha wala loop WAHI dobara chhaap deta

   [[1,2,3]]  ek row
      top row:  1 2 3   ->  top++  ->  top=1, bottom=0
      right col: (i=1..0) kuch nahi  ->  right--  ->  right=1
      bottom row?  top <= bottom ?  1 <= 0  NO  -> skip   (warna 2 1 dobara)

   [[1],[2],[3]]  ek col
      top row: 1  ->  top=1  ·  right col: 2 3  ->  right--  ->  left=0, right=-1
      bottom row: guard paas, par (i=-1..0) kuch nahi  ->  bottom=1
      left col?    left <= right ?  0 <= -1 NO  -> skip   (warna 2 dobara)
```

```cpp
vector<int> spiralOrder(vector<vector<int>> &matrix) {
    int row = matrix.size();
    int col = matrix[0].size();
    int top = 0, bottom = row - 1, left = 0, right = col - 1;   // -1 : inclusive, isliye loop <=
    vector<int> ans;
    while (top <= bottom && left <= right) {   // <= : ek akeli row / col bachi ho tab bhi chhaapni hai
        for (int i = left; i <= right; i++)
            ans.push_back(matrix[top][i]);  // top row L->R : [top][i]
        top++;                              // turant: right col wala loop ye corner dobara na le
        for (int i = top; i <= bottom; i++)
            ans.push_back(matrix[i][right]);   // right col T->B : [i][right]
        right--;                            // turant: bottom row wala loop ye corner dobara na le
        if (top <= bottom)                  // ★ GUARD: bottom row bachi? (ek hi row -> ulti dobara)
            for (int i = right; i >= left; i--)
                ans.push_back(matrix[bottom][i]);   // bottom row R->L : [bottom][i]
        bottom--;
        if (left <= right)                  // ★ GUARD: left col bachi? (ek hi col -> dobara)
            for (int i = bottom; i >= top; i--)
                ans.push_back(matrix[i][left]);     // left col B->T : [i][left]
        left++;
    }
    return ans;
}
// guard sirf 3rd / 4th loop pe: 1st / 2nd ko while(top<=bottom && left<=right) bacha leta
// [[1,2,3]]: top row 1 2 3 -> top=1 > bottom=0 -> bottom row skip (warna 2 1 dobara)
// [[1],[2],[3]]: right col ke baad right=-1 < left=0 -> left col skip (warna 2 dobara)
```


### ▸ TRANSPOSE

```
m x n matrix di hai. Uska transpose lautao, matlab row ko column bana do: matrix[i][j] naye matrix me res[j][i] pe jaayega (naya size n x m).
  e.g. [[1,2,3],[4,5,6]] -> [[1,4],[2,5],[3,6]]
  kyun: pehli row 1,2,3 ban gayi pehla col; 2x3 se 3x2
```

```cpp
vector<vector<int>> transpose(vector<vector<int>> &matrix) {
    int row = matrix.size();
    int col = matrix[0].size();
    vector<vector<int>> ans(col, vector<int>(row, -1));   // ★ naya col x row size (non-square bhi)
    for (int i = 0; i < col; i++)           // outer i < col
        for (int j = 0; j < row; j++)       // inner j < row
            ans[i][j] = matrix[j][i];
    return ans;
}
```

### ▸ COUNT SERVERS THAT COMMUNICATE (LC-1267, JP)  = SET MATRIX ZEROES ka cousin (row/col COUNT 2-pass)

```
Ek grid di hai jisme 1 = server. Do server baat kar sakte hain agar wo ek hi row ya ek hi column me hon.
Kitne server kisi doosre server se baat kar sakte hain, wo ginti lautao.
  e.g. [[1,1,0,0],[0,0,1,0],[0,0,1,1]] -> 5
  kyun: (0,0)-(0,1) row 0, (1,2)-(2,2) col 2, (2,3)-(2,2) row 2 -> paanchon ka saathi = 5

1 = server. communicate = uski ROW ya COL me koi DOOSRA server. row + col dono me akela -> nahi gino
[[1,0],[1,1]] -> 3  ·  [[1,0],[0,1]] -> 0
```

```
TASVEER:

DRY-RUN [[1,0],[1,1]]:
    pass-1:  rowCount=[1,2]  colCount=[2,1]
    (0,0): row0=1, col0=2>1 -> YES
    (1,0): row1=2>1          -> YES
    (1,1): row1=2>1          -> YES     => 3
```

```cpp
int countServers(vector<vector<int>> &grid) {   // set zeroes ki 2 marker trick, MARK ki jagah COUNT
    int m = grid.size();
    int n = grid[0].size();
    vector<int> rowCount(m, 0);             // 0 se: abhi kisi row / col me koi server gina nahi
    vector<int> colCount(n, 0);
    for (int i = 0; i < m; i++)             // pass-1: poori ginti, tabhi pass-2 me pata akela ya nahi
        for (int j = 0; j < n; j++)
            if (grid[i][j] == 1) {
                rowCount[i]++;
                colCount[j]++;
            }
    int count = 0;
    for (int i = 0; i < m; i++)             // pass-2: check
        for (int j = 0; j < n; j++)
            if (grid[i][j] == 1 && (rowCount[i] > 1 || colCount[j] > 1))   // ★ >1 : khud ke alawa koi aur
                count++;                    // || : row ya col koi ek kaafi
    return count;                           // O(m*n)
}
```


