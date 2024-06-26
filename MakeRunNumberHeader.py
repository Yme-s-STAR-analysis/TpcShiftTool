import os
import datetime

# HERE ARE CONFIGURATIONS

runList = './demo.list'
shiftFile = '/path/to/the/shift/file'
    
# Now make it

print('Trying to make run number header.')

header = 'RunNumber.h'

if os.path.exists(header):
    print(f'{header} already exists, deleting it.')
    os.remove(header)

if not os.path.exists(runList):
    print(f'Invalid run list ({runList}).')
    raise Exception(f'{runList = } which did not exist!')

runs = open(runList).readlines()
runs = [item.strip() for item in runs]
nRuns = len(runs)
print(f'{nRuns} runs found in {runList}.')

with open(header, 'w') as f:
    f.write('#ifndef __RunNumberHeader__\n')
    f.write('#define __RunNumberHeader__\n')

    f.write('\n\n')
    f.write('\t/*\n\t\tAutomated generated by Yige HUANG\'s python script.\n')
    f.write(f'\t\tGenerated time: {datetime.datetime.now().ctime()}\n')
    f.write('\t*/\n') # end of the description
    
    f.write('\n\n')
    f.write('#include <map>\n')

    f.write('\n\n')
    f.write('namespace RunNumber {\n')
    f.write('\n')
    f.write('\n\tstatic constexpr const char* mShiftFile = "')
    f.write(shiftFile)
    f.write('";\n')
    f.write('\tstatic const std::map<Int_t, Int_t> mRunIdxMap = {\n')
    for idx, item in enumerate(runs):
        f.write('\t\t{\t')
        f.write(item)
        f.write(',\t')
        f.write(f'{idx:5d}')
        f.write('},\n')
    f.write('\t};\n') # end of the run idx map

    f.write('}\n') # end of the namespace

    f.write('\n\n#endif\n') # end of this header file

print(f'{header} Generated. This is the end.')
