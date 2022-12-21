#include "input/DualGPy.h"
#include "CoMMATypes.h"

int main(int argv, char** argc) {

  DualGPy Data = DualGPy();
    Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, seeds_pool, 2,
        Data.s_anisotropic_compliant_fc);

  Coarse_Cell_Container cc_graph = Coarse_Cell_Container(fc_graph);

  // Check the effective length

  // To test protected variables I use a child class. This is a trick to access.
  class test : public Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> {
   public:
    test(Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>& graph,
         Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>& cc_graph, CoMMAIntT dimension)
        : Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(graph, cc_graph,
                                                            dimension) {};
    ~test() {};
    CoMMAIntT test_variable() {
      return (_threshold_card);
    };
  };

  test* agg = new test(fc_graph, cc_graph, 2);
  CoMMAIndexT nb_agglomeration_lines = 0;
  forward_list<deque<CoMMAIndexT>*> agglomeration_lines;
  // @todo add exception if it is not the first agglomeration (so if we are at a
  // further level)
  // Initialization of nb_agglo_lines in dependency if we are at the first
  // agglomeration or not
  // in this case we are. in the todo we have to treat the case in which we are
  // not
  agg->_v_nb_lines[0] = nb_agglomeration_lines;
  agg->_v_lines[0] = agglomeration_lines;
  CoMMAIntT testing = agg->test_variable();
  vector<CoMMAIndexT> gg = agg->get_fc_2_cc();
  agg->agglomerate_one_level(2, 2, 2, -1);
  Agglomerator<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>* test =
      new Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(fc_graph, cc_graph,
                                                            2);
  test->agglomerate_one_level(2, 2, 2, -1);
  // Check if the structure is correct: Have I really changed the testing
  // variable that is in the Father class Agglomerator by setting it true (by
  // default is false) in the child class??
}
