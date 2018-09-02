# このプログラムについて
このプログラムは、任意の正方行列を入力として、LU分解によりその逆行列を求めます。

# 使い方
1. 本プログラムをダウンロードする
2. 端末で本プログラムのディレクトリに移動
3. "make"コマンドを実行、プログラムをビルドする
4. input_matrix.txtに、以下の例のようなフォーマットで逆行列を求めたい正方行列を記述する
```
3        -> 入力する正方行列の次数
1 2 1    
2 0 1    -> 入力する正方行列
3 4 5       各成分はスペースで区切る。成分として整数/少数を設定可能
```
5. "./lu_decomposition を実行"、以下のような結果が表示され、逆行列が求められる
```
Matrix A            -> 入力された正方行列A
1 2 1
2 0 1
3 4 5

...

Inversed A Matrix   -> 入力された正方行列Aに対し、算出された逆行列
0.4000 0.6000 -0.2000
0.7000 -0.2000 -0.1000
-0.8000 -0.2000 0.4000

Check A * A_INV = A_INV * A = E ... OK -> 算出された結果が逆行列であるかを確認
```

# LU分解による逆行列の求め方
本プログラムは、以下の順序で逆行列を求めている
1. 入力された行列をAとする
2. A = PA によるピボット選択を行う
Aの成分によっては、後述のLU分解/逆行列の算出にて、0除算が発生する可能性がある (Aの全対角成分が0の時など)。
これを回避するため、ピボット選択(行の交換)を行い、行交換を行ったA (A_PV)、及び交換した行を記憶した置換行列Pを生成する
3. PA = LU を求める (LU分解)
PAを、以下の例のようにL(下三角行列)、U(上三角行列)に分解する。
<img src="https://latex.codecogs.com/gif.latex?\begin{bmatrix}&space;a11&space;&a12&space;&a13&space;\\&space;a21&space;&a22&space;&a23&space;\\&space;a31&space;&a32&space;&a33&space;\end{bmatrix}&space;=&space;\begin{bmatrix}&space;1&space;&0&space;&0&space;\\&space;l21&space;&1&space;&0&space;\\&space;l31&space;&l32&space;&1&space;\end{bmatrix}&space;\begin{bmatrix}&space;u11&space;&u12&space;&u13&space;\\&space;0&space;&u22&space;&u23&space;\\&space;0&space;&0&space;&u33&space;\end{bmatrix}" />

Aがn次の場合、LUの各成分は以下のように求める。

<img src="https://latex.codecogs.com/gif.latex?\par&space;for&space;i=1,2...n\{\par&space;\&space;\&space;for&space;j=1,2...n\{">

<img src="https://latex.codecogs.com/gif.latex?l_{ij}=\frac{a_{ij}-\sum_{k=1}^{j-1}l_{ik}u_{kj}}{u_{ij}}&space;\&space;\&space;(i>j)">

<img src="https://latex.codecogs.com/gif.latex?u_{ij}=0&space;\&space;\&space;(i>j)&space;\\">

<img src="https://latex.codecogs.com/gif.latex?l_{ij}=1&space;\&space;\&space;(i=j)&space;\\">

<img src="https://latex.codecogs.com/gif.latex?u_{ij}=a_{ij}-\sum_{k=1}^{i-1}l_{ik}u_{kj}}{u_{ij}&space;\&space;\&space;(i\leq&space;j)&space;\\">

<img src="https://latex.codecogs.com/gif.latex?l_{ij}=0&space;\&space;\&space;(i\leq&space;j)&space;\\">

<img src="https://latex.codecogs.com/gif.latex?\par&space;\&space;\&space;\}\par&space;\}">

4. Aの逆行列 = Uの逆行列 * Lの逆行列 * 置換行列P を求める (Aの逆行列の算出)
L(下三角行列)、U(上三角行列)の逆行列の各成分を、以下のように求める。

- L(下三角行列)の逆行列
<img src="https://latex.codecogs.com/gif.latex?\par&space;for&space;i=1,2...n\{">

<img src="https://latex.codecogs.com/gif.latex?\par&space;\&space;\&space;for&space;j=1,2...i-1\{">

<img src="https://latex.codecogs.com/gif.latex?l_{ij}^{'}=\frac{-\sum_{k=j}^{i-1}l_{ik}l_{kj}^{'}}{l_{ii}}">

<img src="https://latex.codecogs.com/gif.latex?\par&space;\&space;\&space;\}">

<img src="https://latex.codecogs.com/gif.latex?l_{ij}^{'}=\frac{1}{l_{ii}}">

<img src="https://latex.codecogs.com/gif.latex?\par&space;\}">

- U(上三角行列)の逆行列

<img src="https://latex.codecogs.com/gif.latex?\par&space;for&space;i=n,n-1...1\{">

<img src="https://latex.codecogs.com/gif.latex?u_{ij}^{'}=\frac{1}{u_{ii}}">

<img src="https://latex.codecogs.com/gif.latex?\par&space;\&space;\&space;for&space;j=n,n-1...i+1\{">

<img src="https://latex.codecogs.com/gif.latex?u_{ij}^{'}=\frac{-\sum_{k=i&plus;1}^{j}u_{ik}u_{kj}^{'}}{u_{ii}}">

<img src="https://latex.codecogs.com/gif.latex?\par&space;\&space;\&space;\}\par&space;\}">


Uの逆行列 * Lの逆行列 * 置換行列P から、最終的にAの逆行列が求められる。

# プログラムの内部動作

# ファイル構成
```
README.md                 : 本プログラムの解説
Makefile                  : 本プログラムのビルド用Makefile
input_matrix.txt          : 本プログラムへの入力値となる、行列の次数/成分
test_pattern              : ピボット選択が必要である入力行列等、本プログラムのテスト用行列パターンを格納。本プログラムは使用しない
lu_decomposition          : 本プログラムの実行バイナリ
src - common.h            : 本プログラムのデバッグ出力を指定可能。LU分解/逆行列の算出結果の表示、算出結果の検証実行の有無を指定可能
    - inverse_matrix.c    : 分解されたLUの逆行列を求めるプログラム
    - lu_decomposition.c  : LU分解を実施するプログラム
    - matrix.c/h          : 行列操作に関するプログラム (input_matrix.txtからの入力行列Aの読み込み、ピボット選択、行列比較など)
    - main.c              : 本プログラムのmain()プログラム
```
# TODO
- メモリ消費の削減  
行列A, 置換行列P, ピボット選択されたA、下上三角行列LP、逆行列A/L/Uそれぞれで、行列処理のため"double * n次 * n次"分のメモリ確保を行なっている。
流石にメモリを消費しすぎのため、LUを1つのn次行列にまとめる等の対応が必要
- 行列成分の分数管理  
現在、行列の各成分はdouble型で管理している。
LU分解や逆行列の算出にて、割り切れない除算が発生した場合、算出した逆行列の成分として循環小数得られる。
この場合、逆行列であるかの確認、"A * 循環小数含む逆行列A = E"が正確に求められない。
正確に求めるなら、循環小数は分数で管理すべき。
