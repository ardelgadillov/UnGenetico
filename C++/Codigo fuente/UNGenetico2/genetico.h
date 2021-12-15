#ifndef __GENETICO_H
#define __GENETICO_H

#include<stdio.h>
#include<stdlib.h>
#include<iostream.h>
#include<math.h>
#include<float.h>
#include<time.h>

#include "arreglos.h"


/*Sólo incluimos las clases que manejan entorno gráfico si está
definida la constante USAR_VENTANA*/
#if defined(USAR_VENTANA)
#	include "ventana.h"
#endif


/*Para efectos de depuración de la librería*/
#if defined(UN_DEBUG) && defined(USAR_VENTANA)
#	define unASSERT(cond) wxASSERT(cond)
#else 
#	define unASSERT(cond)
#endif

/**************************************************************/
/*				Declaración de constantes					  */
/**************************************************************/

//!Constantes utilizadas por los operadores de adaptación
enum CriteriosDeAdaptacion
{
	ADAPTACION_PROBMUTACION_OFFLINE	=	1,
	ADAPTACION_PROBMUTACION_EXPONENCIAL
};

//!Constantes utilizadas en la definicion de la codificación del individuo.
/*!Se utilizan en la función codificacion(Individuo * pInd ,int estado) de la clase AlgoritmoGenetico*/
enum EstadosIndividuo
{
	ESTADO_CODIFICAR	=	1,
	ESTADO_DECODIFICAR,
	ESTADO_CREAR
};


/************************************************************/
/*					Declaración de clases					*/
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
/*				Declaración de funciones globales			*/
/************************************************************/

long redondear(double);

/*!Comprueba si \a valor se encuentra dentro de un rango determinado y lo
restringe a los límites.
\param valor Valor a restringir
\param min Límite inferior del rango 
\param max Límite superior del rango
\return Valor ubicado dentro de los limites. Si valor < min retorna min. Si valor > max retorna max*/
template<class T> inline T restringir(T valor, T min, T max)
{
	if(valor>max) valor=max;
	else if(valor<min) valor=min;
	return valor;
}


#pragma warning(push, 3)	//Para evitar  "warning(level 4) C4100" ocasionada por parámetros de funciones no referenciados

/************************************************************************/
/*						CLASE Gen										*/
/************************************************************************/

//!Clase abstracta que sirve de base a clases que definen genes de distintos tipos
/*!
Las clases derivadas como GenBool, GenEntero, GenReal, GenArreglo u otras 
que cree el usuario son las que establecen la forma en que se almacena 
y se maneja la información genetica. Estas deben implementar todas las funciones 
puramente virtuales que definen el tratamiento de la información de cada gen. 
Para cada tipo de gen que se cree deben definirse también operadores 
de mutación y cruce cuyas clases deben derivarse de las clases OperadorMutacion 
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
	/*!Debe sobrecargarse en las clases derivadas si se requiere inicializar algún miembro*/
	Gen(){}

	//!Destructor
	/*!Es virtual para poder definirse en las clases derivadas.*/
	virtual ~Gen(){}

	//!Debe especificar la forma de generar de un gen aleatorio
	/*!Debe sobrecargarse en las clases derivadas*/
	virtual void generarAleatorio()=0;

	//!Debe especificar el operador de mutación por defecto para el gen.
	/*!Debe sobrecargarse en las clases derivadas
	return Debe retornar un apuntador a un nuevo objeto de una clase derivada de OperadorMutacion*/
	virtual OperadorMutacion* operadorMutacionDefecto() const =0;

	//!Debe especificar el operador de cruce por defecto para el gen.
	/*!Debe sobrecargarse en las clases derivadas
	return Debe retornar un apuntador a un nuevo objeto de una clase derivada de OperadorCruce*/
	virtual OperadorCruce* operadorCruceDefecto() const =0;

	//!Debe generar una copia (creada con el operador \a new) del objeto Gen actual
	/*!Debe sobrecargarse en las clases derivadas
	\return Apuntador a un nuevo objeto de la clase derivada, con las mismas características que el actual*/
	virtual Gen *crearCopia() const =0;

	//!Debe especificar la asignación a partir de otro objeto Gen
	/*!Debe sobrecargarse en las clases derivadas.
	Esta función se podría reemplazar por el operador de asignación (=) si Gen no 
	fuera una clase abstracta.*/
	virtual void copiar(const Gen& otro)=0;
};


/************************************************************************/
/*						CLASE Individuo									*/
/************************************************************************/

//!Clase que administra la información genética de un individuo
/*!Un objeto de la clase Individuo está conformado por un arreglo de apuntadores 
a objetos de clases derivadas de Gen. Además cuenta con funciones que 
le proporcionan las características básicas necesarias para el algortimo genético.*/
class Individuo
{
public:
	//!Constructor por defecto
	Individuo(AlgoritmoGenetico *pAG);

	//!Constructor por copia
	Individuo(const Individuo& origen);

	//Operador de asignación a partir de otro objeto Individuo 
	const Individuo& operator = (const Individuo& origen);

	//!Destructor virtual
	virtual ~Individuo();

	//!Retorna el número de genes presentes en el genoma del individuo
	/*!return Tamaño del genoma*/
	int getTamGenoma() const 
	{
		return m_pGenoma->getSize();
	}

	//!Devuelve el gen ubicado en una posición determinada del genoma. 
	/*!
	\param pos Posición del gen a obtener
	\return referencia al gen ubicado en la posición \a pos. Este no podrá ser modificado.*/
	const Gen& getGen(int pos) const
	{		
		return m_pGenoma->getObj(pos);
	}
	
	//!Adiciona un gen al final del genoma del individuo. 
	/*!
	\param pGen Aputador al gen a adicionar al genoma.
	\return Posición en la que fue adicionado el gen.*/
	int adicionarGen(Gen* pGen)
	{
		m_objetivoActualizado = false;
		return m_pGenoma->Adicionar(pGen);
	}
	
	//!Remplaza el gen ubicado en una posición determinada del genoma por otro gen
	/*! 
	\param pNuevoGen Apuntador al gen que remplazará al gen ubicado en la posición especificada.
	\param pos Posición del gen a remplazar.
	\return Apuntador al gen reemplazado.*/
	Gen* remplazarGen(Gen* pNuevoGen, int pos)
	{
		m_objetivoActualizado = false;
		return m_pGenoma->remplazar(pNuevoGen, pos);		
	}
	
