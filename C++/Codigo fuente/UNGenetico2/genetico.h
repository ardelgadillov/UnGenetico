#ifndef __GENETICO_H
#define __GENETICO_H

#include<stdio.h>
#include<stdlib.h>
#include<iostream.h>
#include<math.h>
#include<float.h>
#include<time.h>

#include "arreglos.h"


/*S�lo incluimos las clases que manejan entorno gr�fico si est�
definida la constante USAR_VENTANA*/
#if defined(USAR_VENTANA)
#	include "ventana.h"
#endif


/*Para efectos de depuraci�n de la librer�a*/
#if defined(UN_DEBUG) && defined(USAR_VENTANA)
#	define unASSERT(cond) wxASSERT(cond)
#else 
#	define unASSERT(cond)
#endif

/**************************************************************/
/*				Declaraci�n de constantes					  */
/**************************************************************/

//!Constantes utilizadas por los operadores de adaptaci�n
enum CriteriosDeAdaptacion
{
	ADAPTACION_PROBMUTACION_OFFLINE	=	1,
	ADAPTACION_PROBMUTACION_EXPONENCIAL
};

//!Constantes utilizadas en la definicion de la codificaci�n del individuo.
/*!Se utilizan en la funci�n codificacion(Individuo * pInd ,int estado) de la clase AlgoritmoGenetico*/
enum EstadosIndividuo
{
	ESTADO_CODIFICAR	=	1,
	ESTADO_DECODIFICAR,
	ESTADO_CREAR
};


/************************************************************/
/*					Declaraci�n de clases					*/
/************************************************************/

class Gen;
class Individuo;
class Poblacion;
class AlgoritmoGenetico;
class OperadorAdaptacion;
class OperadorFinalizacion;
class OperadorMutacion;
class OperadorCruce;
class OperadorProbabilidad;
class OperadorParejas;
class OperadorReproduccion;
class OperadorSeleccion;


/************************************************************/
/*				Declaraci�n de funciones globales			*/
/************************************************************/

long redondear(double);

/*!Comprueba si \a valor se encuentra dentro de un rango determinado y lo
restringe a los l�mites.
\param valor Valor a restringir
\param min L�mite inferior del rango 
\param max L�mite superior del rango
\return Valor ubicado dentro de los limites. Si valor < min retorna min. Si valor > max retorna max*/
template<class T> inline T restringir(T valor, T min, T max)
{
	if(valor>max) valor=max;
	else if(valor<min) valor=min;
	return valor;
}


#pragma warning(push, 3)	//Para evitar  "warning(level 4) C4100" ocasionada por par�metros de funciones no referenciados

/************************************************************************/
/*						CLASE Gen										*/
/************************************************************************/

//!Clase abstracta que sirve de base a clases que definen genes de distintos tipos
/*!
Las clases derivadas como GenBool, GenEntero, GenReal, GenArreglo u otras 
que cree el usuario son las que establecen la forma en que se almacena 
y se maneja la informaci�n genetica. Estas deben implementar todas las funciones 
puramente virtuales que definen el tratamiento de la informaci�n de cada gen. 
Para cada tipo de gen que se cree deben definirse tambi�n operadores 
de mutaci�n y cruce cuyas clases deben derivarse de las clases OperadorMutacion 
y OperadorCruce respectivamente.*/
class Gen
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	/*Constructor por copia. No se puede crear por copia. 
	  Debe invocarse .crearCopia() del otro Gen*/
	Gen(const Gen&);
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

public:	
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas si se requiere inicializar alg�n miembro*/
	Gen(){}

	//!Destructor
	/*!Es virtual para poder definirse en las clases derivadas.*/
	virtual ~Gen(){}

	//!Debe especificar la forma de generar de un gen aleatorio
	/*!Debe sobrecargarse en las clases derivadas*/
	virtual void generarAleatorio()=0;

	//!Debe especificar el operador de mutaci�n por defecto para el gen.
	/*!Debe sobrecargarse en las clases derivadas
	return Debe retornar un apuntador a un nuevo objeto de una clase derivada de OperadorMutacion*/
	virtual OperadorMutacion* operadorMutacionDefecto() const =0;

	//!Debe especificar el operador de cruce por defecto para el gen.
	/*!Debe sobrecargarse en las clases derivadas
	return Debe retornar un apuntador a un nuevo objeto de una clase derivada de OperadorCruce*/
	virtual OperadorCruce* operadorCruceDefecto() const =0;

	//!Debe generar una copia (creada con el operador \a new) del objeto Gen actual
	/*!Debe sobrecargarse en las clases derivadas
	\return Apuntador a un nuevo objeto de la clase derivada, con las mismas caracter�sticas que el actual*/
	virtual Gen *crearCopia() const =0;

	//!Debe especificar la asignaci�n a partir de otro objeto Gen
	/*!Debe sobrecargarse en las clases derivadas.
	Esta funci�n se podr�a reemplazar por el operador de asignaci�n (=) si Gen no 
	fuera una clase abstracta.*/
	virtual void copiar(const Gen& otro)=0;
};


/************************************************************************/
/*						CLASE Individuo									*/
/************************************************************************/

//!Clase que administra la informaci�n gen�tica de un individuo
/*!Un objeto de la clase Individuo est� conformado por un arreglo de apuntadores 
a objetos de clases derivadas de Gen. Adem�s cuenta con funciones que 
le proporcionan las caracter�sticas b�sicas necesarias para el algortimo gen�tico.*/
class Individuo
{
public:
	//!Constructor por defecto
	Individuo(AlgoritmoGenetico *pAG);

	//!Constructor por copia
	Individuo(const Individuo& origen);

	//Operador de asignaci�n a partir de otro objeto Individuo 
	const Individuo& operator = (const Individuo& origen);

	//!Destructor virtual
	virtual ~Individuo();

	//!Retorna el n�mero de genes presentes en el genoma del individuo
	/*!return Tama�o del genoma*/
	int getTamGenoma() const 
	{
		return m_pGenoma->getSize();
	}

	//!Devuelve el gen ubicado en una posici�n determinada del genoma. 
	/*!
	\param pos Posici�n del gen a obtener
	\return referencia al gen ubicado en la posici�n \a pos. Este no podr� ser modificado.*/
	const Gen& getGen(int pos) const
	{		
		return m_pGenoma->getObj(pos);
	}
	
	//!Adiciona un gen al final del genoma del individuo. 
	/*!
	\param pGen Aputador al gen a adicionar al genoma.
	\return Posici�n en la que fue adicionado el gen.*/
	int adicionarGen(Gen* pGen)
	{
		m_objetivoActualizado = false;
		return m_pGenoma->Adicionar(pGen);
	}
	
	//!Remplaza el gen ubicado en una posici�n determinada del genoma por otro gen
	/*! 
	\param pNuevoGen Apuntador al gen que remplazar� al gen ubicado en la posici�n especificada.
	\param pos Posici�n del gen a remplazar.
	\return Apuntador al gen reemplazado.*/
	Gen* remplazarGen(Gen* pNuevoGen, int pos)
	{
		m_objetivoActualizado = false;
		return m_pGenoma->remplazar(pNuevoGen, pos);		
	}
	
	//!Cambia el algoritmo genetico al que pertenece el individuo
	/*!\param pAG Apuntador al objeto AlgoritmoGen�tico del que har� parte el individuo.*/
	void SetAG(AlgoritmoGenetico *pAG)
	{
		m_pAG = pAG;
		m_objetivoActualizado = false;	//el nuevo algoritmo podr�a tener una funci�n objetivo diferente.
	}

