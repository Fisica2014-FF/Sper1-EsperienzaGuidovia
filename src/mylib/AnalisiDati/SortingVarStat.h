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

template <typename> class Sorting_VarStat;

//Notare come non sia const Sorting_VarStat<U>&, poichè la mediana non viene calcolata finchè non serve, quindi getMediana() potrebbe modificare l'oggetto
template <typename U>
std::ostream& operator <<(std::ostream& os, Sorting_VarStat<U>& rhs) {
	using namespace std;

	//Eclipse dà problemi con endl, modifichiamolo temporaneamente
	#define endl "\n"
	cout << "Numero dati:                       " << rhs.getNumeroDatiEffettivo() << endl;
	cout << "Media:                             " << rhs.getMedia() << endl;
	cout << "Mediana:                           " << rhs.getMediana() << endl;
	cout << "Varianza del campione:             " << rhs.getVarianzaCampione() << endl;
	cout << "Deviazione standard campione:      " << rhs.getDeviazioneStandardCamp() << endl;
	cout << "Varianza della popolazione:        " << rhs.getVarianzaPopolazione() << endl;
	cout << "Deviazione standard popolazione:   " << rhs.getDeviazioneStandardPop() << endl;
	cout << "Errore della media:                " << rhs.getErroreMedia() << endl;
	cout << "Massimo:                           " << rhs.getMax() << endl;
	cout << "Minimo:                            " << rhs.getMin() << endl;
	#undef endl

	return os;
	};

template <class T>
class Sorting_VarStat: public mions::dataAnalisi::VarStat<T> {
public:
	friend std::ostream& operator<<<T>(std::ostream& os, Sorting_VarStat<T>& rhs);

	vector<T> vectDati;
	Sorting_VarStat(const vector<T>&& aDati, bool eliminaTreSigma = true) : mions::dataAnalisi::VarStat<T>(aDati, eliminaTreSigma) {
		vectDati = aDati;//Copiati i dati, così puoi riordinarli in santa pace. In teoria move-assignment
	};

	virtual ~Sorting_VarStat() throw() {};

	double getMediana() {
		if (dMediana != -INFINITY) {
			return dMediana;
		}
		else
		{
			ordinaDati();
			int numDati = this->getNumeroDatiEffettivo();
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
		if (!DatiOrdinati) {
			std::sort(vectDati.begin(),vectDati.end());
			DatiOrdinati = true;
		}
	}
};

} /* namespace dataAnalisi */
} /* namespace mions */

#endif /* SORTINGVARSTAT_H_ */
