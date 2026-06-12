#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;

struct Student {
    int serialNo;
    float greScore;
    float toeflScore;
    int universityRating;
    float sop;
    float lor;
    float cgpa;
    int research;
};

struct CsvMatriz {
    char*** filas;
    int numeroFilas;
    int numeroColumnas;
    int maximoFilas;
    int maximoColumnas;
};

struct EstadisticasColumna {
    string nombre;
    double media;
    double mediana;
    double desviacion;
    double minimo;
    double maximo;
    double rango;
};

CsvMatriz* crearMatriz(int filas, int columnas);
void redimensionarMatriz(CsvMatriz* matriz, int nuevasFilas, int nuevasColumnas);
void liberarMemoriaCsvMatriz(CsvMatriz* matriz);
CsvMatriz* leerCSV(const string& nombreArchivo);
bool esNumerico(const string& valor);
void mostrarMenu();
void mostrarInformacionGeneral(const CsvMatriz* matriz);
void mostrarCabecera(const CsvMatriz* matriz, int n = 5);
void generarEstadisticas(const CsvMatriz* matriz, EstadisticasColumna*& estadisticas, int& numEstadisticas);
void enviarEstadisticasTxt(const EstadisticasColumna* estadisticas, int numEstadisticas);
double calcularCorrelacion(const double* x, const double* y, int n);
void mostrarCorrelaciones(const CsvMatriz* matriz);
void procesarYGuardarBinario(const CsvMatriz* matriz);


CsvMatriz* crearMatriz(int filasIniciales, int columnasIniciales) {
    CsvMatriz* matriz = new CsvMatriz;
    matriz->numeroFilas = 0;
    matriz->numeroColumnas = 0;
    matriz->maximoFilas = filasIniciales;
    matriz->maximoColumnas = columnasIniciales;
    matriz->filas = new char**[filasIniciales];

    for(int i = 0; i < filasIniciales; i++) {
        matriz->filas[i] = new char*[columnasIniciales];
        for(int j = 0; j < columnasIniciales; j++) {
            matriz->filas[i][j] = nullptr;
        }
    }
    return matriz;
}

void redimensionarMatriz(CsvMatriz* matriz, int nuevasFilas, int nuevasColumnas) {
    if(nuevasFilas > matriz->maximoFilas) {
        char*** nuevasFilasPtr = new char**[nuevasFilas];
        for(int i = 0; i < nuevasFilas; i++) {
            nuevasFilasPtr[i] = new char*[matriz->maximoColumnas];
            if(i < matriz->maximoFilas) {
                for(int j = 0; j < matriz->maximoColumnas; ++j) {
                    nuevasFilasPtr[i][j] = matriz->filas[i][j];
                }
                delete[] matriz->filas[i];
            } else {
                for(int j = 0; j < matriz->maximoColumnas; j++) {
                    nuevasFilasPtr[i][j] = nullptr;
                }
            }
        }
        delete[] matriz->filas;
        matriz->filas = nuevasFilasPtr;
        matriz->maximoFilas = nuevasFilas;
    }

    if(nuevasColumnas > matriz->maximoColumnas) {
        for(int i = 0; i < matriz->maximoFilas; i++) {
            char** nuevasColumnasPtr = new char*[nuevasColumnas];
            for(int j = 0; j < nuevasColumnas; ++j) {
                if(j < matriz->maximoColumnas) {
                    nuevasColumnasPtr[j] = matriz->filas[i][j];
                } else {
                    nuevasColumnasPtr[j] = nullptr;
                }
            }
            delete[] matriz->filas[i];
            matriz->filas[i] = nuevasColumnasPtr;
        }
        matriz->maximoColumnas = nuevasColumnas;
    }
}

void liberarMemoriaCsvMatriz(CsvMatriz* matriz) {
    if(!matriz) return;

    for(int i = 0; i < matriz->maximoFilas; ++i) {
        for(int j = 0; j < matriz->maximoColumnas; ++j) {
            delete[] matriz->filas[i][j];
        }
        delete[] matriz->filas[i];
    }
    delete[] matriz->filas;
    delete matriz;
}

