#include <cmath>

#include "TM1814Analyzer.h"
#include "TM1814AnalyzerSettings.h"
#include "TM1814SimulationDataGenerator.h"

#include <AnalyzerHelpers.h>

TM1814SimulationDataGenerator::TM1814SimulationDataGenerator()
    : lednum(0), counter(0) {}

TM1814SimulationDataGenerator::~TM1814SimulationDataGenerator() {}

void TM1814SimulationDataGenerator::Initialize(
    U32 simulation_sample_rate, TM1814AnalyzerSettings *settings) {
  mSimulationSampleRateHz = simulation_sample_rate;
  mSettings = settings;

  mTM1814SimulationData.SetChannel(mSettings->mInputChannel);
  mTM1814SimulationData.SetSampleRate(simulation_sample_rate);
  mTM1814SimulationData.SetInitialBitState(BIT_HIGH);

  mClockGenerator.Init(1000000000, simulation_sample_rate);
}

U32 TM1814SimulationDataGenerator::GenerateSimulationData(
    U64 largest_sample_requested, U32 sample_rate,
    SimulationChannelDescriptor **simulation_channel) {
  U64 adjusted_largest_sample_requested =
      AnalyzerHelpers::AdjustSimulationTargetSample(
          largest_sample_requested, sample_rate, mSimulationSampleRateHz);

  mTM1814SimulationData.Advance(
      mClockGenerator.AdvanceByTimeS(2.0 * TLL_MIN / 1000000000.0));

  double frequency = 0.1;
  while (mTM1814SimulationData.GetCurrentSampleNumber() <
         adjusted_largest_sample_requested) {
    if (lednum == N_LED) {
      mTM1814SimulationData.TransitionIfNeeded(BIT_HIGH);
      mTM1814SimulationData.Advance(mClockGenerator.AdvanceByTimeS(.1));
      lednum = 0;
    }

    int red = (int)(sin(frequency * counter + 0) * 127 + 128);
    int green = (int)(sin(frequency * counter + 2) * 127 + 128);
    int blue = (int)(sin(frequency * counter + 4) * 127 + 128);
    int white = (int)(sin(frequency * counter + 8) * 127 + 128);
    U32 color = green << 16 | red << 8 | blue | white;

    for (int i = 31; i >= 0; i--) {
      int bit = (color >> i) & 0x1;
      int high = bit ? T1H_TYP : T0H_TYP;

      mTM1814SimulationData.TransitionIfNeeded(BIT_LOW);
      mTM1814SimulationData.Advance(
          mClockGenerator.AdvanceByTimeS(high * 1E-9));
      mTM1814SimulationData.TransitionIfNeeded(BIT_HIGH);
      mTM1814SimulationData.Advance(
          mClockGenerator.AdvanceByTimeS(TLD_TYP * 1E-9));
    }

    lednum++;
    counter++;
  }

  *simulation_channel = &mTM1814SimulationData;
  return 1;
}
