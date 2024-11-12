#include <cassert>
#include <cmath>

#include "TM1814Analyzer.h"
#include "TM1814AnalyzerSettings.h"
#include <AnalyzerChannelData.h>

TM1814Analyzer::TM1814Analyzer()
    : Analyzer2(), mSettings(new TM1814AnalyzerSettings()),
      mSimulationInitilized(false), mSampleRateHz(0) {
  SetAnalyzerSettings(mSettings.get());
}

TM1814Analyzer::~TM1814Analyzer() { KillThread(); }

void TM1814Analyzer::SetupResults() {
  mResults.reset(new TM1814AnalyzerResults(this, mSettings.get()));
  SetAnalyzerResults(mResults.get());
  mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannel);
}

void TM1814Analyzer::WorkerThread() {
  mSampleRateHz = GetSampleRate();
  mTM1814 = GetAnalyzerChannelData(mSettings->mInputChannel);

  // Look for a start
  while (SamplesToNS(mTM1814->GetSampleOfNextEdge() -
                     mTM1814->GetSampleNumber()) < TLL_MIN) {
    mTM1814->AdvanceToNextEdge();
  }
  while (mTM1814->GetBitState() != BIT_LOW) {
    mTM1814->AdvanceToNextEdge(); // wait for rising
  }

  U32 ledval = 0;
  U32 lastLED = 0xFFFFFFFF;
  int lednum = 0;
  int bitnum = 0;
  U64 ledstart = mTM1814->GetSampleNumber();
  for (;;) {
    assert(mTM1814->GetBitState() == BIT_LOW);

    U64 start = mTM1814->GetSampleNumber();
    mTM1814->AdvanceToNextEdge();         // falling
    U64 mid = mTM1814->GetSampleNumber(); // mid - start == half bit
    mTM1814->AdvanceToNextEdge();         // rising
    U64 end = mTM1814->GetSampleNumber();

    U64 th = SamplesToNS(mid - start);
    U64 tl = SamplesToNS(end - mid);

    bitnum++;
    ledval <<= 1;
    ledval |= (th > T1H_MIN ? 1 : 0);

    if (bitnum == 32) {
      if (ledval != 0 && lastLED == 0) {
        // we add a cross marker if the previous LED was off
        mResults->AddMarker(end - ((mid - ledstart) / 2),
                            AnalyzerResults::ErrorX, mSettings->mInputChannel);
      } else {
        mResults->AddMarker(end - ((mid - ledstart) / 2), AnalyzerResults::Dot,
                            mSettings->mInputChannel);
      }

      Frame frame;
      frame.mData1 = lednum;
      frame.mData2 = ledval;
      frame.mStartingSampleInclusive = ledstart;
      frame.mEndingSampleInclusive = mid;
      mResults->AddFrame(frame);
      mResults->CommitResults();
      ReportProgress(frame.mEndingSampleInclusive);

      lastLED = ledval;

      ledval = 0;
      bitnum = 0;
      ledstart = mTM1814->GetSampleOfNextEdge();
      lednum++;
    }

    if (tl >= TLL_MIN) {
      ledval = 0;
      bitnum = 0;
      lednum = 0;
      ledstart = mTM1814->GetSampleOfNextEdge();
      lastLED = 0xFFFFFFFF;
    }
  }
}

bool TM1814Analyzer::NeedsRerun() { return false; }

U32 TM1814Analyzer::GenerateSimulationData(
    U64 minimum_sample_index, U32 device_sample_rate,
    SimulationChannelDescriptor **simulation_channels) {
  if (mSimulationInitilized == false) {
    mSimulationDataGenerator.Initialize(GetSimulationSampleRate(),
                                        mSettings.get());
    mSimulationInitilized = true;
  }

  return mSimulationDataGenerator.GenerateSimulationData(
      minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 TM1814Analyzer::GetMinimumSampleRateHz() { return 1000000; }

const char *TM1814Analyzer::GetAnalyzerName() const { return "TM1814"; }

const char *GetAnalyzerName() { return "TM1814"; }

Analyzer *CreateAnalyzer() { return new TM1814Analyzer(); }

void DestroyAnalyzer(Analyzer *analyzer) { delete analyzer; }
