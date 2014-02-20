
#pragma once//L'equivalente delle Include guards

#include <cmath>
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>



#ifdef _MIO_DEBUG_

#include <iostream>//Per cerr

#endif



//Il mio namespace
namespace mions {
//Classi per l'analisi dei dati statistici
namespace dataAnalisi {
using std::vector;

///////////////////////
//					 //
//	  VERSIONE 1.3	 //
//					 //
///////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Funzioni
 */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe per l'analisi di UNA variabile statistica offline, cioè avendo accesso a tutti i dati fin dall'inizio
// O anche, che "rappresenta" una variabile statistica
// TODO: Dovrebbe essere con la lazy evalutation
template <class T>
class VarStat {
public:
	//vector<T> vectDati;
	//Costruttore
	VarStat(const vector<T>& aDati, bool eliminaTreSigma = true) {
		//aDati = {1,2,3};//La classe ha una copia del vector! Non dei dati! Copiare un vector non è troppo impegnativo. O no? NOOO!!!
		int numDatiIniziale = aDati.size();
		vector<int> ListaDatifuori3Sigma; //0.003 = 100% - 99.7% = percentuale atttesa di fuori sigma, più spazio a caso

		//Se il vettore è vuoto la random variable è 0 +- 0 Buona idea?
		if (numDatiIniziale == 0) {

			#ifdef _MIO_DEBUG_
			std::cerr << "Vettore vuoto, metto la variabile a zero+-zero";
			#endif

			iNumero_dati = 0;
			dMedia = 0;
			dDeviazioneStandardCamp = 0;
			dDeviazioneStandardPop = 0;
			dVarianzaCampione = 0;
			dVarianzaPopolazione = 0;
			dMax = 0;//Oppure +INFINITY
			dMin = 0;//O -INFINITY
			dErroreMedia = 0;
			return;
		}

		dMedia=(double)aDati[0];
		dMax=(double)aDati[0];
		dMin=(double)aDati[0];

		for(int i=0; i < numDatiIniziale; i++){
			//Media
			dMedia=(i*dMedia+(double)aDati[i])/(i+1);

			//Massimo e minimo (ottimizzabile?)
			dMax = (aDati[i] > dMax) ? aDati[i] : dMax;
			dMin = (aDati[i] < dMin) ? aDati[i] : dMin;
		}

		dVarianzaCampione=pow(((double)aDati[0]-dMedia),2);
		for(int i=0; i < numDatiIniziale; i++){
			//Varianza
			dVarianzaCampione=(i*dVarianzaCampione+pow(((double)aDati[i]-dMedia),2)) /
					(i+1);
		}

		dDeviazioneStandardCamp = sqrt(dVarianzaCampione);

		//se sigma2c=S/N e sigma2p=S/(N-1), allora, sostituendo S e risolvendo, sigma2p=sigma2c*N/(N-1)
		dVarianzaPopolazione = dVarianzaCampione*double(numDatiIniziale)/(double(numDatiIniziale)-1);
		iNumero_dati = aDati.size();
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
			int i = 0;
			for (typename vector<T>::const_iterator pDato = aDati.begin();
					pDato != aDati.end();
					pDato++, i++)// i indica l'offset dall'inizio del vector, lo useremo dopo per verificare i dati
			{
				//TODO: Finire
				//i++;//Per mettere gli offset degli iterator, probabilmente si può mettere nel ciclo direttamente a questo punto
				if (abs(dMedia - (*pDato) ) >= 3*dDeviazioneStandardCamp) {
					/* Cancelliamo dal Vector i dati inaccettabili. Operazione costosa perchè i dati successivi vengono traslati
					 * indietro, ma è meglio un Vector di una LinkedList perchè i dati possono essere messi nella cache e occuma meno memoria.
					 * erase richiede un iterator, quindi siamo "costretti" a usarlo
					 */
					std::cout << "Eliminato dato: " << *pDato << "\n";

					ListaDatifuori3Sigma.push_back(i);//Aggiungi il dato nella posizione i alla lista degli "incriminati"

					numCancellazioni = numCancellazioni + 1;
				}
			}
			std::cout << "Cancellati " << numCancellazioni << " dati\n\n";
			for (int k = 0; k < numCancellazioni+1; ++k) {
				std::cerr << ListaDatifuori3Sigma[k] << std::endl;
			}

			//Rifacciamo i conti
			numDatiIniziale = aDati.size();
			dMedia=(double)aDati[0];
			dMax=(double)aDati[0];
			dMin=(double)aDati[0];

			// j è l'indice del vettore che contiene gli indici dei dati da scartare. Ovviamente si suppone che questi siano ordinati e strettamente meno di numDatiIniziale
			int j = 0;
			for(int i=0; i < numDatiIniziale; i++) {
				//Media
				//Se il dato è buono prosegui coi calcoli
				if (i != *(ListaDatifuori3Sigma.begin() + j) ) {
					dMedia=(i*dMedia+(double)aDati[i])/(i+1);

					//Massimo e minimo
					dMax = (aDati[i] > dMax) ? aDati[i] : dMax;
					dMin = (aDati[i] < dMin) ? aDati[i] : dMin;
				} else {
					//Il dato è da scartare, prosegui con quello successivo (e avanza al prossimo numero nella lista, intanto)
					j++;
				}
			}

			// Di nuovo j
			j=0;
			dVarianzaCampione=pow(((double)aDati[0]-dMedia),2);
			for(int i=0; i < numDatiIniziale; i++) {
				//Varianza
				if (i != *(ListaDatifuori3Sigma.begin() + j)) {
					dVarianzaCampione=(i*dVarianzaCampione+pow(((double)aDati[i]-dMedia),2)) /
							(i+1);
				} else {
					j++;
				}
			}
			dDeviazioneStandardCamp = sqrt(dVarianzaCampione);

			//se sigma2c=S/N e sigma2p=S/(N-1), allora, sostituendo S e risolvendo, sigma2p=sigma2c*N/(N-1)
			dVarianzaPopolazione = dVarianzaCampione*double(numDatiIniziale)/(double(numDatiIniziale)-1);

		}//EndIf
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//Deviazione standard popolazione
		dDeviazioneStandardPop=sqrt(dVarianzaPopolazione);
		iNumero_dati = numDatiIniziale - ListaDatifuori3Sigma.size();
	}

