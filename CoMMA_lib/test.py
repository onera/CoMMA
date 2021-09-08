from CoMMA import *
import numpy as np
nb_fc = 15
adjMatrix_col_ind_size = 51
adjMatrix_row_ptr= np.array([0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51])
adjMatrix_col_ind= np.array([0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                                  7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,14])
adjMatrix_areaValues=np.array([6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
                                       6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
                                       2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
                                       3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
                                       3.16227766, 7.])
volumes = np.array([2., 1., 2., 1., 2., 1., 2., 1., 0.5, 2., 1., 3., 3., 3., 1.5])
isOnBnd = np.array([1, 1, 0, 1, 1,
                        0, 0, 0, 0, 0,
                        0, 0, 2, 1, 2])
array_isOnRidge=np.array([0, 1, 3, 4, 13])
array_isOnValley=np.array([12,14])
array_isOnCorner=np.array([])
fc_to_cc = np.full((1, nb_fc), -1)
indCoarseCell = 0
minCard = -1
goalCard = -1
maxCard = -1
checks = 1
verbose = 0
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc)
agglomerationLines_Idx = np.zeros(nb_fc)
agglomerationLines = np.zeros(nb_fc)
isFirstAgglomeration = 1
isAnisotropic = 0
dimension = 2
is_basic_or_triconnected = 0


agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
                          arrayOfFineAnisotropicCompliantCells,

                          isOnBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomerationLines_Idx,
                          agglomerationLines,

                          is_basic_or_triconnected,
                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose)

