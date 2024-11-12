#ifndef TM1814_SIMULATION_DATA_GENERATOR
#define TM1814_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

#include <SimulationChannelDescriptor.h>
class ANALYZER_EXPORT TM1814AnalyzerSettings;

#define N_LED 60

class ANALYZER_EXPORT TM1814SimulationDataGenerator {
public:
  TM1814SimulationDataGenerator();
  ~TM1814SimulationDataGenerator();

  void Initialize(U32 simulation_sample_rate, TM1814AnalyzerSettings *settings);
  U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate,
                             SimulationChannelDescriptor **simulation_channel);

protected:
  TM1814AnalyzerSettings *mSettings;
  U32 mSimulationSampleRateHz;

protected:
  SimulationChannelDescriptor mTM1814SimulationData;
  ClockGenerator mClockGenerator;

  int lednum;
  int counter;
};
#endif // TM1814_SIMULATION_DATA_GENERATOR
