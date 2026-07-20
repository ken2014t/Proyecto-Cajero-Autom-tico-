#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <cctype>
#include <vector>

using namespace std;

//Estructura de la cuenta 
struct Cuenta {
	string nombre;
	string cedula; 
	string pin; 
	double saldo; 
	bool bloqueada; 
	int intentos; 
};

//Variables globales: accesibles desde todas las funciones
Cuenta cuentas[100]; 
int totalCuentas = 0; 
int opcion; 

//Vector de operaciones
vector<string> operaciones; 

//Matriz para registrar operaciones
double matrizOperaciones[100][3];
int totalOperaciones = 0;

//Declaración de las funciones del programa
void Pantalla_Inicio(); 
void CrearCuenta(); 
void IniciarSesion(); 
void MenuUsuario(int indiceUsuario); 
void CargarCuentas(); 
void Depositar (int indiceUsuario); 
void Retirar(int indiceUsuario);
void GenerarRecibo(string operacion, int indiceUsuario, double monto); 
void GuardarCambios(); 
void Transferir(int indiceUsuario); 
void GuardarHistorial(string operacion, int indiceUsuario, double monto);
void VerHistorial(int indiceUsuario);  
void CambiarPIN(int indiceUsuario);
void CuentaRegresiva(int segundos); //Función Recursiva declarada
void MostrarVector(); //Vector declarada
void MostrarMatriz(); //Matriz declarada

//Menú principal
int main()
{
	CargarCuentas(); 
    do
    {
        system("cls"); // Limpia la pantalla

        Pantalla_Inicio();

        cout << endl;
        cout << "============== MENU PRINCIPAL ==============" << endl;
        cout << "1. Crear Cuenta" << endl;
        cout << "2. Iniciar Sesion" << endl;
        cout << "3. Salir" << endl;
        cout << "============================================" << endl;

        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        cin.ignore(); // Limpia el Enter del teclado

        switch(opcion)
        {
            case 1:
                CrearCuenta(); 
                break; 

            case 2:
            	IniciarSesion(); 
            	break; 
               

            case 3:
                cout << "\nGracias por utilizar el cajero." << endl;
                break;

            default:
                cout << "\nOpcion invalida." << endl;
                getch();
        }

    }while(opcion != 3);

    return 0;
}
