#ifndef __MANTENIMIENTO_H
#define __MANTENIMIENTO_H

#define USAR_VENTANA
#include "UNGenetico.h"

//declaracion de constantes y variables globales
#define ID_ENTRADA 6000

#define N_MAX 1000	//numero maximo de maquinas
#define NP	48		//numero de periodos
#define NP2	24		//NP/2

#define CASTIGO 3.0

int GNumIndiv;
int GNumGeneraciones;
double GProbMutacion;
int GNumMaq;
wxString *CodEquipo;
int (*Codigos)[4];	//para cada máquina: {Codigo de importancia, Codigo de trabajo, Codigo de mantenimiento, prioridad}

DECLARAR_ALGORITMO(AGMantenimiento)
	void definirOperadores();
	void inicializarParametros();
	void mostrar(Individuo& Ind);
	void guardar(Individuo& Ind, FILE *pArch, int generacion);
	void guardarProgramacion();
	ArregloEntero SemMantenimiento;
FIN_DECLARAR_ALGORITMO



class MiVentana : public AGVentana
{
public:
	MiVentana(wxWindow *parent, wxWindowID id = -1);
	///Controles utilizados en la nueva página
	wxTextCtrl *m_pTextArchivo;
	wxButton *m_pButtonAceptar;
	wxButton *m_pButtonArchivo;

	void OnAceptar(wxCommandEvent &event);
	void OnArchivo(wxCommandEvent &event);

	wxString ObtenerCad(int fila, int col);
	long ObtenerEntero(int fila, int col);
	double ObtenerReal(int fila, int col);
	void ObtenerValores();

	DECLARE_EVENT_TABLE()

	enum botones
	{
		BUTTON_ACEPTAR=5000,
		BUTTON_ARCHIVO
	};
	
	wxTextFile *m_pArchEntrada;
};


#endif