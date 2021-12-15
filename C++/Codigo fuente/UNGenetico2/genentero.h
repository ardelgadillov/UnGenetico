#ifndef __GENENTERO_H
#define __GENENTERO_H


#include "genetico.h"


/*****************************************************
               GEN ENTERO (long)
*****************************************************/

//!Clase derivada de la clase Gen especializada en un gen de tipo entero
/*!Se utiliza para representar y almacenar la información genética de variables
cuya codificación se realiza con números enteros.*/
class GenEntero:public Gen
{
public:
	
	//!Constructor por copia
	/*!Construye el nuevo objeto copiando idénticamente las propiedades 
	de otro objeto de la clase GenEntero.
	\param origen Objeto del que se hace copia.*/
	GenEntero(const GenEntero& origen)
	{
		operator=(origen);
	}

	//!Constructor por defecto
	GenEntero(long Min=-10, long Max=10);

	//!Constructor con valor inicial
	GenEntero(long Min, long Max, long ValorInicial);

	//!Crea una copia exacta del gen
	/*!
	\return Apuntador a un nuevo objeto GenEntero idéntico al actual.*/
	Gen *crearCopia() const 
	{
		return (new GenEntero(*this));
	}

	//!Asigna nuevos valores al gen copiando las propiedades de otro gen
	/*!
	\param origen Objeto del que se hace copia*/
	void copiar(const Gen& origen)
	{
		operator=(*((const GenEntero*)(&origen)));
	}

	//!Operador de asignación a partir de un dato entero.
	/*!Asigna al gen el valor de un dato de tipo entero. 
	\param nuevoValor Número entero que se asigna al gen
	\return Referencia al gen.*/
    const GenEntero& operator=(const long& nuevoValor)
	{
		setVal(nuevoValor);
		return *this;
	}

	//!Conversión implícita a long
	/*!Convierte un objeto GenEntero en un dato de tipo long 
	utilizando su valor actual. Permite hacer operaciones del estilo \f$(long)gen1+(long)gen2\f$.
	\return Valor actual del gen.*/
    operator const long() const 
	{
		return m_Valor; 
	}

	//!Destructor
	~GenEntero(){}

	//!Ajusta el valor actual del dato almacenado por el gen. Retorna el valor asignado.
	/*!
	\param nuevoValor Valor a asignar al gen. Es restringido al rango <i>[m_Minimo, m_Maximo]</i>.
	\return Valor asignado.*/
	long setVal(long nuevoValor)
	{
		return m_Valor = restringir(nuevoValor, m_Minimo, m_Maximo);
	}

	//!Retorna el valor actual del dato almacenado en el gen
	/*!
	\return Valor actual del gen */
	long getVal() const
	{
		return m_Valor;
	}

	//!Retorna el valor máximo que puede tomar el gen
	/*!
	\return Valor máximo permitido para el gen. */
	long getMax() const
	{
		return m_Maximo;
	}

	//!Retorna el valor mínimo que puede tomar el gen
	/*!
	\return Valor mínimo permitido para el gen. */
	long getMin() const
	{
		return m_Minimo;
	}

	//!Genera un valor aleatorio para el dato almacenado en el gen
	/*!Asigna al gen un nuevo valor entero aleatorio en el rango establecido*/
	void generarAleatorio()
	{
		double azar = (double)rand()/(double)RAND_MAX;
		m_Valor = redondear( m_Minimo + azar*(m_Maximo-m_Minimo));	
	}

	//!Retorna un objeto correspondiente al operador de mutación establecido por defecto para genes enteros
	inline OperadorMutacion *operadorMutacionDefecto() const;

	//!Retorna un objeto correspondiente al operador de cruce establecido por defecto para genes enteros
	inline OperadorCruce *operadorCruceDefecto() const;

protected:
	
	//!Almacena la información del gen
	long m_Valor;
    
	//!Mínimo valor que puede tomar el gen. Solo puede modificarse en el constructor
	long m_Minimo;
	
	//!Máximo valor que puede tomar el gen. Solo puede modificarse en el constructor	
	long m_Maximo;	
};


/*****************************************************
                Operadores de Mutación
*****************************************************/


