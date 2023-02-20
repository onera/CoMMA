#ifndef COMMA_PROJECT_TEST_UTIL_H
#define COMMA_PROJECT_TEST_UTIL_H

/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

/** @brief Convert string to the type of the template parameter
 * @tparam T Type to which the string will be converted to
 * @param[in] str The string to convert
 * @return the string converted into the given type
 * @notes Taken from here https://gist.github.com/mark-d-holmberg/862733
 */
template <typename T>
inline T convert_to(const string &str) {
    istringstream ss(str);
    T num;
    ss >> num;
    return num;
}

/** @brief Read a mesh, that is, the CSR graph and info about cell volumes, centers
 * and boundary cells. In total, 6 vectors are read and returned. All the vectors
 * taken in input will be resized to accommodate the data. The following format is
 * expected:
 * - The first line of the file is the dimension, 2 or 3
 * - The data is read in the same order as the input arguments, that is, rows,
 *   columns, and values for the CSR graph, volumes, number of boundary faces, and
 *   centers.
 * - The data of each block is arranged in one line only, even for centers. The
 *   function reads always 6 lines: if less, output data may hold garbage; additional
 *   lines are discarded.
 * - Elements inside the same data block are separated by commas
 * - The reading of a line stops when the line ends or when the expected size of the
 *   current data block is reached. That means that if the data block contains less
 *   element than expected the vector might holds garbage data; if more elements than
 *   expected are found on the line, they will be discarded.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 * @param[in] fname Name of the file to read
 * @param[out] CSR_row The row pointer of the CRS representation
 * @param[out] CSR_col The column index of the CRS representation
 * @param[out] CSR_val The values of the CRS representation
 * @param[out] volumes The volumes of the cells
 * @param[out] n_bnd_faces Vector telling how many boundary faces each cell has
 * @param[out] centers Cell centers
 * @return the dimension
 */

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
CoMMAIntType read_mesh_from_file(
    const string fname,
    vector<CoMMAIndexType> &CSR_row,
    vector<CoMMAIndexType> &CSR_col,
    vector<CoMMAWeightType> &CSR_val,
    vector<CoMMAWeightType> &volumes,
    vector<CoMMAIntType> &n_bnd_faces,
    vector<vector<CoMMAWeightType>> &centers) {
  ifstream file(fname, ios::in);
  CoMMAIntType dim = 0;
  if (file) {
    string line,
           val;
    stringstream ssline;

    // Dimension
    getline(file, line);
    dim = convert_to<CoMMAIntType>(line);
    if ( !(dim == 2 || dim == 3) )
      throw invalid_argument( "Error: dimension must be 2 or 3" );

    // CSR_row
    // The only one which is pushed back
    getline(file, line);
    ssline.str(line);
    while ( getline(ssline, val, ',') )
      CSR_row.push_back(convert_to<CoMMAIndexType>(val));

    // CSR_col
    const auto sz = CSR_row.back();
    CSR_col.resize(sz);
    getline(file, line);
    ssline.clear();
    ssline.str(line);
    CoMMAIndexType i = 0;
    while ( getline(ssline, val, ',') && i < sz )
      CSR_col[i++] = convert_to<CoMMAIndexType>(val);

    // CSR_val
    CSR_val.resize(sz);
    getline(file, line);
    ssline.clear();
    ssline.str(line);
    i = 0;
    while ( getline(ssline, val, ',') && i < sz )
      CSR_val[i++] = convert_to<CoMMAWeightType>(val);

    // volumes
    const auto nc = CSR_row.size() - 1;
    volumes.resize(nc);
    getline(file, line);
    ssline.clear();
    ssline.str(line);
    i = 0;
    while ( getline(ssline, val, ',') && i < nc )
      volumes[i++] = convert_to<CoMMAWeightType>(val);

    // boundary faces
    n_bnd_faces.resize(nc);
    getline(file, line);
    ssline.clear();
    ssline.str(line);
    i = 0;
    while ( getline(ssline, val, ',') && i < nc )
      n_bnd_faces[i++] = convert_to<CoMMAIntType>(val);

    // centers
    centers.resize(nc);
    getline(file, line);
    ssline.clear();
    ssline.str(line);
    i = 0;
    CoMMAIndexType read = 0;
    vector<CoMMAWeightType> tmp(dim);
    for (auto &c : centers)
      c.resize(dim);
    while ( getline(ssline, val, ',') && i < nc ) {
      centers[i][read++] = convert_to<CoMMAWeightType>(val);
      if (read == static_cast<CoMMAIndexType>(dim))
        read = 0, ++i;
    }

  }
  else {
    throw invalid_argument("File not found");
  }
  return dim;

}
#endif
