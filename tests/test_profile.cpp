#include "header_profile.h"
#include "input/DualGPy.h"

int main(int argv, char** argc) {
  InitializePerfetto();
  auto tracing_session = StartTracing();

  // Give a custom name for the traced process.
  perfetto::ProcessTrack process_track = perfetto::ProcessTrack::Current();
  perfetto::protos::gen::TrackDescriptor desc = process_track.Serialize();
  desc.mutable_process()->set_process_name("Example");
  perfetto::TrackEvent::SetTrackDescriptor(process_track, desc);
  // DualGPy class constructor loads the configuration
  // with the graph and all the variables required
  // TODO: maybe change to pointer for homogeneity
  TRACE_EVENT_BEGIN("setup", "DualGPy");
  DualGPy Data = DualGPy();
  TRACE_EVENT_END("setup");
  // Construction of the Dual Graph element

  TRACE_EVENT_BEGIN("setup", "Seeds_Pool");
  Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights, false);
  TRACE_EVENT_END("setup");
  TRACE_EVENT_BEGIN("setup", "Dual_Graph");
  Dual_Graph<TestIndexT, TestWeightT, TestIntT> fc_graph =
      Dual_Graph<TestIndexT, TestWeightT, TestIntT>(Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc);

  TRACE_EVENT_END("setup");
  Coarse_Cell_Container<TestIndexT, TestWeightT, TestIntT> cc_graph = Coarse_Cell_Container<TestIndexT, TestWeightT, TestIntT>(fc_graph, 1);

  // Check the effective length

  // To test protected variables I use a child class. This is a trick to access.
  class test : public Agglomerator_Anisotropic<TestIndexT, TestWeightT, TestIntT> {
   public:
    test(Dual_Graph<TestIndexT, TestWeightT, TestIntT>& graph,
         Coarse_Cell_Container<TestIndexT, TestWeightT, TestIntT>& cc_graph, TestIntT dimension)
        : Agglomerator_Anisotropic<TestIndexT, TestWeightT, TestIntT>(graph, cc_graph,
                                                            dimension) {};
    ~test() {};
    TestIntT test_variable() {
      return (_threshold_card);
    };
  };

  TRACE_EVENT_BEGIN("setup", "Agglomerator");
  test* agg = new test(fc_graph, cc_graph, 2);

  TRACE_EVENT_END("setup");

  TestIntT testing = agg->test_variable();
  TRACE_EVENT_BEGIN("agglomerator", "aniso");
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
  agg->agglomerate_one_level(2, 2, 2, -1);
  TRACE_EVENT_END("agglomerator");
  TRACE_EVENT_BEGIN("agglomerator", "biconnected");
  Agglomerator<TestIndexT, TestWeightT>* test =
      new Agglomerator_Biconnected<TestIndexT, TestWeightT>(fc_graph, cc_graph,
                                                            2);
  test->agglomerate_one_level(2, 2, 2, -1);
  TRACE_EVENT_END("agglomerator");
  StopTracing(std::move(tracing_session));
  return 0;
  // Check if the structure is correct: Have I really changed the testing
  // variable that is in the Father class Agglomerator by setting it true (by
  // default is false) in the child class??
}
