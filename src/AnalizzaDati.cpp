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
#include "mylib/AnalisiDati/SortingVarStat.h"//Le mie classi Template per l'analisi dati

#define VERSIONE 0.2

/////////////////////////////////////////////////////////////////////////////////////
//Prototipi

/////////////////////////////////////////////////////////////////////////////////////
//Il primo argomento è la cartella dei dati
int main(int numParam, char* args[]) {

	using namespace std;

	system("clear");
	cout << "\n";
	cout << "Programma per analizzare i dati della guidovia, versione: " << VERSIONE << endl;
	//Ricordarsi che con 0 gradi l'intervallo era di 20!
	const int NUM_FILE = 7;// 7 file (7 intervalli) per 15, 30, 45 gradi
	const int NUM_DATIPERFILE = 5;// 5 dati in ogni file
	const int ANGOLI_NUM = 3;//15, 30, 45. 0 lo faremo a parte perchè ha un intervallo da venti cm
	try {
		//stringstream ss;
		string nf;//Nome file da aprire
		//FileStream
		fstream FileMedie;//FileStream
		using namespace mions::dataAnalisi;
		vector<VarStat<double> > arrayTempi;
		arrayTempi.reserve(NUM_FILE);

		//TODO: Test
//		vector<double> v1 = {1,1,1,100000,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,3,4,2,1,3,2};
//		vector<double> v2 = {2,3,1,-1};
//		vector<double> v3 = {4,5,6,10};
//		VarStat<double> AnDat(v2,true);
//		Sorting_VarStat<double> var2(std::move(v3),true);//Non usare più v1
//
//		cout << "MEDIANA " <<var2.getMediana() << "\n";
//		AnDat = -1*((-1)*AnDat+var2);
//
//		cout << AnDat << endl;
//
//		return 0;

		//Apri i dati senza peso
		for (int intervallo = 1; intervallo <= NUM_FILE; intervallo++) {
			for (int angoli = 1; angoli <= ANGOLI_NUM; angoli++) {
				stringstream ss;
				ifstream FileDati;
				ss << "./DatiFormattati/DatiStandardizzati/";
				ss << "d";
				ss << intervallo*10 + 40;
				ss << "_";
				ss << angoli*15;
				ss >> nf;
				//ss.flush();

				clog << nf << endl;
				FileDati.open(nf.c_str());//Apro il file indicato nell'argomento dato via shell
				if (!FileDati.is_open())
					throw "Errore: file non aperto";

				vector<double> tempVect(5);// Vector che contiene i dati di un file solo, da cui ricavare il tempo medio
				tempVect.reserve(NUM_DATIPERFILE);

				// File normale, un double per riga
				// Range-for: PER CIASCUN elemento &dato IN data, fai...
				// Notare come lo abbiamo preso per referenza, così da poterlo modificare
				clog << tempVect.size() << endl;
				for (int i = 0; i < tempVect.size(); i++){
					FileDati >> tempVect[i];
					clog << "Pos " << i << ": " << tempVect[i] << endl;
				}

				clog << "Dati letti. Analizzo..." << endl << endl;

				//AnalisiSingVarOffline_Lazy<double>* pAnDat= new AnalisiSingVarOffline_Lazy<double>(tempVect, NUM_DATIPERFILE);
				arrayTempi.emplace_back(tempVect,true);// Forwarda gli argomenti a un oggetto costruito DIRETTAMENTE nel vettore (cioè, manda gli argomenti VarStat dentro al vettore)

				cout << endl;
				cout << "Nome file: " << nf << endl;
				cout << arrayTempi.back();
				ss.clear();
				FileDati.close();
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