	//!Cambia el algoritmo genetico al que pertenece el individuo
	/*!\param pAG Apuntador al objeto AlgoritmoGenético del que hará parte el individuo.*/
	void SetAG(AlgoritmoGenetico *pAG)
	{
		m_pAG = pAG;
		m_objetivoActualizado = false;	//el nuevo algoritmo podría tener una función objetivo diferente.
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

	//!Ordena al individuo que pase por el proceso de mutación
	void mutar();
	
	//!Retorna la probabilidad de supervivencia del individuo
	/*!\return Probabilidad de supervivencia del individuo*/
	double getProbabilidad() const
	{
		return m_Probabilidad;
	}

	//!Retorna un apuntador al individuo con el que se efectúa el cruce
	/*!\return Apuntador al individuo con el que se efectúa el cruce*/
	Individuo* getPareja() const
	{
		return m_pPareja;
	}

	//!Asigna el apuntador al individuo con el que efectuará el cruce 
	/*!\param pPareja Apuntador al individuo con el que efectuará el cruce.*/
	void asignarPareja(Individuo* pPareja)
	{
		m_pPareja = pPareja;
	}

protected:
	
	//!Arreglo de apuntadores a objetos de las clases derivadas de Gen que contienen la información genética del individuo
	Arreglo<Gen> *m_pGenoma;

	//!Apuntador al algoritmo genetico al que pertenece el individuo
	AlgoritmoGenetico* m_pAG;

	//!Almacena la probabilidad de supervivencia del individuo 
	double m_Probabilidad;

	//!Apuntador a la pareja con la que el individuo efectuará el cruce de sus genes
	Individuo *m_pPareja;


private:
	//!Indica si \a m_Objetivo refleja el valor actual de la función objetivo. 
	/*!Se establece a \a false después de cualquier cambio 
	en el genoma para recalcular la funcion objetivo cuando es invocado el método
	\a objetivo().*/
	bool m_objetivoActualizado;

	//!Almacena el valor de la función objetivo
	double m_Objetivo;
};


/************************************************************************/
/*						CLASE Poblacion									*/
/************************************************************************/

//!Clase que administra la información de los individuos de un algoritmo genético
/*!Un objeto de la clase Poblacion contiene un arreglo de apuntadores a objetos de 
la clase Individuo. Además contiene los métodos necesarios para realizar 
las operaciones sobre la población en cada iteración del algoritmo genético*/
class Poblacion
{
public:
	//!Constructor por defecto
	Poblacion(AlgoritmoGenetico* pAG, int nIndividuos, bool crearAleatorios=true);

	//!Constructor por copia de otra poblacion. 
	Poblacion(const Poblacion& origen); 
	
	//!Destructor
	virtual ~Poblacion();
	
	//!Operador de asignación a partir de otra población
	const Poblacion& operator = (const Poblacion& origen);
	
	//!Retorna un apuntador al objeto de la clase AlgoritmoGenetico al que pertenece la población
	/*!\return Apuntador al objeto de la clase AlgoritmoGenetico al que pertenece la población*/
	AlgoritmoGenetico* GetAG() const
	{
		return m_pAG;
	}

	//!Ordena los miembros de la población segun su función de evaluación
	void ordenar(bool Maximizar=false);
	
	//!Cambia el algoritmo genetico al que pertenece la población
	/*!\param Ag Apuntador al objeto AlgoritmoGenético del que hará parte la poblacion.*/
	void SetAG(AlgoritmoGenetico *Ag)
	{
		m_pAG=Ag;
	}
	
	//!Devuelve el individuo ubicado en una posición determinada de la población. 
	/*!
	\param pos Posición del individuo a obtener
	\return referencia al individuo ubicado en la posición \a pos.*/
	Individuo &getIndividuo(int pos) const
	{		
		return m_pGeneracion->getObj(pos);
	}

	//!Inserta un individuo en una posición determinada de la población. 
	int InsertarIndividuo(Individuo* ind, int indice=-1);
	
	//!Remplaza el individuo ubicado en una posición determinada de la población por otro individuo
	/*! 
	\param pNuevoInd Apuntador al idividuo que remplazará al individuo ubicado en la posición especificada.
	\param pos Posición del individuo a remplazar.
	\return Apuntador al individuo reemplazado.*/
	Individuo* remplazarIndividuo(Individuo* pNuevoInd, int pos)
	{		
		return m_pGeneracion->remplazar(pNuevoInd, pos);		
	}

	//!Retorna el número de individuos presentes en la población
	/*!return Tamaño de la población*/
	int getTam() const
	{
		return	m_pGeneracion->getSize();
	}

	//!Cambia el tamaño de la población 
	int setTam(int nuevoTam, bool remplazar=false, bool crearAleatorios=false);

	//!Ordena a la población que pase por el proceso de mutación
	void mutar();

protected:
	//!Apuntador al objeto de la clase AlgoritmoGenetico al que pertenece la poblacion
	AlgoritmoGenetico *m_pAG;

	//!Arreglo de objetos de la clase Individuo que conforman la población
	Arreglo<Individuo> *m_pGeneracion;
}; 


/************************************************************************/
/*						CLASE AlgoritmoGenetico							*/
/************************************************************************/

//!Clase abstracta que administra el proceso de un algoritmo genético
/*!Un objeto de la clase AlgoritmoGenetico contiene una población, los operadores
genéticos y todas las características necesarias para llevar a cabo el 
algoritmo genético y permitir la interacción entre éste y el usuario.
Para utilizarla deben definirse las funciones puramente virtuales \a codificacion(Individuo * pInd ,int estado)
y \a objetivo(). Para realizar todo el proceso del algoritmo genético en un solo 
paso debe invocarse la función \a optimizar().
Pueden cambiarse los parámetros y operadores por defecto sobrecargando las funciones
\a inicializarParametros() y \a definirOperadores()
\nosubgrouping*/
class AlgoritmoGenetico
{
 
public:

#if defined(USAR_VENTANA)

	//!Constructor cuando se usa el entorno gráfico (Debe estar definida la constante USAR_VENTANA)
	AlgoritmoGenetico(AGFrame *m_pFrame);

	//!Informa al algoritmo un cambio de ventana. Se define solo cuando se usa el entorno gráfico (Debe estar definida la constante USAR_VENTANA)
	void SetFrame(AGFrame *pFrame);

	//!Apuntador a la ventana de la aplicación al utilizar entorno gráfico (Debe estar definida la constante USAR_VENTANA)
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

