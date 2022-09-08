#include "input/DualGPy.h"
#include "test_types.h"

int main(int argv, char** argc) {

  DualGPy Data = DualGPy();
  Dual_Graph fc_graph =
      Dual_Graph(Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
                 Data.adjMatrix_areaValues, Data.volumes, Data.d_is_on_bnd,
                 Data.s_is_on_corner, Data.s_is_on_ridge, Data.s_is_on_valley,
                 Data.s_anisotropic_compliant_fc, 0, 2);

  Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph(fc_graph);

  // Check the effective length

  // To test protected variables I use a child class. This is a trick to access.
  class test : public Agglomerator_Anisotropic<TestIndexT, TestWeightT> {
   public:
    test(Dual_Graph<TestIndexT, TestWeightT>& graph,
         Coarse_Cell_Graph& cc_graph, int dimension)
        : Agglomerator_Anisotropic<TestIndexT, TestWeightT>(graph, cc_graph,
                                                            dimension) {};
    ~test() {};
    short test_variable() {
      return (_threshold_card);
    };
  };

  test* agg = new test(fc_graph, cc_graph, 2);
  TestIndexT nb_agglomeration_lines = 0;
  forward_list<deque<TestIndexT>*> agglomeration_lines;
  // @todo add exception if it is not the first agglomeration (so if we are at a
  // further level)
  // Initialization of nb_agglo_lines in dependency if we are at the first
  // agglomeration or not
  // in this case we are. in the todo we have to treat the case in which we are
  // not
  agg->_v_nb_lines[0] = nb_agglomeration_lines;
  agg->_v_lines[0] = agglomeration_lines;
  short testing = agg->test_variable();
  vector<TestIndexT> gg = agg->get_fc_2_cc();
  agg->agglomerate_one_level(2, 2, 2, -1);
  Agglomerator<TestIndexT, TestWeightT>* test =
      new Agglomerator_Biconnected<TestIndexT, TestWeightT>(fc_graph, cc_graph,
                                                            2);
  test->agglomerate_one_level(2, 2, 2, -1);
  // Check if the structure is correct: Have I really changed the testing
  // variable that is in the Father class Agglomerator by setting it true (by
  // default is false) in the child class??
}
