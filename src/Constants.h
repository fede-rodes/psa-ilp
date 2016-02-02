/**
 * @file   Constants.h
 * @brief  Constant set used in PSA-ilp.
 * @author Federico Rodes
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PSA_INFEASIBLE  0
#define PSA_FEASIBLE    1
#define PSA_IMPROVE     2
#define PSA_OPTIMAL     3

#define PSA_NULL                      0
#define PSA_INSPECT_LEVEL             1
#define PSA_ADD_CANDIDATES_TO_L       2
#define PSA_EXTRACT_CANDIDATE_FROM_L  3
#define PSA_REDUCE_PROBLEM            4
#define PSA_NEW_LEVEL                 5
#define PSA_CLASSIFY_CANDIDATE        6

#define PSA_SMALL_VALUE -100000000
#define PSA_BIG_VALUE    100000000

// LP solvers.
#define PSA_GLPK    1
#define PSA_QSOPT   2
#define PSA_SOPLEX  3
#define PSA_LPSOLVE 4


#endif /* CONSTANTS_H_ */
