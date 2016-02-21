#!/bin/bash
#Install program on the system

echo "Copying files..."

mkdir -p /usr/share/qrecipewriter
cp -r QRecipeWriter checkGimp.bash LICENCE.txt modeleHtml SendToWordpress /usr/share/qrecipewriter/
if [ ! -d "/usr/share/icons/hicolor/128x128/apps" ]; then
mkdir -p /usr/share/icons/hicolor/128x128/apps
fi
cp installDir/usr/share/icons/hicolor/128x128/apps/QRecipeWriter.png /usr/share/icons/hicolor/128x128/apps/
if [ ! -d "/usr/share/applications" ]; then
mkdir -p /usr/share/applications
fi
cp installDir/usr/share/applications/QRecipeWriter.desktop /usr/share/applications/

ln -s /usr/share/qrecipewriter/QRecipeWriter /usr/bin/QRecipeWriter
ln -s /usr/share/qrecipewriter/QRecipeWriter /usr/bin/QRecipeWriter
ln -s /usr/share/qrecipewriter/QRecipeWriter /usr/bin/qrecipewriter

echo "DONE !"
