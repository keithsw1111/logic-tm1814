#ifndef TM1814_ANALYZER_SETTINGS
#define TM1814_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

#define COLOR_GRB 1
#define COLOR_RGB 2

class ANALYZER_EXPORT AnalyzerSettingInterfaceNumberList; // Forward declaration

class ANALYZER_EXPORT TM1814AnalyzerSettings : public AnalyzerSettings {
public:
  TM1814AnalyzerSettings();
  virtual ~TM1814AnalyzerSettings();

  virtual bool SetSettingsFromInterfaces();
  void UpdateInterfacesFromSettings();
  virtual void LoadSettings(const char *settings);
  virtual const char *SaveSettings();

  Channel mInputChannel;
  int mColorOrder;

protected:
  std::unique_ptr<AnalyzerSettingInterfaceChannel> mInputChannelInterface;
  std::unique_ptr<AnalyzerSettingInterfaceNumberList> mColorOrderInterface;
};

#endif // TM1814_ANALYZER_SETTINGS