CsvMatriz* leerCSV(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if(!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
        return nullptr;
    }

    CsvMatriz* matriz = crearMatriz(100, 9);
    string linea;
    int numFila = 0;

    while(getline(archivo, linea)) {
        if(numFila >= matriz->maximoFilas) {
            redimensionarMatriz(matriz, matriz->maximoFilas * 2, matriz->maximoColumnas);
        }
        stringstream ss(linea);
        string valor;
        int numColumna = 0;
        while(getline(ss, valor, ',')) {
         if(numColumna >= matriz->maximoColumnas) {
             redimensionarMatriz(matriz, matriz->maximoFilas, matriz->maximoColumnas * 2);
          }
         matriz->filas[numFila][numColumna] = new char[valor.size() + 1];
         strcpy(matriz->filas[numFila][numColumna], valor.c_str());
         numColumna++;
        }

        matriz->numeroColumnas = max(matriz->numeroColumnas, numColumna);
        numFila++;
    }

    matriz->numeroFilas = numFila;
    archivo.close();
    return matriz;
}

bool esNumerico(const string& valor) {
 if(valor.empty()){
    return false;
 } 
  char* endPtr = nullptr;
   strtod(valor.c_str(), &endPtr);
   return (*endPtr == '\0');
}
void mostrarMenu() {
    cout << " Menu de Análisis " << endl;
    cout << " 1. Mostrar información general" << endl;
    cout << " 2. Mostrar primeras filas" << endl;
    cout << " 3. Generar estadísticas descriptivas" << endl;
    cout << " 4. Mostrar correlaciones" << endl;
    cout << " 0. Salir" << endl;
    cout << " Selección: ";
}

void mostrarInformacionGeneral(const CsvMatriz* matriz) {
    cout << "  Información General " << endl;
    cout << "Filas totales: " << matriz->numeroFilas-1<< endl;
    cout << "Columnas totales: " << matriz->numeroColumnas << endl;
    cout << "Nombres de columnas:" << endl;
    if(matriz->numeroFilas > 0) {
        for(int j = 0; j < matriz->numeroColumnas; ++j) {
            cout << " - " << matriz->filas[0][j] << endl;
        }
    }
}

void mostrarCabecera(const CsvMatriz* matriz, int n) {
    cout << "Primeras " << n << " filas " << endl;
    int limite = min(n + 1, matriz->numeroFilas);
    for(int i = 0; i < limite; ++i) {
        for(int j = 0; j < matriz->numeroColumnas; ++j) {
            cout << setw(15) << (matriz->filas[i][j] ? matriz->filas[i][j] : "NULL");
        }
    }
}

void generarEstadisticas(const CsvMatriz* matriz, EstadisticasColumna*& estadisticas, int& numEstadisticas) {
 estadisticas = new EstadisticasColumna[matriz->numeroColumnas];
 numEstadisticas = 0;
double* valores = new double[matriz->numeroFilas];
   for(int j = 0; j < matriz->numeroColumnas; j++) {
      int numValores = 0;
      bool esNumerica = true;
        for(int i = 1; i < matriz->numeroFilas; i++) {
          if(!matriz->filas[i][j] || !esNumerico(matriz->filas[i][j])) {
             esNumerica = false;
             break;
           }
          valores[numValores++] = stod(matriz->filas[i][j]);
        }
        if(esNumerica && numValores > 0) {
         EstadisticasColumna& stats = estadisticas[numEstadisticas];
         stats.nombre = matriz->filas[0][j];
         double suma = 0.0;
            
         for(int k = 0; k < numValores; k++){
            suma+= valores[k];
         }
            stats.media = suma / numValores;
            double sumaCuadrados = 0.0;
         for(int k = 0; k < numValores; k++) {
           sumaCuadrados += pow(valores[k] - stats.media, 2); 
          }
            stats.desviacion = sqrt(sumaCuadrados / numValores);
            stats.minimo = valores[0];
            stats.maximo = valores[0];
         for(int k = 1; k < numValores; k++) {
             if(valores[k] < stats.minimo) stats.minimo = valores[k];
             if(valores[k] > stats.maximo) stats.maximo = valores[k];
          }
           stats.rango = stats.maximo - stats.minimo;
           double* valoresOrdenados = new double[numValores];
            for(int k = 0; k < numValores; ++k){
                valoresOrdenados[k] = valores[k];
            }
            sort(valoresOrdenados, valoresOrdenados + numValores);
              if(numValores%2==0){
              stats.mediana = (valoresOrdenados[numValores/2-1] + valoresOrdenados[numValores/2])/2;
              }else{
              stats.mediana = valoresOrdenados[numValores/2];
              }
            delete[] valoresOrdenados;
            numEstadisticas++;
        }
    }
    delete[] valores;
}

