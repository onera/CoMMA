#include "profiling/trace_categories.h"
#include <chrono>
#include <fstream>
#include <thread>
#include "input/DualGPy.h"

void InitializePerfetto() {
  perfetto::TracingInitArgs args;
  // The backends determine where trace events are recorded. For this example we
  // are going to use the in-process tracing service, which only includes in-app
  // events.
  args.backends = perfetto::kInProcessBackend;

  perfetto::Tracing::Initialize(args);
  perfetto::TrackEvent::Register();
}

std::unique_ptr<perfetto::TracingSession> StartTracing() {
  // The trace config defines which types of data sources are enabled for
  // recording. In this example we just need the "track_event" data source,
  // which corresponds to the TRACE_EVENT trace points.
  perfetto::TraceConfig cfg;
  cfg.add_buffers()->set_size_kb(1024);
  auto* ds_cfg = cfg.add_data_sources()->mutable_config();
  ds_cfg->set_name("track_event");

  auto tracing_session = perfetto::Tracing::NewTrace();
  tracing_session->Setup(cfg);
  tracing_session->StartBlocking();
  return tracing_session;
}

void StopTracing(std::unique_ptr<perfetto::TracingSession> tracing_session) {
  // Make sure the last event is closed for this example.
  perfetto::TrackEvent::Flush();

  // Stop tracing and read the trace data.
  tracing_session->StopBlocking();
  std::vector<char> trace_data(tracing_session->ReadTraceBlocking());

  // Write the result into a file.
  // Note: To save memory with longer traces, you can tell Perfetto to write
  // directly into a file by passing a file descriptor into Setup() above.
  std::ofstream output;
  output.open("example.pftrace", std::ios::out | std::ios::binary);
  output.write(&trace_data[0], trace_data.size());
  output.close();
}

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
    // TODO: maybe change to pointer for omogenity
    TRACE_EVENT_BEGIN("rendering", "DualGPy");
    DualGPy Data = DualGPy();
    TRACE_EVENT_END("rendering");
    // Construction of the Dual Graph element
    Dual_Graph fc_graph = Dual_Graph(Data.nb_fc,
                                     Data.adjMatrix_row_ptr,
                                     Data.adjMatrix_col_ind,
                                     Data.adjMatrix_areaValues,
                                     Data.volumes,
                                     Data.d_is_on_bnd,
                                     Data.s_is_on_corner,
                                     Data.s_is_on_ridge,
                                     Data.s_is_on_valley,
                                     Data.s_anisotropic_compliant_fc,
                                     0,
                                     2
    );   
    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph(fc_graph);
    // Check the effective length

    // To test protected variables I use a child class. This is a trick to access. 
    class test : public Agglomerator_Anisotropic{
	    public:
		 test(Dual_Graph &graph,
			Coarse_Cell_Graph &cc_graph,
                        int verbose,
                        bool is_visu_data_stored,
                        int dimension) : Agglomerator_Anisotropic(graph,cc_graph,verbose,is_visu_data_stored,dimension){};

		    bool test_variable(){
                       return(_is_visu_data_stored); 
		    };
    };
    test* agg = new test(fc_graph,
		         cc_graph,
                                    0,
                                    Data.is_visu_data_stored,
                                    2);
     bool testing = agg->test_variable();
     vector<long> gg = agg->get_fc_2_cc();
     StopTracing(std::move(tracing_session));
     return 0;
    // Check if the structure is correct: Have I really changed the testing variable that is in the Father class Agglomerator by setting it true (by default is false) in the child class??
}
