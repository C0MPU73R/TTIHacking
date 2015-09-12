try:
  #todo add the file
  from gamedataModule import *
except ImportError, err:
  if "DLL loader cannot find" not in str(err):
    raise