void enviarEstadisticasTxt(const EstadisticasColumna* estadisticas, int numEstadisticas) {
    ofstream archivoEstadisticas("transformacion_resultados.txt");
    if(!archivoEstadisticas.is_open()) {
        cerr << "Error: No se pudo abrir transformacion_resultados.txt" << endl;
        return;
    }
    for(int k = 0; k < numEstadisticas; ++k) {
        const EstadisticasColumna& stats = estadisticas[k];
        archivoEstadisticas << "Columna: "<< stats.nombre<< endl;
        archivoEstadisticas << "Media: "<< stats.media<< endl;
        archivoEstadisticas << "Mediana: "<< stats.mediana<< endl;
        archivoEstadisticas << "Desviacion Estandar: "<< stats.desviacion << endl;
        archivoEstadisticas << "Minimo: "<< stats.minimo<< endl;
        archivoEstadisticas << "Maximo: "<< stats.maximo<< endl;
        archivoEstadisticas << "Rango: "<< stats.rango<< endl;
    }

    archivoEstadisticas.close();
    cout << " Estadísticas exportadas" << endl;
}

double calcularCorrelacion(const double* x, const double* y, int n) {
    if(n == 0) { 
        return 0.0;
    }

    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0;
    double sum_x2 = 0.0, sum_y2 = 0.0;
    for(int i = 0; i < n; i++) {
     sum_x+=x[i];
     sum_y+=y[i];
     sum_xy+=x[i]*y[i];
     sum_x2+=x[i]*x[i];
     sum_y2+=y[i]*y[i];
    }
  double numerador= n*sum_xy-sum_x*sum_y;
  double denominador = sqrt((n*sum_x2-sum_x*sum_x)*(n*sum_y2-sum_y*sum_y));
  return denominador != 0.0 ? numerador / denominador : 0.0;
}
void mostrarCorrelaciones(const CsvMatriz* matriz) {
    cout << "Matriz de Correlacion" << endl;
    int maxCols=matriz->numeroColumnas;
    int maxFilasDatos=matriz->numeroFilas-1;
    double** datosNumericos = new double*[maxCols];
    string*nombresColumnas= new string[maxCols];
    int*tamanos = new int[maxCols];
    int numColumnasNumericas = 0;
    for(int j = 0; j<matriz->numeroColumnas; j++) {
        double* columna= new double[maxFilasDatos];
        int numColumna = 0;
        bool esNumerica = true;
        for(int i = 1; i < matriz->numeroFilas; i++) {
         if(!matriz->filas[i][j] || !esNumerico(matriz->filas[i][j])) {
             esNumerica = false;
             break;
         }
            columna[numColumna++] = stod(matriz->filas[i][j]);
        }
        if(esNumerica) {
        datosNumericos[numColumnasNumericas] =columna;
        tamanos[numColumnasNumericas]= numColumna;
        nombresColumnas[numColumnasNumericas] = matriz->filas[0][j];
        numColumnasNumericas++;
        } else {
            delete[] columna;
        }
    }
    int ancho = 12;
    for(int k = 0; k<numColumnasNumericas; k++)
        cout << nombresColumnas[k].substr(0, ancho-2);
    for(int i = 0; i < numColumnasNumericas; i++) {
        cout << nombresColumnas[i].substr(0, ancho-2);
        for(int j = 0; j<numColumnasNumericas; j++) {
            int n = min(tamanos[i], tamanos[j]);
            double correlacion = calcularCorrelacion(datosNumericos[i], datosNumericos[j], n);
            cout << correlacion;
        }
        cout << endl;
    }

    for(int k = 0; k < numColumnasNumericas; ++k)
        delete[] datosNumericos[k];
        delete[] datosNumericos;
        delete[] nombresColumnas;
        delete[] tamanos;
}