	//!En esta función deben definirse los tres estados de codificación de un individuo
	/*!Debe sobrecargarse en las clases derivadas.
	Esta función es invocada definiendo el estado en que se encuentra el individuo.
	Cada individuo invoca esta función cuando es necesario realizar su decodificación. 
	También es invocada por \a iniciarOptimizacion() para la creación del individuo 
	modelo. El usuario solamente debe invocarla cuando requiere hacer actualización 
	explícita de las variables del sistema o de la información genética de algún 
	individuo.
	Existen tres estados: creación, codificación y decodificación, que se 
	determinan mediante el parámetro \a estado, el cual puede tomar los siguientes 
	valores: ESTADO_CREACION, ESTADO_CODIFICACION, y ESTADO_DECODIFICACION. 
	<ul>
	<li>ESTADO_CREACION: En el momento de su construcción, se define la forma
	que adoptará el individuo, determinada por la constitución del genoma, 
	es decir, el número y tipos de genes que contendrá.
	<li>ESTADO_CODIFICACION: Define la forma en que se traducen las variables 
	del modelo en información genética del individuo. Estas variables deben estar
	definidas en la clase derivada.
	<li>ESTADO_DECODIFICACION: Define la forma en que se traduce la información 
	genética del individuo en las variables del modelo. Estas variables deben estar
	definidas en la clase derivada.
	</ul>
	En la implementación de esta función, pueden utilizarse las MACROS 
	que facilitan la adición de genes al individuo, y que proveen los tres 
	estados en una sola instrucción. (Ver <a href="globals_defs.html">Definiciones</a> ).
	\param pInd Apuntador al individuo en proceso de codificación.
	\param estado Estado en que se encuentra el individuo. */
	virtual void codificacion(Individuo * pInd ,int estado)=0;
	
	//!En esta función debe definirse la forma de calcular la función de evaluación
	/*!Debe sobrecargarse en las clases derivadas.
	La función objetivo se calcula haciendo uso de las variables del modelo, 
	definidas en la clase derivada. El usuario no debe invocar esta función explícitamente.
	Esta función es invocada internamente por cada individuo para calcular su función 
	objetivo despues de haber invocado la función codificacion(Individuo * pInd ,int estado) 
	la cual actualiza las variables del sistema. 
	\return Valor de la función de evaluación para las variables actuales del sistema.*/
	virtual double objetivo()=0;

	//!Ejecuta todo el proceso de optimización
	virtual void optimizar();

	//!Prepara el algoritmo genético para su ejecución
	void iniciarOptimizacion();

	//!Efectúa la siguiente iteración del algoritmo
	void iterarOptimizacion();

	//!Indica si el algoritmo debe finalizar
	bool finalizar();

	//!Finaliza el algoritmo genético
	void finalizarOptimizacion();

	//!Presenta en pantalla los resultados intermedios del algoritmo genético
	void mostrarMedidas();

	//!Almacena en el archivo de salida los resultados intermedios del algoritmo genético
	void salvar();

protected:
	
	//!Inicializa las variables y parámetros del algoritmo
	void inicializarVariables();

	//!Inicializa los apuntadores en posiciones nulas
	void inicializarApuntadores();

	//!Crea los operadores a utilizar por el algoritmo
	void crearOperadores();

	//!Efectúa la asignación de probabilidad de supervivencia a los individuos de la población	
	void asignarProbabilidad();	

	//!Efectúa la selección de los individuos que sobreviven para la nueva generación	
	void seleccionar();

	//!Efectúa la asignación de parejas de individuos para el cruce
	void asignarParejas();
	
	//!Ejecuta la estrategia general de reproducción
	void reproducir();

	//!Efectúa la mutación de la población actual
	void mutar();

	//!Efectúa la adaptación de parámetros del algoritmo
	void adaptacion();

	//!Calcula los valores intermedios de las medidas de desempeño del algoritmo genético
	void actualizarMedidas();


private:

	//!Esta función puede sobregargarse para cambiar los valores por defecto del algoritmo
	/*!Es invocada por inicializarVariables()*/		
	virtual void inicializarParametros(){};
	
	//!Esta función puede sobregargarse para definir operadores diferentes a los establecidos por defecto.
	//*Es invocada por \a crearOperadores()*/	
	virtual void definirOperadores(){};


public:


//! @name Apuntadores
//@{

	//APUNTADORES
	
	//!Apuntador a la poblacion del algoritmo genético
	Poblacion *m_pPoblacionActual;
	//!Apuntador al mejor individuo de la generación actual
	Individuo *m_pMejorEnEstaGeneracion;
	//!Apuntador al peor individuo de la generación actual
	Individuo *m_pPeorEnEstaGeneracion;
	//!Apuntador al mejor individuo de la historia del algoritmo
	Individuo *m_pMejorEnLaHistoria;
	//!Apuntador al Individuo que se emplea como modelo para producir copias de él.
	Individuo *m_pModelo;

	//!Arreglo de operadores de mutación
	/*!El tipo del operador en cada posición del arreglo debe coincidir con 
	el tipo de gen en la misma posición del genoma del individuo modelo*/
	Arreglo<OperadorMutacion> *m_pListaOperadorMutacion;

	//!Arreglo de operadores de cruce
	/*!El tipo del operador en cada posición del arreglo debe coincidir con 
	el tipo de gen en la misma posición del genoma del individuo modelo*/	
	Arreglo<OperadorCruce> *m_pListaOperadorCruce;

protected:
	//!Apuntador al operador de asignación de probabilidad de supervivencia
	OperadorProbabilidad *m_pOpProbabilidad;
	//!Apuntador al operador de selección de individuos
	OperadorSeleccion *m_pOpSeleccion;
	//!Apuntador al operador de asignación de parejas de cruce 
	OperadorParejas *m_pOpParejas;
	//!Apuntador al operador de reproducción
	OperadorReproduccion *m_pOpReproduccion;
	//!Arreglo de operadores de adaptación del algoritmo	
	Arreglo<OperadorAdaptacion> *m_pListaOperadorAdaptacion;
	//!Arreglo de operadores de finalizacion
	Arreglo<OperadorFinalizacion> *m_pListaOperadorFinalizacion;

//@}
public:

//! @name Parámetros del algoritmo 
//@{

	//PARÁMETROS DEL ALGORITMO

