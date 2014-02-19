
#pragma once//L'equivalente delle Include guards

#include <cmath>
#include <vector>
#include <algorithm>
#include <memory>

//Il mio namespace
namespace mions {
typedef char bitmask;
typedef const char bit;

//Classi per l'analisi dei dati statistici
namespace dataAnalisi {
using std::vector;

///////////////////////
//					 //
//	  VERSIONE 1.1	 //
//					 //
///////////////////////

// Classe per l'analisi di UNA variabile statistica offline, cioè avendo accesso a tutti i dati fin dall'inizio
// O anche, che "rappresenta" una variabile statistica
// TODO: Dovrebbe essere con la lazy evalutation
template <class T>
class VarStat {
public:
	vector<T> vectDati;
	//Costruttore
	VarStat(const vector<T>& aDati, bool eliminaTreSigma = true) {
		vectDati = aDati;//La classe ha una copia del vector! Non dei dati! Copiare un vector non è troppo impegnativo. O no? NOOO!!!
		int numDati = vectDati.size();

		dMedia=(double)vectDati[0];
		dMax=(double)vectDati[0];
		dMin=(double)vectDati[0];

		for(int i=0; i < numDati; i++){
			//Media
			dMedia=(i*dMedia+(double)vectDati[i])/(i+1);

			//Massimo e minimo (ottimizzabile?)
			dMax = (vectDati[i] > dMax) ? vectDati[i] : dMax;
			dMin = (vectDati[i] < dMin) ? vectDati[i] : dMin;
		}

		dVarianzaCampione=pow(((double)vectDati[0]-dMedia),2);
		for(int i=0; i < numDati; i++){
			//Varianza
			dVarianzaCampione=(i*dVarianzaCampione+pow(((double)vectDati[i]-dMedia),2)) /
					(i+1);
		}

		dDeviazioneStandardCamp = sqrt(dVarianzaCampione);

		//se sigma2c=S/N e sigma2p=S/(N-1), allora, sostituendo S e risolvendo, sigma2p=sigma2c*N/(N-1)
		dVarianzaPopolazione = dVarianzaCampione*double(numDati)/(double(numDati)-1);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Se eliminaTreSigma è true, rifai i conti togliendo i dati inaccettabili
		if (eliminaTreSigma){
			std::cout << "Elimino i dati oltre 3 sigma...\n" ;
			/* pDato è un tipo vector<double>::iterator, e si comporta come un puntatore a un elemento dell'array
			 * Sarebbe più leggibile scrivere "auto pDato = vectDati.begin();", ma per chiarezza mettiamo il tipo completo
			 *
			 * typename è richiesto perchè se qualcuno scrivesse "T::iterator * iter;" e se per esempio T contenesse un int chiamato iterator questa
			 * sarebbe una moltiplicazione (stupido c++), quindi dobbiamo specificare che intendiamo un tipo. Vedere:
			 * http://pages.cs.wisc.edu/~driscoll/typename.html#real_reason
			 *
			 * Non incrementiamo l'iteratore (pDato++) nell'istruzione for, invece lo assegnamo nel ciclo
			 */
			int numCancellazioni = 0;
			for (typename vector<T>::iterator pDato = vectDati.begin();
					pDato != vectDati.end();)
			{
				//TODO: Usando 9*varianza forse si risparmiano conti
				if (abs(dMedia - *pDato ) >= 3*dDeviazioneStandardCamp) {
					/* Cancelliamo dal Vector i dati inaccettabili. Operazione costosa perchè i dati successivi vengono traslati
					 * indietro, ma è meglio un Vector di una LinkedList perchè i dati possono essere messi nella cache e occuma meno memoria.
					 * erase richiede un iterator, quindi siamo "costretti" a usarlo
					 */
					std::cout << "Eliminato dato: " << *pDato << "\n";

					//Erase restituisce l'iteratore dell'elemento successivo a quelli appena cancellati
					pDato = vectDati.erase(pDato);

					numCancellazioni = numCancellazioni + 1;
				} else {
					//Passiamo all'elemento successivo
					pDato++;
				}
			}
			std::cout << "Cancellati " << numCancellazioni << " dati\n\n";

			//Rifacciamo i conti
			numDati = vectDati.size();
			dMedia=(double)vectDati[0];
			dMax=(double)vectDati[0];
			dMin=(double)vectDati[0];

			for(int i=0; i < numDati; i++) {
				//Media
				dMedia=(i*dMedia+(double)vectDati[i])/(i+1);

				//Massimo e minimo
				dMax = (vectDati[i] > dMax) ? vectDati[i] : dMax;
				dMin = (vectDati[i] < dMin) ? vectDati[i] : dMin;
			}

			dVarianzaCampione=pow(((double)vectDati[0]-dMedia),2);
			for(int i=0; i < numDati; i++) {
				//Varianza
				dVarianzaCampione=(i*dVarianzaCampione+pow(((double)vectDati[i]-dMedia),2)) /
						(i+1);
			}
			dDeviazioneStandardCamp = sqrt(dVarianzaCampione);

			//se sigma2c=S/N e sigma2p=S/(N-1), allora, sostituendo S e risolvendo, sigma2p=sigma2c*N/(N-1)
			dVarianzaPopolazione = dVarianzaCampione*double(numDati)/(double(numDati)-1);

		}//EndIf
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//Deviazione standard popolazione
		dDeviazioneStandardPop=sqrt(dVarianzaPopolazione);
	}

	//Getters
	double getMedia() const {return dMedia;};
	//Scarto Quadratico Medio (N)
	double getDeviazioneStandardCamp() const {return dDeviazioneStandardCamp;};
	//Errore Quadrato Medio N-1
	double getDeviazioneStandardPop() const {return dDeviazioneStandardPop;};
	//Su Excel sono invertite, cioè per varianza del campione io intendo la varianza propria dei dati
	double getVarianzaCampione() const {return dVarianzaCampione;}
	//Su Excel sono invertite, cioè per varianza popolazione io considero implicitamente i dati come un campione quindi
	//la varianzaPopolazione è calcolata fratto N-1
	double getVarianzaPopolazione() const {return dVarianzaPopolazione;}
	//double getMediana() ordina i dati come side effect
	double getMediana() {
		if (dMediana != 0) {
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

	double getMax() const {return dMax;}
	double getMin() const {return dMin;}
	//TODO: If a basso costo per via della BPU
	double getErroreMedia() {return (dErroreMedia != 0 ? dErroreMedia : dErroreMedia = getVarianzaPopolazione() / getNumeroDatiEffettivo());}
	long getNumeroDatiEffettivo() const {return vectDati.size();}
	//double getModa() const {return dModa;}

private:
	//Bit-Mask per i vari stati dei dati 1=Ordinati
	bitmask flagStato = 0;

	bit bitDatiOrdinati = 1;

	double dMedia = nan("");
	double dDeviazioneStandardCamp = nan("");
	double dDeviazioneStandardPop = nan("");
	double dVarianzaCampione = nan("");
	double dVarianzaPopolazione = nan("");
	double dMediana = 0;
	double dMax = nan("");
	double dMin = nan("");
	double dErroreMedia = 0;
	//double dModa=0;

	//TODO Idea: un altro array di int con l'ordine dei dati
	inline void ordinaDati() {
		if ((flagStato & bitDatiOrdinati) != bitDatiOrdinati) {
			std::sort(vectDati.begin(),vectDati.end());
			flagStato = flagStato | bitDatiOrdinati;
		}
	}
};



}//Fine DataAnalisi

}//Fine del mio namespace
