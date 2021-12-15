

#define USAR_VENTANA	/*debe definirse esta constante antes de incluir 
						  UNGenetico para utilizar entorno gráfico de wxWindows.*/
#include "UNGenetico.h"


#define ID_ENTRADA 6000
#define PI 3.14159265358979


enum funcion
{
	ESFERICO = 1,
	ROSENBROCK,
	SCHWEFEL,
	SCHWEFEL_DOS,
	RASTRIGIN,
	GRIEWANGK,
	ACKLEY ,
	MICHALEWICKZ,
	KATSUURA
};


DECLARAR_APLICACION(AGApp)


//Otra ventana que se creó para pedir datos de las funciones
class MiVentana : public AGVentana
{
public:
	MiVentana(wxWindow *parent, wxWindowID id = -1);
	///Controles utilizados en la nueva página
	wxChoice *m_ChoiceFuncion;
	wxChoice *m_ChoiceFinalizacion;
	wxSpinCtrl *m_SpinGenMax;
	wxSpinCtrl *m_SpinDimension;
	wxSpinCtrl *m_SpinMax;
	wxSpinCtrl *m_SpinMin;
	wxSpinCtrl *m_SpinTamPob;
	wxButton *m_ButtonAceptar;

	wxSpinCtrl *m_SpinaRAS, *m_SpindGRI, *m_SpinaACK, *m_SpinmMIC;
	wxTextCtrl *m_TextwRAS, *m_TextbACK, *m_TextcACK;

	void OnAceptar(wxCommandEvent &event);
	void OnFuncion(wxCommandEvent &event);

	void Desabilitar();

	DECLARE_EVENT_TABLE()

	enum controls
	{
		BUTTON_ACEPTAR = 5000,
		CHOICE_FUNCION
	};

};

DECLARAR_ALGORITMO(AGFunciones)	
	void inicializarParametros();
	void definirOperadores();
	void mostrar(Individuo& Ind, int generacion);
	void GuardarValores(Individuo& Ind, int generacion);
	void GuardarIteraciones(Individuo& Ind);

	ArregloReal x;
	int Dimension,Emax,Emin, func;
	int  aRAS, dGRI, aACK, mMIC;
	FILE *pValores;
	int m_CriterioFinalizacion;
FIN_DECLARAR_ALGORITMO
