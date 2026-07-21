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

//Declaración de las funciones para programa
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

//Menu principal
int main() {
	CargarCuentas(); 
    do
    {
        system("cls");

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
    
    // las opereciones que hace el ususario se guardan en cuentas.txt 
    
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

//Función depositar (caso 2)
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

//Función retirar (caso 3)
void Retirar(int indiceUsuario){
    double monto;

    system("cls");
    Pantalla_Inicio();

    cout << "========== RETIRAR DINERO ==========\n\n";

    cout << "Saldo actual: $" << cuentas[indiceUsuario].saldo << endl;

    cout << "\nIngrese el monto a retirar: $";
    cin >> monto;

    if(monto <= 0){
        cout << "\nEl monto debe ser mayor que cero." << endl;
    }
    else if(monto > cuentas[indiceUsuario].saldo){
        cout << "\nSaldo insuficiente." << endl;
    }
    else{
        cuentas[indiceUsuario].saldo -= monto;

        cout << "\nRetiro realizado correctamente." << endl;
        cout << "Nuevo saldo: $" << cuentas[indiceUsuario].saldo << endl;
        
        GuardarCambios(); 
        GuardarHistorial("RETIRO", indiceUsuario, monto);
        matrizOperaciones[totalOperaciones][0] = 2; 
        matrizOperaciones[totalOperaciones][1] = monto; 
        matrizOperaciones[totalOperaciones][2] = cuentas[indiceUsuario].saldo; 
        totalOperaciones++; 
		operaciones.push_back("Retiro: $" + to_string((int)monto)); 
        GenerarRecibo("RETIRO", indiceUsuario, monto); 
    }
    getch();
}
//esto generar el recibo de retiro (cas0 3)
void GenerarRecibo(string operacion, int indiceUsuario, double monto)
{
    time_t ahora = time(0);
    tm *t = localtime(&ahora);

    ofstream recibo("Recibo.txt");

    if(recibo.is_open())
    {
        recibo << "======================================" << endl;
        recibo << "        CAJERO AUTOMATICO" << endl;
        recibo << "======================================" << endl;

        recibo << "Operacion : " << operacion << endl;
        recibo << "Nombre    : " << cuentas[indiceUsuario].nombre << endl;
        recibo << "Cedula    : " << cuentas[indiceUsuario].cedula << endl;
        recibo << "Monto     : $" << monto << endl;
        recibo << "Saldo     : $" << cuentas[indiceUsuario].saldo << endl;

        recibo << endl;

        recibo << "Fecha: "
                << t->tm_mday << "/"
                << t->tm_mon + 1 << "/"
                << t->tm_year + 1900 << endl;

        recibo << "Hora : "
                << t->tm_hour << ":"
                << t->tm_min << ":"
                << t->tm_sec << endl;

        recibo << "======================================" << endl;

        recibo.close();
        cout << "\nGenerando recibo..." << endl; 
        system("notepad Recibo.txt");
    }
}
// Actualizar el saldo segun lo guardado en la cuenta (saldo) (caso 3)
void GuardarCambios(){
    ofstream archivo("cuentas.txt");

    for(int i = 0; i < totalCuentas; i++){
        archivo << cuentas[i].nombre << ";";
        archivo << cuentas[i].cedula << ";";
        archivo << cuentas[i].pin << ";";
        archivo << cuentas[i].saldo << ";";
        archivo << cuentas[i].bloqueada << ";";
        archivo << cuentas[i].intentos << endl;
    }
    archivo.close();
}

// Función Transferir (caso 4)
void Transferir(int indiceUsuario)
{
    string cedulaDestino;
    double monto;
    int indiceDestino = -1;

    system("cls");
    Pantalla_Inicio();

    cout << "========== TRANSFERIR DINERO ==========\n\n";

    cout << "Saldo disponible: $" << cuentas[indiceUsuario].saldo << endl;

    cout << "\nIngrese la cedula de la cuenta destino: ";
    cin >> cedulaDestino;

    // Buscar la cuenta destino
    for(int i = 0; i < totalCuentas; i++)
    {
        if(cuentas[i].cedula == cedulaDestino)
        {
            indiceDestino = i;
            break;
        }
    }

    // Verificar que exista
    if(indiceDestino == -1)
    {
        cout << "\nLa cuenta destino no existe." << endl;
        getch();
        return;
    }

    // Verificar que no sea la misma cuenta
    if(indiceDestino == indiceUsuario)
    {
        cout << "\nNo puede transferirse dinero a la misma cuenta." << endl;
        getch();
        return;
    }

    cout << "Cuenta destino: " << cuentas[indiceDestino].nombre << endl;

    cout << "\nIngrese el monto a transferir: $";
    cin >> monto;

    // Validar monto
    if(monto <= 0)
    {
        cout << "\nEl monto debe ser mayor que cero." << endl;
        getch();
        return;
    }

    // Validar saldo
    if(monto > cuentas[indiceUsuario].saldo)
    {
        cout << "\nSaldo insuficiente." << endl;
        getch();
        return;
    }

    // Realizar transferencia
    cuentas[indiceUsuario].saldo -= monto;
    cuentas[indiceDestino].saldo += monto;

    GuardarCambios();
    GuardarHistorial("TRANSFERENCIA", indiceUsuario, monto);
    matrizOperaciones[totalOperaciones][0] = 3;
    matrizOperaciones[totalOperaciones][1] = monto;
    matrizOperaciones[totalOperaciones][2] = cuentas[indiceUsuario].saldo;
    totalOperaciones++; 
    operaciones.push_back("Transferencia: $" + to_string((int)monto)); 
    GenerarRecibo("TRANSFERENCIA", indiceUsuario, monto);

    cout << "\nTransferencia realizada correctamente." << endl;
    cout << "Destino: " << cuentas[indiceDestino].nombre << endl;
    cout << "Monto transferido: $" << monto << endl;
    cout << "Nuevo saldo: $" << cuentas[indiceUsuario].saldo << endl;

    getch();
}

//Funcion para guardar historial 
void GuardarHistorial(string operacion, int indiceUsuario, double monto){
    time_t ahora = time(0);
    tm *t = localtime(&ahora);

    ofstream historial("historial.txt", ios::app);

    if(historial.is_open()){
        historial << cuentas[indiceUsuario].nombre << ";";
        historial << cuentas[indiceUsuario].cedula << ";";
        historial << operacion << ";";
        historial << monto << ";";
        historial << cuentas[indiceUsuario].saldo << ";";

        historial
            << t->tm_mday << "/"
            << t->tm_mon + 1 << "/"
            << t->tm_year + 1900 << ";";

        historial
            << t->tm_hour << ":"
            << t->tm_min << ":"
            << t->tm_sec;

        historial << endl;

        historial.close();
    }
}

void VerHistorial(int indiceUsuario){
    system("cls");

    ifstream historial("historial.txt");

    string nombre;
    string cedula;
    string operacion;
    string monto;
    string saldo;
    string fecha;
    string hora;

    cout << "========== HISTORIAL ==========\n\n";

    while(getline(historial, nombre, ';')){
        getline(historial, cedula, ';');
        getline(historial, operacion, ';');
        getline(historial, monto, ';');
        getline(historial, saldo, ';');
        getline(historial, fecha, ';');
        getline(historial, hora);

        if(cedula == cuentas[indiceUsuario].cedula){
            cout << "Operacion : " << operacion << endl;
            cout << "Monto     : $" << monto << endl;
            cout << "Saldo     : $" << saldo << endl;
            cout << "Fecha     : " << fecha << endl;
            cout << "Hora      : " << hora << endl;
            cout << "-----------------------------------" << endl;
        }
    }
    historial.close();
    getch();
}

//Función para cambiar el PIN del cliente
void CambiarPIN(int indiceUsuario)
{
    string pinActual;
    string nuevoPIN;
    string confirmarPIN;

    system("cls");
    Pantalla_Inicio();

    cout << "========== CAMBIAR PIN ==========\n\n";

    cout << "Ingrese su PIN actual: ";
    cin >> pinActual;

    if(pinActual != cuentas[indiceUsuario].pin)
    {
        cout << "\nPIN incorrecto." << endl;
        getch();
        return;
    }

    cout << "\nIngrese el nuevo PIN (4 digitos): ";
    cin >> nuevoPIN;

    cout << "Confirme el nuevo PIN: ";
    cin >> confirmarPIN;

    if(nuevoPIN != confirmarPIN)
    {
        cout << "\nLos PIN no coinciden." << endl;
        getch();
        return;
    }

    if(nuevoPIN.length() != 4)
    {
        cout << "\nEl PIN debe tener 4 digitos." << endl;
        getch();
        return;
    }

    bool soloNumeros = true;

    for(int i = 0; i < nuevoPIN.length(); i++)
    {
        if(!isdigit(nuevoPIN[i]))
        {
            soloNumeros = false;
            break;
        }
    }

    if(!soloNumeros)
    {
        cout << "\nEl PIN solo puede contener numeros." << endl;
        getch();
        return;
    }

    cuentas[indiceUsuario].pin = nuevoPIN;

    GuardarCambios();

    cout << "\nPIN cambiado correctamente." << endl;

    getch();
}


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
