#
# Problema de otimização: Weighted Partial MaxSAT
#

set C;   # Cláusulas(j)
set V;  # Variáveis(i)

##weight of each clause j
param w{j in C};

##indicates if the clause j is soft
param soft{j in C} binary;

param cla {j in C, i in V} integer;
param claN {j in C, i in V} integer;
##vector containing assigned values for each variable
var x{i in V} binary;


##indicates if clause j was satisfied
var c{j in C} binary;

maximize pesoSoft: sum{j in C} w[j]*soft[j]*c[j];

s.t. clausSat{j in C}: sum{i in V} (cla[j,i] * x[i] + claN[j,i] * xN[i] ) >= (1 - soft[j]);
s.t. clausBin{j in C}: c[j] <= sum{i in V} (cla[j,i] * x[i] + claN[j,i] * xN[i] );
s.t. negVar{i in V}  : x[i] + xN[i] = 1;

/**
 *  Teste: (x1 V x2) ^ (~x1 V ~x2) ^ (x2 V ~x3) ^ (x1 V x3)
 *
 */
data;
set C := c1 c2 c3 c4;
set V := 0 1 2;

param cla :     0 1 2  :=
             c1  1  1  0
             c2  0  0  1
             c3  0  1  0
             c4  1  0  1;

param claN :     0 1 2  :=
             c1  0  0  0
             c2  1  0  0
             c3  0  0  1
             c4  0  0  0;

param w:=    c1 3
             c2 4
             c3 6
             c4 2;

param soft:=  c1 1
              c2 0
              c3 0
              c4 1;
end;
