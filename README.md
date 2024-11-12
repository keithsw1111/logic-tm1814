# TM1814 Analyzer for Saleae Logic

This is heavily based on https://github.com/dustin/logic-ws2812

I know the CMake build works for windows. The rest I am not sure about

This plugin for [Saleae Logic][logic] allows you to decode TM1814 LED
control signals.

## Exporting

CSV export includes both the broken out values and a combined hex
color value:

## Building

Download the [Logic SDK][sdk] and extract it somewhere on your
machine.  In my case, it's `~/stuff/SaleaeAnalyzerSdk-1.1.32`.

Clone the repo and cd into the top level of it:

    git clone https://github.com/keithsw1111/logic-tm1814
    cd logic-tm1814

Symlink the `include` and `lib` directories into your tree:

    ln -s ~/stuff/SaleaeAnalyzerSdk-1.1.32/{include,lib} .

Run the build script:

    ./build_analyzer.py

## Installing

In the Developer tab in Logic preferences, specify the path for
loading new plugins, then copy the built plugin into that location:

    cp release/* /path/specified/in/Logic/preferences

[logic]: https://www.saleae.com/downloads
[sdk]: http://support.saleae.com/hc/en-us/articles/201104644-Analyzer-SDK
