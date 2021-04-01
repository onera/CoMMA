//
// Created by Nicolas Lantos on 10/10/2017.
//

#ifndef UNTITLED_AGGLOMERATOR_H
#define UNTITLED_AGGLOMERATOR_H


bool computeAnisotropicLine(long* sizes,
                           long * adjMatrix_row_ptr,
                           long *adjMatrix_col_ind,
                           double * adjMatrix_areaValues,
                           long * arrayOfFineAnisotropicCompliantCells,
                           long * agglomerationLines_Idx,
                           long * agglomerationLines,
                           bool verbose);

void agglomerate_Anisotropic_One_Level_without_list_lines(long *sizes,
                                                          long *fineAgglomerationLines_array_Idx, long *fineAgglomerationLines_array,
                                                          long *fineCellToCoarseCell,
                                                          bool *isFineCellAgglomerated,
                                                          long *AnisotropicCompliantCoarseCells_array);



#endif //UNTITLED_AGGLOMERATOR_H
