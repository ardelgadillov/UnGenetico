#define USAR_VENTANA
#include "UNGenetico.h"


DECLARAR_ALGORITMO(MiAlgoritmoGenetico)
	void mostrarIndividuo(Individuo & Ind);
	void inicializarParametros();
	void MiAlgoritmoGenetico::definirOperadores();
	ArregloBool x;
	int y;
	double z;
FIN_DECLARAR_ALGORITMO


void MiAlgoritmoGenetico::inicializarParametros()
{
	m_IndicadorMaximizar=true;
	m_TamanoPoblacion=200;
	m_IndicadorInicializarPoblacionAleatoria=false;
}


void MiAlgoritmoGenetico::codificacion(Individuo * Ind ,int estado)
{
	ADICIONAR_GENARREGLO_BOOL(Ind , 0 , x , 1 , 10)
	ADICIONAR_GENENTERO(Ind , 1 , y , -10 , 40 , 0 )
	ADICIONAR_GENREAL(Ind , 2 , z , 0.0 , 6.0 , 0 )
}


void MiAlgoritmoGenetico::mostrarIndividuo(Individuo & Ind)
{
	wxString Cad;
	Cad <<"\nFuncion Objetivo:\t" << Ind.objetivo(true);
	Cad << "\nx: ";
	int k = x.getSize();
	for(int i=0; i<k; i++)
	{
		Cad << x[i];
	}
	Cad << "\ny: " << y;
	Cad << "\nz: " << z;
	m_pFrame->m_pTextCtrl->AppendText(Cad);
}


void MiAlgoritmoGenetico::definirOperadores()
{
	DEFINIR_OPERADOR_PROBABILIDAD(OperadorProbabilidadProporcional)
	DEFINIR_OPERADOR_SELECCION( OperadorSeleccionEstocasticaRemplazo)
	DEFINIR_OPERADOR_PAREJAS(OperadorParejasAdyacentes)
	DEFINIR_OPERADOR_REPRODUCCION(OperadorReproduccionMejorPadreMejorHijo)
	
	ADICIONAR_OPERADOR_ADAPTACION(OperadorAdaptacionProbMutacion(this,ADAPTACION_PROBMUTACION_OFFLINE))
	ADICIONAR_OPERADOR_FINALIZACION(OperadorFinalizacionOffline)

	ADICIONAR_OPERADOR_MUTACION(OperadorMutacionArregloBool)
	ADICIONAR_OPERADOR_CRUCE(OperadorCruceArregloBool)
	ADICIONAR_OPERADOR_MUTACION(OperadorMutacionEnteroNoUniforme(this))
	ADICIONAR_OPERADOR_CRUCE(OperadorCruceEnteroHeuristico)
	ADICIONAR_OPERADOR_MUTACION(OperadorMutacionRealMuhlenbein)
	ADICIONAR_OPERADOR_CRUCE(OperadorCruceRealBLX)	
}



double MiAlgoritmoGenetico::objetivo()
{
	double FO = 0.0;
	int k = x.getSize();
	for(int i=0; i<k; i++)
	{
		FO +=x[i];
	}
	FO += 4*y + sin(15*z)*exp(-z);
	if(y%2==0)
		FO-=100;
	return (FO);
}


////////////////////Procedimiento Principal//////////////////////////////////
	

DECLARAR_APLICACION(AGApp)

bool AGApp::OnInit()
{
	m_pFrame = new AGFrame("Ejemplo de aplicación de UNGenético 2.0", 50, 50, 700, 500);
	SetTopWindow(m_pFrame);
	m_pFrame->Show(true);
	MiAlgoritmoGenetico MiAg(m_pFrame);

	MiAg.iniciarOptimizacion();
	int i=0;
	do
	{
		MiAg.iterarOptimizacion();
		wxString Cad;
		Cad << "\n\nGENERACION "<< i ;
		m_pFrame->m_pTextCtrl->AppendText(Cad);
		MiAg.mostrarIndividuo(*MiAg.m_pMejorEnEstaGeneracion);
		i++;
		EJECUTAR_EVENTOS
	}while(!MiAg.finalizar());
	
	MiAg.mostrarMedidas();
	return true;
}