	//!Número máximo de generaciones
	long m_GeneracionMaxima;
	//!Intervalo de iteraciones en que deben salvarse y/o mostrarse los valores intermedios
	long m_IntervaloSalvar;
	//!Nombre del archivo en que se guardan los valores intermedios de las iteraciones
	char m_NombreArchivo[400];
	
//@}

//! @name Indicadores
//@{

    //INDICADORES//

	//!Indica si se va a utilizar alguna estrategia de adaptación en el algoritmo genético
	bool m_IndicadorUsarAdaptacion;
	//!Indica si al inicializar la poblacion se crean individuos aleatorios o con los valores iniciales especificados
	bool m_IndicadorInicializarPoblacionAleatoria;
	//!Indica si debe maximizarse o minimizarse la función objetivo
	bool m_IndicadorMaximizar;
	//!Indica si deben salvarse los resultados intermedios de las iteraciones del algoritmo genético
	bool m_IndicadorArchivo;
	//!Indica si deben mostrarse en pantalla los resultados intermedios de las iteraciones del algoritmo genético
	bool m_IndicadorMostrar;
	//!Indica si se debe salvar y/o mostrar la función objetivo del mejor individuo en la historia
	bool m_IndicadorMostrarMejorEnHistoria;
	//!Indica si se debe salvar y/o mostrar la generación en que apareció el mejor individuo en la historia
	bool m_IndicadorMostrarGeneracionMejorHistorico;
	//!Indica si se debe salvar y/o mostrar la función objetivo del mejor individuo de la generación actual
	bool m_IndicadorMostrarMejorEnGeneracion;
	//!Indica si se debe salvar y/o mostrar la función objetivo del peor individuo de la generación actual
	bool m_IndicadorMostrarPeorEnGeneracion;
	//!Indica si se debe salvar y/o mostrar el promedio de las funciones objetivo de la generación actual
	bool m_IndicadorMostrarMedia;
	//!Indica si se debe salvar y/o mostrar la desviación estándar de las funciones objetivo de la generación actual
	bool m_IndicadorMostrarDesviacion;
	//!Indica si se debe salvar y/o mostrar la medida OnLine para la generación actual
	bool m_IndicadorMostrarOnLine;
	//!Indica si se debe salvar y/o mostrar  la medida OffLine para la generación actual
	bool m_IndicadorMostrarOffLine;

//@}

//! @name Valores propios de cada iteración
//@{

	//VALORES PROPIOS DE CADA ITERACION

	//!Número de individuos de la generación actual
	int m_TamanoPoblacion;
	//!Número de generación actual
	long m_Generacion;
	//!Número de generación en la que apareció el mejor individuo de la historia del algoritmo
	long m_GeneracionDelMejorEnLaHistoria;
	//!Promedio aritmético de las funciones objetivo de los individuos de la generación actual
	double m_Media;
	//!Promedio aritmético acumulado de las funciones objetivo de todos los individuos que han existido en la historia del algoritmo.
	double m_MedidaOnLine;
	//!Promedio aritmético acumulado de las funciones objetivo de los mejores individuos de cada generación en la historia del algoritmo.
	double m_MedidaOffLine;
	//!Medida OnLine de la generación anterior
	double m_MedidaOnLineAnterior;
	//!Medida OffLine de la generación anterior
	double m_MedidaOffLineAnterior;
	//!Desviación estándar de las funciones objetivo de los individuos presentes en la generación actual.
	double m_Desviacion;

//@}
};


/************************************************************************************/
/*								OPERADORES BÁSICOS									*/					
/************************************************************************************/


//!Clase abstracta que define el proceso de adaptación del algoritmo genético
/*!El proceso de adaptación hace que se modifiquen los parámetros del algoritmo 
genético durante su ejecución. Un objeto de una clase derivada de OperadorAdaptacion 
opera sobre el algoritmo genético realizando cambios en su estructura funcional con el fin 
de variar su desempeño.*/
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
	\param pAG Apuntador al algoritmo genético sobre el que opera*/
	virtual void adaptacion(AlgoritmoGenetico *pAG)=0;
};


//!Clase abstracta que administra el proceso de mutación en el algoritmo genético
/*!El proceso de mutación consiste en alterar la información genética 
de un individuo. Un objeto de una clase derivada de OperadorMutacion
opera sobre un objeto de una clase derivada de Gen, por lo tanto, los dos objetos deben
manejar el mismo tipo de dato.*/
class OperadorMutacion
{
public:
	//!Constructor por defecto
	/*!Inicializa la probabilidad de mutación para cada el objeto derivado de Gen 
	sobre el que opera.
	\param ProbabilidadMutacion Probabilidad de mutación del gen sobre el que opera. Por defecto es igual a 0.1*/
	OperadorMutacion(double ProbabilidadMutacion=0.1)
	{
		m_ProbabilidadMutacion = ProbabilidadMutacion;
	}

	//!Destructor
	/*!Es virutal para poder definirse en las clases derivadas.*/
	virtual ~OperadorMutacion(){}

	//!Decide si debe realizarse el proceso de mutación sobre un gen
	/*!La selección de los genes que deben mutar se realiza aleatoriamente
	teniendo en cuenta la probabilidad de mutación de cada gen.
	\param pGen Apuntador al objeto derivado de Gen sobre el que se toma la decisión.*/
	virtual void mutar(Gen *pGen)
	{
		double azar= (double)rand()/(double)RAND_MAX;
		if(azar<m_ProbabilidadMutacion)	mutarGen(pGen);
	}

	//!Retorna la probabilidad de mutación del gen sobre el que opera
	/*!\return Probabilidad de mutación del gen*/
	double ObtenerProbabilidadMutacion() const
	{
		return m_ProbabilidadMutacion;
	}

	//!Asigna un nueva probababilidad de mutación al gen sobre el que opera
	/*!\param Probabilidad Probabilidad de mutación a asignar al gen.
	\return Probabilidad de mutación asignada al gen.*/
	virtual double AsignarProbabilidadMutacion(double Probabilidad)
	{
		return (m_ProbabilidadMutacion = restringir(Probabilidad, 0.0, 1.0));
	}

protected:
	//!Almacena la probabilidad de mutación del gen sobre el que opera
	double m_ProbabilidadMutacion;

private:
	//!Efectúa el procedimiento de mutación sobre un gen
	/*!Debe sobrecargarse en las clases derivadas
	\param pGen Apuntador al gen sobre el que opera*/
	virtual void mutarGen(Gen *pGen)=0;
};


