//============================================================================
// Name        : Misure.cpp
// Author      : Francesco Forcher
// Version     : 0.1
// Description : Programma per analizzare i dati sul pendolo raccolti in laboratorio
//============================================================================

// VERSIONE PER I COMPAGNI DEL GRUPPO!!! NON CONSEGNARE QUESTA!!!!
// Quando scrivo TODO: è perchè con eclipse dopo mi consente di trovare i commenti da togliere più in fretta

/////////////////////////////////////////////////////////////////////////////////////
//Librerie
#include <iostream>//cin e cout
#include <fstream>//FileStream
#include <exception>//Eccezioni
#include <string>
#include <cstdlib>
#include <algorithm>//Sort?
#include <string>
#include <sstream>//StringStream

/////////////////////////////////////////////////////////////////////////////////////
//le mie classi
#include "mylib/AnalisiDati/VarStat.h"//Le mie classi Template per l'analisi dati
#define VERSIONE 0.1

/////////////////////////////////////////////////////////////////////////////////////
//Prototipi
/**legge il file di dati formattato "NomeFile" e analizza i dati ivi contenuti
 * @param: NomeFile, nome del file da aprire
 * @throw: string
 * @return: void
 */
inline void leggiFile(char const * const NomeFile) throw(std::string);

//////////////////////////////////////////////////////////////////////////////////////

int main(int numParam, char* args[]) {

	using namespace std;

	system("clear");
	cout << "\n";
	cout << "Programma per analizzare i dati della guidovia, versione: " << VERSIONE << endl;
	/* Esempio di file di dati:
	 * #FORMATO_D
	 * #NUMDATI_3
	 * 03.1
	 * 2.4
	 * 2.67
	 */
	//Ricordarsi che con 0 gradi l'intervallo era di 20!
	const int NUM_FILE = 7*3;// 7 file (7 intervalli) per 15, 30, 45 gradi
	const int NUM_DATIPERFILE = 5;// 5 dati in ogni file
	const int ANGOLI_NUM = 3;//15, 30, 45. 0 lo faremo a parte perchè ha un intervallo da venti cm
	try {
		stringstream ss;
		string nf;//Nome file da aprire
		fstream FileDati;//FileStream
		using namespace mions::dataAnalisi;
		vector<VarStat<double> > arrayTempi;
		arrayTempi.reserve(NUM_DATIPERFILE);

		//Apri i ddati senza peso
		for (int i = 1; i <= ANGOLI_NUM; i++) {
			for (int j = 1; j <= NUM_FILE; ++j) {
				ss << "d";
				ss << j*10 + 40;
				ss << "_";
				ss << j*15;
				ss >> nf;

				FileDati.open(nf.c_str(), fstream::in | fstream::out);//Apro il file indicato nell'argomento dato via shell
				if (!FileDati.is_open())
					throw "Errore: file non aperto";

				vector<double> tempVect(NUM_DATIPERFILE);// Vector che contiene i dati di un file solo, da cui ricavare il tempo medio

				// File normale, un double per riga
				// Range-for: PER CIASCUN elemento &dato IN data, fai...
				// Notare come lo abbiamo preso per referenza, così da poterlo modificare
				for (double& dato : tempVect)
					FileDati >> dato;

				cout << "Dati letti. Analizzo..." << endl << endl;

				//AnalisiSingVarOffline_Lazy<double>* pAnDat= new AnalisiSingVarOffline_Lazy<double>(tempVect, NUM_DATIPERFILE);
				arrayTempi.emplace_back(tempVect, NUM_DATIPERFILE);// Forwarda gli argomenti a un oggetto costruito DIRETTAMENTE nel vettore
			}
		}
	} catch (exception &e) {
		cout << e.what() << endl;
		return -1;
	} catch (string &e) {
		cout << e << endl;
		return -2;
	}
	//cout << "\n";
	return 0;
}

































//FORSE LA TOGLIAMO
/**legge il file di dati formattato "NomeFile" e analizza i dati ivi contenuti
 * @param: NomeFile, nome del file da aprire
 * @throw: string
 * @return: void
 */
