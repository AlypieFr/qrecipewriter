#!/bin/bash
#Install program on the system

echo "Copying files..."

mkdir -p /usr/share/qcecwriter
cp -r CeCWriter checkGimp.bash LICENCE.txt modeleHtml SendToWordpress /usr/share/qcecwriter/
if [ ! -d "/usr/share/icons/hicolor/128x128/apps" ]; then
mkdir -p /usr/share/icons/hicolor/128x128/apps
fi
cp installDir/usr/share/icons/hicolor/128x128/apps/QCeCWriter.png /usr/share/icons/hicolor/128x128/apps/
if [ ! -d "/usr/share/applications" ]; then
mkdir -p /usr/share/applications
fi
cp installDir/usr/share/applications/QCeCWriter.desktop /usr/share/applications/

ln -s /usr/share/qcecwriter/CeCWriter /usr/bin/CeCWriter
ln -s /usr/share/qcecwriter/CeCWriter /usr/bin/QCeCWriter
ln -s /usr/share/qcecwriter/CeCWriter /usr/bin/qcecwriter

echo "DONE !"
