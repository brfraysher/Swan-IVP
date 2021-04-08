#!/usr/bin/env bash

echo "Launching kayak MOOS Community. WARP is" $TIME_WARP
pAntler kayak.moos >& /dev/null &

uMAC kayak.moos

echo "Murdering MOOS"
murderMOOS
echo ":)"
