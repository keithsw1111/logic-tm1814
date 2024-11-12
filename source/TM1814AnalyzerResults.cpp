#include "TM1814AnalyzerResults.h"
#include "TM1814Analyzer.h"
#include "TM1814AnalyzerSettings.h"
#include <AnalyzerHelpers.h>
#include <fstream>

TM1814AnalyzerResults::TM1814AnalyzerResults(TM1814Analyzer *analyzer,
                                             TM1814AnalyzerSettings *settings)
    : AnalyzerResults(), mSettings(settings), mAnalyzer(analyzer) {}

TM1814AnalyzerResults::~TM1814AnalyzerResults() {}

void TM1814AnalyzerResults::GenerateBubbleText(U64 frame_index,
                                               Channel &channel,
                                               DisplayBase display_base) {

  static U32 lastC1 = 0xFFFFFFFF;

  ClearResultStrings();
  Frame frame = GetFrame(frame_index);

  char long_str[128] = {0};
  char short_str[128] = {0};

  uint8_t red = (uint8_t)r((U32)frame.mData2);
  uint8_t green = (uint8_t)g((U32)frame.mData2);
  uint8_t blue = (uint8_t)b((U32)frame.mData2);
  uint8_t white = (uint8_t)w((U32)frame.mData2);

  if (frame.mData1 == 0) {
    // C1
    lastC1 = frame.mData2;
    char type[20] = {0};
    if ((red & 0xC0) != 0 || (green & 0xC0) != 0 || (blue & 0xC0) != 0 ||
        (white & 0xC0) != 0) {
      strcpy(type, "C1 INVLD");
    } else {
      strcpy(type, "C1");
    }

    snprintf(long_str, sizeof(long_str), "%s: W %02x R %02x G %02x B %02x",
             type, red, green, blue, white);

    snprintf(short_str, sizeof(short_str), "%s: #%02x%02x%02x%02x", type, red,
             green, blue, white);

  } else if (frame.mData1 == 1) {
    // C2
    char type[20] = {0};
    if (frame.mData2 != ~lastC1 && lastC1 != 0xFFFFFFFF) {
      strcpy(type, "C2 INVLD");
    } else {
      strcpy(type, "C2");
    }

    snprintf(long_str, sizeof(long_str), "%s: W %02x R %02x G %02x B %02x",
             type, red, green, blue, white);

    snprintf(short_str, sizeof(short_str), "%s: #%02x%02x%02x%02x", type, red,
             green, blue, white);
  } else {
    snprintf(long_str, sizeof(long_str), "%d: r=%02x g=%02x b=%02x w=%02x",
             (int)frame.mData1 - 2, red, green, blue, white);

    snprintf(short_str, sizeof(short_str), "%d: #%02x%02x%02x%02x",
             (int)frame.mData1 - 2, red, green, blue, white);
  }

  AddResultString(long_str);
  AddResultString(short_str);
}

void TM1814AnalyzerResults::GenerateExportFile(const char *file,
                                               DisplayBase display_base,
                                               U32 export_type_user_id) {
  std::ofstream file_stream(file, std::ios::out);

  U64 trigger_sample = mAnalyzer->GetTriggerSample();
  U32 sample_rate = mAnalyzer->GetSampleRate();

  file_stream << "Time [s],LED,R,G,B,W,RGBW" << std::endl;

  U64 num_frames = GetNumFrames();
  for (U32 i = 0; i < num_frames; i++) {
    Frame frame = GetFrame(i);

    char time_str[128];
    AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive,
                                   trigger_sample, sample_rate, time_str, 128);

    char number_str[128];
    if (frame.mData1 == 0) {
      snprintf(number_str, sizeof(number_str),
               "C1,%d,%d,%d,%d,#%02x%02x%02x%02x", r((U32)frame.mData2),
               g((U32)frame.mData2), b((U32)frame.mData2), w((U32)frame.mData2),
               r((U32)frame.mData2), g((U32)frame.mData2), b((U32)frame.mData2),
               w((U32)frame.mData2));
    } else if (frame.mData1 == 1) {
      snprintf(number_str, sizeof(number_str),
               "C2,%d,%d,%d,%d,#%02x%02x%02x%02x", r((U32)frame.mData2),
               g((U32)frame.mData2), b((U32)frame.mData2), w((U32)frame.mData2),
               r((U32)frame.mData2), g((U32)frame.mData2), b((U32)frame.mData2),
               w((U32)frame.mData2));
    } else {
      snprintf(number_str, sizeof(number_str),
               "%d,%d,%d,%d,%d,#%02x%02x%02x%02x", (int)frame.mData1 - 2,
               r((U32)frame.mData2), g((U32)frame.mData2), b((U32)frame.mData2),
               w((U32)frame.mData2), r((U32)frame.mData2), g((U32)frame.mData2),
               b((U32)frame.mData2), w((U32)frame.mData2));
    }

    file_stream << time_str << "," << number_str << std::endl;

    if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true) {
      file_stream.close();
      return;
    }
  }

  file_stream.close();
}

void TM1814AnalyzerResults::GenerateFrameTabularText(U64 frame_index,
                                                     DisplayBase display_base) {
  Frame frame = GetFrame(frame_index);
  ClearTabularText();

  char long_str[128] = {0};

  if (frame.mData1 == 0) {
    snprintf(long_str, sizeof(long_str), "C1: r=%02x g=%02x b=%02x w=%02x",
             r((U32)frame.mData2), g((U32)frame.mData2), b((U32)frame.mData2),
             w((U32)frame.mData2));
  } else if (frame.mData1 == 1) {
    snprintf(long_str, sizeof(long_str), "C2: r=%02x g=%02x b=%02x w=%02x",
             r((U32)frame.mData2), g((U32)frame.mData2), b((U32)frame.mData2),
             w((U32)frame.mData2));
  } else {
    snprintf(long_str, sizeof(long_str), "%d: r=%02x g=%02x b=%02x w=%02x",
             (int)frame.mData1 - 2, r((U32)frame.mData2), g((U32)frame.mData2),
             b((U32)frame.mData2), w((U32)frame.mData2));
  }

  AddTabularText(long_str);
}

void TM1814AnalyzerResults::GeneratePacketTabularText(
    U64 packet_id, DisplayBase display_base) {
  ClearResultStrings();
  AddResultString("not supported");
}

void TM1814AnalyzerResults::GenerateTransactionTabularText(
    U64 transaction_id, DisplayBase display_base) {
  ClearResultStrings();
  AddResultString("not supported");
}

int TM1814AnalyzerResults::c(U32 v, int pos) {
  switch (pos) {
  case 0:
    return (v & 0xff0000) >> 16;
  case 1:
    return (v & 0xff00) >> 8;
  case 2:
    return v & 0xff;
  case 3:
    return (v & 0xff000000) >> 24;
  }
  abort();
}

int TM1814AnalyzerResults::r(U32 v) {
  return mSettings->mColorOrder == COLOR_RGB ? c(v, 0) : c(v, 1);
};
int TM1814AnalyzerResults::g(U32 v) {
  return mSettings->mColorOrder == COLOR_RGB ? c(v, 1) : c(v, 0);
};