	//!Asigna la probabilidad de supervivencia del individuo
	/*!\param probabilidad Nueva probabilidad de supervivencia*/
	void asignarProbabilidad(double probabilidad)
	{
		#if defined(UN_DEBUG)
			unASSERT(probabilidad >=0.0 && probabilidad <= 1.0);
		#endif
		m_Probabilidad = restringir(probabilidad,0.0,1.0);
	}
	
	//!Ordena al individuo que se genere aleatoriamente
	void generarAleatorio();
	
	/*!Retorna el valor de la funcion objetivo del individuo*/
	double objetivo(bool actualizarAG = false);

	//!Ordena al individuo que pase por el proceso de mutaci�n
	void mutar();
	
	//!Retorna la probabilidad de supervivencia del individuo
	/*!\return Probabilidad de supervivencia del individuo*/
	double getProbabilidad() const
	{
		return m_Probabilidad;
	}

	//!Retorna un apuntador al individuo con el que se efect�a el cruce
	/*!\return Apuntador al individuo con el que se efect�a el cruce*/
	Individuo* getPareja() const
	{
		return m_pPareja;
	}

	//!Asigna el apuntador al individuo con el que efectuar� el cruce 
	/*!\param pPareja Apuntador al individuo con el que efectuar� el cruce.*/
	void asignarPareja(Individuo* pPareja)
	{
		m_pPareja = pPareja;
	}

protected:
	
	//!Arreglo de apuntadores a objetos de las clases derivadas de Gen que contienen la informaci�n gen�tica del individuo
	Arreglo<Gen> *m_pGenoma;

	//!Apuntador al algoritmo genetico al que pertenece el individuo
	AlgoritmoGenetico* m_pAG;

	//!Almacena la probabilidad de supervivencia del individuo 
	double m_Probabilidad;

	//!Apuntador a la pareja con la que el individuo efectuar� el cruce de sus genes
	Individuo *m_pPareja;


private:
	//!Indica si \a m_Objetivo refleja el valor actual de la funci�n objetivo. 
	/*!Se establece a \a false despu�s de cualquier cambio 
	en el genoma para recalcular la funcion objetivo cuando es invocado el m�todo
	\a objetivo().*/
	bool m_objetivoActualizado;

	//!Almacena el valor de la funci�n objetivo
	double m_Objetivo;
};


/************************************************************************/
/*						CLASE Poblacion									*/
/************************************************************************/

//!Clase que administra la informaci�n de los individuos de un algoritmo gen�tico
/*!Un objeto de la clase Poblacion contiene un arreglo de apuntadores a objetos de 
la clase Individuo. Adem�s contiene los m�todos necesarios para realizar 
las operaciones sobre la poblaci�n en cada iteraci�n del algoritmo gen�tico*/
class Poblacion
{
public:
	//!Constructor por defecto
	Poblacion(AlgoritmoGenetico* pAG, int nIndividuos, bool crearAleatorios=true);

	//!Constructor por copia de otra poblacion. 
	Poblacion(const Poblacion& origen); 
	
	//!Destructor
	virtual ~Poblacion();
	
	//!Operador de asignaci�n a partir de otra poblaci�n
	const Poblacion& operator = (const Poblacion& origen);
	
	//!Retorna un apuntador al objeto de la clase AlgoritmoGenetico al que pertenece la poblaci�n
	/*!\return Apuntador al objeto de la clase AlgoritmoGenetico al que pertenece la poblaci�n*/
	AlgoritmoGenetico* GetAG() const
	{
		return m_pAG;
	}

	//!Ordena los miembros de la poblaci�n segun su funci�n de evaluaci�n
	void ordenar(bool Maximizar=false);
	
	//!Cambia el algoritmo genetico al que pertenece la poblaci�n
	/*!\param Ag Apuntador al objeto AlgoritmoGen�tico del que har� parte la poblacion.*/
	void SetAG(AlgoritmoGenetico *Ag)
	{
		m_pAG=Ag;
	}
	
	//!Devuelve el individuo ubicado en una posici�n determinada de la poblaci�n. 
	/*!
	\param pos Posici�n del individuo a obtener
	\return referencia al individuo ubicado en la posici�n \a pos.*/
	Individuo &getIndividuo(int pos) const
	{		
		return m_pGeneracion->getObj(pos);
	}

	//!Inserta un individuo en una posici�n determinada de la poblaci�n. 
	int InsertarIndividuo(Individuo* ind, int indice=-1);
	
	//!Remplaza el individuo ubicado en una posici�n determinada de la poblaci�n por otro individuo
	/*! 
	\param pNuevoInd Apuntador al idividuo que remplazar� al individuo ubicado en la posici�n especificada.
	\param pos Posici�n del individuo a remplazar.
	\return Apuntador al individuo reemplazado.*/
	Individuo* remplazarIndividuo(Individuo* pNuevoInd, int pos)
	{		
		return m_pGeneracion->remplazar(pNuevoInd, pos);		
	}

	//!Retorna el n�mero de individuos presentes en la poblaci�n
	/*!return Tama�o de la poblaci�n*/
	int getTam() const
	{
		return	m_pGeneracion->getSize();
	}

	//!Cambia el tama�o de la poblaci�n 
	int setTam(int nuevoTam, bool remplazar=false, bool crearAleatorios=false);

	//!Ordena a la poblaci�n que pase por el proceso de mutaci�n
	void mutar();

protected:
	//!Apuntador al objeto de la clase AlgoritmoGenetico al que pertenece la poblacion
	AlgoritmoGenetico *m_pAG;

	//!Arreglo de objetos de la clase Individuo que conforman la poblaci�n
	Arreglo<Individuo> *m_pGeneracion;
}; 


/************************************************************************/
/*						CLASE AlgoritmoGenetico							*/
/************************************************************************/

//!Clase abstracta que administra el proceso de un algoritmo gen�tico
/*!Un objeto de la clase AlgoritmoGenetico contiene una poblaci�n, los operadores
gen�ticos y todas las caracter�sticas necesarias para llevar a cabo el 
algoritmo gen�tico y permitir la interacci�n entre �ste y el usuario.
Para utilizarla deben definirse las funciones puramente virtuales \a codificacion(Individuo * pInd ,int estado)
y \a objetivo(). Para realizar todo el proceso del algoritmo gen�tico en un solo 
paso debe invocarse la funci�n \a optimizar().
Pueden cambiarse los par�metros y operadores por defecto sobrecargando las funciones
\a inicializarParametros() y \a definirOperadores()
\nosubgrouping*/
class AlgoritmoGenetico
{
 
public:

#if defined(USAR_VENTANA)

	//!Constructor cuando se usa el entorno gr�fico (Debe estar definida la constante USAR_VENTANA)
	AlgoritmoGenetico(AGFrame *m_pFrame);

	//!Informa al algoritmo un cambio de ventana. Se define solo cuando se usa el entorno gr�fico (Debe estar definida la constante USAR_VENTANA)
	void SetFrame(AGFrame *pFrame);

	//!Apuntador a la ventana de la aplicaci�n al utilizar entorno gr�fico (Debe estar definida la constante USAR_VENTANA)
	AGFrame *m_pFrame;

#else

	//!Constructor por defecto.
	AlgoritmoGenetico();

#endif

	//!Destructor
	/*!Destruye todos los objetos creados. Es virtual para poder 
	definirse en las clases derivadas.*/
	virtual ~AlgoritmoGenetico()
	{	
		finalizarOptimizacion();
	}