//!Clase abstracta que administra el proceso de cruce entre los individuos del algoritmo genético
/*!El proceso de cruce consiste en intercambiar la información genética de dos individuos
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

	//!Método virtual que ejecuta el procedimiento de cruce entre dos indivduos
	/*!Debe sobrecargarse en las clases derivadas.
	\param pMejor Apuntador al gen padre con la mejor función de evaluación.
	\param pPeor Apuntador al gen padre con la peor función de evaluación.
	\param pHijos Arreglo de genes al cual se adicionan los genes hijos.
	\param numHijos Numero de genes hijos a crear en el cruce.
	\param indice Valor que identifica al individuo de cuyos genes se realiza el cruce. No es utilizado en todas las clases derivadas*/
	virtual void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos,int numHijos, int indice)=0;
};


//!Clase abstracta que administra la asignación de la probabilidad de supervivencia a los individuos de la población 
/*!Generalmente, la probabilidad de superviviencia se asigna con base en la función de 
evaluación de cada individuo.*/
class OperadorProbabilidad
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse  en las clases derivadas*/
	OperadorProbabilidad(){}

	//!Destructor
	/*!Es virtual para poder definirse en las clases derivadas*/
	virtual ~OperadorProbabilidad(){}

	//!Asigna una probabilidad de supervivencia a los individuos de la población
	/*!Debe sobrecargarse en las clases derivadas
	\param Pob Referencia a la población sobre la que opera.
	\param Maximizar Si si valor es \a true a los individuos se les asigan probabilidades de supervivencia directamente proporcionales a su función objetivo, de lo contrario se asignan probabiliades inversamente proporcionales.*/
	virtual void asignarProbabilidad(Poblacion &Pob, bool Maximizar)=0;
};


//!Clase abstracta que administra el proceso de selección de individuos en la población
/*!Un objeto de una clase derivada de OperadorSeleccion selecciona un conjunto de individuos
presentes en la generación actual que servirán de base para formar la nueva generación del
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

	//!Efectúa el proceso de selección en la población
	/*!Debe sobrecargarse en las clases derivadas 
	\param pPob Referencia a la población sobre la que opera.*/
	virtual void seleccionar(Poblacion& pPob)=0;
};


//!Clase abstracta que administra la asignación de parejas para cada individuo de la población
/*!Para cada individuo presente en la población se asigna otro individuo que servirá 
de pareja para intercambiar su información genética.*/
class OperadorParejas
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas*/
	OperadorParejas(){}

	//!Destructor
	/*!Es virtual para poder defirnirse en las clases derivadas*/
	virtual ~OperadorParejas(){}

	//!Asigna las parejas a los individuos de la población
	/*!Debe sobrecargarse en las clases derivadas. 
	\param Pob Referencia a la población sobre la que opera.*/
	virtual void asignarParejas(Poblacion& Pob)=0;
};

//!Clase abstracta que define la estrategia general de reproducción del algoritmo
/*!La estrategia general de reproducción define cuáles individuos conformarán la
siguiente generación del algoritmo.*/
class OperadorReproduccion
{
public:
	//!Constructor por defecto
	/*!Debe sobrecargarse en las clases derivadas.*/
	OperadorReproduccion(){}

	//!Destructor
	/*!Es virtual para poder definirse en las clases derivadas.*/
	virtual ~OperadorReproduccion(){}

	//!Ejecuta la estrategia general reproducción para una población
	/*!Debe sobrecargarse en las clases derivadas.
	\param Pob Referencia a la población sobre la que opera.
	\param Maximizar Su valor sirve como referencia para determinar qué individuo es mejor a otro dependiendo de su función de evaluación.*/
	virtual void reproducir(Poblacion& Pob, bool Maximizar)=0;
};


//!Clase abstracta que define la estrategia de finalización del algoritmo genético
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
/*!El elitismo consiste en obtener el mejor individuo de la generación actual y 
copiarlo en la siguiente generación.*/
class OperadorAdaptacionElitismo:public OperadorAdaptacion
{
public:
	//!Constructor 
	OperadorAdaptacionElitismo(){};

	//!Destructor
	~OperadorAdaptacionElitismo(){}

	//!Método que efectúa el proceso de elitismo
	/*!\param pAG Apuntador al algoritmo genético sobre el cual opera.*/
	void adaptacion(AlgoritmoGenetico *pAG);
};


//!Clase derivada de la clase OperadorAdaptacion que define la estrategia de adaptación para la probabilidad de mutación de los genes de cada individuo de la población
/*!Existen dos métodos de adaptación que varían la probabilidad de mutación: 
<ul>
<li>Variación escalonada: aumenta la probabilidad de mutación de forma gradual y escalonada para los genes 
cuando la variación de la medida Offline permanece en un rango establecido durante un número determinado de 
generaciones consecutivas.
<li>Variación exponencial: varía la probabilidad de mutación de todos los genes siguiendo un comportamiento 
exponencial decreciente determinado por \f$Pm(t)=P_{max} e^{\frac{-t}{T}}\f$
donde \a Pmax es el valor máximo que puede tomar la probabiliad de mutación de un gen, \a t corresponde 
al número de la generación actual y \a T es un valor positivo preestablecido. 
</ul>*/
class OperadorAdaptacionProbMutacion:public OperadorAdaptacion
{
public:
	//!Constructor 
	OperadorAdaptacionProbMutacion(AlgoritmoGenetico *pAG, int TipoAdaptacion=ADAPTACION_PROBMUTACION_EXPONENCIAL);
	
	//!Destructor
	~OperadorAdaptacionProbMutacion(){}

	//!Efectúa el proceso de adaptacion de variación de la probabilidad de mutación
	void adaptacion(AlgoritmoGenetico *pAG);
	
	//!Cambia los parámetros para la ADAPTACION_PROBMUTACION_OFFLINE
	void setParamsOffline(double MaxProb=0.5, double FactorVariacion=0.001, double Escalon = 0.1, int MaxCont = 10);
	//!Cambia los parámetros para la ADAPTACION_PROBMUTACION_EXPONENCIAL
	void setParamsExponencial(double MaxProb=0.5, double T=-1);

protected:

