//============================================================================
// Name        : Misure.cpp
// Author      : Francesco Forcher
// Version     : 0.1
// Description : Programma per analizzare i dati sul pendolo raccolti in laboratorio
//============================================================================

// VERSIONE PER I COMPAGNI DEL GRUPPO!!! NON CONSEGNARE QUESTA!!!!

/////////////////////////////////////////////////////////////////////////////////////
//Librerie
#include <iostream>//cin e cout
#include <fstream>//FileStream
#include <exception>//Eccezioni
#include <string>
#include <cstdlib>//system(clear)
#include <algorithm>//Sort?
#include <sstream>//StringStream

/////////////////////////////////////////////////////////////////////////////////////
//le mie classi
#include "mylib/AnalisiDati/VarStat.h"//Le mie classi Template per l'analisi dati
#include "mylib/AnalisiDati/SortingVarStat.h"//Le mie classi Template per l'analisi dati

#define VERSIONE 1.5

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
	const int NUM_FILE_0GRADI = 6; //  file per 60,70,80,90,100,110
	const int NUM_DATIPERFILE = 5;// 5 dati in ogni file
	const int ANGOLI_NUM = 3;//15, 30, 45 0 e 45 li facciamo a parte
	//const auto INTERVALLO = VarStat<double>(0.1, 0.001 / sqrt(6));//Distribuzione triangolare

	try {
		//stringstream ss;
		string nf;//Nome file da aprire
		//FileStream
		ofstream FileMedie;//FileStream
		ofstream FileRiassunto;
		using namespace mions::dataAnalisi;
		vector<VarStat<double> > arrayRiassunti;//Contiene le informazioni come la deviazione standard, etc
		vector<VarStat<double> > arrayTempi;
		arrayRiassunti.reserve(NUM_FILE);
		const auto INTERVALLO_PV = VarStat<double>(0.1, 0.001 / sqrt(6));//Distribuzione triangolare

//		vector<double> v1 = {1,1,1,100000,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,3,4,2,1,3,2};
//
//		vector<double> v2 = {3,4,7,8};
//		vector<double> v3 = {1,2,5,6};
//		VarStat<double> var2(v2,true);
//		VarStat<double> var3(v3,true);
//
//		VarStat<double> AnDat(v2,true);
//		//Sorting_VarStat<double> var2(std::move(v3));//Non usare più v1
//		//cout << "MEDIANA " <<var2.getMediana() << "\n";
//		AnDat = (AnDat-var2)/(AnDat*3*var2);
//		cout << "var2: \n" << var2 << endl;
//		cout << "var3: \n" << var3 << endl;
//		cout << "somma elementi: \n" << VarStat<double>({4,6,12,14}) << endl;
//		cout << "somma variabili: \n" << (var2+var3) << endl;
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
		vector<VarStat<double> > arrayTempi;

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
			//TODO aggiunto zero all'inizio
			arrayTempi.emplace_back(0.0);

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
				for (unsigned int i = 0; i < tempVect.size(); i++) {
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

				arrayTempi.emplace_back(arrayRiassunti.back());

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
					//const auto intervallo = VarStat<double>(0.1, 0.1 / sqrt(6));
					for (int i = 0; i < NUM_FILE; i++)
					{
						FileMedie << ((arrayTempi[i+1]+arrayTempi[i])/2).getMedia() << endl;//sette medie di cinque tempi ciascuns
						FileVelocita << (INTERVALLO_PV / (arrayTempi[i+1] - arrayTempi[i]) ).getMedia() << " "
								<< (INTERVALLO_PV / (arrayTempi[i+1] - arrayTempi[i]) ).getDeviazioneStandardPop() << endl;//10 cm di intervallo/cinque_tempi_media << endl;//10 cm di intervallo
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
					for (int i = 0; i < NUM_FILE; i++)
					{
						FileMedie << ( (arrayTempi[i+1] + arrayTempi[i]) / 2 ).getMedia() << endl;//sette medie di cinque tempi ciascuns
						FileVelocita << (INTERVALLO_PV / (arrayTempi[i+1] - arrayTempi[i]) ).getMedia() << " "
								<< (INTERVALLO_PV / (arrayTempi[i+1] - arrayTempi[i]) ).getDeviazioneStandardPop() << endl;//10 cm di intervallo/cinque_tempi_media << endl;//10 cm di intervallo
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
					for (int i = 0; i < NUM_FILE; i++)
					{
						FileMedie << ((arrayTempi[i+1]+arrayTempi[i])/2).getMedia() << endl;//sette medie di cinque tempi ciascuns
						FileVelocita << (INTERVALLO_PV / (arrayTempi[i+1] - arrayTempi[i]) ).getMedia() << " "
								<< (INTERVALLO_PV / (arrayTempi[i+1] - arrayTempi[i]) ).getDeviazioneStandardPop() << endl;//10 cm di intervallo/cinque_tempi_media << endl;//10 cm di intervallo
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
	  }//Blocco prima esperienza senza peso
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

			string nf;						// Nome file
			ofstream FileMedie;				// File con le medie dei tempi
			ofstream FileRiassunto;			// File con le informazioni sulle cinquine di dati
			ofstream FileVelocita;
			using namespace mions::dataAnalisi;
			vector<VarStat<double> > arrayRiassunti;//Contiene le informazioni come la deviazione standard, etc
			vector<VarStat<double> > arrayTempi;
			//TODO aggiunto zero all'inizio
			arrayTempi.emplace_back(0.0);

			arrayRiassunti.reserve(NUM_FILE);
			string nomeoutputfile;
			string nomefilemedie;
			string nomefilevelocita;
			string nomeoutputvelocita;


				string tipodati;
				nomeoutputfile = "./Risultati/peso_dati.txt";
				tipodati = "cd";
				nomefilemedie = "arrayTempi_PrimaVolta_peso.txt";
				nomefilevelocita = "velocita_PV_peso.txt";


			//string nomeoutputfile = "./Risultati/nopeso_dati.txt";
			FileRiassunto.open(nomeoutputfile.c_str());
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

					arrayTempi.emplace_back(arrayRiassunti.back());

					ss.clear();
					FileImputDati.close();
				}//Intervalli

				//Ex switch
					//Ricicliamo nomeoutputfile per indicare i file di uscita
					nomeoutputfile = string("./Risultati/MetaDati/45/")
							+ nomefilemedie;
					nomeoutputvelocita = string("./Risultati/MetaDati/45/")
							+ nomefilevelocita;
					FileMedie.open(nomeoutputfile.c_str());
					FileVelocita.open(nomeoutputvelocita.c_str());
					for (int i = 0; i < NUM_FILE; i++)
					{
						FileMedie << ((arrayTempi[i+1]+arrayTempi[i])/2).getMedia() << endl;//sette medie di cinque tempi ciascuns
						FileVelocita << (INTERVALLO_PV / (arrayTempi[i+1] - arrayTempi[i]) ).getMedia() << " "
								<< (INTERVALLO_PV / (arrayTempi[i+1] - arrayTempi[i]) ).getDeviazioneStandardPop() << endl;//10 cm di intervallo/cinque_tempi_media << endl;//10 cm di intervallo
					}
					FileMedie.close();
					FileVelocita.close();

			arrayTempi.clear();

			FileRiassunto.close();
			arrayRiassunti.clear();
		}					//Blocco prima esperienza 45 con peso
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////















//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Seconda giornata %
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//Seconda esperienza, 0 gradi con alluminio e peso
		/* Vari casi:
		 * 	1. nè peso nè alluminio (file "d...")
		 * 	2. peso (file "cd...")
		 * 	4. alluminio
		 * 	3. peso e alluminio ("cad...")
		 */
		for (int varicasi = 1; varicasi <= 4; varicasi++)
		{
			string nf;						//Nome file da aprire
			ofstream FileMedie;				//File per le medie
			ofstream FileRiassunto;
			ofstream FileVelocita;
			using namespace mions::dataAnalisi;
			vector<VarStat<double> > arrayRiassunti;//Contiene le informazioni come la deviazione standard, etc
			vector<VarStat<double> > arrayTempi;//Niente zero perchè non devo dividere per la differenza
			arrayRiassunti.reserve(NUM_FILE_0GRADI);
			string nomeoutputfile;
			string nomefilemedie;
			string nomefilevelocita;
			string nomeoutputvelocita;
			const auto INTERVALLO_SV = VarStat<double>(0.2, 0.001 / sqrt(6));//Distribuzione triangolare


			/* Vari casi:
			 * 	1. nè peso nè alluminio (file "d...")
			 * 	2. peso (file "cd...")
			 * 	4. alluminio
			 * 	3. peso e alluminio ("cad...")
			 */
			string tipodati;
			switch (varicasi) {
				case 1: // normale
					nomeoutputfile = "./Risultati/normale_0gradi_dati.txt";
					tipodati = "d";
					nomefilemedie = "arrayTempi_0gradi_normale.txt";
					nomefilevelocita = "velocita_0gradi_normale.txt";
					break;
				case 2: // peso
					nomeoutputfile = "./Risultati/peso_0gradi_dati.txt";
					tipodati = "cd";
					nomefilemedie = "arrayTempi_0gradi_peso.txt";
					nomefilevelocita = "velocita_0gradi_peso.txt";
					break;
				case 3: // alluminio
					nomeoutputfile = "./Risultati/alluminio_0gradi_dati.txt";
					tipodati = "ad";
					nomefilemedie = "arrayTempi_0gradi_alluminio.txt";
					nomefilevelocita = "velocita_0gradi_alluminio.txt";
					break;
				case 4: // normale
					nomeoutputfile = "./Risultati/pesoalluminio_0gradi_dati.txt";
					tipodati = "cad";
					nomefilemedie = "arrayTempi_0gradi_pesoalluminio.txt";
					nomefilevelocita = "velocita_0gradi_pesoalluminio.txt";
					break;
				default:
					throw "Errore: Seconda esperienza: variante non nota";
					break;
			}

			FileRiassunto.open(nomeoutputfile.c_str());

			for (int intervallo = 1; intervallo <=  NUM_FILE_0GRADI; intervallo++) {
				stringstream ss;
				ifstream FileImputDati;					//File di input
				ss << "./DatiFormattati/DatiStandardizzati/";
				ss << tipodati;	//Tipo dei dati contenuti nel file: d, cd, ad, cad
				ss << intervallo * 10 + 50;
				ss << "_";
				ss << "0";
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

				arrayTempi.emplace_back(arrayRiassunti.back());

				ss.clear();
				FileImputDati.close();
			}					//Intervalli

			//Ex switch
				//Ricicliamo nomeoutputfile per indicare i file di uscita
				nomeoutputfile = string("./Risultati/MetaDati/0/")
						+ nomefilemedie;//Nomefilemedie scelto all'inizio nello switch
				nomeoutputvelocita = string("./Risultati/MetaDati/0/")
						+ nomefilevelocita;//Idem per nomefilevelocita
				FileMedie.open(nomeoutputfile.c_str());
				FileVelocita.open(nomeoutputvelocita.c_str());
				for (int i = 0; i < NUM_FILE_0GRADI; i++)
				{
					//Qui non c'è lo zero, quindi niente media
					FileMedie << (arrayTempi[i]).getMedia() << endl;//sette medie di cinque tempi ciascuns
					FileVelocita << (INTERVALLO_SV / arrayTempi[i]).getMedia() << " "
							<< (INTERVALLO_SV / arrayTempi[i]).getDeviazioneStandardPop() << endl;//10 cm di intervallo/cinque_tempi_media << endl;//10 cm di intervallo
				}
				FileMedie.close();
				FileVelocita.close();

		arrayTempi.clear();
		FileRiassunto.close();
		arrayRiassunti.clear();

		}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////










//////////////////////////////////////////////////
//Analizza i dati per la gravità
typedef VarStat<double> vs;
const double G15 = 0.25*M_PI/180.0;//15 primi di grado
const double G30 = 0.50*M_PI/180.0;//15 primi di grado
const double G45 = 0.75*M_PI/180.0;//15 primi di grado

ofstream AnalisiGravita;
AnalisiGravita.open("./Risultati/Analisi_Dati/DatiGravità");

////Media tra gravità a 15, 30, 45, 45peso presi dai valori di Gnuplot
//AnalisiGravita << "Media gravità della prima giornata: (a15 + a30 + a45 + a45p)/4" << endl;
//AnalisiGravita << ((vs(0.0388864,0.001)*(1/sin(G15))).getMedia() + //15 norm
//				  (vs(0.0811784,0.001408)*(1/sin(G30))).getMedia() + //30 norm
//				  (vs(0.121616,0.001795)*(1/sin(G45))).getMedia() + //45 norm
//				  (vs(0.122322,0.00114)*(1/sin(G45))).getMedia() ) / //45 peso
//						  4
//				  << endl << endl;

//Media tra gravità a 15, 30, 45, 45peso presi dai valori di Gnuplot
vs stimaGravita_orig = ((vs(0.0388864,0.001,7)*(1/sin(G15))) + //15 norm
				  	   (vs(0.0811784,0.001503,7)*(1/sin(G30))) + //30 norm
				  	   (vs(0.121616,0.001991,7)*(1/sin(G45))) + //45 norm
				  	   (vs(0.122322,0.00114,7)*(1/sin(G45))) ) *  //45 peso
						  (0.25);

AnalisiGravita << "Dati gravità della prima giornata: (a15 + a30 + a45 + a45p)/4" << endl;
AnalisiGravita << stimaGravita_orig << endl << endl;

AnalisiGravita << "a15: " << endl << vs(0.0388864,0.001,7)*(1/sin(G15)) << endl;
AnalisiGravita << "a30: " << endl << vs(0.0811784,0.001503,7)*(1/sin(G30)) << endl;
AnalisiGravita << "a45: " << endl << vs(0.121616,0.001991,7)*(1/sin(G45)) << endl;
AnalisiGravita << "a45p: " << endl << vs(0.122322,0.00114,7)*(1/sin(G45)) << endl << endl;

//Media Coefficienti rette senza peso
vs b_np =  (vs(0.00707886,0.004894,6) + //0 norm
		  	vs(0.0135994,0.005114,6) ) * //0 alluminio
				  (0.5);
AnalisiGravita << "B (seconda giornata), senza peso: (b_norm + b_alluminio)/2" << endl;
AnalisiGravita << b_np << endl << endl;

//Media Coefficienti rette con peso
vs b_conp = (vs(0.0282376,0.001601,6) + //0 peso
		  	 vs(0.0395786,0.001124,6) ) * //0 pesoalluminio
				  (0.5);
AnalisiGravita << "B (seconda giornata), con peso: (b_peso + b_peso)/2" << endl;
AnalisiGravita << b_conp << endl << endl;

///////////////////////////////////////////////
//Stime gravità
// Velocita media norm (0 gradi)
vs vMed_norm = (vs(0.172473,0.001) + vs(0.170765,0.00122783) ) * 0.5;
vs vMed_allum = (vs(0.121344,0.00126562) + vs(0.117702,0.00208813)) * 0.5;
vs vMed_totnopeso = (vMed_norm + vMed_allum) * 0.5;

// Velocità media col peso (0 gradi)
vs vMed_peso = (vs(0.103018,0.00142386) + vs(0.116131,0.000705806)) * 0.5;
vs vMed_pesoallum = (vs(0.0541008,0.00236734) + vs(0.0751428,0.000329625)) * 0.5;
vs vMed_totpeso = (vMed_peso + vMed_pesoallum) * 0.5;

AnalisiGravita << "Delta G: Rispettivamente a 15, 30, 45 e 45 con peso " << endl;
AnalisiGravita << "15: " << endl << (vMed_totnopeso*b_np) * (1/sin(G15)) << endl;
AnalisiGravita << "30: " << endl << (vMed_totnopeso*b_np) * (1/sin(G30)) << endl;
AnalisiGravita << "45: " << endl << (vMed_totnopeso*b_np) * (1/sin(G45)) << endl;
AnalisiGravita << "45 con peso: " << endl << (vMed_totpeso*b_conp) * (1/sin(G45)) << endl << endl;

vs stimaGravita_corr = (((vs(0.0388864,0.001,7) + vMed_totnopeso*b_np) * (1/sin(G15)) ) + //15 norm
	  	   	   	   	   ((vs(0.0811784,0.001503,7) + vMed_totnopeso*b_np) * (1/sin(G30)) ) + //30 norm
	  	   	   	   	   ((vs(0.121616,0.001991,7) + vMed_totnopeso*b_np) * (1/sin(G45)) ) + //45 norm
	  	   	   	   	   ((vs(0.122322,0.00114,7) + vMed_totpeso*b_conp) * (1/sin(G45)) ) ) *  //45 peso
	  	   	   	   			   	(0.25);
AnalisiGravita << "Stima gravità corretta: G = G0 + DeltaG" << endl;
AnalisiGravita << stimaGravita_corr << endl << endl;

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


