fakeroot dpkg-deb --build dpkg
dpkg-name ./dpkg.deb
mkdir ../../packages
mv -f ./*.deb ../../packages/
