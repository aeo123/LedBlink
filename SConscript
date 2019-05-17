from building import *

cwd     = GetCurrentDir()
CPPPATH = [cwd, str(Dir('#'))]

src     = Glob('*.c')


group = DefineGroup('LedBlink', src, depend = [''], CPPPATH = CPPPATH)

Return('group')

