#ifndef TM1814_ANALYZER_RESULTS
#define TM1814_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class ANALYZER_EXPORT TM1814Analyzer;
class ANALYZER_EXPORT TM1814AnalyzerSettings;

class ANALYZER_EXPORT TM1814AnalyzerResults : public AnalyzerResults {
public:
  TM1814AnalyzerResults(TM1814Analyzer *analyzer,
                        TM1814AnalyzerSettings *settings);
  virtual ~TM1814AnalyzerResults();

  virtual void GenerateBubbleText(U64 frame_index, Channel &channel,
                                  DisplayBase display_base);
  virtual void GenerateExportFile(const char *file, DisplayBase display_base,
                                  U32 export_type_user_id);

  virtual void GenerateFrameTabularText(U64 frame_index,
                                        DisplayBase display_base);
  virtual void GeneratePacketTabularText(U64 packet_id,
                                         DisplayBase display_base);
  virtual void GenerateTransactionTabularText(U64 transaction_id,
                                              DisplayBase display_base);

protected: // functions
  int c(U32 v, int pos);
  int r(U32);
  int g(U32);
  int b(U32 v) { return c(v, 2); };
  int w(U32 v) { return c(v, 3); };

protected: // vars
  TM1814AnalyzerSettings *mSettings;
  TM1814Analyzer *mAnalyzer;
};

#endif // TM1814_ANALYZER_RESULTS
