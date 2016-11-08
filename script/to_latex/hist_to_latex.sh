#!/bin/bash

filename=$1

if [ ${2//_nofloat} == $2 ]
then
	float="h"
else
	float="H"
fi

testo="
	 \begin{figure}[$float]
	  \centering
	   \includegraphics[width=\textwidth]{../../$2}"
if [ -f ./hist/$filename.txt ]
then
	testo=$testo"\caption{$(cat ./hist/$filename.txt)}"
else
	testo=$testo"\caption{${filename//_/ }}"
fi
testo=$testo"\label{hist:$filename}
	 \end{figure}
	"
	
echo $testo > ./latex/hist/$filename.tex

exit 0;
