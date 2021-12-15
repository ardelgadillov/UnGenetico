#ifndef __GENBOOL_H
#define __GENBOOL_H

#include "genetico.h"

/*****************************************************
                GEN BOOL (bool)
*****************************************************/

#pragma warning( push, 3)		//Para evitar  "warning(level 4) C4100" ocasionada por parámetros de funciones no referenciados

//!Clase derivada de la clase Gen especializada en un gen de tipo booleano
/*!Se utiliza para representar y almacenar la información genética de variables
cuya codificación se realiza con booleanos (0's y 1's).*/
class GenBool:public Gen
{
public:

	//!Constructor por copia
	/*!Construye el nuevo objeto copiando idénticamente las propiedades 
	de otro objeto de la clase GenBool.
	\param origen Objeto del que se hace copia.*/
	GenBool(const GenBool& origen)
	{
		operator=(origen);
	}
    
	//!Constructor por defecto
	/*!Inicializa el dato contenido por el gen en un valor aleatorio
	\param Min Es ignorado. Existe por compatibilidad.
	\param Max Es ignorado. Existe por compatibilidad.*/
	GenBool(bool Min=false, bool Max=true)
	{
		generarAleatorio();
	}
	
	//!Constructor con valor inicial
	/*!Inicializa el valor del gen con un valor suministrado por el usuario.
	\param Min Es ignorado. Existe por compatibilidad.
	\param Max Es ignorado. Existe por compatibilidad.
	\param ValorInicial Valor inicial del gen. */
	GenBool(bool Min, bool Max, bool ValorInicial)
	{
		m_Valor=ValorInicial;
	}

	//!Crea una copia exacta del gen
	/*!
	\return Apuntador a un nuevo objeto GenBool idéntico al actual.*/
	Gen *crearCopia() const 
	{
		return (new GenBool(*this));
	}

	//!Asigna nuevos valores al gen copiando las propiedades de otro gen
	/*!
	\param origen Objeto del que se hace copia*/
	void copiar(const Gen& origen)
	{
		operator=(*((const class GenBool*)(&origen))); 
	}

	//!Operador de asignación a partir de un dato tipo bool.
	/*!Asigna al gen el valor de un dato de tipo bool. 
	\param nuevoValor Booleano que se asigna al gen
	\return Referencia al gen.*/
    const GenBool& operator=(const bool& nuevoValor)
	{		
		m_Valor = nuevoValor;
		return *this;
	}

	//!Conversión implícita a bool
	/*!Convierte un objeto GenBool en un dato de tipo bool 
	utilizando su valor actual. Permite hacer conversión explícita del estilo \f$(bool)gen1\f$
	\return Valor actual del gen.*/
    operator const bool() const 
	{
		return m_Valor; 
	}

    //!Destructor
    ~GenBool(){}
	
	//!Ajusta el valor actual del dato almacenado por el gen. Retorna el valor asignado.
	/*!
	\param nuevoValor Valor a asignar al gen. 
	\return Valor asignado.*/
	bool setVal(bool nuevoValor)
	{
		return m_Valor = nuevoValor;
	}
	
	//!Retorna el valor actual del dato almacenado en el gen
	/*!
	\return Valor actual del gen */
	bool getVal() const
	{
		return m_Valor;
	}
	
    //!Genera un valor aleatorio para el dato almacenado en el gen
	void generarAleatorio()
	{
		double azar=(double)rand()/(double)RAND_MAX;
		azar<0.5?  m_Valor=false :  m_Valor=true;
	}

	//!Retorna un objeto correspondiente al operador de mutación establecido por defecto para genes booleanos
    inline OperadorMutacion *operadorMutacionDefecto() const;
    //!Retorna un objeto correspondiente al operador de cruce establecido por defecto para genes booleanos
    inline OperadorCruce *operadorCruceDefecto() const;

protected:
	//!Almacena la información del gen
    bool m_Valor;
};

#pragma warning( pop ) 


/*****************************************************
                Operadores de Mutación
*****************************************************/

//!Clase derivada de la clase OperadorMutacion que efectúa una mutación uniforme sobre un gen de tipo booleano
/*!El gen cambia su valor de \a false a \a true o viceversa*/
class OperadorMutacionBoolUniforme:public OperadorMutacion
{
public:
    //!Constructor
	/*!Inicializa el valor de la probabilidad de mutación.
	\param ProbabilidadMutacion Establece el valor inicial para la probabilidad de mutación propia del operador. Por defecto es 0.1.*/
    OperadorMutacionBoolUniforme(double ProbabilidadMutacion=0.1):OperadorMutacion(ProbabilidadMutacion){}
    //!Destructor
    ~OperadorMutacionBoolUniforme(){}
    //!Ejecuta una mutación uniforme sobre un objeto de la clase GenBool
    void mutarGen(Gen *pGen);
};



/*****************************************************
                Operadores de Cruce
*****************************************************/

//!Clase derivada de la clase OperadorCruce que define el cruce discreto entre dos genes de tipo booleano
/*!Crea \a numHijos genes cuyos valores corresponden a
datos booleanos escogidos aleatoriamente entre los valores de \a pMejor y \a pPeor.
Los valores resultantes se agregan al arreglo de genes \a pHijos.*/
class OperadorCruceBoolDiscreto:public OperadorCruce
{
public:
    //!Constructor 
    OperadorCruceBoolDiscreto(){}
    //!Destructor 
    ~OperadorCruceBoolDiscreto(){}
    //!Ejecuta un cruce discreto sobre genes booleanos
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};




/****************************************************************************/
/*			DEFINICION DE MACROS PARA USO DE GENBOOL		*/
/****************************************************************************/

//!Macro que adiciona un gen de tipo booleano al individuo
/*!Se debe utilizar solamente en la función \a codificacion() de la clase derivada de 
AlgoritmoGenetico. Tiene el propósito particular de insertar
un objeto GenBool en una posición fija del individuo apuntado por \a pIndividuo.
Este objeto estará asociado con una de las variables del sistema a optimizar perteneciente 
a la clase derivada de AlgoritmoGenetico de la cual se establece su valor inicial.
\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posición en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado.
\param valInicial Valor inicial que toma el gen.*/
#define ADICIONAR_GENBOOL(pIndividuo, pos, var, valInicial)	\
__ADICIONAR_GEN(pIndividuo, pos, var, bool, GenBool, (false, true, valInicial) )


#endif	//__GENBOOL_H
