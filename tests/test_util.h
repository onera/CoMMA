/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

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
#include <map>
#include <set>
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
 * @param[out] CSR_row The row pointer of the CSR representation
 * @param[out] CSR_col The column index of the CSR representation
 * @param[out] CSR_val The values of the CSR representation
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
    if ( !(dim == 2 || dim == 3) ) {
      file.close();
      throw invalid_argument( "Error: dimension must be 2 or 3" );
    }

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
  file.close();
  return dim;

}

/** @brief Starting to the description of a fine mesh / graph and the result of an
 * agglomeration (\p fc2cc), build the description of the coarse graph
 * @param[in] fc2cc Result of an agglomeration telling giving the relation FC to CC
 * @param[in] fine_CSR_row The row pointer of the CSR representation of the fine
 * graph
 * @param[in] fine_CSR_col The column index of the CSR representation of the fine
 * graph
 * @param[in] fine_CSR_val The values of the CSR representation of the fine graph
 * @param[in] fine_volumes The volumes of the cells of the fine graph
 * @param[in] fine_n_bnd_faces Vector telling how many boundary faces each cell of
 * the fine graph has
 * @param[in] fine_centers Cell centers of the fine graph
 * @param[out] fine_CSR_row The row pointer of the CSR representation of the coarse
 * graph
 * @param[out] fine_CSR_col The column index of the CSR representation of the coarse
 * graph
 * @param[out] fine_CSR_val The values of the CSR representation of the coarse graph
 * @param[out] fine_volumes The volumes of the cells of the coarse graph
 * @param[out] fine_n_bnd_faces Vector telling how many boundary faces each cell of
 * the coarse graph has
 * @param[out] fine_centers Cell centers of the coarse graph
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
void build_coarse_CSR(
    const vector<CoMMAIndexType> &fc2cc,
    const vector<CoMMAIndexType> &fine_CSR_row,
    const vector<CoMMAIndexType> &fine_CSR_col,
    const vector<CoMMAWeightType> &fine_CSR_val,
    const vector<CoMMAWeightType> &fine_volumes,
    const vector<CoMMAIntType> &fine_n_bnd_faces,
    const vector<vector<CoMMAWeightType>> &fine_centers,
    vector<CoMMAIndexType> &coarse_CSR_row,
    vector<CoMMAIndexType> &coarse_CSR_col,
    vector<CoMMAWeightType> &coarse_CSR_val,
    vector<CoMMAWeightType> &coarse_volumes,
    vector<CoMMAIntType> &coarse_n_bnd_faces,
    vector<vector<CoMMAWeightType>> &coarse_centers) {
  const auto dim_pts = fine_centers[0].size();
  const CoMMAIndexType n_fc = static_cast<CoMMAIndexType>(fc2cc.size());
  CoMMAIndexType n_cc = 0;
  // Building the CC
  map<CoMMAIndexType, set<CoMMAIndexType>> ccs{};
  for (CoMMAIndexType i = 0; i < n_fc; ++i) {
    const auto cc = fc2cc[i];
    if (cc > n_cc)
      n_cc = cc;
    ccs[cc].insert(i);
  }
  ++n_cc; // From index to number
  // Preparing outputs
  coarse_CSR_row.resize(n_cc+1);
  coarse_volumes.resize(n_cc);
  coarse_n_bnd_faces.resize(n_cc);
  coarse_centers.resize(n_cc);
  coarse_CSR_row[0] = 0;
  for (const auto &[cc, fcs] : ccs) {
    CoMMAWeightType vol{0.};
    vector<CoMMAWeightType> cen(dim_pts, 0.);
    CoMMAIntType n_bnd = 0;
    map<CoMMAIndexType, CoMMAWeightType> coarse_val;
    for (const auto &fc : fcs) {
      vol += fine_volumes[fc];
      if (fine_n_bnd_faces[fc] > n_bnd)
        n_bnd = fine_n_bnd_faces[fc];
      for (auto i = decltype(dim_pts){0}; i < dim_pts; ++i)
        cen[i] += fine_centers[fc][i];
      auto fine_neigh = fine_CSR_col.cbegin() + fine_CSR_row[fc];
      auto fine_val = fine_CSR_val.cbegin() + fine_CSR_row[fc];
      for (; fine_neigh != fine_CSR_col.cbegin() + fine_CSR_row[fc + 1];
           ++fine_neigh, ++fine_val) {
        if (fcs.find(*fine_neigh) == fcs.end()) {
          const auto coarse_neigh = fc2cc[*fine_neigh];
          // Here we rely on the fact that operator[] inserts 0 if does not exist
          coarse_val[coarse_neigh] = coarse_val[coarse_neigh] + *fine_val;
        }
      }
    }
    // Setting output
    for (const auto &[idx, val] : coarse_val) {
      coarse_CSR_col.push_back(idx);
      coarse_CSR_val.push_back(val);
    }
    coarse_CSR_row[cc + 1] = coarse_CSR_row[cc] + coarse_val.size();
    coarse_volumes[cc] = vol;
    coarse_n_bnd_faces[cc] = n_bnd;
    coarse_centers[cc].resize(dim_pts);
    for (auto i = decltype(dim_pts){0}; i < dim_pts; ++i)
      coarse_centers[cc][i] = cen[i] / fcs.size();
  }
}
#endif
