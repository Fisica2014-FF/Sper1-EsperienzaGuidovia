/*
 * SortingVarStat.h
 *
 *  Created on: Feb 19, 2014
 *      Author: francesco
 */

#ifndef SORTINGVARSTAT_H_
#define SORTINGVARSTAT_H_

#include "VarStat.h"

///////////////////////
//					 //
//	  VERSIONE 0.1	 //
//					 //
///////////////////////


namespace mions {
namespace dataAnalisi {

template <class T>
class Sorting_VarStat: public mions::dataAnalisi::VarStat<T> {
public:
	vector<T> vectDati;
	Sorting_VarStat(const vector<T>& aDati, bool eliminaTreSigma = true) : VarStat(aDati, eliminaTreSigma) {
		vectDati = aDati;//Copiati i dati, così puoi riordinaarli in santa pace
	};

	virtual ~Sorting_VarStat();

	double getMediana() {
		if (dMediana != -INFINITY) {
			return dMediana;
		}
		else
		{
			ordinaDati();
			int numDati = getNumeroDatiEffettivo();
			if (numDati % 2 == 1) {
				return dMediana = (double)vectDati[(numDati-1)/2];
			} else {
				return dMediana = (double)(vectDati[numDati/2-1]+vectDati[numDati/2])/2;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////
private:

	bool DatiOrdinati = false;
	double dMediana = -INFINITY;//Un valore che non dovrebbe assumere mai...


	inline void ordinaDati() {
		if (!DatiOrdinati){
			std::sort(aDati.begin(),aDati.end());
			DatiOrdinati = true;
		}
	}
};

} /* namespace dataAnalisi */
} /* namespace mions */

#endif /* SORTINGVARSTAT_H_ */