	//!En esta funci�n deben definirse los tres estados de codificaci�n de un individuo
	/*!Debe sobrecargarse en las clases derivadas.
	Esta funci�n es invocada definiendo el estado en que se encuentra el individuo.
	Cada individuo invoca esta funci�n cuando es necesario realizar su decodificaci�n. 
	Tambi�n es invocada por \a iniciarOptimizacion() para la creaci�n del individuo 
	modelo. El usuario solamente debe invocarla cuando requiere hacer actualizaci�n 
	expl�cita de las variables del sistema o de la informaci�n gen�tica de alg�n 
	individuo.
	Existen tres estados: creaci�n, codificaci�n y decodificaci�n, que se 
	determinan mediante el par�metro \a estado, el cual puede tomar los siguientes 
	valores: ESTADO_CREACION, ESTADO_CODIFICACION, y ESTADO_DECODIFICACION. 
	<ul>
	<li>ESTADO_CREACION: En el momento de su construcci�n, se define la forma
	que adoptar� el individuo, determinada por la constituci�n del genoma, 
	es decir, el n�mero y tipos de genes que contendr�.
	<li>ESTADO_CODIFICACION: Define la forma en que se traducen las variables 
	del modelo en informaci�n gen�tica del individuo. Estas variables deben estar
	definidas en la clase derivada.
	<li>ESTADO_DECODIFICACION: Define la forma en que se traduce la informaci�n 
	gen�tica del individuo en las variables del modelo. Estas variables deben estar
	definidas en la clase derivada.
	</ul>
	En la implementaci�n de esta funci�n, pueden utilizarse las MACROS 
	que facilitan la adici�n de genes al individuo, y que proveen los tres 
	estados en una sola instrucci�n. (Ver <a href="globals_defs.html">Definiciones</a> ).
	\param pInd Apuntador al individuo en proceso de codificaci�n.
	\param estado Estado en que se encuentra el individuo. */
	virtual void codificacion(Individuo * pInd ,int estado)=0;
	
	//!En esta funci�n debe definirse la forma de calcular la funci�n de evaluaci�n
	/*!Debe sobrecargarse en las clases derivadas.
	La funci�n objetivo se calcula haciendo uso de las variables del modelo, 
	definidas en la clase derivada. El usuario no debe invocar esta funci�n expl�citamente.
	Esta funci�n es invocada internamente por cada individuo para calcular su funci�n 
	objetivo despues de haber invocado la funci�n codificacion(Individuo * pInd ,int estado) 
	la cual actualiza las variables del sistema. 
	\return Valor de la funci�n de evaluaci�n para las variables actuales del sistema.*/
	virtual double objetivo()=0;

	//!Ejecuta todo el proceso de optimizaci�n
	virtual void optimizar();

	//!Prepara el algoritmo gen�tico para su ejecuci�n
	void iniciarOptimizacion();

	//!Efect�a la siguiente iteraci�n del algoritmo
	void iterarOptimizacion();

	//!Indica si el algoritmo debe finalizar
	bool finalizar();

	//!Finaliza el algoritmo gen�tico
	void finalizarOptimizacion();

	//!Presenta en pantalla los resultados intermedios del algoritmo gen�tico
	void mostrarMedidas();

	//!Almacena en el archivo de salida los resultados intermedios del algoritmo gen�tico
	void salvar();

protected:
	
	//!Inicializa las variables y par�metros del algoritmo
	void inicializarVariables();

	//!Inicializa los apuntadores en posiciones nulas
	void inicializarApuntadores();

	//!Crea los operadores a utilizar por el algoritmo
	void crearOperadores();

	//!Efect�a la asignaci�n de probabilidad de supervivencia a los individuos de la poblaci�n	
	void asignarProbabilidad();	

	//!Efect�a la selecci�n de los individuos que sobreviven para la nueva generaci�n	
	void seleccionar();

	//!Efect�a la asignaci�n de parejas de individuos para el cruce
	void asignarParejas();
	
	//!Ejecuta la estrategia general de reproducci�n
	void reproducir();

	//!Efect�a la mutaci�n de la poblaci�n actual
	void mutar();

	//!Efect�a la adaptaci�n de par�metros del algoritmo
	void adaptacion();

	//!Calcula los valores intermedios de las medidas de desempe�o del algoritmo gen�tico
	void actualizarMedidas();


private:

	//!Esta funci�n puede sobregargarse para cambiar los valores por defecto del algoritmo
	/*!Es invocada por inicializarVariables()*/		
	virtual void inicializarParametros(){};
	
	//!Esta funci�n puede sobregargarse para definir operadores diferentes a los establecidos por defecto.
	//*Es invocada por \a crearOperadores()*/	
	virtual void definirOperadores(){};


public:


//! @name Apuntadores
//@{

	//APUNTADORES
	
	//!Apuntador a la poblacion del algoritmo gen�tico
	Poblacion *m_pPoblacionActual;
	//!Apuntador al mejor individuo de la generaci�n actual
	Individuo *m_pMejorEnEstaGeneracion;
	//!Apuntador al peor individuo de la generaci�n actual
	Individuo *m_pPeorEnEstaGeneracion;
	//!Apuntador al mejor individuo de la historia del algoritmo
	Individuo *m_pMejorEnLaHistoria;
	//!Apuntador al Individuo que se emplea como modelo para producir copias de �l.
	Individuo *m_pModelo;

	//!Arreglo de operadores de mutaci�n
	/*!El tipo del operador en cada posici�n del arreglo debe coincidir con 
	el tipo de gen en la misma posici�n del genoma del individuo modelo*/
	Arreglo<OperadorMutacion> *m_pListaOperadorMutacion;

	//!Arreglo de operadores de cruce
	/*!El tipo del operador en cada posici�n del arreglo debe coincidir con 
	el tipo de gen en la misma posici�n del genoma del individuo modelo*/	
	Arreglo<OperadorCruce> *m_pListaOperadorCruce;

protected:
	//!Apuntador al operador de asignaci�n de probabilidad de supervivencia
	OperadorProbabilidad *m_pOpProbabilidad;
	//!Apuntador al operador de selecci�n de individuos
	OperadorSeleccion *m_pOpSeleccion;
	//!Apuntador al operador de asignaci�n de parejas de cruce 
	OperadorParejas *m_pOpParejas;
	//!Apuntador al operador de reproducci�n
	OperadorReproduccion *m_pOpReproduccion;
	//!Arreglo de operadores de adaptaci�n del algoritmo	
	Arreglo<OperadorAdaptacion> *m_pListaOperadorAdaptacion;
	//!Arreglo de operadores de finalizacion
	Arreglo<OperadorFinalizacion> *m_pListaOperadorFinalizacion;

//@}
public:

//! @name Par�metros del algoritmo 
//@{

	//PAR�METROS DEL ALGORITMO

	//!N�mero m�ximo de generaciones
	long m_GeneracionMaxima;
	//!Intervalo de iteraciones en que deben salvarse y/o mostrarse los valores intermedios
	long m_IntervaloSalvar;
	//!Nombre del archivo en que se guardan los valores intermedios de las iteraciones
	char m_NombreArchivo[400];
	
//@}

//! @name Indicadores
//@{

    //INDICADORES//

