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

#define VERSIONE 0.5

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
	const int ANGOLI_NUM = 3;//15, 30, 45 0 e 45 li facciamo a parte
	try {
		//stringstream ss;
//		string nf;//Nome file da aprire
//		//FileStream
//		ofstream FileMedie;//FileStream
//		ofstream FileRiassunto;
//		using namespace mions::dataAnalisi;
//		vector<VarStat<double> > arrayRiassunti;//Contiene le informazioni come la deviazione standard, etc
//		vector<double> arrayTempi;
//		arrayRiassunti.reserve(NUM_FILE);

		//Test
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



//Prima esperianza, guidovia inclinata a 15, 30 e 45 gradi senza peso
///////////////////////////////////////////////////////////////////////////////////////////////////
		{

		//stringstream ss;
		string nf;//Nome file da aprire
		//FileStream
		ofstream FileMedie;//FileStream
		ofstream FileRiassunto;
		ofstream FileVelocita;
		using namespace mions::dataAnalisi;
		vector<VarStat<double> > arrayRiassunti;//Contiene le informazioni come la deviazione standard, etc
		vector<double> arrayTempi;
		arrayRiassunti.reserve(NUM_FILE);
		string nomeoutputfile;
		string nomefilemedie;
		string nomefilevelocita;
		string nomeoutputvelocita;//Per tenere temporaneamente la variabile ed evitare il self assignment

		/* Vari casi:
		 * 	1. nè peso nè alluminio
		 */
		string tipodati;
				nomeoutputfile = "./Risultati/normale_dati.txt";
				//tipodati = "d";
				nomefilemedie = "arrayTempi_PrimaVolta_normale.txt";
				nomefilevelocita = "velocita_PV_normale.txt";//PV = prima volta

		FileRiassunto.open(nomeoutputfile.c_str());

		for (int angoli = 1; angoli <= ANGOLI_NUM; angoli++) {

			for (int intervallo = 1; intervallo <= NUM_FILE; intervallo++) {

				stringstream ss;
				ifstream FileImputDati;//File di imput
				ss << "./DatiFormattati/DatiStandardizzati/";
				ss << "d";//Tipo dei dati contenuti nel file: d, cd
				ss << intervallo*10 + 40;
				ss << "_";
				ss << angoli*15;
				ss >> nf;
				//ss.flush();


				clog << nf << endl;
				FileImputDati.open(nf.c_str());//Apro il file indicato nell'argomento dato via shell
				if (!FileImputDati.is_open())
					throw "Errore: file non aperto";

				vector<double> tempVect(5);// Vector che contiene i dati di un file solo, da cui ricavare il tempo medio
				tempVect.reserve(NUM_DATIPERFILE);

				clog << tempVect.size() << endl;
				for (unsigned int i = 0; i < tempVect.size(); i++){
					FileImputDati >> tempVect[i];
					clog << "Pos " << i << ": " << tempVect[i] << endl;
				}
				clog << "Dati letti. Analizzo..." << endl << endl;

				arrayRiassunti.emplace_back(tempVect,true);// Forwarda gli argomenti a un oggetto costruito DIRETTAMENTE nel vettore (cioè, manda gli argomenti VarStat dentro al vettore)

				cout << endl;
				cout << "Nome file: " << nf << endl;
				cout << arrayRiassunti.back();

				FileRiassunto << endl;
				FileRiassunto << "Nome file: " << nf << endl;
				FileRiassunto << arrayRiassunti.back() << endl;

				arrayTempi.push_back(arrayRiassunti.back().getMedia());

				ss.clear();
				FileImputDati.close();
			}//Intervalli


			switch (angoli) {
				case 1:
					//Ricicliamo nomeoutputfile per indicare i file di uscita
					nomeoutputfile = string("./Risultati/MetaDati/15/") + nomefilemedie;
					nomeoutputvelocita = string("./Risultati/MetaDati/15/") + nomefilevelocita;
					FileMedie.open(nomeoutputfile.c_str());
					FileVelocita.open(nomeoutputvelocita.c_str());
					for (auto& cinque_tempi_media : arrayTempi)
					{
						FileMedie << cinque_tempi_media << endl;//sette medie di cinque tempi ciascuns
						FileVelocita << 10/cinque_tempi_media << endl;//10 cm di intervallo
					}
					FileMedie.close();
					FileVelocita.close();
					break;
				case 2:
					//Ricicliamo nomeoutputfile per indicare i file di uscita
					nomeoutputfile = string("./Risultati/MetaDati/30/") + nomefilemedie;
					nomeoutputvelocita = string("./Risultati/MetaDati/30/") + nomefilevelocita;
					FileMedie.open(nomeoutputfile.c_str());
					FileVelocita.open(nomeoutputvelocita.c_str());
					for (auto& cinque_tempi_media : arrayTempi)
					{
						FileMedie << cinque_tempi_media << endl;//sette medie di cinque tempi ciascuns
						FileVelocita << 10/cinque_tempi_media << endl;//10 cm
					}
					FileMedie.close();
					FileVelocita.close();
					break;
				case 3:
					//Ricicliamo nomeoutputfile per indicare i file di uscita
					nomeoutputfile = string("./Risultati/MetaDati/45/") + nomefilemedie;
					nomeoutputvelocita = string("./Risultati/MetaDati/45/") + nomefilevelocita;
					FileMedie.open(nomeoutputfile.c_str());
					FileVelocita.open(nomeoutputvelocita.c_str());
					for (auto& cinque_tempi_media : arrayTempi)
					{
						FileMedie << cinque_tempi_media << endl;//sette medie di cinque tempi ciascuns
						FileVelocita << 10/cinque_tempi_media << endl;//10 cm
					}
					FileMedie.close();
					FileVelocita.close();
					break;
				default:
					throw "Errore: numero casi angoli sbagliato";
					break;
			}//switch
			arrayTempi.clear();
		}//Angoli

		FileRiassunto.close();
		arrayRiassunti.clear();
	  }//Blocco prima esperienza
///////////////////////////////////////////////////////////////////////////////////////////////////













//Prima esperianza, guidovia inclinata a 45 con peso
///////////////////////////////////////////////////////////////////////////////////////////////////
		/*
		 * Vari casi:
		 * 	1. nè peso nè alluminio
		 * 	2. peso
		 */
//for (int varicasi = 1; varicasi <= 2; varicasi++)
		{

			//stringstream ss;
			string nf;		//Nome file da aprire
			//FileStream
			ofstream FileMedie;				//FileStream
			ofstream FileRiassunto;
			ofstream FileVelocita;
			using namespace mions::dataAnalisi;
			vector<VarStat<double> > arrayRiassunti;//Contiene le informazioni come la deviazione standard, etc
			vector<double> arrayTempi;
			arrayRiassunti.reserve(NUM_FILE);
			string nomeoutputfile;
			string nomefilemedie;
			string nomefilevelocita;
			string nomeoutputvelocita;

			/* Vari casi:
			 * 	1. nè peso nè alluminio
			 * 	2. peso
			 */
				string tipodati;
				nomeoutputfile = "./Risultati/peso_dati.txt";
				tipodati = "cd";
				nomefilemedie = "arrayTempi_PrimaVolta_peso.txt";
				nomefilevelocita = "velocita_PV_peso.txt";


			//string nomeoutputfile = "./Risultati/nopeso_dati.txt";
			FileRiassunto.open(nomeoutputfile.c_str());

				//Se la slitta è carica fai solo i 45
				//TODO: Sporco trucco, ma sono disperato
				for (int intervallo = 1; intervallo <= NUM_FILE; intervallo++) {

					stringstream ss;
					ifstream FileImputDati;					//File di imput
					ss << "./DatiFormattati/DatiStandardizzati/";
					ss << tipodati;	//Tipo dei dati contenuti nel file: d, cd
					ss << intervallo * 10 + 40;
					ss << "_";
					ss << "45";
					ss >> nf;
					//ss.flush();

					clog << nf << endl;
					FileImputDati.open(nf.c_str());	//Apro il file indicato nell'argomento dato via shell
					if (!FileImputDati.is_open())
						throw "Errore: file non aperto";

					vector<double> tempVect(5);	// Vector che contiene i dati di un file solo, da cui ricavare il tempo medio
					tempVect.reserve(NUM_DATIPERFILE);

					clog << tempVect.size() << endl;
					for (unsigned int i = 0; i < tempVect.size(); i++) {
						FileImputDati >> tempVect[i];
						clog << "Pos " << i << ": " << tempVect[i] << endl;
					}
					clog << "Dati letti. Analizzo..." << endl << endl;

					arrayRiassunti.emplace_back(tempVect, true);// Forwarda gli argomenti a un oggetto costruito DIRETTAMENTE nel vettore (cioè, manda gli argomenti VarStat dentro al vettore)

					cout << endl;
					cout << "Nome file: " << nf << endl;
					cout << arrayRiassunti.back();

					FileRiassunto << endl;
					FileRiassunto << "Nome file: " << nf << endl;
					FileRiassunto << arrayRiassunti.back() << endl;

					arrayTempi.push_back(arrayRiassunti.back().getMedia());

					ss.clear();
					FileImputDati.close();
				}					//Intervalli

				//Ex switch
					//Ricicliamo nomeoutputfile per indicare i file di uscita
					nomeoutputfile = string("./Risultati/MetaDati/45/")
							+ nomefilemedie;
					nomeoutputvelocita = string("./Risultati/MetaDati/45/")
							+ nomefilevelocita;
					FileMedie.open(nomeoutputfile.c_str());
					FileVelocita.open(nomeoutputvelocita.c_str());
					for (auto& cinque_tempi_media : arrayTempi) {
						FileMedie << cinque_tempi_media << endl;//sette medie di cinque tempi ciascuns
						FileVelocita << 10 / cinque_tempi_media << endl;//10 cm
					}
					FileMedie.close();
					FileVelocita.close();

				arrayTempi.clear();

			FileRiassunto.close();
			arrayRiassunti.clear();
		}					//Blocco prima esperienza 45 con peso
///////////////////////////////////////////////////////////////////////////////////////////////////


































//Seconda, guidovia a zero e alluminio
///////////////////////////////////////////////////////////////////////////////////////////////////
//		/*
//		 * Vari casi:
//		 * 	1. nè peso nè alluminio
//		 * 	2. peso
//		 * 	3. peso e alluminio
//		 */
//		for (int varicasi = 1; varicasi <= 2; varicasi++) {
//
//			//stringstream ss;
//			string nf;				//Nome file da aprire
//			//FileStream
//			ofstream FileMedie;		//FileStream
//			ofstream FileRiassunto;
//			ofstream FileVelocita;
//			using namespace mions::dataAnalisi;
//			vector<VarStat<double> > arrayRiassunti;//Contiene le informazioni come la deviazione standard, etc
//			vector<double> arrayTempi;
//			arrayRiassunti.reserve(NUM_FILE);
//			string nomeoutputfile;
//			string tipodati;
//			string nomefilemedie;
//			string nomefilevelocita;
//
//			/* Vari casi:
//			 * 	1. nè peso nè alluminio
//			 * 	2. peso
//			 * 	3. peso e alluminio
//			 */
//			switch (varicasi) {
//			case 1:
//				nomeoutputfile = "./Risultati/normale_dati.txt";
//				tipodati = "d";
//				nomefilemedie = "arrayTempi_PrimaVolta_normale.txt";
//				break;
//
//			case 2:
//				nomeoutputfile = "./Risultati/peso_dati.txt";
//				tipodati = "cd";
//				nomefilemedie = "arrayTempi_PrimaVolta_peso.txt";
//				break;
//
//			case 3:
//				nomeoutputfile = "./Risultati/peso_alluminio_dati.txt";
//				tipodati = "cd";
//				nomefilemedie = "arrayTempi_PrimaVolta_peso_alluminio.txt";
//				break;
//
//			default:
//				throw "Errore: variante non contemplata";
//				break;
//			}
//
//			//string nomeoutputfile = "./Risultati/nopeso_dati.txt";
//			FileRiassunto.open(nomeoutputfile.c_str());
//
//			//for (int angoli = 1; angoli <= ANGOLI_NUM; angoli++) {
//
//			for (int intervallo = 1; intervallo <= NUM_FILE; intervallo++) {
//
//				stringstream ss;
//				ifstream FileImputDati;		//File di imput
//				ss << "./DatiFormattati/DatiStandardizzati/";
//				ss << tipodati;		//Tipo dei dati contenuti nel file: d, cd
//				ss << intervallo * 10 + 40;
//				ss << "_";
//				ss << 0;		// Solo zero l'ultima esperienza
//				ss >> nf;
//				//ss.flush();
//
//				clog << nf << endl;
//				FileImputDati.open(nf.c_str());	//Apro il file indicato nell'argomento dato via shell
//				if (!FileImputDati.is_open())
//					throw "Errore: file non aperto";
//
//				vector<double> tempVect(5);	// Vector che contiene i dati di un file solo, da cui ricavare il tempo medio
//				tempVect.reserve(NUM_DATIPERFILE);
//
//				clog << tempVect.size() << endl;
//				for (unsigned int i = 0; i < tempVect.size(); i++) {
//					FileImputDati >> tempVect[i];
//					clog << "Pos " << i << ": " << tempVect[i] << endl;
//				}
//				clog << "Dati letti. Analizzo..." << endl << endl;
//
//				arrayRiassunti.emplace_back(tempVect, true);// Forwarda gli argomenti a un oggetto costruito DIRETTAMENTE nel vettore (cioè, manda gli argomenti VarStat dentro al vettore)
//
//				cout << endl;
//				cout << "Nome file: " << nf << endl;
//				cout << arrayRiassunti.back();
//
//				FileRiassunto << endl;
//				FileRiassunto << "Nome file: " << nf << endl;
//				FileRiassunto << arrayRiassunti.back() << endl;
//
//				arrayTempi.push_back(arrayRiassunti.back().getMedia());
//
//				ss.clear();
//				FileImputDati.close();
//			}		//Intervalli
//
//			//Ricicliamo nomeoutputfile per indicare i file di uscita
//			nomeoutputfile = string("./Risultati/MetaDati/0/") + nomefilemedie;
//			FileMedie.open(nomeoutputfile.c_str());
//			for (auto& cinque_tempi_media : arrayTempi) {
//				FileMedie << cinque_tempi_media << endl;
//			}
//			FileMedie.close();
//			break;
//
//			arrayTempi.clear();
//			//}//Angoli
//
//			FileRiassunto.close();
//			arrayRiassunti.clear();
//		}		//Blocco seconda esperienza
///////////////////////////////////////////////////////////////////////////////////////////////////

























	} catch (exception &e) {
		cout << e.what() << endl;
		return -1;
	} catch (string &e) {
		cout << e << endl;
		return -2;
	} catch (const char* e) {
		cout << e << endl;
		return -3;
	}
	//cout << "\n";
	return 0;
}


