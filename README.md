# Simplex
A simple implementation of the simplex algorithm I did as a project in university.

This does not provide a numerically stable implementation in any way as it uses doubles as variables and does not use any technique to get around any imprecision that naturally occurs in this type of problem. However, it is effective in providing the backbone of the algorithm where every entry is expected to be a maximization problem with restrictions of the form $Ax \leq b$.

To compile the code, type "make" in the project directory. A valid entry needs to provide the dimensions of the matrix in the first line, followed by the coefficients of the linear function to be optimized. Finally, the linear system corresponds to the restrictions where every line contains the coefficients of the linear inequality and the upper bound.