	//!Indica si se va a utilizar alguna estrategia de adaptaci�n en el algoritmo gen�tico
	bool m_IndicadorUsarAdaptacion;
	//!Indica si al inicializar la poblacion se crean individuos aleatorios o con los valores iniciales especificados
	bool m_IndicadorInicializarPoblacionAleatoria;
	//!Indica si debe maximizarse o minimizarse la funci�n objetivo
	bool m_IndicadorMaximizar;
	//!Indica si deben salvarse los resultados intermedios de las iteraciones del algoritmo gen�tico
	bool m_IndicadorArchivo;
	//!Indica si deben mostrarse en pantalla los resultados intermedios de las iteraciones del algoritmo gen�tico
	bool m_IndicadorMostrar;
	//!Indica si se debe salvar y/o mostrar la funci�n objetivo del mejor individuo en la historia
	bool m_IndicadorMostrarMejorEnHistoria;
	//!Indica si se debe salvar y/o mostrar la generaci�n en que apareci� el mejor individuo en la historia
	bool m_IndicadorMostrarGeneracionMejorHistorico;
	//!Indica si se debe salvar y/o mostrar la funci�n objetivo del mejor individuo de la generaci�n actual
	bool m_IndicadorMostrarMejorEnGeneracion;
	//!Indica si se debe salvar y/o mostrar la funci�n objetivo del peor individuo de la generaci�n actual
	bool m_IndicadorMostrarPeorEnGeneracion;
	//!Indica si se debe salvar y/o mostrar el promedio de las funciones objetivo de la generaci�n actual
	bool m_IndicadorMostrarMedia;
	//!Indica si se debe salvar y/o mostrar la desviaci�n est�ndar de las funciones objetivo de la generaci�n actual
	bool m_IndicadorMostrarDesviacion;
	//!Indica si se debe salvar y/o mostrar la medida OnLine para la generaci�n actual
	bool m_IndicadorMostrarOnLine;
	//!Indica si se debe salvar y/o mostrar  la medida OffLine para la generaci�n actual
	bool m_IndicadorMostrarOffLine;

//@}

//! @name Valores propios de cada iteraci�n
//@{

	//VALORES PROPIOS DE CADA ITERACION

	//!N�mero de individuos de la generaci�n actual
	int m_TamanoPoblacion;
	//!N�mero de generaci�n actual
	long m_Generacion;
	//!N�mero de generaci�n en la que apareci� el mejor individuo de la historia del algoritmo
	long m_GeneracionDelMejorEnLaHistoria;
	//!Promedio aritm�tico de las funciones objetivo de los individuos de la generaci�n actual
	double m_Media;
	//!Promedio aritm�tico acumulado de las funciones objetivo de todos los individuos que han existido en la historia del algoritmo.
	double m_MedidaOnLine;
	//!Promedio aritm�tico acumulado de las funciones objetivo de los mejores individuos de cada generaci�n en la historia del algoritmo.
	double m_MedidaOffLine;
	//!Medida OnLine de la generaci�n anterior
	double m_MedidaOnLineAnterior;
	//!Medida OffLine de la generaci�n anterior
	double m_MedidaOffLineAnterior;
	//!Desviaci�n est�ndar de las funciones objetivo de los individuos presentes en la generaci�n actual.
	double m_Desviacion;

//@}
};


/************************************************************************************/
/*								OPERADORES B�SICOS									*/					
/************************************************************************************/


//!Clase abstracta que define el proceso de adaptaci�n del algoritmo gen�tico
/*!El proceso de adaptaci�n hace que se modifiquen los par�metros del algoritmo 
gen�tico durante su ejecuci�n. Un objeto de una clase derivada de OperadorAdaptacion 
opera sobre el algoritmo gen�tico realizando cambios en su estructura funcional con el fin 
de variar su desempe�o.*/
class OperadorAdaptacion
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas*/
	OperadorAdaptacion(){}

	//!Destructor
	/*!Es virtual para poder sobrecargarse en las clases derivadas*/
	virtual ~OperadorAdaptacion(){}

	//!Ejecuta el procedimiento de adaptacion
	/*!Debe sobrecargarse en las clases derivadas
	\param pAG Apuntador al algoritmo gen�tico sobre el que opera*/
	virtual void adaptacion(AlgoritmoGenetico *pAG)=0;
};


//!Clase abstracta que administra el proceso de mutaci�n en el algoritmo gen�tico
/*!El proceso de mutaci�n consiste en alterar la informaci�n gen�tica 
de un individuo. Un objeto de una clase derivada de OperadorMutacion
opera sobre un objeto de una clase derivada de Gen, por lo tanto, los dos objetos deben
manejar el mismo tipo de dato.*/
class OperadorMutacion
{
public:
	//!Constructor por defecto
	/*!Inicializa la probabilidad de mutaci�n para cada el objeto derivado de Gen 
	sobre el que opera.
	\param ProbabilidadMutacion Probabilidad de mutaci�n del gen sobre el que opera. Por defecto es igual a 0.1*/
	OperadorMutacion(double ProbabilidadMutacion=0.1)
	{
		m_ProbabilidadMutacion = ProbabilidadMutacion;
	}

	//!Destructor
	/*!Es virutal para poder definirse en las clases derivadas.*/
	virtual ~OperadorMutacion(){}

	//!Decide si debe realizarse el proceso de mutaci�n sobre un gen
	/*!La selecci�n de los genes que deben mutar se realiza aleatoriamente
	teniendo en cuenta la probabilidad de mutaci�n de cada gen.
	\param pGen Apuntador al objeto derivado de Gen sobre el que se toma la decisi�n.*/
	virtual void mutar(Gen *pGen)
	{
		double azar= (double)rand()/(double)RAND_MAX;
		if(azar<m_ProbabilidadMutacion)	mutarGen(pGen);
	}

	//!Retorna la probabilidad de mutaci�n del gen sobre el que opera
	/*!\return Probabilidad de mutaci�n del gen*/
	double ObtenerProbabilidadMutacion() const
	{
		return m_ProbabilidadMutacion;
	}

	//!Asigna un nueva probababilidad de mutaci�n al gen sobre el que opera
	/*!\param Probabilidad Probabilidad de mutaci�n a asignar al gen.
	\return Probabilidad de mutaci�n asignada al gen.*/
	virtual double AsignarProbabilidadMutacion(double Probabilidad)
	{
		return (m_ProbabilidadMutacion = restringir(Probabilidad, 0.0, 1.0));
	}

protected:
	//!Almacena la probabilidad de mutaci�n del gen sobre el que opera
	double m_ProbabilidadMutacion;

private:
	//!Efect�a el procedimiento de mutaci�n sobre un gen
	/*!Debe sobrecargarse en las clases derivadas
	\param pGen Apuntador al gen sobre el que opera*/
	virtual void mutarGen(Gen *pGen)=0;
};


//!Clase abstracta que administra el proceso de cruce entre los individuos del algoritmo gen�tico
/*!El proceso de cruce consiste en intercambiar la informaci�n gen�tica de dos individuos
para generar nuevos individuos. Un objeto de una clase derivada de OperadorCruce
opera sobre objetos de clases derivadas de Gen, por lo tanto, estos objetos deben
manejar el mismo tipo de dato.*/ 
class OperadorCruce
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas*/
	OperadorCruce(){}

	//!Destructor
	/*!Es virtual para definirse en las clases derivadas*/
	virtual ~OperadorCruce(){}

	//!M�todo virtual que ejecuta el procedimiento de cruce entre dos indivduos
	/*!Debe sobrecargarse en las clases derivadas.
	\param pMejor Apuntador al gen padre con la mejor funci�n de evaluaci�n.
	\param pPeor Apuntador al gen padre con la peor funci�n de evaluaci�n.
	\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
	\param numHijos Numero de genes hijos a crear en el cruce.
	\param indice Valor que identifica al individuo de cuyos genes se realiza el cruce. No es utilizado en todas las clases derivadas*/
	virtual void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos,int numHijos, int indice)=0;
};


//!Clase abstracta que administra la asignaci�n de la probabilidad de supervivencia a los individuos de la poblaci�n 
/*!Generalmente, la probabilidad de superviviencia se asigna con base en la funci�n de 
evaluaci�n de cada individuo.*/
class OperadorProbabilidad
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse  en las clases derivadas*/
	OperadorProbabilidad(){}

	//!Destructor
	/*!Es virtual para poder definirse en las clases derivadas*/
	virtual ~OperadorProbabilidad(){}

	//!Asigna una probabilidad de supervivencia a los individuos de la poblaci�n
	/*!Debe sobrecargarse en las clases derivadas
	\param Pob Referencia a la poblaci�n sobre la que opera.
	\param Maximizar Si si valor es \a true a los individuos se les asigan probabilidades de supervivencia directamente proporcionales a su funci�n objetivo, de lo contrario se asignan probabiliades inversamente proporcionales.*/
	virtual void asignarProbabilidad(Poblacion &Pob, bool Maximizar)=0;
};


