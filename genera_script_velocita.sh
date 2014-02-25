#!/bin/bash
for template in ./Gnuplot/TemplateScript/*.templatescript
do
	for datafile in ./Risultati/MetaDati/*/velocita*
	do
		# Togli templatescriptdal nome del file
		nometemplate=$(echo "$template" | grep --perl-regexp --only-matching '(.*)\.(.*)' | perl -pe 's/(.*\/)(.+)\.(.+)/\2/g')
		nomedata=$(echo "$datafile" | grep --perl-regexp --only-matching '(.*)\.(.*)' | perl -pe 's/(.*\/)(.+)\.(.+)/\2/g')
		nomescript=$(echo "$(echo $nometemplate)_$(echo $nomedata).gnuplotscript")
		tipodati=$(echo "$nomedata" | perl -pe 's/velocita_(.*)/\1/g' )
		
		echo "Nome template: $nometemplate"
		echo "Nome data: $nomedata"
		echo "Dati: $datafile"
		echo "Nome Script: $nomescript"
		echo "Nome Script: $tipodati"
		
		#cp --force $template ./Gnuplot/${nometemplate}_${nomedata}.gnuplotscript
		#cp --force $template $nomescript
		
		#Media=$(./AnalizzatoreDati $datafile | perl -pe 's/(Media)\:\s(.*)/$2/g;' | grep --perl-regexp "^\d")
		
		#Togli le prime quattro righe del file, che sono i commenti del template
		#tail -$(($(wc -l "/dev/fd/0" | grep -Po "(\d)*\s")-4))
		strnomelog="s{__NOMELOGFILE__}{../Risultati/Analisi_Dati/ris_$tipodati}g"
		strfiletempo="s{__PATHFILETEMPO__}{../Risultati/Analisi_Dati/arrayTempi_$nomedata\.txt}g"
		strvelocita="s{__PATHFILEVELOCITA__}{../Risultati/Analisi_Dati/velocita_$nomedata\.txt}g"
		
		echo "$strnomelog"
		echo "$strfiletempo"
		
		cat $template | \
		perl -pe $strnomelog | \
		perl -pe $strfiletempo | \
		perl -pe $strvelocita | \
		tee ./Gnuplot/$nomescript
	done
done

exit 0



