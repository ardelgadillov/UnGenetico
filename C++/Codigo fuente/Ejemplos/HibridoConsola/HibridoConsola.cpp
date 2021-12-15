#include "UNGenetico.h"

DECLARAR_ALGORITMO(MiAlgoritmoGenetico)
	void inicializarParametros();
	void definirOperadores();
	void mostrarIndividuo(Individuo& Ind);
	
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
  
void MiAlgoritmoGenetico::mostrarIndividuo(Individuo& Ind)
{
	cout <<"\nFuncion Objetivo:\t" << Ind.objetivo(true);
	cout << "\nx:\t";
	int k = x.getSize();
	for(int i=0; i<k; i++)
	{
		cout << x[i] ;
	}	
	cout << "\ny:\t" << y;
	cout << "\nz:\t" << z;	
}	


double MiAlgoritmoGenetico::objetivo()
{
	double FO=0.0;
	
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


//////////////////Programa Principal////////////////////////////////

int main()
{
	MiAlgoritmoGenetico MiAg;

	cout << "\t\tAplicación de UNGenético 2.0\n\n";

	int i=0;
	MiAg.iniciarOptimizacion();
	do
	{
		MiAg.iterarOptimizacion();
		cout << "\n\nGENERACION "<< i << "\n";
		MiAg.mostrarIndividuo(*MiAg.m_pMejorEnEstaGeneracion);
		i++;
	}while(!MiAg.finalizar());

	MiAg.mostrarMedidas();
	return(0);
}