//!Clase abstracta que administra el proceso de selecci�n de individuos en la poblaci�n
/*!Un objeto de una clase derivada de OperadorSeleccion selecciona un conjunto de individuos
presentes en la generaci�n actual que servir�n de base para formar la nueva generaci�n del
algoritmo.*/
class OperadorSeleccion
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas*/
	OperadorSeleccion(){}

	//!Destructor
	/*!Es virtual para poder definirse en las clases derivadas*/
	virtual ~OperadorSeleccion(){}

	//!Efect�a el proceso de selecci�n en la poblaci�n
	/*!Debe sobrecargarse en las clases derivadas 
	\param pPob Referencia a la poblaci�n sobre la que opera.*/
	virtual void seleccionar(Poblacion& pPob)=0;
};


//!Clase abstracta que administra la asignaci�n de parejas para cada individuo de la poblaci�n
/*!Para cada individuo presente en la poblaci�n se asigna otro individuo que servir� 
de pareja para intercambiar su informaci�n gen�tica.*/
class OperadorParejas
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas*/
	OperadorParejas(){}

	//!Destructor
	/*!Es virtual para poder defirnirse en las clases derivadas*/
	virtual ~OperadorParejas(){}

	//!Asigna las parejas a los individuos de la poblaci�n
	/*!Debe sobrecargarse en las clases derivadas. 
	\param Pob Referencia a la poblaci�n sobre la que opera.*/
	virtual void asignarParejas(Poblacion& Pob)=0;
};

//!Clase abstracta que define la estrategia general de reproducci�n del algoritmo
/*!La estrategia general de reproducci�n define cu�les individuos conformar�n la
siguiente generaci�n del algoritmo.*/
class OperadorReproduccion
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas.*/
	OperadorReproduccion(){}

	//!Destructor
	/*!Es virtual para poder definirse en las clases derivadas.*/
	virtual ~OperadorReproduccion(){}

	//!Ejecuta la estrategia general reproducci�n para una poblaci�n
	/*!Debe sobrecargarse en las clases derivadas.
	\param Pob Referencia a la poblaci�n sobre la que opera.
	\param Maximizar Su valor sirve como referencia para determinar qu� individuo es mejor a otro dependiendo de su funci�n de evaluaci�n.*/
	virtual void reproducir(Poblacion& Pob, bool Maximizar)=0;
};


//!Clase abstracta que define la estrategia de finalizaci�n del algoritmo gen�tico
class OperadorFinalizacion
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas.*/
	OperadorFinalizacion(){}

	//!Destructor
	/*!Es virtual para poder definirse en las clases derivadas.*/
	virtual ~OperadorFinalizacion(){}

	//!Indica si el algoritmo debe finalizar  
	/*!Debe sobrecargarse en las clases derivadas
	\return \a true para indicar que el algoritmo debe finalizar, \a false en caso contrario*/
	virtual bool finalizar(const AlgoritmoGenetico& AG)=0;
};



///////////////////////////////////CLASES DERIVADAS DE LOS OPERADORES/////////////////////////////



//!Clase derivada de la clase OperadorAdaptacion encargada de efectuar el proceso de elitismo para el algoritmo
/*!El elitismo consiste en obtener el mejor individuo de la generaci�n actual y 
copiarlo en la siguiente generaci�n.*/
class OperadorAdaptacionElitismo:public OperadorAdaptacion
{
public:
	//!Constructor 
	OperadorAdaptacionElitismo(){};

	//!Destructor
	~OperadorAdaptacionElitismo(){}

	//!M�todo que efect�a el proceso de elitismo
	/*!\param pAG Apuntador al algoritmo gen�tico sobre el cual opera.*/
	void adaptacion(AlgoritmoGenetico *pAG);
};


//!Clase derivada de la clase OperadorAdaptacion que define la estrategia de adaptaci�n para la probabilidad de mutaci�n de los genes de cada individuo de la poblaci�n
/*!Existen dos m�todos de adaptaci�n que var�an la probabilidad de mutaci�n: 
<ul>
<li>Variaci�n escalonada: aumenta la probabilidad de mutaci�n de forma gradual y escalonada para los genes 
cuando la variaci�n de la medida Offline permanece en un rango establecido durante un n�mero determinado de 
generaciones consecutivas.
<li>Variaci�n exponencial: var�a la probabilidad de mutaci�n de todos los genes siguiendo un comportamiento 
exponencial decreciente determinado por \f$Pm(t)=P_{max} e^{\frac{-t}{T}}\f$
donde \a Pmax es el valor m�ximo que puede tomar la probabiliad de mutaci�n de un gen, \a t corresponde 
al n�mero de la generaci�n actual y \a T es un valor positivo preestablecido. 
</ul>*/
class OperadorAdaptacionProbMutacion:public OperadorAdaptacion
{
public:
	//!Constructor 
	OperadorAdaptacionProbMutacion(AlgoritmoGenetico *pAG, int TipoAdaptacion=ADAPTACION_PROBMUTACION_EXPONENCIAL);
	
	//!Destructor
	~OperadorAdaptacionProbMutacion(){}

	//!Efect�a el proceso de adaptacion de variaci�n de la probabilidad de mutaci�n
	void adaptacion(AlgoritmoGenetico *pAG);
	
	//!Cambia los par�metros para la ADAPTACION_PROBMUTACION_OFFLINE
	void setParamsOffline(double MaxProb=0.5, double FactorVariacion=0.001, double Escalon = 0.1, int MaxCont = 10);
	//!Cambia los par�metros para la ADAPTACION_PROBMUTACION_EXPONENCIAL
	void setParamsExponencial(double MaxProb=0.5, double T=-1);

protected:

	//!Determina la forma de cambiar la probabilidad de mutaci�n
	int m_tipoAdaptacion;
	//!Determina la m�xima probabilidad de mutaci�n en un gen
	double m_maxProb;
	//!Determina el n�mero m�ximo de generaciones que pueden pasar sin que var�e la probabilidad de mutaci�n cuando la medida offline no var�a en un factor mayor a \a m_factorVariacion. S�lo se utiliza en la ADAPTACION_PROBMUTACION_OFFLINE
	int	m_maxCont;
	//!Determina el factor de variaci�n para la medida Offline. S�lo se utiliza en la ADAPTACION_PROBMUTACION_OFFLINE
	double m_factorVariacion;
	//!Determina la cantidad que se sumar� a la probabilidad de mutaci�n en cada variaci�n. S�lo se utiliza en la ADAPTACION_PROBMUTACION_OFFLINE
	double m_escalon;
	//!Valor positivo determina la atenuaci�n de la curva exponencial. S�lo se utiliza en la ADAPTACION_PROBMUTACION_EXPONENCIAL
	double m_T;
};


//!Clase derivada de la clase OperadorAdaptacion que hace una variaci�n del n�mero de individuos del algoritmo gen�tico
/*!La primera generaci�n del algoritmo contiene un n�mero m�ximo de individuos establecido, este n�mero 
disminuye linealmente hasta alcanzar un n�mero m�nimo de individuos predefinido. Este l�mite se alcanza 
cuando se llega a la �ltima generaci�n establecida para el algoritmo.*/
class OperadorAdaptacionNumIndividuos:public OperadorAdaptacion
{
public:
	//!Constructor 
	OperadorAdaptacionNumIndividuos(AlgoritmoGenetico *pAG, int NumIndivInicio=0, int NumIndivFin=0);

