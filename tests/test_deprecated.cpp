/*
 * CoMMA
 *
 * Copyright © 2024 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <catch2/catch_test_macros.hpp>
#include <set>
#include <vector>

#include "CoMMA/CoMMADefs.h"
#include "CoMMA/Dual_Graph.h"
#include "CoMMA/deprecated/Bimap.h"
#include "CoMMA/deprecated/Priority_Pair.h"
#include "CoMMA/deprecated/Queue.h"
#include "CoMMA/deprecated/Tree.h"

using namespace comma;  // NOLINT
using namespace std;  // NOLINT

using PairValueTestT = int;  // Leave this since we might try something more
                             // fancy than what usually found in CoMMA
using NodeType = Node<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using TreeType = Tree<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;

SCENARIO("Test of the Queue", "[Queue]") {
  GIVEN("A simple Queue of CoMMAIndexT type") {
    Queue<long> st_long;
    WHEN("We push a certain number of elements") {
      st_long.push(0);
      st_long.push(1);
      st_long.push(2);
      THEN(
        "We can check that the operation on the stack are correctly "
        "realized"
      ) {
        REQUIRE(st_long.top() == 0);
        CoMMAIndexT top = st_long.pop();
        REQUIRE(top == 0);
        REQUIRE(st_long.top() == 1);
        top = st_long.pop();
        REQUIRE(top == 1);
        REQUIRE(st_long.top() == 2);
        top = st_long.pop();
        REQUIRE(top == 2);
        REQUIRE(st_long.top() == -1);
      }
    }
  }
}

#if 0
SCENARIO("Test of the tree", "[Tree]") {
  GIVEN("The node") {
    shared_ptr<NodeType> node = make_shared<NodeType>(2,1);
    shared_ptr<TreeType> tree = make_shared<TreeType>(node);
    WHEN("We add something to the tree") {
      tree->insertSon(2, 1, 1, 1);
      THEN("The tree has a child") {
        tree->insertSon(2, 3, 1, 1);
        tree->insertSon(2, 4, 1, 1);
        tree->insertSon(4, 2, 1, 0);
        REQUIRE(tree->_root->_sonc == 3);
        tree->deleteNode(4);
        REQUIRE(tree->_root->_sonc == 2);
      }
    }
  }
}
#endif

SCENARIO("Test of Priority Pair", "[Priority Pair]") {
  GIVEN("Some pairs") {
    const Priority_Pair<PairValueTestT, PairValueTestT> p1{0, 1}, p1bis{0, 1},
      p2{3, 0}, p3{0, 0};
    WHEN("We compare two equal pairs") {
      THEN("Equal pairs are identified") { REQUIRE(p1 == p1bis); }
    }
    WHEN("We compare two pair with different first element") {
      THEN("The one with the greater first element is identified as smaller") {
        REQUIRE(p2 < p1);
      }
    }
    WHEN("We compare two pair with the same first element") {
      THEN("The one with the smaller first element is identified as smaller") {
        REQUIRE(p3 < p1);
      }
    }
  }
  GIVEN("A set of pairs") {
    set<Priority_Pair<PairValueTestT, PairValueTestT>> st = {
      {0, 1}, {0, 1}, {3, 0}, {0, 0}, {2, -1}
    };
    WHEN("We iterate over the set") {
      vector<PairValueTestT> fe1 = {3, 2, 0, 0};
      vector<PairValueTestT> se1 = {0, -1, 0, 1};
      THEN("The expected order is obtained") {
        for (auto it = st.begin(); it != st.end(); ++it) {
          const auto idx = distance(st.begin(), it);
          REQUIRE(it->first() == fe1[idx]);
          REQUIRE(it->second() == se1[idx]);
        }
      }
    }
    WHEN("We add an item and iterate on the new set") {
      st.emplace(1, 4);
      vector<int> fe2 = {3, 2, 1, 0, 0};
      vector<int> se2 = {0, -1, 4, 0, 1};
      THEN("The expected order is obtained") {
        for (auto it = st.begin(); it != st.end(); ++it) {
          const auto idx = distance(st.begin(), it);
          REQUIRE(it->first() == fe2[idx]);
          REQUIRE(it->second() == se2[idx]);
        }
      }
    }
  }
}

SCENARIO("Test of the in-house Bimap", "[Bimap]") {
  GIVEN(
    "We have a 4x4 square 2D matrix of 16 elements that we consider divided "
    "in 4 Subgraph the structure of the subgraph is the same, changes "
    "fundamentally the mapping"
  ) {
    vector<CoMMAIndexT> adjMatrix_row_ptr = {0, 2, 4, 6, 8};
    vector<CoMMAIndexT> adjMatrix_col_ind = {0, 1, 0, 2, 1, 3, 0, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues = {1, 1, 1, 1, 1, 1, 1, 1};
    vector<CoMMAWeightT> volumes = {1, 1, 1, 1};
    vector<CoMMAIndexT> mapping_l_to_g_0 = {0, 1, 5, 4};
    vector<CoMMAIndexT> mapping_l_to_g_1 = {2, 3, 7, 6};
    vector<CoMMAIndexT> mapping_l_to_g_2 = {8, 3, 13, 12};
    vector<CoMMAIndexT> mapping_l_to_g_3 = {10, 11, 15, 14};
    auto cc0 = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      4,
      adjMatrix_row_ptr,
      adjMatrix_col_ind,
      adjMatrix_areaValues,
      volumes,
      mapping_l_to_g_0,
      true
    );
    auto cc1 = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      4,
      adjMatrix_row_ptr,
      adjMatrix_col_ind,
      adjMatrix_areaValues,
      volumes,
      mapping_l_to_g_1,
      true
    );
    auto cc2 = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      4,
      adjMatrix_row_ptr,
      adjMatrix_col_ind,
      adjMatrix_areaValues,
      volumes,
      mapping_l_to_g_2,
      true
    );
    auto cc3 = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      4,
      adjMatrix_row_ptr,
      adjMatrix_col_ind,
      adjMatrix_areaValues,
      volumes,
      mapping_l_to_g_3,
      true
    );
    WHEN("We Collect the cells in the Bimap") {
      Bimap<
        CoMMAIndexT,
        shared_ptr<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>>
        Collection;
      Collection.insert(0, cc0);
      Collection.insert(1, cc1);
      Collection.insert(2, cc2);
      Collection.insert(3, cc3);
      THEN("We eliminate an element and we check the dimension changed") {
        auto lung = Collection.lung();
        REQUIRE(lung == 4);
        const decltype(lung) elim = 2;
        Collection.erase_B(elim);
        lung = Collection.lung();
        REQUIRE(lung == 3);
        for (auto i = elim + 1; i != lung + 1; i++) {
          Collection.update_nodeB(i, i - 1);
        }
        // Collection.print();
      }
    }
  }

  GIVEN("We have a Bimap of CoMMAIndexT and shared_ptr") {
    vector<CoMMAIndexT> adjMatrix_row_ptr = {0, 2, 4, 7, 9, 10};
    vector<CoMMAIndexT> adjMatrix_col_ind = {1, 3, 0, 2, 1, 3, 4, 0, 2, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    vector<CoMMAWeightT> volumes = {1, 1, 1, 1, 1};
    vector<CoMMAIndexT> _mapping_l_to_g = {20, 30, 40, 50, 60};
    auto Marion = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      5,
      adjMatrix_row_ptr,
      adjMatrix_col_ind,
      adjMatrix_areaValues,
      volumes,
      _mapping_l_to_g,
      true
    );
    Bimap<
      CoMMAIndexT,
      shared_ptr<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>>
      Collection;
    WHEN("We insert an element and we delete it") {
      const CoMMAIndexT ins = 0;
      Collection.insert(ins, Marion);
      // Collection.print();
      auto prova = Collection.get_B(ins);
      Collection.erase_B(ins);
      THEN("Bimap is empty") { REQUIRE(Collection.empty() == true); }
    }
  }
}