//!Clase derivada de la clase OperadorMutacion empleada en genes enteros
/*!Efectúa una mutación uniforme sobre un gen de tipo entero*/
class OperadorMutacionEnteroUniforme:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutación.
	\param ProbabilidadMutacion Establece el valor inicial para la probabilidad de mutación propia del operador. Por defecto es 0.1.*/
	OperadorMutacionEnteroUniforme(double ProbabilidadMutacion=0.1):OperadorMutacion(ProbabilidadMutacion){}

	//!Destructor
	~OperadorMutacionEnteroUniforme(){}

	//!Ejecuta una mutación uniforme sobre un objeto de la clase GenEntero
	void mutarGen(Gen *pGen);
};


//!Clase derivada de la clase OperadorMutacion empleada en genes enteros
/*!Efectúa una mutación no uniforme sobre un gen de tipo entero*/
class OperadorMutacionEnteroNoUniforme:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutación y el valor de b*******************.
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutación propia del operador. Por defecto es 0.1
	\param b Valor Valor que determina el grado de dependencia con el numero de generaciones. Por defecto es 0.5 */
	OperadorMutacionEnteroNoUniforme(AlgoritmoGenetico *pAG, double ProbabilidadMutacion=0.1, double b=0.5):OperadorMutacion(ProbabilidadMutacion)
    {
		m_pAG=pAG;        
		setB(b);
    }

	//!Cambia el valor del parámetro \a b
	/*!\param b Valor a asignar al parámetro \a b.*/ 
	void setB(double b)
	{
		m_b = b;
	}

	//!Retorna el valor del parámetro \a b
	double getB() const
	{
		return m_b;
	}
	
	//!Destructor
	~OperadorMutacionEnteroNoUniforme(){}

	//!Ejecuta una mutación no uniforme sobre un objeto de la clase GenEntero
	void mutarGen(Gen *pGen);

private:

	//!*Parámetro que determina el grado de dependencia con el numero de generaciones. 
	double m_b;	

	//!Apuntador a un objeto de la clase AlgoritmoGenetico
	AlgoritmoGenetico *m_pAG;
};

//!Clase derivada de la clase OperadorMutacion empleada en genes enteros
/*!Efectúa una mutación Muhlenbein sobre un gen de de tipo entero*/
class OperadorMutacionEnteroMuhlenbein:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutación y el valor del factor que define el rango de mutación.
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutación propia del operador. Por defecto es 0.1.
	\param Factor Valor que define el rango de mutación. Por defecto es 0.1.*/
	OperadorMutacionEnteroMuhlenbein(double ProbabilidadMutacion=0.1, double Factor=0.1):OperadorMutacion(ProbabilidadMutacion)
	{		
		setFactor(Factor);
	}	

	//!Cambia el valor del parámetro \a Factor
	/*!\param Factor Valor que define el rango de mutación.*/
	void setFactor(double Factor)
	{
		m_Factor=Factor;
	}

	//!Retorna el valor del parámetro Factor
	double getFactor() const
	{
		return m_Factor;
	}

	//!Destructor
	~OperadorMutacionEnteroMuhlenbein(){}

	//!Ejecuta una mutación Muhlenbein sobre un objeto de la clase GenEntero
	void mutarGen(Gen *pGen);

private:

	//!Valor que define el rango de mutación. Solo puede modificarse en el constructor
	double m_Factor;
};


/*****************************************************
                Operadores de Cruce
*****************************************************/

//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efectúa un cruce plano entre dos genes de tipo entero*/
class OperadorCruceEnteroPlano:public OperadorCruce
{
public:
    //!Constructor
    OperadorCruceEnteroPlano(){}

    //!Destructor
    ~OperadorCruceEnteroPlano(){}

    //!Ejecuta un cruce plano sobre genes enteros
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};


//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efectúa un cruce aritmético entre dos genes de tipo entero*/
class OperadorCruceEnteroAritmetico:public OperadorCruce
{
public:
	//!Constructor
	/*!Inicializa el valor del parámetro \a Lambda.
	\param Lambda Valor que pondera cada uno de los genes padres. Por defecto es igual a 0.7.*/
	OperadorCruceEnteroAritmetico(double Lambda=0.7)
	{
		setLambda(Lambda);
	}
	
	//!Cambia el valor del parámetro \a Lambda comprobando los límites
	/*!\param Lambda Valor a asignar al parámetro \a Lambda.*/
	void setLambda(double Lambda)
	{
		m_Lambda = restringir(Lambda, 0.0, 1.0);
	}
	
	//!Retorna el valor del parámetro \a Lambda
	double getLambda()
	{
		return m_Lambda;
	}
	
