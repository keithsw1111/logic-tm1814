#include "TM1814AnalyzerSettings.h"
#include <AnalyzerHelpers.h>

TM1814AnalyzerSettings::TM1814AnalyzerSettings()
    : mInputChannel(UNDEFINED_CHANNEL), mColorOrder(COLOR_RGB) {
  mInputChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
  mInputChannelInterface->SetTitleAndTooltip("TM1814", "TM1814 LED Analyzer");
  mInputChannelInterface->SetChannel(mInputChannel);

  mColorOrderInterface.reset(new AnalyzerSettingInterfaceNumberList());
  mColorOrderInterface->SetTitleAndTooltip(
      "Color Order", "What is the color order in these LEDs?");
  mColorOrderInterface->AddNumber(COLOR_RGB, "RGBW",
                                  "Red, Green, Blue, White (normal)");
  mColorOrderInterface->AddNumber(COLOR_GRB, "GRBW", "Green, Red, Blue, White");
  mColorOrderInterface->SetNumber(mColorOrder);

  AddInterface(mInputChannelInterface.get());
  AddInterface(mColorOrderInterface.get());

  AddExportOption(0, "Export as csv file");
  AddExportExtension(0, "csv", "csv");

  ClearChannels();
  AddChannel(mInputChannel, "TM1814", false);
}

TM1814AnalyzerSettings::~TM1814AnalyzerSettings() {}

bool TM1814AnalyzerSettings::SetSettingsFromInterfaces() {
  mInputChannel = mInputChannelInterface->GetChannel();
  mColorOrder = mColorOrderInterface->GetNumber();

  ClearChannels();
  AddChannel(mInputChannel, "TM1814 Analyzer", true);

  return true;
}

void TM1814AnalyzerSettings::UpdateInterfacesFromSettings() {
  mInputChannelInterface->SetChannel(mInputChannel);
  mColorOrderInterface->SetNumber(mColorOrder);
}

void TM1814AnalyzerSettings::LoadSettings(const char *settings) {
  SimpleArchive text_archive;
  text_archive.SetString(settings);

  text_archive >> mInputChannel;
  text_archive >> mColorOrder;

  ClearChannels();
  AddChannel(mInputChannel, "Simple TM1814 Analyzer", true);

  UpdateInterfacesFromSettings();
}

const char *TM1814AnalyzerSettings::SaveSettings() {
  SimpleArchive text_archive;

  text_archive << mInputChannel;
  text_archive << mColorOrder;

  return SetReturnString(text_archive.GetString());
}
