Problem:    final
Rows:       20
Columns:    29 (29 integer, 29 binary)
Non-zeros:  74
Status:     INTEGER EMPTY
Objective:  weightSum = 0 (MAXimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 weightSum                   0                             
     2 softR[0]                    0             2               
     3 softR[1]                    0            -4               
     4 softR[2]                    0             2               
     5 softR[3]                    0            -4               
     6 softR[4]                    0             2               
     7 softR[5]                    0            -4               
     8 softR[6]                    0             2               
     9 softR[7]                    0            -4               
    10 softR[10]                   0             2               
    11 softR[11]                   0            -4               
    12 softR[12]                   0             2               
    13 softR[13]                   0            -4               
    14 softR[14]                   0             2               
    15 softR[15]                   0            -4               
    16 softR[16]                   0             2               
    17 softR[17]                   0            -4               
    18 softR[18]                   0             1               
    19 hardR[8]                    0             3               
    20 hardR[9]                    0            -3               

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 x[1]         *              0             0             1 
     2 x[2]         *              0             0             1 
     3 x[3]         *              0             0             1 
     4 x[4]         *              0             0             1 
     5 x[5]         *              0             0             1 
     6 x[7]         *              0             0             1 
     7 x[8]         *              0             0             1 
     8 x[12]        *              0             0             1 
     9 x[17]        *              0             0             1 
    10 x[21]        *              0             0             1 
    11 x[31]        *              0             0             1 
    12 x[33]        *              0             0             1 
    13 b[0]         *              0             0             1 
    14 b[1]         *              0             0             1 
    15 b[2]         *              0             0             1 
    16 b[3]         *              0             0             1 
    17 b[4]         *              0             0             1 
    18 b[5]         *              0             0             1 
    19 b[6]         *              0             0             1 
    20 b[7]         *              0             0             1 
    21 b[10]        *              0             0             1 
    22 b[11]        *              0             0             1 
    23 b[12]        *              0             0             1 
    24 b[13]        *              0             0             1 
    25 b[14]        *              0             0             1 
    26 b[15]        *              0             0             1 
    27 b[16]        *              0             0             1 
    28 b[17]        *              0             0             1 
    29 b[18]        *              0             0             1 

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 3.00e+00 on row 19
        max.rel.err = 7.50e-01 on row 19
        SOLUTION IS INFEASIBLE

End of output
