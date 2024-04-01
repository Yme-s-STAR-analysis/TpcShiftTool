# TPC shift Tool

* version: 2.0

* author: yghuang

## Usage

0. This guide is for v2.0

1. Before you start, plase modify and run `python3 MakeRunNumberHeader.py`

    > The path to the file of mapping run IDs (it is NOT good run ID, but the correct mapping run list)

2. Then you can get `RunNumber.h`

3. In your analysis code, just write the following:

    ```c++
        TpcShiftTool* mtShift = new mtShift();
        mtShift->Init("/path/to/shift_file.root");
        /* in track loop */
            double shift = mtShift->GetShift(runId, pt, eta);
    ```

4. Then you can cons and run

## Change Log

01.04.2024 by yghuang (v2.0):

> New shift, with Ashish's root file format

20.11.2023 by yghuang (v1.0):

> Extract from previous StCFMult module, now is an independent module