	//!Destructor
	~OperadorAdaptacionNumIndividuos(){}

	//!M�todo para efectuar el proceso de adaptaci�n de n�mero de individuos 
	void adaptacion(AlgoritmoGenetico *pAG);
protected:
	
	//!Indica el n�mero inicial de individuos en la poblaci�n 
	int m_nIndivInicio;
	//!Indica el n�mero final de individuos en la poblaci�n
	int m_nIndivFin;
};


//!Clase derivada de la clase OperadorProbabilidad que define el proceso de asignaci�n de probabilidad de supervivencia proporcional a cada individuo de la poblaci�n
/*!En una poblaci�n de \a N individuos, se asigna una probabilidad de supervivencia a cada individuo proporcional al valor de su 
funci�n objetivo \a FObj(i), as�:
<ul>  
<li>Al minimizar:
\f[P(i)=\frac{\frac{1}{FObjProp(i)}}
{\Sigma_{i=1}^N \frac{1}{FObj(i)}}
\f]

<li>Al maximizar:
\f[P(i)=\frac{FObjProp(i)}
{\Sigma_{i=1}^N FObj(i)}\f]
</ul>

Donde, \f[FObjProp(i)=FObj(i)+(1-FObjmin)\f]
\f[FObjmin = min\left\{1,FObj(i)\right\},\ \ \ i=1, \ldots, N.\f]*/
class OperadorProbabilidadProporcional:public OperadorProbabilidad
{
public:
	//!Constructor
	OperadorProbabilidadProporcional(){}
	//!Destructor
	~OperadorProbabilidadProporcional(){}
	//!Efect�a el proceso de asignaci�n de probabilidad de supervivencia proporcional a cada individuo de la poblaci�n
	void asignarProbabilidad(Poblacion& Pob, bool Maximizar);
};

//!Clase derivada de la clase OperadorProbabilidad que define el proceso de asignaci�n de probabilidad de supervivencia lineal
/*!En una poblaci�n de \a N individuos, se ordenan de mejor a peor y se asigna a cada uno una probabilidad de supervivencia
que depende de su posici�n en la poblaci�n, as�:

\f[P(i)=\frac{\eta_{max}-\frac{(\eta_{max}-\eta_{min})i}{N+1}}{N}\f]

Donde,
 	\f[\eta_{max}=2-\eta_{min}\f]

\f$\eta_{min}\f$ se escoge aleatoriamente del intervalo [0, 2].*/
class OperadorProbabilidadLineal:public OperadorProbabilidad
{
public:
	//!Constructor 
	/*!\param Nmin Valor que pondera el grado de proporcionalidad. Por defecto es igual a 0.5.*/
	OperadorProbabilidadLineal(double Nmin=0.5)
	{
		setNmin(Nmin);
	}
	//!Cambia el valor del par�metro \a Nmin comprobando los l�mites
	/*!\param Nmin Valor a asignar al par�metro \a Nmin*/
	void setNmin(double Nmin)
	{
		m_nmin = restringir(Nmin, 0.0, 2.0);
	}
	//!Retorna el valor del par�metro \a Nmin
	/*!\return Valor del par�metro \a Nmin */
	double getNmin() const
	{
		return m_nmin;
	}
	
	//!Destructor 
	~OperadorProbabilidadLineal(){}
	
	//!Efect�a el proceso de asignaci�n de probabilidad de supervivencia lineal
	void asignarProbabilidad(Poblacion &Pob, bool Maximizar);

protected:	
	/*!Valor que pondera el grado de proporcionalidad*/
	double m_nmin;
};


//!Clase derivada de la clase OperadorProbabilidad que define el proceso de asignaci�n de probabilidad de supervivencia homog�nea
/*!En una poblaci�n de \a N individuos, se asigna la misma probabilidad de supervivencia a todos ellos, dada por:
\f[P(i)=\frac{1}{N}\f]*/
class OperadorProbabilidadHomogenea:public OperadorProbabilidad
{
public:
	//!Constructor 
	OperadorProbabilidadHomogenea(){}
	//!Destructor 
	~OperadorProbabilidadHomogenea(){}
	//!Efect�a el proceso de asignaci�n de probabilidad de supervivencia homog�nea 
	void asignarProbabilidad(Poblacion &Pob, bool Maximizar);
};



//!Clase derivada de la clase OperadorSeleccion que define el proceso de selecci�n estoc�stica con reemplazo de los individuos
/*!Los individuos que componen la poblaci�n son ordenados como segmentos de una l�nea, cada segmento 
corresponde a la probabilidad de supervivencia del individuo. La selecci�n se produce al generarse un valor aleatorio
que coincida con el segmento del individuo. El proceso se repite hasta obtener el n�mero de
individuos deseados, creando as� la base para la nueva poblaci�n.*/
class OperadorSeleccionEstocasticaRemplazo:public OperadorSeleccion
{
public:
	//!Constructor
	OperadorSeleccionEstocasticaRemplazo(){}
	//!Destructor 
	~OperadorSeleccionEstocasticaRemplazo(){}
	//!Ejecuta el proceso de selecci�n estoc�stica con reemplazo de los individuos de la plobaci�n
	void seleccionar(Poblacion& pPob);
};

//!Clase derivada de OperadorParejas que define la asignaci�n de parejas aleatorias para los individuos de la poblaci�n
/*!A cada individuo de la poblaci�n se le asigna otro individuo que corresponder� a su pareja en el proceso de reproducci�n. 
La asignaci�n se realiza aleatoriamente teniendo en cuenta que no deben asignarse individuos que ya tengan
pareja establecida y que un individuo no puede ser asignado como su propia pareja. Cuando el numero de individuos 
es impar, a un individuo de la poblaci�n no se le asigna pareja.*/
class OperadorParejasAleatorias:public OperadorParejas
{
public:
	//!Constructor 
	OperadorParejasAleatorias(){}
	//!Destructor
	~OperadorParejasAleatorias(){}
	//!Asigna parejas aleatoriamente para los individuos de la poblaci�n
	void asignarParejas(Poblacion& Pob);
};


//!Clase derivada de la clase OperadorParejas que define la asignaci�n de parejas adyacentes para los individuos de la poblaci�n
/*!A cada individuo de la poblaci�n se le asigna otro individuo que corresponder� a su pareja en el proceso de reproducci�n. 
Al individuo \a i de la poblaci�n se le asiga como pareja el individuo <i>i+1</i>, el proceso contin�a con los individuos
a los que a�n no se les ha asignado pareja. Cuando el numero de individuos es impar, 
al �ltimo individuo de la poblaci�n no se le asigna una pareja.*/
class OperadorParejasAdyacentes:public OperadorParejas
{
public:
	//!Constructor 
	OperadorParejasAdyacentes(){}
	//!Destructor 
	~OperadorParejasAdyacentes(){}
	//!Asigna parejas adyacentes para los individuos de la poblaci�n 
	void asignarParejas(Poblacion& Pob);
};

//!Clase derivada de la clase OperadorParejas que define la asignaci�n de parejas  extremas para los individuos de la poblaci�n
/*!
A cada individuo de la poblaci�n se le asigna otro individuo que corresponder� a su pareja en el proceso de reproducci�n. 
Al primer individuo del arreglo de la poblaci�n se le asigna como pareja el �ltimo individuo del arreglo, al segundo individuo se 
le asigna el pen�ltimo. El proceso contin�a sucesivamente hasta asignar parejas a todos los individuos de la generaci�n. 
Cuando el numero de individuos es impar, al individuo ubicado en el centro de la poblaci�n no se le asigna pareja.*/
class OperadorParejasExtremos:public OperadorParejas
{
public:
	//!Constructor 
	OperadorParejasExtremos(){}
	//!Destructor 
	~OperadorParejasExtremos(){}
	//!Asigna parejas extremas para los individuos de la poblaci�n 
	void asignarParejas(Poblacion& Pob);
};