	//!Determina la forma de cambiar la probabilidad de mutación
	int m_tipoAdaptacion;
	//!Determina la máxima probabilidad de mutación en un gen
	double m_maxProb;
	//!Determina el número máximo de generaciones que pueden pasar sin que varíe la probabilidad de mutación cuando la medida offline no varía en un factor mayor a \a m_factorVariacion. Sólo se utiliza en la ADAPTACION_PROBMUTACION_OFFLINE
	int	m_maxCont;
	//!Determina el factor de variación para la medida Offline. Sólo se utiliza en la ADAPTACION_PROBMUTACION_OFFLINE
	double m_factorVariacion;
	//!Determina la cantidad que se sumará a la probabilidad de mutación en cada variación. Sólo se utiliza en la ADAPTACION_PROBMUTACION_OFFLINE
	double m_escalon;
	//!Valor positivo determina la atenuación de la curva exponencial. Sólo se utiliza en la ADAPTACION_PROBMUTACION_EXPONENCIAL
	double m_T;
};


//!Clase derivada de la clase OperadorAdaptacion que hace una variación del número de individuos del algoritmo genético
/*!La primera generación del algoritmo contiene un número máximo de individuos establecido, este número 
disminuye linealmente hasta alcanzar un número mínimo de individuos predefinido. Este límite se alcanza 
cuando se llega a la última generación establecida para el algoritmo.*/
class OperadorAdaptacionNumIndividuos:public OperadorAdaptacion
{
public:
	//!Constructor 
	OperadorAdaptacionNumIndividuos(AlgoritmoGenetico *pAG, int NumIndivInicio=0, int NumIndivFin=0);

	//!Destructor
	~OperadorAdaptacionNumIndividuos(){}

	//!Método para efectuar el proceso de adaptación de número de individuos 
	void adaptacion(AlgoritmoGenetico *pAG);
protected:
	
	//!Indica el número inicial de individuos en la población 
	int m_nIndivInicio;
	//!Indica el número final de individuos en la población
	int m_nIndivFin;
};


//!Clase derivada de la clase OperadorProbabilidad que define el proceso de asignación de probabilidad de supervivencia proporcional a cada individuo de la población
/*!En una población de \a N individuos, se asigna una probabilidad de supervivencia a cada individuo proporcional al valor de su 
función objetivo \a FObj(i), así:
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
	//!Efectúa el proceso de asignación de probabilidad de supervivencia proporcional a cada individuo de la población
	void asignarProbabilidad(Poblacion& Pob, bool Maximizar);
};

//!Clase derivada de la clase OperadorProbabilidad que define el proceso de asignación de probabilidad de supervivencia lineal
/*!En una población de \a N individuos, se ordenan de mejor a peor y se asigna a cada uno una probabilidad de supervivencia
que depende de su posición en la población, así:

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
	//!Cambia el valor del parámetro \a Nmin comprobando los límites
	/*!\param Nmin Valor a asignar al parámetro \a Nmin*/
	void setNmin(double Nmin)
	{
		m_nmin = restringir(Nmin, 0.0, 2.0);
	}
	//!Retorna el valor del parámetro \a Nmin
	/*!\return Valor del parámetro \a Nmin */
	double getNmin() const
	{
		return m_nmin;
	}
	
	//!Destructor 
	~OperadorProbabilidadLineal(){}
	
	//!Efectúa el proceso de asignación de probabilidad de supervivencia lineal
	void asignarProbabilidad(Poblacion &Pob, bool Maximizar);

protected:	
	/*!Valor que pondera el grado de proporcionalidad*/
	double m_nmin;
};


//!Clase derivada de la clase OperadorProbabilidad que define el proceso de asignación de probabilidad de supervivencia homogénea
/*!En una población de \a N individuos, se asigna la misma probabilidad de supervivencia a todos ellos, dada por:
\f[P(i)=\frac{1}{N}\f]*/
class OperadorProbabilidadHomogenea:public OperadorProbabilidad
{
public:
	//!Constructor 
	OperadorProbabilidadHomogenea(){}
	//!Destructor 
	~OperadorProbabilidadHomogenea(){}
	//!Efectúa el proceso de asignación de probabilidad de supervivencia homogénea 
	void asignarProbabilidad(Poblacion &Pob, bool Maximizar);
};



//!Clase derivada de la clase OperadorSeleccion que define el proceso de selección estocástica con reemplazo de los individuos
/*!Los individuos que componen la población son ordenados como segmentos de una línea, cada segmento 
corresponde a la probabilidad de supervivencia del individuo. La selección se produce al generarse un valor aleatorio
que coincida con el segmento del individuo. El proceso se repite hasta obtener el número de
individuos deseados, creando así la base para la nueva población.*/
class OperadorSeleccionEstocasticaRemplazo:public OperadorSeleccion
{
public:
	//!Constructor
	OperadorSeleccionEstocasticaRemplazo(){}
	//!Destructor 
	~OperadorSeleccionEstocasticaRemplazo(){}
	//!Ejecuta el proceso de selección estocástica con reemplazo de los individuos de la plobación
	void seleccionar(Poblacion& pPob);
};

//!Clase derivada de OperadorParejas que define la asignación de parejas aleatorias para los individuos de la población
/*!A cada individuo de la población se le asigna otro individuo que corresponderá a su pareja en el proceso de reproducción. 
La asignación se realiza aleatoriamente teniendo en cuenta que no deben asignarse individuos que ya tengan
pareja establecida y que un individuo no puede ser asignado como su propia pareja. Cuando el numero de individuos 
es impar, a un individuo de la población no se le asigna pareja.*/
class OperadorParejasAleatorias:public OperadorParejas
{
public:
	//!Constructor 
	OperadorParejasAleatorias(){}
	//!Destructor
	~OperadorParejasAleatorias(){}
	//!Asigna parejas aleatoriamente para los individuos de la población
	void asignarParejas(Poblacion& Pob);
};


//!Clase derivada de la clase OperadorParejas que define la asignación de parejas adyacentes para los individuos de la población
/*!A cada individuo de la población se le asigna otro individuo que corresponderá a su pareja en el proceso de reproducción. 
Al individuo \a i de la población se le asiga como pareja el individuo <i>i+1</i>, el proceso continúa con los individuos
a los que aún no se les ha asignado pareja. Cuando el numero de individuos es impar, 
al último individuo de la población no se le asigna una pareja.*/
class OperadorParejasAdyacentes:public OperadorParejas
{
public:
	//!Constructor 
	OperadorParejasAdyacentes(){}
	//!Destructor 
	~OperadorParejasAdyacentes(){}
	//!Asigna parejas adyacentes para los individuos de la población 
	void asignarParejas(Poblacion& Pob);
};