	//Distruttore
	virtual ~VarStat() = default;//Virtual perchè devono ereditare da questa. Lecito il default? Bè compila

	//Getters
	inline double getMedia() const {return dMedia;};
	//Scarto Quadratico Medio (N)
	inline double getDeviazioneStandardCamp() const {return dDeviazioneStandardCamp;};
	//Errore Quadrato Medio N-1
	inline double getDeviazioneStandardPop() const {return dDeviazioneStandardPop;};
	//Su Excel sono invertite, cioè per varianza del campione io intendo la varianza propria dei dati
	inline double getVarianzaCampione() const {return dVarianzaCampione;}
	//Su Excel sono invertite, cioè per varianza popolazione io considero implicitamente i dati come un campione quindi
	//la varianzaPopolazione è calcolata fratto N-1
	inline double getVarianzaPopolazione() const {return dVarianzaPopolazione;}
	//double getMediana() ordina i dati come side effect
	//Tolta
	inline double getMax() const {return dMax;}
	inline double getMin() const {return dMin;}
	// Errore della media
	inline double getErroreMedia() {return (dErroreMedia != -INFINITY ? dErroreMedia : dErroreMedia = getVarianzaPopolazione() / getNumeroDatiEffettivo());}
	long getNumeroDatiEffettivo() const {return iNumero_dati;}
	//Range della variabile
	inline long getRange() const {return dMax - dMin;}
	//double getModa() const {return dModa;}

	//Operatori
	//Somma una variabile statistica a un'altra e memorizzala nella prima. Vedi commento su -=, sotto
	inline VarStat<T>& operator+=(const VarStat<T>& rhs) {
		//Obiettivo: dare gli stessi risultati come se avessi sommato i dati di due insiemi (i dati due a due, non gli insiemi) ma senza un ordine definito tra i due
		//iNumero_dati = iNumero_dati; Non sto unendo gli insiemi di dati, ma sommando i singoli elementi fra loro
		dMedia = rhs.getMedia() + getMedia();//Somma le medie delle due variabili
		dVarianzaCampione = pow(rhs.getVarianzaCampione(),2) + pow(getVarianzaCampione(),2);//Propagazione dell'errore
		dVarianzaPopolazione = pow(rhs.getVarianzaPopolazione(),2) + pow(getVarianzaPopolazione(),2);

		//La nuova varianza permette di calcolare direttamente la nuova std
		dDeviazioneStandardCamp = sqrt(getVarianzaCampione());
		dDeviazioneStandardPop = sqrt(getVarianzaPopolazione());

		// Il massimo della somma è la somma dei due massimi
		// Worst-case max? Non ben definito, ma se ho due set di dati, il massimo (tra tutte le possibilità) è la somma dei due massimi precedenti
		dMax = rhs.getMax() +getMax();

		//Idem per il minimo, il minimo "minore" è la somma dei minimi
		dMin = rhs.getMin() + getMin();

		dErroreMedia = getVarianzaPopolazione() / getNumeroDatiEffettivo();
		return *this;	//Idiozia ma dicono che serva
	}

	//Sottrai una variabile statistica a un'altra e memorizzala nella prima. v1 -= v2 è come v.operator-=(v2), quindi le funzioni get, etc qui dentro si riferiscono a v1!!! E rhs.get... a v2

