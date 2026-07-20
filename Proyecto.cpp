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
int main() {
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

//Pantalla de inicio
void Pantalla_Inicio() {
    time_t ahora = time(0);
    tm *t = localtime(&ahora);

    cout << "==========================================" << endl;
    cout << "          CAJERO AUTOMATICO" << endl;
    cout << "==========================================" << endl;

    cout << "Fecha: "
         << t->tm_mday << "/"
         << t->tm_mon + 1 << "/"
         << t->tm_year + 1900 << endl;

    cout << "Hora : "
         << t->tm_hour << ":"
         << t->tm_min << ":"
         << t->tm_sec << endl;

}

//El usuario crea su cuenta

void CrearCuenta() {
    system("cls");

    cout << "========== CREAR CUENTA ==========\n\n";

    cout << "Nombre: ";
    getline(cin, cuentas[totalCuentas].nombre);

    cout << "Cedula: ";
    getline(cin, cuentas[totalCuentas].cedula);

    cout << "PIN (4 digitos): ";
    getline(cin, cuentas[totalCuentas].pin);

    cout << "Saldo inicial: $";
    cin >> cuentas[totalCuentas].saldo;

    cuentas[totalCuentas].bloqueada = false;
    cuentas[totalCuentas].intentos = 0;
    
    // Guardar en cuentas.txt
    
    ofstream archivo("cuentas.txt", ios::app);
    
    if(archivo.is_open()){
    	archivo << cuentas[totalCuentas].nombre << ";"; 
    	archivo << cuentas[totalCuentas].cedula << ";"; 
    	archivo << cuentas[totalCuentas].pin << ";"; 
    	archivo << cuentas[totalCuentas].saldo << ";"; 
    	archivo << cuentas[totalCuentas].bloqueada << ";"; 
    	archivo << cuentas[totalCuentas].intentos << endl; 
    	
    	archivo.close(); 
    	
    	cout << "\nCuenta creada y guardada correctamente." << endl;     	
	}
	else {
		cout << "\nError al abrir el archivo cuentas.txt" << endl; 
	}
	
    totalCuentas++;
    
    getch();
}

//Verifica la cuenta antes de ingresar
void CargarCuentas(){
	
	ifstream archivo("cuentas.txt");

    if(!archivo.is_open()){
        return;
    }

    totalCuentas = 0;

    while(getline(archivo, cuentas[totalCuentas].nombre, ';')) {
        getline(archivo, cuentas[totalCuentas].cedula, ';');
        getline(archivo, cuentas[totalCuentas].pin, ';');

        archivo >> cuentas[totalCuentas].saldo;
        archivo.ignore();

        archivo >> cuentas[totalCuentas].bloqueada;
        archivo.ignore();

        archivo >> cuentas[totalCuentas].intentos;
        archivo.ignore();

        totalCuentas++;
    }
    archivo.close();
}

//Inicio de sesion del cliente
void IniciarSesion(){
   
    system("cls");
    string cedula, pin;
    cout << "=========== INICIAR SESION ===========\n\n";
    cout << "Cedula: ";
    getline(cin, cedula);

    cout << "PIN: ";
    getline(cin, pin);
    bool encontrado = false;

    for(int i = 0; i < totalCuentas; i++)
    {
        if(cuentas[i].cedula == cedula)
        {
            encontrado = true;

            if(cuentas[i].bloqueada)
            {
                cout << "\nLa cuenta esta bloqueada." << endl;
                getch();
                return;
            }

            if(cuentas[i].pin == pin){
                cuentas[i].intentos = 0;

                GuardarCambios();

                cout << "\nBienvenido " << cuentas[i].nombre << endl;
                getch();
                MenuUsuario(i);
                return;
            }else{
                cuentas[i].intentos++;

                if(cuentas[i].intentos >= 3){
                    cuentas[i].bloqueada = true;
                    GuardarCambios();
                    cout << "\nCuenta bloqueada por 3 intentos fallidos." << endl;
                }
                else{
                    GuardarCambios();

                    cout << "\nPIN incorrecto." << endl;
                    cout << "Intento "
                         << cuentas[i].intentos
                         << " de 3." << endl;
                }

                getch();
                return;
            }
        }
    }

    if(!encontrado)
    {
        cout << "\nLa cedula no existe." << endl;
        getch();
    }
}

//Menú usuario al ingresar con su cedula y contraseña
void MenuUsuario(int indiceUsuario){
	
	int opcionUsuario; 
	
	do{
		
		system("cls"); 
		Pantalla_Inicio(); 
		cout << "\nBienvenido: " << cuentas[indiceUsuario].nombre << endl; 
		cout << "Cedula: " << cuentas[indiceUsuario].cedula << endl;
		
		cout << "\n=========== MENU DEL USUARIO ===========" << endl; 
		cout << "1. Consultar saldo" << endl; 
		cout << "2. Depositar dinero" << endl;
		cout << "3. Retirar Dinero" << endl; 
		cout << "4. Transferir dinero" << endl; 
		cout << "5. Ver historial" << endl; 
		cout << "6. Cambiar PIN" << endl; 
		cout << "7. Cerrar sesion" << endl;
		cout << "8. Ver operaciones de la sesion" << endl;
		cout << "9. Ver matriz de la operacion" << endl;   
		cout << "==========================================" << endl; 
		
		cout << "Seleccione una opcion: "; 
		cin >> opcionUsuario; 
		
		switch(opcionUsuario){
			case 1: 
			cout << "\nSaldo actual: $"
			<< cuentas [indiceUsuario].saldo << endl; 
			getch(); 
			break; 
			
			case 2:{
				Depositar(indiceUsuario); 
				break;  
			}
			
			case 3: 
			Retirar(indiceUsuario); 
			break;
			
			case 4:
			Transferir(indiceUsuario); 
			break;  
			
			
			case 5:
			VerHistorial(indiceUsuario); 
			break;  
	
			case 6:
				CambiarPIN(indiceUsuario); 
				break; 
			
			case 7:
			CuentaRegresiva(3); 
			cout << "\nCerrando sesion..." << endl;  
			getch(); 
			break; 
			
			case 8:
			MostrarVector(); 
			break;
			
			case 9:
			MostrarMatriz(); 
			break; 
		  	
			default: 
			cout << "\nOpcion invalida." << endl; 
			getch(); 
				
		}
	} while(opcionUsuario !=7); 
}

//Función depositar (case 2)
void Depositar(int indiceUsuario){
    double monto;

    system("cls");
    Pantalla_Inicio();

    cout << "========== DEPOSITAR DINERO ==========\n\n";
    cout << "Saldo actual: $" << cuentas[indiceUsuario].saldo << endl;
    cout << "\nIngrese el monto a depositar: $";
    cin >> monto;

    if(monto <= 0){
        cout << "\nEl monto debe ser mayor que cero." << endl;
    }
    else{
        cuentas[indiceUsuario].saldo += monto;

        GuardarCambios();
        GuardarHistorial("DEPOSITO", indiceUsuario, monto);
        matrizOperaciones[totalOperaciones][0] = 1; 
        matrizOperaciones[totalOperaciones][1] = monto; 
        matrizOperaciones[totalOperaciones][2] = cuentas[indiceUsuario].saldo; 
        totalOperaciones++; 
        operaciones.push_back("Deposito: $" + to_string((int)monto)); 
        GenerarRecibo("DEPOSITO", indiceUsuario, monto);

        cout << "\nDeposito realizado correctamente." << endl;
        cout << "Nuevo saldo: $" << cuentas[indiceUsuario].saldo << endl;
    }
    getch();
}

//Función cambiar PIN
void CambiarPIN(int indiceUsuario){
    string pinActual;
    string nuevoPIN;
    string confirmarPIN;
    system("cls");
    Pantalla_Inicio();
    cout << "========== CAMBIAR PIN ==========\n\n";
    cout << "Ingrese su PIN actual: ";
    cin >> pinActual;

    if(pinActual != cuentas[indiceUsuario].pin){
        cout << "\nPIN incorrecto." << endl;
        getch();
        return;
    }

    cout << "\nIngrese el nuevo PIN (4 digitos): ";
    cin >> nuevoPIN;

    cout << "Confirme el nuevo PIN: ";
    cin >> confirmarPIN;

    if(nuevoPIN != confirmarPIN){
        cout << "\nLos PIN no coinciden." << endl;
        getch();
        return;
    }

    if(nuevoPIN.length() != 4){
        cout << "\nEl PIN debe tener 4 digitos." << endl;
        getch();
        return;
    }

    bool soloNumeros = true;

    for(int i = 0; i < nuevoPIN.length(); i++){
        if(!isdigit(nuevoPIN[i]))
        {
            soloNumeros = false;
            break;
        }
    }

    if(!soloNumeros){
        cout << "\nEl PIN solo puede contener numeros." << endl;
        getch();
        return;
    }

    cuentas[indiceUsuario].pin = nuevoPIN;
    GuardarCambios();
    cout << "\nPIN cambiado correctamente." << endl;
    getch();
}

//Función recursiva
void CuentaRegresiva(int segundos)
{
    if(segundos == 0)
    {
        cout << "\nSesion cerrada." << endl;
        return;
    }

    cout << "Cerrando sesion en " << segundos << "..." << endl;

    Sleep(1000);

    CuentaRegresiva(segundos - 1);
}

//vector
void MostrarVector()
{
    system("cls");

    cout << "====== OPERACIONES DE LA SESION ======\n\n";

    if(operaciones.empty())
    {
        cout << "No hay operaciones registradas." << endl;
    }
    else
    {
        for(int i = 0; i < operaciones.size(); i++)
        {
            cout << i + 1 << ". " << operaciones[i] << endl;
        }
    }

    getch();
}

//Matriz
void MostrarMatriz()
{
    system("cls");

    cout << "========== MATRIZ DE OPERACIONES ==========\n\n";

    cout << "TIPO\tMONTO\tSALDO\n";

    for(int i = 0; i < totalOperaciones; i++)
    {
        cout << matrizOperaciones[i][0] << "\t";
        cout << matrizOperaciones[i][1] << "\t";
        cout << matrizOperaciones[i][2] << endl;
    }

    getch();
}