	//!Destructor
	~OperadorCruceEnteroAritmetico(){}	
	
	//!Ejecuta un cruce aritmético sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);

private:
	//!Parámetro que pondera cada uno de los genes padres.
	double m_Lambda;
};

//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efectúa un cruce BLX -\f$\alpha\f$ entre dos genes de tipo entero*/
class OperadorCruceEnteroBLX:public OperadorCruce
{
public:
	//!Constructor
	/*!Inicializa Inicializa el valor de \a Alfa
	\param Alfa Establece la amplitud del intervalo de definición. Por defecto es igual a 0.3.*/
	OperadorCruceEnteroBLX(double Alfa=0.3)
	{
		setAlfa(Alfa);
	}
	//!Cambia el valor del parámetro \a Alfa comprobando los límites
	/*!\param Alfa Valor a asignar al parámetro \a Alfa*/
	void setAlfa(double Alfa)
	{
		m_Alfa = restringir(Alfa, 0.0, 1.0);
	}
	//!Retorna el valor del parámetro \a Alfa
	double getAlfa()
	{
		return m_Alfa;
	}
	//!Destructor
	~OperadorCruceEnteroBLX(){}
	//!Ejecuta un cruce BLX - \f$\alpha\f$ sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
private:
	//!Establece la amplitud del intervalo de definición.
	double m_Alfa;
};

//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efectúa un cruce lineal entre dos genes de tipo entero*/
class OperadorCruceEnteroLineal:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceEnteroLineal(){}
	//!Destructor 
	~OperadorCruceEnteroLineal(){}
	//!Ejecuta un cruce lineal sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efectúa un cruce discreto entre dos genes de tipo entero*/
class OperadorCruceEnteroDiscreto:public OperadorCruce
{
public:
  //!Constructor
    OperadorCruceEnteroDiscreto(){}
  //!Destructor
    ~OperadorCruceEnteroDiscreto(){}
  //!Ejecuta un cruce discreto sobre genes enteros
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};


//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efectúa un cruce extendido intermedio entre dos genes de tipo entero*/
class OperadorCruceEnteroIntermedioExtendido:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceEnteroIntermedioExtendido(){}
	//!Destructor
	~OperadorCruceEnteroIntermedioExtendido(){}
	//!Ejecuta un cruce intermedio extendido sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};


//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efectúa un cruce heurístico entre dos genes de tipo entero*/
class OperadorCruceEnteroHeuristico:public OperadorCruce
{
public:
  //!Constructor
    OperadorCruceEnteroHeuristico(){}
  //!Destructor
    ~OperadorCruceEnteroHeuristico(){}
  //!Ejecuta un cruce heurístico sobre genes enteros
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};
 
//!Clase derivada de la clase OperadorCruce empleada en genes enteros
/*!Efectúa un cruce BGA lineal entre dos genes de tipo entero*/
class OperadorCruceEnteroLinealBGA:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceEnteroLinealBGA(AlgoritmoGenetico *pAG){m_pAG=pAG;}
	//!Destructor
	~OperadorCruceEnteroLinealBGA(){}
	//!Ejecuta el cruce lineal BGA sobre genes enteros
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
private:
	//!Apuntador al objeto AlgoritmoGenetico en el que opera
	AlgoritmoGenetico *m_pAG;
};



/****************************************************************************/
/*		DEFINICION DE MACROS PARA USO DE GENENTERO					
/****************************************************************************/

//!Macro que adiciona un gen de tipo entero a un individuo
/*!
Se debe utilizar solamente en la función codificacion() de la clase derivada de 
AlgoritmoGenetico.
Su propósito particular es insertar un objeto GenEntero en una posición determinada
del individuo apuntado por \a pIndividuo.
Este objeto estará asociado con una de las variables del sistema a optimizar, perteneciente
a la clase derivada de AlgoritmoGenetico.
De esta variable se establecen los valores mínimo, máximo e inicial.

\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posición en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado.
\param valMin Valor mínimo que puede tomar el gen.
\param valMax Valor máximo que puede tomar el gen.
\param valInicial Valor inicial que toma el gen.*/
#define ADICIONAR_GENENTERO(pIndividuo, pos, var, valMin, valMax, valInicial)	\
	__ADICIONAR_GEN(pIndividuo, pos, var, long, GenEntero, (valMin, valMax, valInicial) )


#endif		//__GENENTERO_H