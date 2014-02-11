#!/bin/bash
for template in ./Gnuplot/TemplateScript/*.templatescript
do
	for datafile in ./formatteddata/*.formdat
	do
		# Togli templatescriptdal nome del file
		nometemplate=$(echo "$template" | grep --perl-regexp --only-matching '(.*)\.(.*)' | perl -pe 's/(.*\/)(.+)\.(.+)/\2/g')
		nomedata=$(echo "$datafile" | grep --perl-regexp --only-matching '(.*)\.(.*)' | perl -pe 's/(.*\/)(.+)\.(.+)/\2/g')
		nomescript=$(echo "$(echo $nometemplate)_$(echo $nomedata).gnuplotscript")
		
		echo "Nome template: $nometemplate"
		echo "Nome data: $nomedata"
		echo "Dati: $datafile"
		echo "Nome Script: $nomescript"
		
		#cp --force $template ./Gnuplot/${nometemplate}_${nomedata}.gnuplotscript
		#cp --force $template $nomescript
		
		Media=$(./AnalizzatoreDati $datafile | perl -pe 's/(Media)\:\s(.*)/$2/g;' | grep --perl-regexp "^\d")
		Dev_Standard_P=$(./AnalizzatoreDati $datafile | perl -pe 's/(Deviazione\sstandard\scampione)\:\s(.*)/$2/g;' | grep --perl-regexp "^\d")
		Massimo=$(./AnalizzatoreDati $datafile | perl -pe 's/(Massimo)\:\s(.*)/$2/g;' | grep --perl-regexp "^\d")
		Minimo=$(./AnalizzatoreDati $datafile | perl -pe 's/(Minimo)\:\s(.*)/$2/g;' | grep --perl-regexp "^\d")
		NumeroDati=$(./AnalizzatoreDati $datafile | perl -pe 's/(Numero\sdati)\:\s(.*)/$2/g;' | grep --perl-regexp "^\d")
		
		#Togli le prime quattro righe del file, che sono i commenti del template
		#tail -$(($(wc -l "/dev/fd/0" | grep -Po "(\d)*\s")-4))
		cat $template | \
		perl -pe "s/__MAX__/$Massimo/g" | \
		perl -pe "s/__MIN__/$Minimo/g" | \
		perl -pe "s/__MEDIA__/$Media/g" | \
		perl -pe "s{__PATHDATI__}{$datafile}g" | \
		perl -pe "s/__SIGMAP__/$Dev_Standard_P/g" | \
		perl -pe "s/__NUMDATI__/$NumeroDati/g" | \
		perl -pe "s/__NOMEDATI__/$nomedata/g" \
		>./Gnuplot/$nomescript
	done
done

exit 0



