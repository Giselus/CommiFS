rm -rf comiFolder
rm -rf mountComiFolder
mkdir comiFolder
mkdir mountComiFolder
cd comiFolder
mkdir comiData
mkdir pliki
cd pliki
echo "zawartosc pliku a jakas tam" > a.txt
mkdir A
mkdir B
mkdir C
echo "zawartosc pliku a w A" > A/a.txt
echo "zawartosc pliku b w B" > B/b.txt
echo "zawartosc pliku c w C" > C/c.txt