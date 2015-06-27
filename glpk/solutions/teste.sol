Problem:    wpmaxsat
Rows:       12
Columns:    10 (10 integer, 10 binary)
Non-zeros:  29
Status:     INTEGER OPTIMAL
Objective:  pesoSoft = 13 (MAXimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 pesoSoft                   13                             
     2 clausSat[c0]                1            -0               
     3 clausSat[c1]                1            -0               
     4 clausSat[c2]                1            -0               
     5 clausSat[c3]                1             1               
     6 clausBin[c0]                0                          -0 
     7 clausBin[c1]                0                          -0 
     8 clausBin[c2]                0                          -0 
     9 clausBin[c3]               -1                          -0 
    10 negVar[1]                   1             1             = 
    11 negVar[2]                   1             1             = 
    12 negVar[3]                   1             1             = 

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 x[1]         *              1             0             1 
     2 x[2]         *              0             0             1 
     3 x[3]         *              0             0             1 
     4 xN[1]        *              0             0             1 
     5 xN[2]        *              1             0             1 
     6 xN[3]        *              1             0             1 
     7 c[c0]        *              1             0             1 
     8 c[c1]        *              1             0             1 
     9 c[c2]        *              1             0             1 
    10 c[c3]        *              0             0             1 

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