//!Clase derivada de la clase OperadorParejas que define la asignación de parejas  extremas para los individuos de la población
/*!
A cada individuo de la población se le asigna otro individuo que corresponderá a su pareja en el proceso de reproducción. 
Al primer individuo del arreglo de la población se le asigna como pareja el último individuo del arreglo, al segundo individuo se 
le asigna el penúltimo. El proceso continúa sucesivamente hasta asignar parejas a todos los individuos de la generación. 
Cuando el numero de individuos es impar, al individuo ubicado en el centro de la población no se le asigna pareja.*/
class OperadorParejasExtremos:public OperadorParejas
{
public:
	//!Constructor 
	OperadorParejasExtremos(){}
	//!Destructor 
	~OperadorParejasExtremos(){}
	//!Asigna parejas extremas para los individuos de la población 
	void asignarParejas(Poblacion& Pob);
};


//!Clase derivada de la clase OperadorReproduccion que define el cruce simple entre dos individuos de la población
/*!El cruce simple genera dos individuos hijos por cada cruce entre dos individuos padres intercambiando los genes
respectivos en un punto de cruce. 
El punto de cruce se selecciona aleatoriamente para cada pareja en el intervalo [1, n-1] donde \a n
corresponde al número de genes en el genoma. A partir del gen siguiente al punto de cruce hasta 
el final del genoma se intercambian los genes del individuo padre con los de la madre, estos nuevos individuos 
corresponden a los dos hijos generados que reemplzarán a sus padres en la siguiente generación del algoritmo.*/
class OperadorReproduccionCruceSimple:public OperadorReproduccion
{
public:
	//!Constructor 
	OperadorReproduccionCruceSimple(){}
	//!Destructor 
	~OperadorReproduccionCruceSimple(){}
	//!Ejecuta el cruce simple entre dos individuos de la población 
	void reproducir(Poblacion& Pob, bool Maximizar);
};


//!Clase derivada de la clase OperadorReproduccion que define la estrategia de reproducción dos padres dos hijos
/*!Por cada pareja de individuos que efectúen el cruce se crean dos hijos que 
reemplazarán a sus padres en la siguiente iteración del algoritmo*/
class OperadorReproduccionDosPadresDosHijos:public OperadorReproduccion
{
public:
	//!Constructor 
	OperadorReproduccionDosPadresDosHijos(){}
	//!Destructor de la clase OperadorReproduccionDosPadresDosHijos
	~OperadorReproduccionDosPadresDosHijos(){}
	//!Ejecuta la estrategia de reproducción dos padres dos hijos
	void reproducir(Poblacion& Pob, bool Maximizar);
};

//!Clase derivada de la clase OperadorReproduccion que define la estrategia de reproducción mejor padre mejor hijo
/*!Por cada pareja de individuos que efectúen el cruce se crean 
dos hijos. El mejor individuo hijo reemplazará al peor individuo 
padre en la siguiente iteración del algoritmo*/
class OperadorReproduccionMejorPadreMejorHijo:public OperadorReproduccion
{
public:
	//!Constructor 
	OperadorReproduccionMejorPadreMejorHijo(){}
	//!Destructor 
	~OperadorReproduccionMejorPadreMejorHijo(){}
	//!Ejecuta la estrategia de reproducción mejor padre mejor hijo
	void reproducir(Poblacion& Pob, bool Maximizar);
};


//!Clase derivada de la clase OperadorReproduccion que define la estrategia de reproducción mejores entre padres e hijos
/*!Por cada pareja de individuos que efectúen el cruce se crean 
dos hijos. Los individuos que conformarán la siguiente generación 
serán los dos que tengan la mejor función de evaluación entre los cuatro.*/
class OperadorReproduccionMejoresEntrePadresEHijos:public OperadorReproduccion
{
public:
	//!Constructor 
	OperadorReproduccionMejoresEntrePadresEHijos(){}
	//!Destructor 
	~OperadorReproduccionMejoresEntrePadresEHijos(){}
	//!Ejecuta la estrategia de reproducción mejores entre padres e hijos
	void reproducir(Poblacion& Pob, bool Maximizar);
};



//!Clase derivada de la clase OperadorFinalizacion que define la finalización del algoritmo basándose en su medida online
/*!Ordena la interrupción del algoritmo genético cuando éste no presenta una variación significativa en su
medida online después de un número determinado de generaciones.*/
class OperadorFinalizacionOnline:public OperadorFinalizacion
{
public:
	//!Constructor
	/*!Inicializa los valores de los parámetros \a FactorVariacion y \a MaxCont
	\param FactorVariacion 	Determina el factor de variación para la medida online. Por defecto es igual a 0.001.
	\param MaxCont Determina el número máximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida online no varía en un factor mayor a \a FactorVariacion. Por defecto es igual a 30.*/
	OperadorFinalizacionOnline(double FactorVariacion=0.001, int MaxCont=30)
	{
		setParams(FactorVariacion, MaxCont);
	}	

	//!Cambia el valor de los parámetros \a FactorVariacion y \a maxCont
	/*!\param FactorVariacion Valor a asignar al parámetro \a FactorVariacion.
	\param MaxCont Valor a asignar al parámetro MaxCont.*/
	void setParams(double FactorVariacion, int MaxCont)
	{
		m_factorVariacion = FactorVariacion;
		if(MaxCont<1) MaxCont = 1;
		m_maxCont = MaxCont;
	}

	//!Suministra información sobre los valores de los parámetros \a FactorVariacion y \a MaxCont
	/*!\param FactorVariacion Determina el factor de variación para la medida online.
	\param MaxCont Determina el número máximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida online no varía en un factor mayor a \a FactorVariacion.*/
	void getParams(double& FactorVariacion, int& MaxCont) const
	{
		FactorVariacion = m_factorVariacion;
		MaxCont = m_maxCont;
	}

	//!Destructor
	~OperadorFinalizacionOnline(){}

	//!Especifica cuándo debe darse la orden de interrupción del algoritmo genético
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
	//!Determina el factor de variación para la medida online
	double m_factorVariacion;
	//!Determina el número máximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida online no varía en un factor mayor a \a m_factorVariacion.
	int m_maxCont;
	//!Almacena el número de generaciones que se han ejecutado sin que la medida online varíe en un factor mayor a \a m_factorVariacion
	int m_contador;
};


