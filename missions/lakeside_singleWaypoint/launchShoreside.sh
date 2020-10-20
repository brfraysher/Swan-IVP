#!/usr/bin/env bash

echo "Launching Shoreside MOOS Community. WARP is" $TIME_WARP
pAntler shoreside.moos >& /dev/null &

uMAC shoreside.moos

echo "Murdering MOOS"
murderMOOS
echo ":)"
