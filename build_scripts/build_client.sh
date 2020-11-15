#!/bin/bash

source build_scripts/config.sh

die()
{
    echo Fail $1
    play /usr/share/sounds/purple/alert.wav 2> /dev/null
    exit 1
}

rsync -avu --delete --cvs-exclude --exclude="/.git" \
      --exclude="/Assets" \
      --exclude="/Content" \
      --exclude="/thirdparty" \
      --exclude="Binaries" \
      --exclude="Build" \
      --exclude="DerivedDataCache" \
      --exclude="Intermediate" \
      --exclude="Saved" \
      --exclude="lib" \
      --filter="dir-merge,- .gitignore" . win:$UnixifiedWinPrjLocation || die "rsync client"

echo Entering directory \`Source/${CPrjName}\'
while : ; do
    ssh win "TEMP=C:/Users/teant/AppData/Local/Temp ${EngineLocation}Engine/Build/BatchFiles/Build.bat ${CPrjName}Editor Win64 Development -Project=${WinPrjLocation}${CPrjName}.uproject -WaitMutex -FromMsBuild 2>&1" || die "client"
    play /usr/share/sounds/sound-icons/percussion-12.wav 2> /dev/null
    break
done | sed 's|\\|/|g' | sed "s|${WinPrjLocation}Source/${CPrjName}/||g"
echo Leaving directory \`Source/${CPrjName}\'