void procesarYGuardarBinario(const CsvMatriz* matriz) {
    string archivoSalida = "data/Admission_Predict_new.bin";
    ofstream archivo(archivoSalida, ios::binary);
    if(!archivo) {
        cerr << "Error al crear archivo binario: " << archivoSalida << endl;
        return;
    }
    archivo.write(reinterpret_cast<const char*>(&matriz->numeroFilas),sizeof(int));
    archivo.write(reinterpret_cast<const char*>(&matriz->numeroColumnas),sizeof(int));
    for(int i = 0; i < matriz->numeroFilas; i++) {
        for(int j = 0; j < matriz->numeroColumnas; j++) {
            if(matriz->filas[i][j]) {
                size_t longitud = strlen(matriz->filas[i][j]);
                archivo.write(reinterpret_cast<const char*>(&longitud), sizeof(size_t));
                archivo.write(matriz->filas[i][j], longitud);
            } else {
                size_t longitud = 0;
                archivo.write(reinterpret_cast<const char*>(&longitud), sizeof(size_t));
            }
        }
    }
    bool exitoEscritura = archivo.good();
    archivo.close();
    if(!exitoEscritura) {
        cerr << "Error al guardar la matriz en binario" << endl;
        return;
    }
    cout << "Matriz guardada en: " << archivoSalida << endl;
    cout << "Filas: " << matriz->numeroFilas << " Columnas: " << matriz->numeroColumnas << endl;
    ifstream archivoLectura(archivoSalida, ios::binary);
    if(!archivoLectura) {
        cerr << "Error al abrir archivo binario para validación" << endl;
        return;
    }
    int filasLeidas, columnasLeidas;
    archivoLectura.read(reinterpret_cast<char*>(&filasLeidas),    sizeof(int));
    archivoLectura.read(reinterpret_cast<char*>(&columnasLeidas), sizeof(int));

    cout << "Dimensiones leídas: " << filasLeidas << " filas, " << columnasLeidas << " columnas" << endl;
    if(filasLeidas != matriz->numeroFilas || columnasLeidas != matriz->numeroColumnas) {
        cout << "Error: las dimensiones no coinciden" << endl;
        archivoLectura.close();
        return;
    }
    string*encabezados= new string[columnasLeidas];
    int numEncabezados = 0;
    for(int j = 0; j < columnasLeidas; j++) {
        int longitud;
        archivoLectura.read(reinterpret_cast<char*>(&longitud), sizeof(int));
        if(longitud > 0) {
            char* buffer = new char[longitud+1];
            archivoLectura.read(buffer, longitud);
            buffer[longitud] = '\0';
            encabezados[numEncabezados++] = buffer;
            delete[] buffer;
        } else {
            encabezados[numEncabezados++] = "nullptr";
        }
    }
    cout << "Encabezados: ";
    for(int k = 0; k < numEncabezados; ++k)
        cout << encabezados[k] << endl;
    delete[] encabezados;
    for(int i = 1; i <= min(5, filasLeidas-1); i++) {
        cout << "Registro "<<i<<":"<<endl;
         for(int j = 0; j < columnasLeidas; j++) {
            int longitud;
            archivoLectura.read(reinterpret_cast<char*>(&longitud), sizeof(int));
            if(longitud > 0) {
              char* buffer = new char[longitud+1];
              archivoLectura.read(buffer, longitud);
              buffer[longitud] = '\0';
              delete[] buffer;
            } else {
                cout << "  ";
            }
        }
        for(int j = 0; j < columnasLeidas; j++) {
            int longitud;
            archivoLectura.read(reinterpret_cast<char*>(&longitud), sizeof(int));
            if(longitud > 0) {
                char* buffer = new char[longitud + 1];
                archivoLectura.read(buffer, longitud);
                buffer[longitud] = '\0';
                cout << buffer;
                delete[] buffer;
            } else {
                cout << " ";
            }
        }
    }
    archivoLectura.close();
}

int main() {
    string rutaArchivo = "C:\\Users\\ivans\\Downloads\\Admission_Predict.csv";
    cout << "Cargando datos desde: " << rutaArchivo << endl;
    CsvMatriz* matriz = leerCSV(rutaArchivo);

    if(!matriz) {
        cerr << "Error al cargar el archivo CSV" << endl;
        return 1;
    }
    procesarYGuardarBinario(matriz);
    int opcion;
    do {
     mostrarMenu();
       cin >> opcion;
        switch(opcion) {
            case 1:
                mostrarInformacionGeneral(matriz);
                break;
            case 2:
                mostrarCabecera(matriz);
                break;
            case 3: {
                EstadisticasColumna* stats = nullptr;
                int numStats = 0;
                generarEstadisticas(matriz, stats, numStats);
                for(int k = 0; k < numStats; ++k) {
                    cout << "Columna: "<< stats[k].nombre<<endl;
                    cout << "Media: "<< endl;
                    cout << "Mediana: "<< stats[k].mediana<<endl;
                    cout << "Desviacion Estandar: "<< stats[k].desviacion <<endl;
                    cout << "Minimo: "<< stats[k].minimo <<endl;
                    cout << "Máximo: "<< stats[k].maximo<<endl;
                    cout << "Rango: "<< stats[k].rango<<endl;
                }
                enviarEstadisticasTxt(stats, numStats);
                delete[] stats;
                break;
            }
            case 4:
                mostrarCorrelaciones(matriz);
                break;
            case 0:
                break;
            default:
                cout << "Opcion invalida" << endl;
        }
    } while(opcion != 0);
    liberarMemoriaCsvMatriz(matriz);
    return 0;
}
