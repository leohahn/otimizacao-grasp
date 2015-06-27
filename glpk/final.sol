Problem:    final
Rows:       5
Columns:    6 (6 integer, 6 binary)
Non-zeros:  14
Status:     INTEGER OPTIMAL
Objective:  weightSum = 13 (MAXimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 weightSum                  13                             
     2 softR[c1]                   0            -0               
     3 softR[c2]                  -2            -2               
     4 softR[c3]                  -1            -1               
     5 hardR[c4]                   1             1               

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 x[x1]        *              1             0             1 
     2 x[x2]        *              0             0             1 
     3 x[x3]        *              0             0             1 
     4 b[c1]        *              1             0             1 
     5 b[c2]        *              1             0             1 
     6 b[c3]        *              1             0             1 

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
