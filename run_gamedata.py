# try importing gamedata.pyd XD no infintiecipher leak yet..
# must have infinitecipher removed from deploy.py first before you can deploy gamedata
from gamedata.GameData import *
# Test for ClientStart
# TODO: figure out how to properly import data from a pyd (must be decrypted data, dont import a raw module, import the decrypted data located in the file)
ToontownStart()
