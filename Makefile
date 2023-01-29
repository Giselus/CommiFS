scripts = usefulScripts
dataFolder = comiFolder
files = ${dataFolder}/files
data = ${dataFolder}/comiData

all: build run

build:
	./${scripts}/makeFolder.sh
	./${scripts}/systemInit.sh ${data} ${files}

run:
	./${scripts}/mountFolder.sh 

clean:
	rm -rf *Folder
	rm comiFS*

restore:
	./${scripts}/restoreFiles.sh dataFolder