//!Clase derivada de la clase OperadorFinalizacion que define la finalización del algoritmo basándose en su medida online
/*!Ordena la interrupción del algoritmo genético cuando éste no presenta una variación significativa en su
medida offline después de un número determinado de generaciones.*/
class OperadorFinalizacionOffline:public OperadorFinalizacion
{
public:
	//!Constructor
	/*!Inicializa los valores de los parámetros \a FactorVariacion y \a MaxCont
	\param FactorVariacion 	Determina el factor de variación para la medida offline. Por defecto es igual a 0.005.
	\param MaxCont Determina el número máximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida offline no varía en un factor mayor a \a FactorVariacion. Por defecto es igual a 30.*/
	OperadorFinalizacionOffline(double FactorVariacion=0.0005, int MaxCont=30)
	{
		setParams(FactorVariacion, MaxCont);
	}	

	//!Cambia el valor de los parámetros \a FactorVariacion y \a maxCont
	/*!\param FactorVariacion Valor a asignar al parámetro \a FactorVariacion.
	\param MaxCont Valor a asignar al parámetro MaxCont.*/
	void setParams(double FactorVariacion, int MaxCont)
	{
		m_factorVariacion = FactorVariacion;
		if(MaxCont<1) MaxCont = 1;
		m_maxCont = MaxCont;
	}

	//!Suministra información sobre los valores de los parámetros \a FactorVariacion y \a MaxCont
	/*!\param FactorVariacion Determina el factor de variación para la medida offline.
	\param MaxCont Determina el número máximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida offline no varía en un factor mayor a \a FactorVariacion.*/
	void getParams(double& FactorVariacion, int& MaxCont) const
	{
		FactorVariacion = m_factorVariacion;
		MaxCont = m_maxCont;
	}

	//!Destructor
	~OperadorFinalizacionOffline(){}

	//!Especifica cuándo debe darse la orden de interrupción del algoritmo genético
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
	//!Determina el factor de variación para la medida offline
	double m_factorVariacion;
	//!Determina el número máximo de generaciones que pueden pasar sin interrumpir el algoritmo cuando la medida offline no varía en un factor mayor a \a m_factorVariacion.
	int m_maxCont;
	//!Almacena el número de generaciones que se han ejecutado sin que la medida offline varíe en un factor mayor a \a m_factorVariacion
	int m_contador;
};


#pragma warning(pop)

/****************************************************************************/
/*				DEFINICION DE MACROS PARA USO DE LA LIBRERIA				*/
/****************************************************************************/

/*!Macro que retorna el número menor entre \a a y \a b*/
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define _MINIMO(a,b)  ((a)<(b)? (a):(b) )
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/*!Macro que retorna el número mayor entre \a a y \a b*/
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define _MAXIMO(a,b)  ((a)>(b)? (a):(b) )
#endif /* DOXYGEN_SHOULD_SKIP_THIS */



//!Macro que crea una nueva clase derivada de AlgoritmoGenetico
/*!Declara e implementa el constructor por defecto de esta clase,
también declara implicitamente las funciones de caracter obligatorio, 
las cuales deben ser implementadas en el proyecto de optiización, usando la forma:
<pre>
tipoRetornado nombreAlgoritmo::nombreFuncion(parametros)
{
	//cuerpo de la función
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
Pueden declararse e implementarse adicionalmente otros constructores, métodos y variables.
Tambien puede adicionarse un destructor, en caso de necesitar destruir apuntadores a
objetos creados dinámicamente.*/
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


//!Macro que termina la creación de una clase derivada de AlgoritmoGenetico
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


//!Macro que define el operador de probabilidad que se empleará en el algoritmo genético
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus parámetros iniciales.*/
#define DEFINIR_OPERADOR_PROBABILIDAD(tipoOperador)		\
	{													\
		if(m_pOpProbabilidad) delete m_pOpProbabilidad;	\
		m_pOpProbabilidad=new tipoOperador;				\
	}

//!Macro que define el operador de seleccción que se empleará en el algoritmo genético
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus parámetros iniciales.*/
#define DEFINIR_OPERADOR_SELECCION(tipoOperador)		\
	{													\
		if(m_pOpSeleccion) delete m_pOpSeleccion;		\
		m_pOpSeleccion=new tipoOperador;				\
	}

//!Macro que define el operador de parejas que se empleará en el algoritmo genético
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus parámetros iniciales.*/
#define DEFINIR_OPERADOR_PAREJAS(tipoOperador)			\
	{													\
		if(m_pOpParejas) delete m_pOpParejas;			\
		m_pOpParejas=new tipoOperador;					\
	}

//!Macro que define el operador de reproducción que se empleará en el algoritmo genético
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus parámetros iniciales.*/
#define DEFINIR_OPERADOR_REPRODUCCION(tipoOperador)		\
	{													\
		if(m_pOpReproduccion) delete m_pOpReproduccion;	\
		m_pOpReproduccion=new tipoOperador;				\
	}

//!Macro que define los operadores de adaptación que se emplearán en el algoritmo genético
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus parámetros iniciales.*/
#define ADICIONAR_OPERADOR_ADAPTACION( tipoOperador )	\
	{													\
		OperadorAdaptacion *OA=new tipoOperador;		\
		m_pListaOperadorAdaptacion->Adicionar(OA);		\
	}

//!Macro que define los operadores de finalización que se emplearán en el algoritmo genético
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus parámetros iniciales.*/
#define ADICIONAR_OPERADOR_FINALIZACION( tipoOperador )	\
	{													\
		OperadorFinalizacion *OF=new tipoOperador;		\
		m_pListaOperadorFinalizacion->Adicionar(OF);	\
	}

//!Macro que define el operador de mutación que usará el algoritmo genético para un gen en particular 
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus parámetros iniciales.*/
#define ADICIONAR_OPERADOR_MUTACION( tipoOperador )		\
	{													\
		OperadorMutacion *OM=new tipoOperador;			\
		m_pListaOperadorMutacion->Adicionar(OM);		\
	}

//!Macro que define el operador de cruce que usará el algoritmo genético para un gen en particular 
/*!\param tipoOperador Especifica el tipo de operador a utilizar junto con sus parámetros iniciales.*/
#define ADICIONAR_OPERADOR_CRUCE( tipoOperador )		\
	{													\
		OperadorCruce *OC=new tipoOperador;				\
		m_pListaOperadorCruce->Adicionar(OC);			\
	}

#endif	//__GENETICO_H