//!Clase derivada de la clase OperadorReproduccion que define el cruce simple entre dos individuos de la poblaci�n
/*!El cruce simple genera dos individuos hijos por cada cruce entre dos individuos padres intercambiando los genes
respectivos en un punto de cruce. 
El punto de cruce se selecciona aleatoriamente para cada pareja en el intervalo [1, n-1] donde \a n
corresponde al n�mero de genes en el genoma. A partir del gen siguiente al punto de cruce hasta 
el final del genoma se intercambian los genes del individuo padre con los de la madre, estos nuevos individuos 
corresponden a los dos hijos generados que reemplzar�n a sus padres en la siguiente generaci�n del algoritmo.*/
class OperadorReproduccionCruceSimple:public OperadorReproduccion
{
public:
	//!Constructor 
	OperadorReproduccionCruceSimple(){}
	//!Destructor 
	~OperadorReproduccionCruceSimple(){}
	//!Ejecuta el cruce simple entre dos individuos de la poblaci�n 
	void reproducir(Poblacion& Pob, bool Maximizar);
};


//!Clase derivada de la clase OperadorReproduccion que define la estrategia de reproducci�n dos padres dos hijos
/*!Por cada pareja de individuos que efect�en el cruce se crean dos hijos que 
reemplazar�n a sus padres en la siguiente iteraci�n del algoritmo*/
class OperadorReproduccionDosPadresDosHijos:public OperadorReproduccion
{
public:
	//!Constructor 
	OperadorReproduccionDosPadresDosHijos(){}
	//!Destructor de la clase OperadorReproduccionDosPadresDosHijos
	~OperadorReproduccionDosPadresDosHijos(){}
	//!Ejecuta la estrategia de reproducci�n dos padres dos hijos
	void reproducir(Poblacion& Pob, bool Maximizar);
};

//!Clase derivada de la clase OperadorReproduccion que define la estrategia de reproducci�n mejor padre mejor hijo
/*!Por cada pareja de individuos que efect�en el cruce se crean 
dos hijos. El mejor individuo hijo reemplazar� al peor individuo 
padre en la siguiente iteraci�n del algoritmo*/
class OperadorReproduccionMejorPadreMejorHijo:public OperadorReproduccion
{
public:
	//!Constructor 
	OperadorReproduccionMejorPadreMejorHijo(){}
	//!Destructor 
	~OperadorReproduccionMejorPadreMejorHijo(){}
	//!Ejecuta la estrategia de reproducci�n mejor padre mejor hijo
	void reproducir(Poblacion& Pob, bool Maximizar);
};


//!Clase derivada de la clase OperadorReproduccion que define la estrategia de reproducci�n mejores entre padres e hijos
/*!Por cada pareja de individuos que efect�en el cruce se crean 
dos hijos. Los individuos que conformar�n la siguiente generaci�n 
ser�n los dos que tengan la mejor funci�n de evaluaci�n entre los cuatro.*/
class OperadorReproduccionMejoresEntrePadresEHijos:public OperadorReproduccion
{
public:
	//!Constructor 
	OperadorReproduccionMejoresEntrePadresEHijos(){}
	//!Destructor 
	~OperadorReproduccionMejoresEntrePadresEHijos(){}
	//!Ejecuta la estrategia de reproducci�n mejores entre padres e hijos
	void reproducir(Poblacion& Pob, bool Maximizar);
};



//!Clase derivada de la clase OperadorFinalizacion que define la finalizaci�n del algoritmo bas�ndose en su medida online
/*!Ordena la interrupci�n del algoritmo gen�tico cuando �ste no presenta una variaci�n significativa en su
medida online despu�s de un n�mero determinado de generaciones.*/
class OperadorFinalizacionOnline:public OperadorFinalizacion
{
public:
	//!Constructor
	/*!Inicializa los valores de los par�metros \a FactorVariacion y \a MaxCont
	\param FactorVariacion 	Determina el factor de variaci�n para la medida online. Por defecto es igual a 0.001.
	\param MaxCont Determina el n�mero m�ximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida online no var�a en un factor mayor a \a FactorVariacion. Por defecto es igual a 30.*/
	OperadorFinalizacionOnline(double FactorVariacion=0.001, int MaxCont=30)
	{
		setParams(FactorVariacion, MaxCont);
	}	

	//!Cambia el valor de los par�metros \a FactorVariacion y \a maxCont
	/*!\param FactorVariacion Valor a asignar al par�metro \a FactorVariacion.
	\param MaxCont Valor a asignar al par�metro MaxCont.*/
	void setParams(double FactorVariacion, int MaxCont)
	{
		m_factorVariacion = FactorVariacion;
		if(MaxCont<1) MaxCont = 1;
		m_maxCont = MaxCont;
	}

	//!Suministra informaci�n sobre los valores de los par�metros \a FactorVariacion y \a MaxCont
	/*!\param FactorVariacion Determina el factor de variaci�n para la medida online.
	\param MaxCont Determina el n�mero m�ximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida online no var�a en un factor mayor a \a FactorVariacion.*/
	void getParams(double& FactorVariacion, int& MaxCont) const
	{
		FactorVariacion = m_factorVariacion;
		MaxCont = m_maxCont;
	}

	//!Destructor
	~OperadorFinalizacionOnline(){}

	//!Especifica cu�ndo debe darse la orden de interrupci�n del algoritmo gen�tico
	/*!\return \a true si el algoritmo debe finalizar, o \a false en caso contrario.*/
	bool finalizar(const AlgoritmoGenetico& AG)
	{			
		if(AG.m_Generacion<=1) m_contador=0;	//inicializar contador
		double dif = fabs(AG.m_MedidaOnLine - AG.m_MedidaOnLineAnterior);

		if (dif<=fabs(AG.m_MedidaOnLine*m_factorVariacion)) 
			m_contador++;
		else 
			m_contador=0;
		
		return (m_contador>=m_maxCont);
	}
	
private:	
	//!Determina el factor de variaci�n para la medida online
	double m_factorVariacion;
	//!Determina el n�mero m�ximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida online no var�a en un factor mayor a \a m_factorVariacion.
	int m_maxCont;
	//!Almacena el n�mero de generaciones que se han ejecutado sin que la medida online var�e en un factor mayor a \a m_factorVariacion
	int m_contador;
};


//!Clase derivada de la clase OperadorFinalizacion que define la finalizaci�n del algoritmo bas�ndose en su medida online
/*!Ordena la interrupci�n del algoritmo gen�tico cuando �ste no presenta una variaci�n significativa en su
medida offline despu�s de un n�mero determinado de generaciones.*/
class OperadorFinalizacionOffline:public OperadorFinalizacion
{
public:
	//!Constructor
	/*!Inicializa los valores de los par�metros \a FactorVariacion y \a MaxCont
	\param FactorVariacion 	Determina el factor de variaci�n para la medida offline. Por defecto es igual a 0.005.
	\param MaxCont Determina el n�mero m�ximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida offline no var�a en un factor mayor a \a FactorVariacion. Por defecto es igual a 30.*/
	OperadorFinalizacionOffline(double FactorVariacion=0.0005, int MaxCont=30)
	{
		setParams(FactorVariacion, MaxCont);
	}	

	//!Cambia el valor de los par�metros \a FactorVariacion y \a maxCont
	/*!\param FactorVariacion Valor a asignar al par�metro \a FactorVariacion.
	\param MaxCont Valor a asignar al par�metro MaxCont.*/
	void setParams(double FactorVariacion, int MaxCont)
	{
		m_factorVariacion = FactorVariacion;
		if(MaxCont<1) MaxCont = 1;
		m_maxCont = MaxCont;
	}

