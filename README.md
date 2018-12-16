# SKFlatAnalyzer

## Manual

https://jskim.web.cern.ch/jskim/SKFlat/Manual_SKFlat.pdf

## First time setup

Edit setup.sh :
```bash
export SKFlatLogEmail='' # <- put your email address here
```

## Initializing
Everytime when using new shell,
```bash
source setup.sh
voms-proxy-init --voms cms -valid 24:00
```

## Test job
```bash
python python/SKFlat.py -a ExampleRun -i ZToMuMu_M_50_120 -n 50 &
```

## SMPValidation job
```bash
source SMPValidation.sh
```

### SMPValidation plotting
Edit $TString filedir$ in plot.cc  
Save plots
```bash
root -b <<EOF
.L plot.cc
SaveAll()
EOF
```

## Making a new Ananlyzer
```bash
cd script/MakeCycleSkeleton/
```
Edit MakeCycleSkeleton.py :
```bash
cyclename = "" # <- put new Analyzer name
```
Then, run
```bash
python MakeCycleSkeleton.py
```
It will print below lines :
```bash
mv NewAnalyzer.h ../../include/
mv NewAnalyzer.C ../../src
## For Single job test ##
mv run_NewAnalyzer.C ../../
```
First two lines will move the files to inclue/ and src/ directory

Last line is to run a job with the machine, not batch.

This is good for test your analyzer