	inline VarStat<T>& operator-=(const VarStat<T>& rhs) {
		//TODO: Possibile farlo come lhs += (-1)*rhs ?
		//Obiettivo: dare gli stessi risultati come se avessi sottratto i dati di due insiemi (i dati due a due, non gli insiemi) ma senza un ordine definito tra i due
		//iNumero_dati = iNumero_dati; Non sto unendo gli insiemi di dati, ma sottraendo i singoli elementi fra loro
		dMedia = getMedia() - rhs.getMedia();//Sottrai le medie delle due variabili
		dVarianzaCampione = pow(rhs.getVarianzaCampione(),2) + pow(getVarianzaCampione(),2);//Propagazione dell'errore
		dVarianzaPopolazione = pow(rhs.getVarianzaPopolazione(),2) + pow(getVarianzaPopolazione(),2);

		//La nuova varianza permette di calcolare direttamente la nuova std
		dDeviazioneStandardCamp = sqrt(getVarianzaCampione());
		dDeviazioneStandardPop = sqrt(getVarianzaPopolazione());

		//Worst-case min
		// Il massimo della differenza è la differenza tra il valore più alto del minuendo e quello più basso del sottraendo
		dMax = getMax() - rhs.getMin();

		//Il minimo "minore" è il minimo del minuendo a cui abbiamo tolto il più possibile, cioè il massimo del sottraendo
		dMin = getMin() - rhs.getMax();

		dErroreMedia = getVarianzaPopolazione() / getNumeroDatiEffettivo();
		return *this;	//Idiozia ma dicono che serva
	}

	//Moltiplicazione per scalare, compound assignment. v *= d è come v.operator*=(d), quindi le funzioni get, etc qui dentro si riferiscono a v
	inline VarStat<T>& operator*=(const double& rhs) {
		//Obiettivo: dare gli stessi risultati come se avessi preso tutti i dati e, moltiplicato ciascuno per rhs, li avessi passati a lhs
		dMedia = rhs * getMedia();//Moltiplica la media della VarStat a sinistra del simbolo per il double a destra
		dVarianzaCampione = rhs * rhs * getVarianzaCampione();
		dVarianzaPopolazione = rhs * rhs * getVarianzaPopolazione();
		dDeviazioneStandardCamp = abs(rhs) * getDeviazioneStandardCamp();
		dDeviazioneStandardPop = abs(rhs) * getDeviazioneStandardCamp();

		//Se moltiplico per uno scalare negativo, il minimo nei positivi diventa il massimo nei negativi (es se min=-20 e max=40, se li moltiplico per -2 allora min=-40 e max=20)
		dMax = (rhs >= 0 ? rhs * getMax() : rhs * getMin());
		dMin = (rhs >= 0 ? rhs * getMin() : rhs * getMax());

		dErroreMedia = getVarianzaPopolazione() / getNumeroDatiEffettivo();
		return *this;	//Idiozia ma dicono che serva
	}

	//Somma di due VarStat
	//Trucchetto per riutilizzare il lavoro svolto con +=
	const VarStat<T> operator+(const VarStat<T> &other) const {
		VarStat<T> result = *this; // Copia il primo oggettp
		result += other;            // Aggiungici dentro l'altro
		return result;              // Ritorna il risultato
	}

	//Sottrazione di due VarStat
	//Trucchetto per riutilizzare il lavoro svolto con -=
	const VarStat<T> operator-(const VarStat<T>& other) const {
		VarStat<T> result = *this; // Copia il primo oggettp
		result -= other;            // Aggiungici dentro l'altro
		return result;              // Ritorna il risultato
	}

	//
	std::ostream& operator<<(std::ostream& out) {
		using namespace std;
		cout << getNumeroDatiEffettivo() << "Numero dati: " << endl;
		cout << getMedia() << "Media: " << endl;
		//cout << "Mediana: "<< AnDat.getMediana() << endl;
		cout << getVarianzaCampione() << "Varianza del campione: " << endl;
		cout << getDeviazioneStandardCamp() << "Deviazione standard campione: " << endl;
		cout << getVarianzaPopolazione() << "Varianza della popolazione: " << endl;
		cout << getDeviazioneStandardPop() << "Deviazione standard popolazione: " << endl;
		cout << getErroreMedia() << "Errore della media: " << endl;
		cout << getMax() << "Massimo: " << endl;
		cout << getMin() << "Minimo: " << endl;
		return out;
	}
	;

	//Moltiplicazione per uno scalare
	//Trucchetto per riutilizzare il lavoro svolto con *= double,
	const VarStat<T> operator*(const double rhs) const {
		VarStat<T> result = *this; // Copia il primo oggettp
		result *= rhs;            // Aggiungici dentro l'altro
		return result;              // Ritorna il risultato
	}

private:
	//Bit-Mask per i vari stati dei dati 1=Ordinati

	double dMedia = -INFINITY;
	double dDeviazioneStandardCamp = -INFINITY;
	double dDeviazioneStandardPop = -INFINITY;
	double dVarianzaCampione = -INFINITY;
	double dVarianzaPopolazione = -INFINITY;
	//double dMediana = -INFINITY;
	double dMax = -INFINITY;
	double dMin = -INFINITY;
	double dErroreMedia = -INFINITY;
	int iNumero_dati = 0;
	//double dModa=0;

};




}//Fine DataAnalisi

}//Fine del mio namespace