	//!Suministra informaci�n sobre los valores de los par�metros \a FactorVariacion y \a MaxCont
	/*!\param FactorVariacion Determina el factor de variaci�n para la medida offline.
	\param MaxCont Determina el n�mero m�ximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida offline no var�a en un factor mayor a \a FactorVariacion.*/
	void getParams(double& FactorVariacion, int& MaxCont) const
	{
		FactorVariacion = m_factorVariacion;
		MaxCont = m_maxCont;
	}

	//!Destructor
	~OperadorFinalizacionOffline(){}

	//!Especifica cu�ndo debe darse la orden de interrupci�n del algoritmo gen�tico
	/*!\return \a true si el algoritmo debe finalizar, o \a false en caso contrario.*/
	bool finalizar(const AlgoritmoGenetico& AG)
	{			
		if(AG.m_Generacion<=1) m_contador=0;	//inicializar contador
		double dif = fabs(AG.m_MedidaOffLine - AG.m_MedidaOffLineAnterior);

		if (dif<=fabs(AG.m_MedidaOffLine*m_factorVariacion)) 
			m_contador++;
		else 
			m_contador=0;
		
		return (m_contador>=m_maxCont);
	}
	
private:	
	//!Determina el factor de variaci�n para la medida offline
	double m_factorVariacion;
	//!Determina el n�mero m�ximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida offline no var�a en un factor mayor a \a m_factorVariacion.
	int m_maxCont;
	//!Almacena el n�mero de generaciones que se han ejecutado sin que la medida offline var�e en un factor mayor a \a m_factorVariacion
	int m_contador;
};


#pragma warning(pop)

/****************************************************************************/
/*				DEFINICION DE MACROS PARA USO DE LA LIBRERIA				*/
/****************************************************************************/

/*!Macro que retorna el n�mero menor entre \a a y \a b*/
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define _MINIMO(a,b)  ((a)<(b)? (a):(b) )
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/*!Macro que retorna el n�mero mayor entre \a a y \a b*/
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define _MAXIMO(a,b)  ((a)>(b)? (a):(b) )
#endif /* DOXYGEN_SHOULD_SKIP_THIS */



//!Macro que crea una nueva clase derivada de AlgoritmoGenetico
/*!Declara e implementa el constructor por defecto de esta clase,
tambi�n declara implicitamente las funciones de caracter obligatorio, 
las cuales deben ser implementadas en el proyecto de optiizaci�n, usando la forma:
<pre>
tipoRetornado nombreAlgoritmo::nombreFuncion(parametros)
{
	//cuerpo de la funci�n
}
</pre>
Por ejemplo:
<pre>
double nombreAlgoritmo::objetivo()
{
	double FO;
	//instrucciones correspondientes a la funcion objetivo
	return FO;
}
</pre>
Pueden declararse e implementarse adicionalmente otros constructores, m�todos y variables.
Tambien puede adicionarse un destructor, en caso de necesitar destruir apuntadores a
objetos creados din�micamente.*/
#if defined(USAR_VENTANA)

/*Si estamos usando wxWindows, el constructor debe comunicar 
a la clase base el apuntador al frame*/
#define DECLARAR_ALGORITMO(nombreAlgoritmo)							\
	class nombreAlgoritmo : public AlgoritmoGenetico				\
	{																\
	public:															\
		nombreAlgoritmo(AGFrame* pFrame):AlgoritmoGenetico(pFrame){}\
		void codificacion(Individuo * pInd ,int estado);			\
		double objetivo();

#else

#define DECLARAR_ALGORITMO(nombreAlgoritmo)					\
	class nombreAlgoritmo : public AlgoritmoGenetico		\
	{														\
	public:													\
		nombreAlgoritmo(){}									\
		void codificacion(Individuo * pInd ,int estado);	\
		double objetivo();

#endif


//!Macro que termina la creaci�n de una clase derivada de AlgoritmoGenetico
#define FIN_DECLARAR_ALGORITMO							\
	};			



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define __ADICIONAR_GEN(pIndividuo, pos, var, TipoDato, TipoGen, constructor)	\
	{																\
		if(estado==ESTADO_CREAR)									\
		{															\
			Gen *pGen=new TipoGen constructor;						\
			pIndividuo->adicionarGen(pGen);							\
		}															\
		else if(estado==ESTADO_CODIFICAR)							\
		{															\
			TipoGen &_gen = (TipoGen&)(pIndividuo->getGen(pos));	\
			_gen = var;												\
		}															\
		else if(estado==ESTADO_DECODIFICAR)							\
		{															\
			TipoGen &_gen = (TipoGen&)(pIndividuo->getGen(pos));	\
			var = (TipoDato)(_gen);									\
		}															\
	}
#endif  /* DOXYGEN_SHOULD_SKIP_THIS */


//!Macro que define el operador de probabilidad que se emplear� en el algoritmo gen�tico
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus par�metros iniciales.*/
#define DEFINIR_OPERADOR_PROBABILIDAD(tipoOperador)		\
	{													\
		if(m_pOpProbabilidad) delete m_pOpProbabilidad;	\
		m_pOpProbabilidad=new tipoOperador;				\
	}

//!Macro que define el operador de seleccci�n que se emplear� en el algoritmo gen�tico
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus par�metros iniciales.*/
#define DEFINIR_OPERADOR_SELECCION(tipoOperador)		\
	{													\
		if(m_pOpSeleccion) delete m_pOpSeleccion;		\
		m_pOpSeleccion=new tipoOperador;				\
	}

//!Macro que define el operador de parejas que se emplear� en el algoritmo gen�tico
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus par�metros iniciales.*/
#define DEFINIR_OPERADOR_PAREJAS(tipoOperador)			\
	{													\
		if(m_pOpParejas) delete m_pOpParejas;			\
		m_pOpParejas=new tipoOperador;					\
	}

//!Macro que define el operador de reproducci�n que se emplear� en el algoritmo gen�tico
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus par�metros iniciales.*/
#define DEFINIR_OPERADOR_REPRODUCCION(tipoOperador)		\
	{													\
		if(m_pOpReproduccion) delete m_pOpReproduccion;	\
		m_pOpReproduccion=new tipoOperador;				\
	}

//!Macro que define los operadores de adaptaci�n que se emplear�n en el algoritmo gen�tico
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus par�metros iniciales.*/
#define ADICIONAR_OPERADOR_ADAPTACION( tipoOperador )	\
	{													\
		OperadorAdaptacion *OA=new tipoOperador;		\
		m_pListaOperadorAdaptacion->Adicionar(OA);		\
	}

//!Macro que define los operadores de finalizaci�n que se emplear�n en el algoritmo gen�tico
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus par�metros iniciales.*/
#define ADICIONAR_OPERADOR_FINALIZACION( tipoOperador )	\
	{													\
		OperadorFinalizacion *OF=new tipoOperador;		\
		m_pListaOperadorFinalizacion->Adicionar(OF);	\
	}

//!Macro que define el operador de mutaci�n que usar� el algoritmo gen�tico para un gen en particular 
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus par�metros iniciales.*/
#define ADICIONAR_OPERADOR_MUTACION( tipoOperador )		\
	{													\
		OperadorMutacion *OM=new tipoOperador;			\
		m_pListaOperadorMutacion->Adicionar(OM);		\
	}

//!Macro que define el operador de cruce que usar� el algoritmo gen�tico para un gen en particular 
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus par�metros iniciales.*/
#define ADICIONAR_OPERADOR_CRUCE( tipoOperador )		\
	{													\
		OperadorCruce *OC=new tipoOperador;				\
		m_pListaOperadorCruce->Adicionar(OC);			\
	}

#endif	//__GENETICO_H