inline void leggiFile(char const * const NomeFile) throw(std::string) {
	using namespace std;

	fstream FileDati;//FileStream

	cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
	cout << "Apro il file di dati: \n\n";
	cout << NomeFile << endl << endl;
	FileDati.open(NomeFile, fstream::in | fstream::out);//Apro il file indicato nell'argomento dato via shell
	if (!FileDati.is_open())
		throw "Errore: file non aperto";

	//Leggi il formato dalla prima riga
	string formato = "";
	{
		// Il file deve iniziare con un riga con il numero dei dati preceduto da #NUMDATI_ e uno spazio, esempio "#NUMDATI_100\n"
		string cancellettoPiuFormato;
		stringstream togliCancelletto;
		getline(FileDati,cancellettoPiuFormato);
		string tst = cancellettoPiuFormato.substr(0,9);
		if (tst != (string)"#FORMATO_")
			throw "Errore: formattazione file errata:\n"+
			(string)"	Atteso #FORMATO_ all'inizio del file";
		//togli i primi 9 caratteri, cioè "#NUMDATI_" (leggi dal decimo in poi, npos significa "fino alla fine della stringa")
		togliCancelletto << cancellettoPiuFormato.substr(9,string::npos);
		togliCancelletto >> formato;
	}

	//Leggi il numero dei dati dalla seconda riga
	long numeroDati=0;
	{
		// Il file deve iniziare con un riga con il numero dei dati preceduto da #NUMDATI_ e uno spazio, esempio "#NUMDATI_100\n"
		string CancellettoPiuNumero;
		stringstream TogliCancelletto;
		getline(FileDati,CancellettoPiuNumero);
		string tst = CancellettoPiuNumero.substr(0,9);
		if (tst != (string)"#NUMDATI_")
			throw "Errore: formattazione file errata:\n"+
			(string)"	Atteso #NUMDATI_n come seconda riga del file";
		//togli i primi 9 caratteri, cioè "#NUMDATI_" (leggi dal decimo in poi, npos significa "fino alla fine della stringa")
		TogliCancelletto << CancellettoPiuNumero.substr(9,string::npos);
		TogliCancelletto >> numeroDati;
	}


	//TODO: Un Vector è un contenitore, una specie di array ridimensionabile automaticamente
	vector<double> dati(numeroDati);// TODO: Array (un Vector in realtà) dei dati.

	//Leggi i dati dal file a seconda del formato indicato all'inizio
	if (formato == "D") {
		// File normale, un double per riga
		// Range-for: PER CIASCUN elemento &dato IN data, fai...
		// Notare come lo abbiamo preso per referenza, così da poterlo modificare
		for (double& dato : dati)
			FileDati >> dato;
	}
	else if (formato == "D,I") {
		// File csv, comma separated values. In questo caso un double e un int separati da una virgola
		int _i;
		char _sep;
		for (double& dato : dati)
			FileDati >> dato >> _sep >> _i;
	}
	else {
		// Lancia un errore se il formato non è uno di questi
		throw "Errore: Formato dati non riconosciuto";
	}


	cout << "Dati letti. Analizzo..." << endl << endl;

	using namespace mions::dataAnalisi;
	//TODO: Costruisci la classe che contiene le stime statistiche dei dati (andatevi a leggere il file analisiDati.h)
	VarStat<double> AnDat(dati, numeroDati);

	cout << "Numero dati: " << AnDat.getNumeroDatiEffettivo() << endl;
	cout << "Media: "<< AnDat.getMedia() << endl;
	//cout << "Mediana: "<< AnDat.getMediana() << endl;
	cout << "Varianza del campione: " << AnDat.getVarianzaCampione() << endl;
	cout << "Deviazione standard campione: " << AnDat.getDeviazioneStandardCamp() << endl;
	cout << "Varianza della popolazione: " << AnDat.getVarianzaPopolazione() << endl;
	cout << "Deviazione standard popolazione: " << AnDat.getDeviazioneStandardPop() << endl;
	cout << "Errore della media: " << AnDat.getErroreMedia() << endl;
	cout << "Massimo: "<< AnDat.getMax() << endl;
	cout << "Minimo: "<< AnDat.getMin() << endl;

	//Se è il file dell 990 misure dividilo in clasi e analizzalo
	if (string(NomeFile).find("990mis") != string::npos) {
		cout << "\n\nRILEVATO FILE 990mis" << endl;
		cout << "Analisi classi di 10 classi di dati\n" << endl;

		//Array delle classi
		vector<vector<double> > classiDati;
		classiDati.reserve(10);

		//Riempi le 10 classi
		double media_attuale = 0;
		double sigma2p_attuale = 0;//Varianza (N-1)
		//long numero_mis_attuale = 0;
		for(int i = 0; i < 10; i++) {
			cout << "" << endl;
			vector<double> TempVect(dati.begin() + 99 * i, dati.begin() + 99 * (i+1));
			classiDati.push_back(TempVect);
			VarStat<double> AnDatClassi(classiDati[i], false);

			//Misure precedenti
			double media_precedente = media_attuale;
			double sigma2p_precedente = sigma2p_attuale;//Varianza (N-1)
			//long numero_mis_precedente = numero_mis_attuale;

			//Misure attuali
			media_attuale = AnDatClassi.getMedia();
			sigma2p_attuale = AnDatClassi.getVarianzaPopolazione();
			//numero_mis_attuale = AnDatClassi.getNumeroDatiEffettivo();

			//cout << "Media classe " << i+1 << ": " << AnDatClassi.getMedia() << endl;

			//Memorizza i dati della prima classe
			double sigma2p_prima;
			double media_prima;
			if (i==0) {
				sigma2p_prima = sigma2p_attuale;
				media_prima = media_attuale;
			}

			//Analizza la compatibilità fra classi contigue
			if (i>0) {
				double compatibilita = abs(media_attuale - media_precedente) / sqrt(sigma2p_attuale + sigma2p_precedente);
				cout << "Compatibilità tra la " << i+1 << "a e " << i << "a classe: " << compatibilita << endl;
			}

			//Alla fine confronta la prima e l'ultima
			if (i==9) {
				double compatibilita_prima_ultima = abs(media_prima - media_attuale) / sqrt(sigma2p_prima + sigma2p_attuale);
				cout << "\nCompatibilità tra la prima e l\'ultima classe: " << compatibilita_prima_ultima << endl;
			}
		}
	}
}
