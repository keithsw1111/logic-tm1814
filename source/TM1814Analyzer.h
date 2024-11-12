#ifndef TM1814_ANALYZER_H
#define TM1814_ANALYZER_H

#include "TM1814AnalyzerResults.h"
#include "TM1814SimulationDataGenerator.h"
#include <Analyzer.h>

/*
From
http://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/

| Symbol | Parameter                 |   Min | Typical | Max   | Units |
| T0H    | 0 code ,high voltage time |   200 |     350 | 500   | ns    |
  T0L                                    310       360   410
| T1H    | 1 code ,high voltage time |   550 |     700 | 5,500 | ns    |
  T1L                                    650       720   1000
| TLD    | data, low voltage time    |   450 |     600 | 5,000 | ns    |
******** what are these | TLL    | latch, low voltage time   | 6,000 |         |
| ns    |
*/

#define T0H_MIN 310
#define T0H_TYP 360
#define T0H_MAX 410
#define T1H_MIN 650
#define T1H_TYP 720
#define T1H_MAX 1000

#define TLD_MIN 360
#define TLD_TYP 600
#define TLD_MAX 5500
#define TLL_MIN 20000

class ANALYZER_EXPORT TM1814AnalyzerSettings;
class ANALYZER_EXPORT TM1814Analyzer : public Analyzer2 {
public:
  TM1814Analyzer();
  virtual ~TM1814Analyzer();
  virtual void WorkerThread();
  virtual void SetupResults();

  virtual U32
  GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate,
                         SimulationChannelDescriptor **simulation_channels);
  virtual U32 GetMinimumSampleRateHz();

  virtual const char *GetAnalyzerName() const;
  virtual bool NeedsRerun();

  U64 SamplesToNS(U64 samples) {
    return (samples * 1000000000) / mSampleRateHz;
  }

protected: // vars
  std::unique_ptr<TM1814AnalyzerSettings> mSettings;
  std::unique_ptr<TM1814AnalyzerResults> mResults;
  AnalyzerChannelData *mTM1814;

  TM1814SimulationDataGenerator mSimulationDataGenerator;
  bool mSimulationInitilized;

  // TM1814 analysis vars:
  U32 mSampleRateHz;
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif // TM1814_ANALYZER_H
