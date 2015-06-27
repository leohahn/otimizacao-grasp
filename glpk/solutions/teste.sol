Problem:    final
Rows:       5
Columns:    6 (6 integer, 6 binary)
Non-zeros:  14
Status:     INTEGER OPTIMAL
Objective:  weightSum = 9 (MAXimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 weightSum                   9                             
     2 softR[0]                    3             2               
     3 softR[1]                   -4            -4               
     4 softR[2]                   -1            -1               
     5 hardR[3]                    4             3               

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 x[1]         *              1             0             1 
     2 x[2]         *              1             0             1 
     3 x[3]         *              1             0             1 
     4 b[0]         *              1             0             1 
     5 b[1]         *              0             0             1 
     6 b[2]         *              1             0             1 

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
