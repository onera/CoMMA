"""/!\ WARNING /!\
I was not able to reconstruct the cell centers, so this test could be used in the newest CoMMA version"""
from CoMMA import *
import numpy as np
nb_fc = 15
adjMatrix_col_ind_size = 51
adjMatrix_row_ptr= np.array([0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51] , dtype='long')
adjMatrix_col_ind= np.array([0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,14],dtype='long')
adjMatrix_areaValues=np.array([6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,3.16227766, 7.],dtype='double')
volumes = np.array([2., 1., 2., 1., 2., 1., 2., 1., 0.5, 2., 1., 3., 3., 3., 1.5],dtype='double')
weights = np.arange(start = nb_fc-1, stop = 0, step = -1, dtype = 'double')
isOnBnd = np.array([1, 1, 0, 1, 1,0, 0, 0, 0, 0,0,0, 2, 1, 2])
array_isOnRidge=np.array([0, 1, 3, 4, 13])
array_isOnValley=np.array([12,14])
array_isOnCorner=np.array([],dtype='long')
fc_to_cc = np.full(nb_fc, -1,dtype='long')
indCoarseCell = 0
minCard = -1
goalCard = -1
maxCard = -1
checks = 1
verbose = 0
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc,dtype='long')
agglomerationLines_Idx = np.zeros(nb_fc,dtype='long')
agglomerationLines = np.zeros(nb_fc,dtype='long')
isFirstAgglomeration = True
isAnisotropic = False
dimension = 2
corrections = True
threshold_anisotropy = 4.
iso_agglo_type = 0


fc_to_cc_res,agglomerationLines_Idx_res,agglomerationLines_res=agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,weights,arrayOfFineAnisotropicCompliantCells,isOnBnd,array_isOnValley,array_isOnRidge,array_isOnCorner,isFirstAgglomeration,isAnisotropic,threshold_anisotropy,fc_to_cc,agglomerationLines_Idx,agglomerationLines,corrections,dimension,goalCard,minCard,maxCard,iso_agglo_type)

print(fc_to_cc_res)
