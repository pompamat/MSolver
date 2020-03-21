# MSolver #
MSolver is designed as an *educational tool*, aiming to show how to solve systems of linear equations by Gauss Elimination or to compute matrix determinants (with a help of Laplace Expansion). Tool may be adapted to show solutions to the problem in various contexts.

To install it, clone or download the repository and compile files `m_solver.cpp` and `determinant.cpp`. Compilation requires C++11 standard or newer.

Below we describe basic usages of MSolver with examples.

## m_solver.cpp file ##
m_solver.cpp contains an implementation of Gaussian Elimination algorithm, explainig the steps executed along the computation. Input specification is as follows:
- number of equations (E);
- number of variables (V);
- number of augmented columns (A);
- coefficient matrix (dim E x (V + A));

Number format is either an integer or a rational of the form `p/q`.
Augmented columns may contain polynomial symbolic expressions, using single-lettered variable names. Exponent is specified via `^` symbol. For example, a correct token could be `-1/2+2p^2-3/2pq`.
A single token cannot contain whitespaces to avoid ambiguity.

Example usages:

### System of Linear Equation Solver ###
To solve a linear system:
```
2x + 3y -  z = 4
 x +  y + 2z = 1
-x      + 3z = 1
```
A correct input would be:
```
3
3
1
2 3 -1 4
1 1 2 1
-1 0 3 1
```
Producing result of 
```
1 0 0  -1
0 1 0   2
0 0 1   0
```
Meaning, that `(x, y, z)` we search for is `(-1, 2, 0)`.

### Matrix rank computation ###
To compute rank of a matrix:
```
    | 2 1 -1 0 -2 |
M = | 4 3 -1 2 -4 |
    | 1 1  0 1 -1 |
    | 0 1  1 2  0 |
```
A correct input would be:
```
4
5
0
2 1 -1 0 -2
4 3 -1 2 -4
1 1 0 1 -1
0 1 1 2 0
```
Producing result of
```
1 1 0 1 -1
0 1 1 2  0
0 0 0 0  0
0 0 0 0  0
```
Meaning, that the rank of the matrix is 2.

### Matrix inverse computation ###
Compute the inverse of a matrix:
```
    |  0.25 -0.25 -0.5 |
M = |  0.25  0.75  1.5 |
    | -0.25 -0.75 -2.5 |
```
An input of a form would be apropriate:
```
3
3
3
1/4 -1/4 -1/2   1 0 0
1/4 3/4 3/2     0 1 0
-1/4 -3/4 -5/2  0 0 1
```
Producing result of a form:
```
1 0 0   3  1  0
0 1 0  -1  3  2
0 0 1   0 -1 -1
```

### Basis representation ###
To represent vectors `(1, 1, 1, 0)`, `(1, 2, 0, -1)` and `(2, 0.5, 0.5, -0.5)` in a basis consisting of `(2, 1, -1, 0)`, `(1, 1, 0, 1)`, `(0, 0, 1, -1)` and `(1, -1, 1, -1)`, we can use an input of a from:
```
4
4
3
2 1 0 1   1 1 2
1 1 0 -1  1 2 1/2
-1 0 1 1  1 0 1/2
0 1 -1 -1 0 -1 -1/2
```
yelding a result equal to
```
1 0 0 0  0  1 1/2
0 1 0 0  1  0 1/2
0 0 1 0  1  2 1/2
0 0 0 1  0 -1 1/2
```
meaning that the vectors' coordinates are (respectively) `[0, 1, 1, 0]`, `[1, 0, 2, -1]` and `[0.5, 0.5, 0.5, 0.5]`.

### Linear map kernel and image ###
Given a linear map `f` in a matrix form, compute `Ker f` and `Im f`, where
```
      | 1  0 -3 -1 |
M_f = | 2 -1 -4 -1 |
      |-1 -1  5  2 |
```
This can be solved by an input:
```
3
4
2
1 0 -3 -1  0 x
2 -1 -4 -1 0 y
-1 -1 5 2  0 z
```
Producing an output:
```
1 0 -3 -1  0      x
0 1 -2 -1  0   2x-y
0 0  0  0  0 3x-y+z
```
Since last row in the variable part is zeros and `rank(M_f) = 2`, `dim Im f = rank(M_f) = 2` and `dim Ker f = 4 - dim Im f = 2`. If last two variables are parameters, say `p` and `q`, `Ker f` is of the form `(3p + q, 2p + q, p, q)`, thus spanned by `(3, 2, 1, 0)` and `(1, 1, 0, 1)`. `Im f` is constrained by `0 = 3x-y+z`, meaning it's of a form `(x, y, y - 3x)`, thus spanned by `(1, 0, -3)` and `(0, 1, 1)`.


## determinant.cpp ##
determinant.cpp contains an implementation of mixture of Gaussian Elimination, Laplace Expansion and Sarus's Method. It supports polynomial elements of a form described in the previous section.

Input specification is as follows:
- the determinant's dimension (N);
- matrix of polynomials - determinant entries (dim N x N);

### Characteristic Polynomial ###
To compute characteristic polynomial of the matrix `M`, we compute `det (M - tI)`, so for matrix:
```
    |    1    0  0 |
M = |    1    4 10 |
    | -0.5 -1.5 -4 |
```
We can input (please remember that polynomial tokens must not contain any whitespaces)
```
3
1-t 0 0
1 4-t 10
-1/2 -3/2 -4-t
```
Yelding result (the characteristic polynomial of `M`) in a form `Wyznacznik D = -1+t+t^2-t^3` which can be decomposed using different methods (not implemented here) to a form `-(t-1)^2 (t + 1)`, which means that the eigenvalues of the matrix `M` are `1` (with corresponding eigenspace dimension of 2) and `-1`.